# Protobuf介绍

## 最新消息

- 2026.03.30：发布 varint packed 编解码优化，并使能 SIMDUTF 校验算法。

## 简介

Protocol Buffers（简称 Protobuf）是 Google 开源的**语言中立、平台中立、可扩展**的结构化数据序列化机制，用于在分布式系统间实现高效的数据交换与存储，同时支持与 RPC 框架（如 gRPC）无缝集成。其核心优势为序列化后体积小、解析速度快、支持多版本协议兼容演进，且提供了跨 C++/Java/Python/Go 等多语言的运行时库。

### 核心模块

Protobuf 整体分为**编译器模块**（`protoc`）和**运行时模块**（各语言 Runtime）两大核心，其内部模块划分如下：

- **描述符模块**：负责定义消息/枚举/服务的元数据结构（如 `Descriptor`/`FieldDescriptor`）；
- **消息核心模块**：提供消息的序列化、反序列化、字段访问等基础能力；
- **I/O 编解码模块**：实现二进制流的读写与编码（如变长整数、打包数组）；
- **工具类模块**：提供 JSON 转换、消息比对、时间类型处理等辅助能力；
- **编译器插件模块**：支持自定义代码生成逻辑，适配不同语言的代码输出；
- **Editions 特性模块**：实现 `edition` 协议版本（2023/2024）的语法解析与特性控制（如字段默认值、重复字段编码）。

### 对外核心接口函数

以下为 Protobuf（以 C++ 为例）对外暴露的核心接口函数，覆盖消息操作、I/O 编解码、工具类等核心能力：

| 模块分类 | 接口函数/方法 | 功能描述 |
|--|--|--|
| 消息基础操作 | `MessageLite::SerializeToString` | 将消息序列化为二进制字符串 |
| | `MessageLite::ParseFromString` | 从二进制字符串反序列化消息 |
| | `Message::CopyFrom` | 从另一个同类型消息复制字段数据 |
| | `Message::Clear` | 清空消息所有字段的数值 |
| 内存分配（Arena） | `Arena::CreateMessage<T>` | 在指定 Arena 内存池创建消息实例，减少内存碎片 |
| | `Arena::Reset` | 重置 Arena 内存池，释放所有分配的对象 |
| I/O 编解码 | `CodedInputStream::ReadVarint32` | 从输入流读取 32 位变长整数 |
| | `CodedOutputStream::WriteString` | 将字符串写入输出流并编码 |
| JSON 工具类 | `util::JsonPrintOptions::SetAlwaysPrintEnumsAsInts` | 配置 JSON 输出时枚举是否以整数形式展示 |
| | `util::MessageToJsonString` | 将 Protobuf 消息转换为 JSON 字符串 |
| | `util::JsonStringToMessage` | 从 JSON 字符串解析为 Protobuf 消息 |
| 消息比对 | `util::MessageDifferencer::Equals` | 比对两个消息的字段值是否完全一致 |
| 描述符查询 | `Descriptor::FindFieldByName` | 根据字段名查询消息的字段描述符 |
| 动态消息 | `DynamicMessageFactory::GetPrototype` | 根据描述符创建动态消息的原型 |

## 目录结构

```text
# 文档目录
README.md                          # 项目说明
LICENSE                            # 代码许可证
docs/
├── LICENSE                        # 文档许可证
└── zh/
    ├── api.md                     # API参考
    ├── installation_guide.md      # 编译与安装
    ├── quick_start.md             # 快速上手
    └── release_notes.md           # 版本说明书
    
# 代码目录
protobuf/
├── src/                          # C++ 运行时和 protoc 编译器源码
│   ├── google/protobuf/          # 核心运行时库
│   │   ├── compiler/             # 代码生成器（C++、Java、Python 等）
│   │   ├── io/                   # I/O 和编解码
│   │   ├── util/                 # 工具类（JSON 转换等）
│   │   └── ...
│   └── ...
├── java/                         # Java 运行时
├── python/                       # Python 运行时
├── rust/                         # Rust 运行时
├── ruby/                         # Ruby 运行时
├── php/                          # PHP 运行时
├── csharp/                       # C# 运行时
├── objectivec/                   # Objective-C 运行时
├── upb/                          # 微型 Protobuf C 实现
├── hpb/                          # 高性能 C++ 绑定
├── conformance/                  # 跨语言一致性测试
├── benchmarks/                   # 性能基准测试
├── cmake/                        # CMake 配置文件
├── bazel/                        # Bazel 构建规则
├── third_party/                  # 第三方依赖
├── docs/                         # 文档
└── BUILD.bazel                   # Bazel 构建文件
```

