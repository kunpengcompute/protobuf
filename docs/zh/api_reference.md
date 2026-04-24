# API参考

## 函数说明

Protobuf varint编解码相关优化函数如[**表 1** varint优化函数列表](#varint优化函数列表)所示。

**表 1** varint优化函数列表<a id="varint优化函数列表"></a>

|名称|说明|
|--|--|
|WriteUInt32Packed|对 `uint32` 的packed数组进行varint编码，优先走SVE向量化路径。|
|WriteSInt32Packed|对 `sint32` 的packed数组进行ZigZag + varint编码，优先走SVE向量化路径。|
|varint_sve|SVE向量化批量varint编码实现。|
|UnsafeVarint(uint32_t)|32位varint循环展开编码实现。|
|UnsafeVarint(uint64_t)|64位varint循环展开编码实现。|
|ReadPackedVarintArray|SVE2优化的packed varint数组批量解析实现。|

## 函数定义

### WriteUInt32Packed

**函数功能**

对 `uint32` 的packed repeated字段进行varint编码，优先使用SVE向量化实现。

**函数定义**

```cpp
template <typename T>
PROTOBUF_ALWAYS_INLINE uint8_t* WriteUInt32Packed(int num, const T& r,
                                                  int size, uint8_t* ptr);
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|num|字段编号|正整数|输入|
|r|连续数组数据|具备 `data()` 与 `size()` 的容器|输入|
|size|编码后的长度|非负整数|输入|
|ptr|输出缓冲区指针|有效指针|输入/输出|

**返回值**

返回写入后的位置指针。

### WriteSInt32Packed

**函数功能**

对 `sint32` 的packed repeated字段进行ZigZag + varint编码，优先使用SVE向量化实现。

**函数定义**

```cpp
template <typename T>
PROTOBUF_ALWAYS_INLINE uint8_t* WriteSInt32Packed(int num, const T& r,
                                                  int size, uint8_t* ptr);
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|num|字段编号|正整数|输入|
|r|连续数组数据|具备 `data()` 与 `size()` 的容器|输入|
|size|编码后的长度|非负整数|输入|
|ptr|输出缓冲区指针|有效指针|输入/输出|

**返回值**

返回写入后的位置指针。

### varint_sve

**函数功能**

SVE向量化批量varint编码实现，每批次处理8个 `uint32_t` 或 `int32_t`。

**函数定义**

```cpp
template <typename T>
PROTOBUF_ALWAYS_INLINE uint8_t* varint_sve(const T* loc, uint8_t* ptr, int Size);
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|loc|输入数组指针|有效指针|输入|
|ptr|输出缓冲区指针|有效指针|输入/输出|
|Size|数组元素数量|非负整数|输入|

**返回值**

返回写入后的位置指针。

### UnsafeVarint(uint32_t)

**函数功能**

32位varint循环展开编码，实现固定分支写入1 ~ 5字节。

**函数定义**

```cpp
PROTOBUF_ALWAYS_INLINE static uint8_t* UnsafeVarint(uint32_t value,
                                                    uint8_t* ptr);
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|value|待编码值|`uint32_t`|输入|
|ptr|输出缓冲区指针|有效指针|输入/输出|

**返回值**

返回写入后的位置指针。

### UnsafeVarint(uint64_t)

**函数功能**

64位varint循环展开编码，实现固定分支写入 1 ~ 10 字节。

**函数定义**

```cpp
PROTOBUF_ALWAYS_INLINE static uint8_t* UnsafeVarint(uint64_t value,
                                                    uint8_t* ptr);
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|value|待编码值|`uint64_t`|输入|
|ptr|输出缓冲区指针|有效指针|输入/输出|

**返回值**

返回写入后的位置指针。

### ReadPackedVarintArray

**函数功能**

SVE2优化的packed varint数组批量解析实现，优先在当前buffer范围内完成解析。

**函数定义**

```cpp
template <typename Add>
const char* ReadPackedVarintArray(const char* ptr, const char* end, Add add);
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|ptr|输入缓冲区指针|有效指针|输入|
|end|缓冲区结束指针|有效指针|输入|
|add|回调写入器|可调用对象|输入|

**返回值**

返回解析结束位置指针，失败返回 `nullptr`。
