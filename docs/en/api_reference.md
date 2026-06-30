# API Reference

## Functions

The following table lists Kunpeng-optimized Protobuf functions.

| Category | Function Name | Description |
| -- | -- | -- |
|**Varint encoding/decoding optimization**|WriteUInt32Packed|Performs varint encoding on a packed `uint32` array, prioritizing the SVE vectorized path.|
|**Varint encoding/decoding optimization**|WriteSInt32Packed|Performs ZigZag + varint encoding on a packed `sint32` array, prioritizing the SVE vectorized path.|
|**Varint encoding/decoding optimization**|varint_sve|Implements SVE vectorized batch varint encoding.|
|**Varint encoding/decoding optimization**|UnsafeVarint(uint32_t)|Implements 32-bit varint encoding with loop unrolling.|
|**Varint encoding/decoding optimization**|UnsafeVarint(uint64_t)|Implements 64-bit varint encoding with loop unrolling.|
|**Varint encoding/decoding optimization**|ReadPackedVarintArray|Implements SVE2-optimized batch parsing for packed varint arrays.|
|**Map container optimization**|KeyMapBase::InsertNodeWithoutResizeCheck|Inserts a Map node while skipping the capacity expansion check, used in conjunction with pre-allocation.|
|**Map container optimization**|KeyMapBase::InternalReserve|Pre-allocates Map capacity to reduce the number of rehashes.|
|**StringBlock memory optimization**|EpsCopyInputStream::AppendString|Appends string reads to reuse already allocated memory.|
|**StringBlock memory optimization**|SafeTlsBlockCache::Push|Pushes released StringBlocks into the thread-local cache.|
|**StringBlock memory optimization**|SafeTlsBlockCache::Pop|Retrieves a StringBlock from the thread-local cache to reduce memory allocations.|
|**C++23 feature support**|MessageLite::AppendPartialToString|Optimizes serialization performance using C++23 `resize_and_overwrite`.|

## Function Description

### Varint Encoding/Decoding Optimization

#### WriteUInt32Packed

**Function Usage**

Performs varint encoding on packed `uint32` repeated fields, prioritizing the SVE vectorization.

**Function Syntax**

```cpp
template <typename T>
PROTOBUF_ALWAYS_INLINE uint8_t* WriteUInt32Packed(int num, const T& r,
                                                  int size, uint8_t* ptr);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|num|Field number|Positive integer|Input|
|r|Contiguous array data|Container with `data()` and `size()`|Input|
|size|Encoded length|Non-negative integer|Input|
|ptr|Output buffer pointer|Valid pointer|Input/Output|

**Return Value**

Pointer to the position where data is written.

#### WriteSInt32Packed

**Function Usage**

Performs ZigZag + varint encoding on packed `sint32` repeated fields, prioritizing the SVE vectorization.

**Function Syntax**

```cpp
template <typename T>
PROTOBUF_ALWAYS_INLINE uint8_t* WriteSInt32Packed(int num, const T& r,
                                                  int size, uint8_t* ptr);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|num|Field number|Positive integer|Input|
|r|Contiguous array data|Container with `data()` and `size()`|Input|
|size|Encoded length|Non-negative integer|Input|
|ptr|Output buffer pointer|Valid pointer|Input/Output|

**Return Value**

Pointer to the position where data is written.

#### varint_sve

**Function Usage**

SVE vectorized batch varint encoding implementation, processing eight `uint32_t` or `int32_t` arrays per batch.

**Function Syntax**

```cpp
template <typename T>
PROTOBUF_ALWAYS_INLINE uint8_t* varint_sve(const T* loc, uint8_t* ptr, int Size);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|loc|Input array pointer|Valid pointer|Input|
|ptr|Output buffer pointer|Valid pointer|Input/Output|
|Size|Number of array elements|Non-negative integer|Input|

**Return Value**

Pointer to the position where data is written.

#### UnsafeVarint(uint32_t)

**Function Usage**

32-bit varint encoding with loop unrolling, allowing fixed-branch writes of 1 to 5 bytes.

**Function Syntax**

```cpp
PROTOBUF_ALWAYS_INLINE static uint8_t* UnsafeVarint(uint32_t value,
                                                    uint8_t* ptr);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|value|Value to be encoded|`uint32_t`|Input|
|ptr|Output buffer pointer|Valid pointer|Input/Output|

**Return Value**

Pointer to the position where data is written.

#### UnsafeVarint(uint64_t)

**Function Usage**

64-bit varint encoding with loop unrolling, allowing fixed-branch writes of 1 to 10 bytes.

**Function Syntax**