## 版本说明

版本对齐与优化说明参见 [《版本说明书》](docs/zh/release_notes.md)。

## 环境部署

编译环境、依赖获取与安装步骤参见 [《编译与安装》](docs/zh/installation_guide.md)。

## 快速入门

快速入门说明参见 [《快速入门》](docs/zh/quick_start.md)。

## 文档

|资源名称|资源简介|
|--|--|
|[版本说明书](docs/zh/release_notes.md)|提供 Protobuf 鲲鹏优化的每个发布版本的基础信息和特性更新说明。|
|[快速入门](docs/zh/quick_start.md)|提供 Protobuf 鲲鹏优化的快速上手示例与编译运行说明。|
|[API参考](docs/zh/api.md)|提供 varint 编解码优化相关接口说明与函数定义。|
|[安装指南](docs/zh/installation_guide.md)|提供 Protobuf 鲲鹏优化代码的环境配置与编译安装的详细指导。|

## 免责声明

**致 Protobuf 使用者**

- 本软件仅供调试和开发之用，使用者需自行承担使用风险，并理解以下内容：
    - 此代码仓计划参与 Protobuf 软件开源，仅对 Protobuf 部分函数在鲲鹏处理器上进行性能优化，编码风格遵照原生开源软件，继承原生开源软件安全设计，不破坏原生开源软件设计及编码风格和方式。软件的任何漏洞与安全问题，均由相应的上游社区根据其漏洞和安全响应机制解决。请密切关注上游社区发布的通知和版本更新。鲲鹏计算社区对软件的漏洞及安全问题不承担任何责任。
    - 数据处理及删除：用户在使用本软件过程中产生的数据属于用户责任范畴。建议用户在使用完毕后及时删除相关数据，以防信息泄露。
    - 数据保密与传播：使用者了解并同意不得将通过本软件产生的数据随意外发或传播。对于由此产生的信息泄露、数据泄露或其他不良后果，本软件及其开发者概不负责。
    - 用户输入安全性：用户需自行保证输入的命令行的安全性，并承担因输入不当而导致的任何安全风险或损失。对于输入命令行不当所导致的问题，本软件及其开发者概不负责。

- 免责声明范围：本免责声明适用于所有使用本软件的个人或实体。使用本软件即表示您同意并接受本声明的内容，并愿意承担因使用该功能而产生的风险和责任，如有异议请停止使用本软件。
- 在使用本软件之前，请**谨慎阅读并理解以上免责声明的内容**。对于使用本软件所产生的任何问题或疑问，请及时联系开发者。

**致数据所有者**

如果您不希望您的数据集等信息在 Protobuf 优化仓库中被提及，或希望更新相关描述，请在 GitCode 提交 issue，我们将根据您的要求删除或更新相关描述。感谢您的理解与支持。


## License

Protocol Buffers 采用 BSD-3-Clause 许可证，具体请参见[LICENSE文件](LICENSE)。

Protocol Buffers项目的文档适用 CC-BY 4.0 许可证，具体请参见[LICENSE文件](docs/LICENSE)。

## 贡献声明

欢迎大家为社区做贡献，如果使用过程中有任何问题/建议，或者需要反馈特性需求和bug报告，可以提交[Issues](https://gitcode.com/boostkit/community/blob/master/docs/contributor/issue-submit.md)联系我们，具体贡献方法可参考[这里](https://gitcode.com/boostkit/community/blob/master/docs/contributor/contributing.md)。同时也欢迎大家在[讨论专区](https://gitcode.com/boostkit/community/discussions)展开讨论交流。感谢您的支持。


## 致谢

Protobuf由华为公司的下列部门联合贡献：

- 鲲鹏计算Boostkit开发部

感谢来自社区的每一个PR，欢迎贡献Protocol Buffers！
