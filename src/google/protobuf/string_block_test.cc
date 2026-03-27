// Protocol Buffers - Google's data interchange format
// Copyright 2023 Google Inc.  All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd
//
// This file defines tests for the internal StringBlock class

#include "google/protobuf/string_block.h"

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
#include <thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Must be included last
#include "google/protobuf/port_def.inc"

using ::testing::Eq;
using ::testing::Ne;

namespace google {
namespace protobuf {
namespace internal {
namespace {

size_t EffectiveSizeFor(int size) {
  size -= sizeof(StringBlock);
  return static_cast<size_t>(size - (size % sizeof(std::string)));
}

size_t AllocatedSizeFor(int size) {
  return EffectiveSizeFor(size) + sizeof(StringBlock);
}

TEST(StringBlockTest, HeapAllocateOneBlock) {
  StringBlock* block = StringBlock::New(nullptr);

  ASSERT_THAT(block, Ne(nullptr));
  EXPECT_THAT(block->next(), Eq(nullptr));
  ASSERT_TRUE(block->heap_allocated());
  EXPECT_THAT(block->allocated_size(), Eq(AllocatedSizeFor(256)));
  EXPECT_THAT(block->effective_size(), Eq(EffectiveSizeFor(256)));
  EXPECT_THAT(block->begin(), Eq(block->AtOffset(0)));
  EXPECT_THAT(block->end(), Eq(block->AtOffset(block->effective_size())));

  EXPECT_THAT(StringBlock::Delete(block), Eq(AllocatedSizeFor(256)));
}

TEST(StringBlockTest, EmplaceOneBlock) {
  // NextSize() returns unrounded 'min_size()' on first call.
  size_t size = StringBlock::NextSize(nullptr);
  EXPECT_THAT(size, Eq(256));

  auto buffer = std::make_unique<char[]>(size);
  StringBlock* block = StringBlock::Emplace(buffer.get(), size, nullptr);

  ASSERT_THAT(block, Ne(nullptr));
  EXPECT_THAT(block->next(), Eq(nullptr));
  ASSERT_FALSE(block->heap_allocated());
  EXPECT_THAT(block->allocated_size(), Eq(AllocatedSizeFor(256)));
  EXPECT_THAT(block->effective_size(), Eq(EffectiveSizeFor(256)));
  EXPECT_THAT(block->begin(), Eq(block->AtOffset(0)));
  EXPECT_THAT(block->end(), Eq(block->AtOffset(block->effective_size())));

  EXPECT_THAT(StringBlock::Delete(block), Eq(0));
}

TEST(StringBlockTest, HeapAllocateMultipleBlocks) {
  // Note: first two blocks are 256
  StringBlock* previous = StringBlock::New(nullptr);

  for (int size = 256; size <= 8192; size *= 2) {
    StringBlock* block = StringBlock::New(previous);
    ASSERT_THAT(block, Ne(nullptr));
    ASSERT_THAT(block->next(), Eq(previous));
    ASSERT_TRUE(block->heap_allocated());
    ASSERT_THAT(block->allocated_size(), Eq(AllocatedSizeFor(size)));
    ASSERT_THAT(block->effective_size(), Eq(EffectiveSizeFor(size)));
    ASSERT_THAT(block->begin(), Eq(block->AtOffset(0)));
    ASSERT_THAT(block->end(), Eq(block->AtOffset(block->effective_size())));
    previous = block;
  }

  // Capped at 8K from here on
  StringBlock* block = StringBlock::New(previous);
  ASSERT_THAT(block, Ne(nullptr));
  ASSERT_THAT(block->next(), Eq(previous));
  ASSERT_TRUE(block->heap_allocated());
  ASSERT_THAT(block->allocated_size(), Eq(AllocatedSizeFor(8192)));
  ASSERT_THAT(block->effective_size(), Eq(EffectiveSizeFor(8192)));
  ASSERT_THAT(block->begin(), Eq(block->AtOffset(0)));
  ASSERT_THAT(block->end(), Eq(block->AtOffset(block->effective_size())));

  while (block) {
    size_t size = block->allocated_size();
    StringBlock* next = block->next();
    EXPECT_THAT(StringBlock::Delete(block), Eq(AllocatedSizeFor(size)));
    block = next;
  }
}

TEST(StringBlockTest, EmplaceMultipleBlocks) {
  std::vector<std::unique_ptr<char[]>> buffers;

  // Convenience lambda to allocate a buffer and invoke Emplace on it.
  auto EmplaceBlock = [&](StringBlock* previous) {
    size_t size = StringBlock::NextSize(previous);
    buffers.push_back(std::make_unique<char[]>(size));
    return StringBlock::Emplace(buffers.back().get(), size, previous);
  };

  // Note: first two blocks are 256
  StringBlock* previous = EmplaceBlock(nullptr);

  for (int size = 256; size <= 8192; size *= 2) {
    StringBlock* block = EmplaceBlock(previous);
    ASSERT_THAT(block, Ne(nullptr));
    ASSERT_THAT(block->next(), Eq(previous));
    ASSERT_FALSE(block->heap_allocated());
    ASSERT_THAT(block->allocated_size(), Eq(AllocatedSizeFor(size)));
    ASSERT_THAT(block->effective_size(), Eq(EffectiveSizeFor(size)));
    ASSERT_THAT(block->begin(), Eq(block->AtOffset(0)));
    ASSERT_THAT(block->end(), Eq(block->AtOffset(block->effective_size())));
    previous = block;
  }

  // Capped at 8K from here on
  StringBlock* block = EmplaceBlock(previous);
  ASSERT_THAT(block, Ne(nullptr));
  EXPECT_THAT(block->next(), Eq(previous));
  ASSERT_FALSE(block->heap_allocated());
  ASSERT_THAT(block->allocated_size(), Eq(AllocatedSizeFor(8192)));
  ASSERT_THAT(block->effective_size(), Eq(EffectiveSizeFor(8192)));
  ASSERT_THAT(block->begin(), Eq(block->AtOffset(0)));
  ASSERT_THAT(block->end(), Eq(block->AtOffset(block->effective_size())));

  while (block) {
    StringBlock* next = block->next();
    EXPECT_THAT(StringBlock::Delete(block), Eq(0));
    block = next;
  }
}

TEST(StringBlockTest, TlsCacheReuseAndLifo) {
  auto clear_tls_cache = []() {
    while (safe_tls_string_block_cache.head != nullptr) {
      auto* temp = safe_tls_string_block_cache.head;
      safe_tls_string_block_cache.head = 
          static_cast<decltype(temp)>(temp->next);
      internal::SizedDelete(temp, temp->size);
    }
    safe_tls_string_block_cache.count = 0;
  };

  clear_tls_cache();

  StringBlock* b1 = StringBlock::New(nullptr);
  StringBlock* b2 = StringBlock::New(nullptr);
  void* addr1 = b1;
  void* addr2 = b2;

  StringBlock::Delete(b1);
  StringBlock::Delete(b2);

  EXPECT_THAT(safe_tls_string_block_cache.count, Eq(2));

  StringBlock* b3 = StringBlock::New(nullptr);
  EXPECT_THAT(static_cast<void*>(b3), Eq(addr2));

  StringBlock* b4 = StringBlock::New(nullptr);
  EXPECT_THAT(static_cast<void*>(b4), Eq(addr1));

  EXPECT_THAT(safe_tls_string_block_cache.count, Eq(0));

  StringBlock::Delete(b3);
  StringBlock::Delete(b4);
}

TEST(StringBlockTest, TlsCacheCapacityLimit) {
  auto clear_tls_cache = []() {
    while (safe_tls_string_block_cache.head != nullptr) {
      auto* temp = safe_tls_string_block_cache.head;
      safe_tls_string_block_cache.head = 
          static_cast<decltype(temp)>(temp->next);
      internal::SizedDelete(temp, temp->size);
    }
    safe_tls_string_block_cache.count = 0;
  };

  clear_tls_cache();

  std::vector<StringBlock*> blocks;
  for (int i = 0; i < 40; ++i) {
    blocks.push_back(StringBlock::New(nullptr));
  }

  for (int i = 0; i < 40; ++i) {
    StringBlock::Delete(blocks[i]);
  }
  EXPECT_THAT(safe_tls_string_block_cache.count, Eq(SafeTlsBlockCache::kMaxCacheBlocks));
}

TEST(StringBlockTest, TlsCacheThreadIsolation) {
  int main_thread_count = safe_tls_string_block_cache.count;

  std::thread t([]() {
    EXPECT_THAT(safe_tls_string_block_cache.count, Eq(0));

    StringBlock* b1 = StringBlock::New(nullptr);
    StringBlock::Delete(b1);

    EXPECT_THAT(safe_tls_string_block_cache.count, Eq(1));
  });

  t.join();

  EXPECT_THAT(safe_tls_string_block_cache.count, Eq(main_thread_count));
}

}  // namespace
}  // namespace internal
}  // namespace protobuf
}  // namespace google

#include "google/protobuf/port_undef.inc"