```cpp
PROTOBUF_ALWAYS_INLINE static uint8_t* UnsafeVarint(uint64_t value,
                                                    uint8_t* ptr);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|value|Value to be encoded|`uint64_t`|Input|
|ptr|Output buffer pointer|Valid pointer|Input/Output|

**Return Value**

Pointer to the position where data is written.

#### ReadPackedVarintArray

**Function Usage**

SVE2-optimized batch parsing implementation of packed varint arrays, prioritizing completion within the current buffer range.

**Function Syntax**

```cpp
template <typename Add>
const char* ReadPackedVarintArray(const char* ptr, const char* end, Add add);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|ptr|Input buffer pointer|Valid pointer|Input|
|end|Buffer end pointer|Valid pointer|Input|
|add|Callback writer|Callable object|Input|

**Return Value**

The function returns a pointer to the parsing end position. If the parsing fails, it returns `nullptr`.

### Map Container Optimization

#### KeyMapBase::InsertNodeWithoutResizeCheck

**Function Usage**

Inserts a node into the Map while skipping the capacity expansion check. This is applicable to scenarios where capacity has been pre-allocated via `InternalReserve`, avoiding repetitive load factor checks and rehash operations.

**Function Syntax**

```cpp
template <typename Key>
bool KeyMapBase<Key>::InsertNodeWithoutResizeCheck(KeyNode* node);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|node|Map node to be inserted|Valid pointer|Input|

**Return Value**

Returns `true` if a new node is inserted, and `false` if an existing node is replaced.

**Instructions**

This function skips the capacity shrinkage check via the `allow_shrink = false` parameter, and performs rehash only when capacity expansion is necessary. It must be used in conjunction with `InternalReserve`; otherwise, it may lead to an excessively high load factor on the hash table.

#### KeyMapBase::InternalReserve

**Function Usage**

Pre-allocates Map capacity based on the expected number of new elements, ensuring that subsequent insertion operations do not trigger a rehash.

**Function Syntax**

```cpp
template <typename Key>
void KeyMapBase<Key>::InternalReserve(size_t additional_count);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|additional_count|Expected number of new elements to be added.|Non-negative integer|Input|

**Return Value**

None

### StringBlock Memory Optimization

#### EpsCopyInputStream::AppendString

**Function Usage**

Appends data from an input stream to a string, reusing the allocated capacity of the string to reduce memory reallocations.

**Function Syntax**

```cpp
const char* EpsCopyInputStream::AppendString(const char* ptr, int size,
                                             std::string* s);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|ptr|Pointer to the input buffer|Valid pointer|Input|
|size|Number of bytes to read|Non-negative integer|Input|
|s|Target string|Valid pointer|Input/Output|

**Return Value**

Returns the position pointer after reading, or `nullptr` if the operation fails.

**Instructions**

Unlike `ReadString`, `AppendString` utilizes `std::string::append` instead of clearing the string before writing, thereby reusing the remaining capacity of the string. This is particularly applicable to parsing scenarios of `RepeatedField<std::string>` and is used in conjunction with the StringBlock memory pool.

#### SafeTlsBlockCache::Push

**Function Usage**

Pushes released StringBlocks into the thread-local cache for reuse in subsequent allocations, reducing system memory allocation calls.

**Function Syntax**

```cpp
void SafeTlsBlockCache::Push(void* block, size_t size);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|block|Memory block to be cached|Valid pointer|Input|
|size|Memory block size|Positive integer|Input|

**Return Value**

None

#### SafeTlsBlockCache::Pop

**Function Usage**

Retrieves a StringBlock of a specified size from the thread-local cache. If a cache hit occurs, a system memory allocation is avoided.

**Function Syntax**

```cpp
void* SafeTlsBlockCache::Pop(size_t requested_size);
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|requested_size|Requested memory block size|Positive integer|Input|

**Return Value**

Returns a pointer to the available memory block, or `nullptr` if a cache miss occurs.

### C++23 Feature Support

#### MessageLite::AppendPartialToString

**Function Usage**

Serializes a message and appends it to a string. When the compiler supports the C++23 `resize_and_overwrite` feature, this method is used to avoid unnecessary zero-initialization, thereby improving serialization performance.

**Function Syntax**

```cpp
bool MessageLite::AppendPartialToString(std::string* output) const;
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|output|Target string|Valid pointer|Input/Output|

**Return Value**

Returns `true` if serialization succeeds, and `false` if it fails.

**Instructions**

The internal implementation of this function automatically selects the optimized path based on compiler features.

* C++23 optimized path (The compiler supports `__cpp_lib_string_resize_and_overwrite >= 202110L`.)

  ```cpp
  output->resize_and_overwrite(
      new_size, [this, old_size, byte_size](char* data, size_t /*size*/) {
          SerializeToArrayImpl(*this,
                               reinterpret_cast<uint8_t*>(data) + old_size,
                               byte_size);
          return old_size + byte_size;
      });
  ```

* Legacy path (The compiler does not support C++23 features.)

  ```cpp
  absl::strings_internal::STLStringResizeUninitializedAmortized(output, new_size);
  uint8_t* start = reinterpret_cast<uint8_t*>(io::mutable_string_data(output) + old_size);
  SerializeToArrayImpl(*this, start, byte_size);
  ```
