# Protobuf

## Latest Updates

- 2026.03.30: Released varint packed encoding and decoding optimization and enabled the SIMDUTF verification algorithm.

## Introduction

Protocol Buffers (Protobuf) is an open-source mechanism developed by Google for serializing structured data. It is **language-neutral**, **platform-neutral**, and **extensible**, designed for efficient data exchange and storage in distributed systems. Protobuf also supports seamless integration with RPC frameworks such as gRPC. Its core advantages include small serialized size, fast parsing speed, and support for multi-version protocol compatibility and evolution. Additionally, it provides runtime libraries across multiple languages including C++, Java, Python, and Go.

### Core Modules

Protobuf consists of two main core components: the **compiler module** (`protoc`) and the **runtime module** (language-specific runtime). The internal modules are organized as follows:

- **Descriptor module**: Defines metadata structures for messages, enums, and services (e.g., `Descriptor`, `FieldDescriptor`).
- **Message core module**: Provides fundamental capabilities for message serialization, deserialization, and field access.
- **I/O encoding and decoding module**: Implements reading, writing, and encoding of binary streams (e.g., variable-length integers, packed arrays).
- **Utility module**: Provides auxiliary capabilities such as JSON conversion, message comparison, and time type processing.
- **Compiler plugin module**: Supports custom code generation logic to adapt to different language outputs.
- **Editions feature module**: Implements syntax parsing and feature control for different protocol `editions` (e.g., 2023/2024), including features like default field values and repeated field encoding.

### Core External API Functions

The following table lists the core API functions exposed by Protobuf (using C++ as an example), such as message operations, I/O encoding and decoding, utility functions.

| Module Category| Function/Method| Description|
|--|--|--|
| Basic message operations| `MessageLite::SerializeToString` | Serializes a message into a binary string.|
| | `MessageLite::ParseFromString` | Deserializes a message from a binary string.|
| | `Message::CopyFrom` | Copies field data from another message of the same type.|
| | `Message::Clear` | Clears the values of all fields in a message.|
| Memory allocation (Arena)| `Arena::CreateMessage<T>` | Creates a message instance in a specified arena memory pool to reduce memory fragments.|
| | `Arena::Reset` | Resets the arena memory pool, releasing all allocated objects.|
| I/O encoding and decoding| `CodedInputStream::ReadVarint32` | Reads a 32-bit variable-length integer from the input stream.|
| | `CodedOutputStream::WriteString` | Writes a string to the output stream and encodes it.|
| JSON utility classes| `util::JsonPrintOptions::SetAlwaysPrintEnumsAsInts` | Configures JSON output to display enums as integers.|
| | `util::MessageToJsonString` | Converts a Protobuf message into a JSON string.|
| | `util::JsonStringToMessage` | Parses a JSON string into a Protobuf message.|
| Message comparison| `util::MessageDifferencer::Equals` | Compares the field values of two messages to check if they are identical.|
| Descriptor query| `Descriptor::FindFieldByName` | Queries the field descriptor of a message based on the field name.|
| Dynamic message| `DynamicMessageFactory::GetPrototype` | Creates a prototype for a dynamic message based on the descriptor.|

## Directory Structure

```text
# Document Directory
README_EN.md                          # Project description
LICENSE                            # Code license
docs/
├── LICENSE                        # Document license
└── en/
    ├── api_reference.md           # API Reference
    ├── installation_guide.md      # Compilation and Installation
    ├── quick_start.md             # Quick Start
    └── release_notes.md           # Release Notes
    
# Code Directory
protobuf/
├── src/                          # Source code of the C++ runtime and protoc compiler
│   ├── google/protobuf/          # Core runtime library
│   │   ├── compiler/             # Code generator (C++, Java, Python, etc.)
│   │   ├── io/                   # I/O encoding and decoding
│   │   ├── util/                 # Utility classes (JSON conversion, etc.)
│   │   └── ...
│   └── ...
├── java/                         # Java runtime
├── python/                       # Python runtime
├── rust/                         # Rust runtime
├── ruby/                         # Ruby runtime
├── php/                          # PHP runtime
├── csharp/                       # C# runtime
├── objectivec/                   # Objective-C runtime
├── upb/                          # Micro Protobuf C implementation
├── hpb/                          # High-performance C++ binding
├── conformance/                  # Cross-language consistency test
├── benchmarks/                   # Performance benchmark test
├── cmake/                        # CMake configuration file
├── bazel/                        # Bazel build rules
├── third_party/                  # Third-party dependencies
├── docs/                         # Documents
└── BUILD.bazel                   # Bazel build file
```

