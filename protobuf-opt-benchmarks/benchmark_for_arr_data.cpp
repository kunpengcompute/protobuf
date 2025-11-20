#include <benchmark/benchmark.h>
#include <string>
#include <random>
#include <vector>
#include <cstring>
#include <iostream>
#include "arr_data32.pb.h"
#include "arr_data64.pb.h"
#include "test.pb.h"

uint64_t genInt(uint8_t byteNum)
{
    std::string data;
    std::random_device rd;
    std::mt19937 engine(rd());
    uint64_t start = 1;
    if (byteNum >= 10) {
        // Max value of uint64
        std::uniform_int_distribution<uint64_t> dist(start << 63, start << 64 - 1);
        return dist(engine);
    } else {
        std::uniform_int_distribution<uint64_t> dist(start << ((byteNum - 1) * 7), start << (byteNum * 7) - 1);
        return dist(engine);
    }
}

pb::test::ArrData32 genData32(size_t arrSize, uint8_t byteNum)
{
    if (byteNum > 5) {
        std::cout << "Uint32 overflow detected." << std::endl;
    }
    pb::test::ArrData32 data;
    for (size_t i = 1; i <= arrSize; ++i) {
        uint64_t n = genInt(byteNum);
        // Max value of uint32 when overflow occurs
        data.add_numbers(n > std::numeric_limits<uint32_t>::max() ? std::numeric_limits<uint32_t>::max() : n);
    }
    return data;
}

pb::test::ArrData64 genData64(size_t arrSize, uint8_t byteNum)
{
    pb::test::ArrData64 data;
    for (size_t i = 1; i <= arrSize; ++i) {
        uint64_t n = genInt(byteNum);
        // Max value of uint64 when overflow occurs
        data.add_numbers(n > std::numeric_limits<uint64_t>::max() ? std::numeric_limits<uint64_t>::max() : n);
    }
    return data;
}

std::string res;

/*
 * type: 32/64
 * size: the number of repeated elements
 * len：the byte length of each encoded element
 */
