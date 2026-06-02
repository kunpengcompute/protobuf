# API Reference

## Functions

[**Table 1**](#varint-optimization-functions) lists the optimization functions related to Protobuf varint encoding and decoding.

**Table 1** varint optimization functions<a id="varint-optimization-functions"></a>

|Name|Description|
|--|--|
|WriteUInt32Packed|Performs varint encoding on packed `uint32` arrays, prioritizing the SVE vectorization.|
|WriteSInt32Packed|Performs ZigZag + varint encoding on packed `sint32` arrays, prioritizing the SVE vectorization.|
|varint_sve|SVE vectorized batch varint encoding implementation.|
|UnsafeVarint(uint32_t)|32-bit varint encoding implementation with loop unrolling.|
|UnsafeVarint(uint64_t)|64-bit varint encoding implementation with loop unrolling.|
|ReadPackedVarintArray|SVE2-optimized batch parsing implementation of packed varint arrays.|

## Function Description

### WriteUInt32Packed

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

- Pointer to the position where data is written.

### WriteSInt32Packed

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

- Pointer to the position where data is written.

### varint_sve

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

- Pointer to the position where data is written.

### UnsafeVarint(uint32_t)

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

- Pointer to the position where data is written.

### UnsafeVarint(uint64_t)

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

- Pointer to the position where data is written.

### ReadPackedVarintArray

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

- The function returns a pointer to the parsing end position. If the parsing fails, it returns `nullptr`.