## Release Notes

For details about version mapping, see [Release Notes](docs/en/release_notes.md).

## Environment Deployment

For details about the compilation environment, dependency obtaining, and installation procedure, see [Installation Guide](docs/en/installation_guide.md).

## Quick Start

For details, see [Quick Start](docs/en/quick_start.md).

## Documents

|Name|Description|
|--|--|
|[Release Notes](docs/en/release_notes.md)|Provides basic information and feature updates of each version of optimized Kunpeng Protobuf.|
|[Quick Start](docs/en/quick_start.md)|Provides quick start examples and compilation instructions for optimized Kunpeng Protobuf.|
|[API Reference](docs/en/api_reference.md)|Provides descriptions and definitions of APIs related to varint encoding and decoding optimization.|
|[Installation Guide](docs/en/installation_guide.md)|Provides detailed guidance on environment configuration, compilation, and installation of optimized Kunpeng Protobuf code.|

## Disclaimer

**To Protobuf users**

- This software is intended solely for debugging and development. You are responsible for any risks and should carefully review the following information:
    - This code repository contributes to the Protobuf open-source project solely for performance optimization of certain Protobuf functions on Kunpeng processors. It strictly adheres to the coding style and methods, as well as security design of the native open-source software. Any vulnerability and security issues of the software shall be resolved by the corresponding upstream communities according to their response mechanisms. Please pay attention to the notifications and version updates released by the upstream communities. The Kunpeng computing community does not assume any responsibility for software vulnerabilities and security issues.
    - Data processing and deletion: Users are responsible for managing and deleting any data generated while using this software. Users are advised to delete such data promptly after use to prevent information leakage.
    - Data confidentiality and transmission: Users understand and agree not to share or transmit any data generated by this software. Neither the software nor its developers are responsible for any information leakage, data breaches, or other negative consequences.
    - User input security: Users are responsible for the security of any commands they enter and for any risks or losses resulting from improper input. The software and its developers are not liable for issues caused by incorrect command usage.

- Disclaimer scope: This disclaimer applies to all individuals and entities using this software. By using the software, you acknowledge and accept this statement and assume all risks and responsibilities arising from its use. If you do not agree, please stop using the software immediately.
- Before using this software, please **read and understand the preceding disclaimer**. If you have any questions, contact the developer.

**To data owners**

If you do not want your dataset to be mentioned in the Protobuf optimization repository, or if you wish to update its description, please submit an issue on GitCode. We will delete or update your description according to your request. Thank you for your understanding and support.

## License

Protobuf is licensed under BSD-3-Clause. For details, see [LICENSE](https://gitcode.com/boostkit/protobuf/blob/master/LICENSE).

The documents of Protobuf are licensed under CC-BY 4.0. For details, see [LICENSE](docs/LICENSE).

## Contribution Statement

We welcome your contributions to the community. If you have any questions/suggestions or want to provide feedback on feature requirements and bug reports, you can submit [issues](https://gitcode.com/boostkit/community/blob/master/docs/contributor/issue-submit.md). For details, see the [contribution guideline](https://gitcode.com/boostkit/community/blob/master/docs/contributor/contributing.md). You are also welcome to share insights in the [Discussions](https://gitcode.com/boostkit/community/discussions). Thank you for your support.

## Acknowledgments

Protobuf is jointly developed by the following Huawei department:

- Kunpeng Computing BoostKit Development Dept

Thank you to everyone in the community for your PRs. We warmly welcome contributions to Protobuf!