#define DEFINE_BENCHMARK(type, size, len)                                                \
    pb::test::ArrData##type arrayData##type##_##size##_##len = genData##type(size, len); \
    static void BM_arr_##type##_##size##_##len(benchmark::State &state)                  \
    {                                                                                    \
        for (auto _ : state) {                                                           \
            res = arrayData##type##_##size##_##len.SerializeAsString();                  \
        }                                                                                \
    }                                                                                    \
    BENCHMARK(BM_arr_##type##_##size##_##len);

#define DEFINE_DESERIALIZE_BENCHMARK(type, size, len)                                                \
    pb::test::ArrData##type arrayDataDe##type##_##size##_##len = genData##type(size, len);           \
    pb::test::ArrData##type arrayDataDe2##type##_##size##_##len;                                     \
    std::string res##type##_##size##_##len = arrayDataDe##type##_##size##_##len.SerializeAsString(); \
    static void BM_Deserialize_arr_##type##_##size##_##len(benchmark::State &state)                  \
    {                                                                                                \
        for (auto _ : state) {                                                                       \
            arrayDataDe2##type##_##size##_##len.ParseFromString(res##type##_##size##_##len);         \
        }                                                                                            \
    }                                                                                                \
    BENCHMARK(BM_Deserialize_arr_##type##_##size##_##len);

// uint32 benchmarks
DEFINE_BENCHMARK(32, 20, 2)
DEFINE_BENCHMARK(32, 40, 2)
DEFINE_BENCHMARK(32, 100, 2)
DEFINE_BENCHMARK(32, 500, 2)
DEFINE_BENCHMARK(32, 1000, 2)
DEFINE_BENCHMARK(32, 5000, 2)
DEFINE_BENCHMARK(32, 10000, 2)

DEFINE_BENCHMARK(32, 20, 3)
DEFINE_BENCHMARK(32, 40, 3)
DEFINE_BENCHMARK(32, 100, 3)
DEFINE_BENCHMARK(32, 500, 3)
DEFINE_BENCHMARK(32, 1000, 3)
DEFINE_BENCHMARK(32, 5000, 3)
DEFINE_BENCHMARK(32, 10000, 3)

DEFINE_BENCHMARK(32, 20, 4)
DEFINE_BENCHMARK(32, 40, 4)
DEFINE_BENCHMARK(32, 100, 4)
DEFINE_BENCHMARK(32, 500, 4)
DEFINE_BENCHMARK(32, 1000, 4)
DEFINE_BENCHMARK(32, 5000, 4)
DEFINE_BENCHMARK(32, 10000, 4)

// uint64 benchmarks
DEFINE_BENCHMARK(64, 20, 2)
DEFINE_BENCHMARK(64, 40, 2)
DEFINE_BENCHMARK(64, 100, 2)
DEFINE_BENCHMARK(64, 500, 2)
DEFINE_BENCHMARK(64, 1000, 2)
DEFINE_BENCHMARK(64, 5000, 2)
DEFINE_BENCHMARK(64, 10000, 2)

DEFINE_BENCHMARK(64, 20, 3)
DEFINE_BENCHMARK(64, 40, 3)
DEFINE_BENCHMARK(64, 100, 3)
DEFINE_BENCHMARK(64, 500, 3)
DEFINE_BENCHMARK(64, 1000, 3)
DEFINE_BENCHMARK(64, 5000, 3)
DEFINE_BENCHMARK(64, 10000, 3)

DEFINE_BENCHMARK(64, 20, 4)
DEFINE_BENCHMARK(64, 40, 4)
DEFINE_BENCHMARK(64, 100, 4)
DEFINE_BENCHMARK(64, 500, 4)
DEFINE_BENCHMARK(64, 1000, 4)
DEFINE_BENCHMARK(64, 5000, 4)
DEFINE_BENCHMARK(64, 10000, 4)

DEFINE_BENCHMARK(64, 20, 5)
DEFINE_BENCHMARK(64, 40, 5)
DEFINE_BENCHMARK(64, 100, 5)
DEFINE_BENCHMARK(64, 500, 5)
DEFINE_BENCHMARK(64, 1000, 5)
DEFINE_BENCHMARK(64, 5000, 5)
DEFINE_BENCHMARK(64, 10000, 5)

DEFINE_BENCHMARK(64, 20, 8)
DEFINE_BENCHMARK(64, 40, 8)
DEFINE_BENCHMARK(64, 100, 8)
DEFINE_BENCHMARK(64, 500, 8)
DEFINE_BENCHMARK(64, 1000, 8)
DEFINE_BENCHMARK(64, 5000, 8)
DEFINE_BENCHMARK(64, 10000, 8)

DEFINE_BENCHMARK(64, 20, 9)
DEFINE_BENCHMARK(64, 40, 9)
DEFINE_BENCHMARK(64, 100, 9)
DEFINE_BENCHMARK(64, 500, 9)
DEFINE_BENCHMARK(64, 1000, 9)
DEFINE_BENCHMARK(64, 5000, 9)
DEFINE_BENCHMARK(64, 10000, 9)

DEFINE_DESERIALIZE_BENCHMARK(32, 20, 1)
DEFINE_DESERIALIZE_BENCHMARK(32, 40, 1)
DEFINE_DESERIALIZE_BENCHMARK(32, 100, 1)
DEFINE_DESERIALIZE_BENCHMARK(32, 500, 1)
DEFINE_DESERIALIZE_BENCHMARK(32, 1000, 1)
DEFINE_DESERIALIZE_BENCHMARK(32, 5000, 1)
DEFINE_DESERIALIZE_BENCHMARK(32, 10000, 1)

DEFINE_DESERIALIZE_BENCHMARK(64, 20, 1)
DEFINE_DESERIALIZE_BENCHMARK(64, 40, 1)
DEFINE_DESERIALIZE_BENCHMARK(64, 100, 1)
DEFINE_DESERIALIZE_BENCHMARK(64, 500, 1)
DEFINE_DESERIALIZE_BENCHMARK(64, 1000, 1)
DEFINE_DESERIALIZE_BENCHMARK(64, 5000, 1)
DEFINE_DESERIALIZE_BENCHMARK(64, 10000, 1)

BENCHMARK_MAIN();
