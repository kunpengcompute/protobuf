# API参考

## 函数说明

Protobuf 鲲鹏优化相关函数列表如下：

|分类|函数名称|说明|
|--|--|--|
|**Varint编解码优化**|WriteUInt32Packed|对 `uint32` 的packed数组进行varint编码，优先走SVE向量化路径。|
||WriteSInt32Packed|对 `sint32` 的packed数组进行ZigZag + varint编码，优先走SVE向量化路径。|
||varint_sve|SVE向量化批量varint编码实现。|
||UnsafeVarint(uint32_t)|32位varint循环展开编码实现。|
||UnsafeVarint(uint64_t)|64位varint循环展开编码实现。|
||ReadPackedVarintArray|SVE2优化的packed varint数组批量解析实现。|
|**Map容器优化**|KeyMapBase::InsertNodeWithoutResizeCheck|跳过扩容检查的Map节点插入，配合预分配使用。|
||KeyMapBase::InternalReserve|Map容量预分配，减少rehash次数。|
|**StringBlock内存优化**|EpsCopyInputStream::AppendString|追加式字符串读取，复用已分配内存。|
||SafeTlsBlockCache::Push|将释放的StringBlock放入线程局部缓存。|
||SafeTlsBlockCache::Pop|从线程局部缓存获取StringBlock，减少内存分配。|
|**C++23特性支持**|MessageLite::AppendPartialToString|使用C++23 `resize_and_overwrite` 优化序列化性能。|

## 函数定义

### Varint编解码优化

#### WriteUInt32Packed

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

#### ReadPackedVarintArray

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

---

### Map容器优化

#### KeyMapBase::InsertNodeWithoutResizeCheck

**函数功能**

在Map中插入节点，跳过扩容检查。适用于已通过 `InternalReserve` 预分配容量的场景，避免重复的负载因子检查和rehash操作。

**函数定义**

```cpp
template <typename Key>
bool KeyMapBase<Key>::InsertNodeWithoutResizeCheck(KeyNode* node);
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|node|待插入的Map节点|有效指针|输入|

**返回值**

返回 `true` 表示插入新节点，`false` 表示替换了已存在的节点。

**使用说明**

此函数通过 `allow_shrink = false` 参数跳过缩容检查，仅在扩容必要时执行rehash。必须配合 `InternalReserve` 使用，否则可能导致哈希表负载过高。

### KeyMapBase::InternalReserve

**函数功能**

根据预期新增元素数量预分配Map容量，确保后续插入操作不会触发rehash。

**函数定义**

```cpp
template <typename Key>
void KeyMapBase<Key>::InternalReserve(size_t additional_count);
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|additional_count|预期新增的元素数量|非负整数|输入|

**返回值**

无返回值。


---

### StringBlock内存优化

#### EpsCopyInputStream::AppendString

**函数功能**

从输入流追加数据到字符串，复用字符串已分配的容量，减少内存重新分配。

**函数定义**

```cpp
const char* EpsCopyInputStream::AppendString(const char* ptr, int size,
                                             std::string* s);
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|ptr|输入缓冲区指针|有效指针|输入|
|size|要读取的字节数|非负整数|输入|
|s|目标字符串|有效指针|输入/输出|

**返回值**

返回读取后的位置指针，失败返回 `nullptr`。

**使用说明**

与 `ReadString` 不同，`AppendString` 使用 `std::string::append` 而非先清空再写入，因此能复用字符串的剩余容量。特别适用于RepeatedField<std::string>的解析场景，配合StringBlock内存池使用。

#### SafeTlsBlockCache::Push

**函数功能**

将释放的StringBlock放入线程局部缓存，供后续分配时重用，减少系统内存分配调用。

**函数定义**

```cpp
void SafeTlsBlockCache::Push(void* block, size_t size);
```

**参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|block|待缓存的内存块|有效指针|输入|
|size|内存块大小|正整数|输入|

**返回值**

无返回值。

### SafeTlsBlockCache::Pop

**函数功能**

从线程局部缓存获取指定大小的StringBlock，如果缓存命中则避免一次系统内存分配。

**函数定义**

```cpp
void* SafeTlsBlockCache::Pop(size_t requested_size);
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|requested_size|请求的内存块大小|正整数|输入|

**返回值**

返回可用的内存块指针，缓存未命中返回 `nullptr`。

---

### C++23特性支持

#### MessageLite::AppendPartialToString

**函数功能**

将消息序列化并追加到字符串。当编译器支持C++23 `resize_and_overwrite` 特性时，使用该方法避免不必要的零初始化，提升序列化性能。

**函数定义**

```cpp
bool MessageLite::AppendPartialToString(std::string* output) const;
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|output|目标字符串|有效指针|输入/输出|

**返回值**

返回 `true` 表示序列化成功，`false` 表示失败。

**使用说明**

该函数内部实现根据编译器特性自动选择优化路径：

**C++23 优化路径**（编译器支持 `__cpp_lib_string_resize_and_overwrite >= 202110L`）：
```cpp
output->resize_and_overwrite(
    new_size, [this, old_size, byte_size](char* data, size_t /*size*/) {
        SerializeToArrayImpl(*this,
                             reinterpret_cast<uint8_t*>(data) + old_size,
                             byte_size);
        return old_size + byte_size;
    });
```

**传统路径**（不支持C++23特性）：
```cpp
absl::strings_internal::STLStringResizeUninitializedAmortized(output, new_size);
uint8_t* start = reinterpret_cast<uint8_t*>(io::mutable_string_data(output) + old_size);
SerializeToArrayImpl(*this, start, byte_size);
```

