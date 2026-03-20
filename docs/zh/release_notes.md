# 版本说明书

## 版本配套说明

### 产品版本信息

| 产品名称 | Kunpeng BoostKit |
| :--- | :--- |
| **产品版本** | 26.0.0 |
| **软件名称** | protobuf（Kunpeng Protobuf 优化版） |
| **软件包版本** | 1.0.0 |

### 与操作系统、编译器和CPU配套说明

|操作系统|CPU类型|编译器|
|--|--|--|
|openEuler 22.03 LTS SP3|鲲鹏920处理器|clang 16.0.6+<br>GCC 9.0+|

## 版本更新说明

### V1.0.0

**新增特性**

|特性描述|更新说明|
|--|--|
|新增 varint 编解码优化|针对 `coded_stream.h` 与 `parse_context.h` 的 varint 编解码路径进行循环展开与向量化优化，提升 packed 数组编解码性能。|
