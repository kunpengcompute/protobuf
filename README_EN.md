# Protobuf

## Latest Updates

- [2026-06-30]: Released V1.0.1: Added parsing optimizations for Map and StringBlock and supports the C++23 `resize_and_overwrite` feature.
- [2026-03-30]: Released V1.0.0: Added varint packed encoding and decoding optimizations and enabled the SIMDUTF verification algorithm.

## Project Introduction

### Overview

Protocol Buffers (Protobuf) is an open-source mechanism developed by Google for serializing structured data. It is **language-neutral**, **platform-neutral**, and **extensible**, designed for efficient data exchange and storage in distributed systems. Protobuf also supports seamless integration with RPC frameworks such as bRPC/gRPC. Its core advantages include small serialized size, fast parsing speed, and support for multi-version protocol compatibility and evolution. Additionally, it provides runtime libraries across multiple languages including C++, Java, Python, and Go.

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

#### Message Serialization and Deserialization

| Function/Method | Description |
| -- | -- |
| `MessageLite::SerializeToString` | Serializes a message into a binary string. |
| `MessageLite::SerializeToArray` | Serializes a message into a byte array. |
| `MessageLite::SerializeToCodedStream` | Serializes a message to a coded stream. |
| `MessageLite::SerializeToOstream` | Serializes a message to an output stream. |
| `MessageLite::ParseFromString` | Deserializes a message from a binary string. |
| `MessageLite::ParseFromArray` | Deserializes a message from a byte array. |
| `MessageLite::ParseFromCodedStream` | Deserializes a message from a coded stream. |
| `MessageLite::ParseFromIstream` | Deserializes a message from an input stream. |
| `MessageLite::ByteSizeLong` | Calculates the byte size after serialization. |

#### Basic Message Operations

| Function/Method | Description |
| -- | -- |
| `MessageLite::Clear` | Clears the values of all fields in the message. |
| `MessageLite::IsInitialized` | Checks if all required fields are initialized. |
| `MessageLite::GetTypeName` | Gets the message type name. |
| `Message::CopyFrom` | Copies field data from another message of the same type. |
| `Message::MergeFrom` | Merges fields from another message into the current message. |
| `Message::DebugString` | Generates a human-readable debugging string. |
| `Message::SpaceUsedLong` | Calculates the memory size occupied by the message. |

#### Reflection Interfaces

| Function/Method | Description |
| -- | -- |
| `Message::GetDescriptor` | Gets the descriptor of the message. |
| `Message::GetReflection` | Gets the reflection interface of the message. |
| `Reflection::GetInt32/Int64/...` | Gets the field value via reflection (supporting various types). |
| `Reflection::SetInt32/Int64/...` | Sets the field value via reflection (supporting various types). |
| `Reflection::HasField` | Checks if a field has been set. |
| `Reflection::ClearField` | Clears a specified field. |
| `Reflection::ListFields` | Lists all fields that have been set. |

#### Descriptor Queries

| Function/Method | Description |
| -- | -- |
| `Descriptor::name` | Gets the message type name. |
| `Descriptor::full_name` | Gets the fully qualified name of the message. |
| `Descriptor::field_count` | Gets the number of fields. |
| `Descriptor::FindFieldByName` | Finds a field descriptor by field name. |
| `Descriptor::FindFieldByNumber` | Finds a field descriptor by field tag number. |
| `FieldDescriptor::name` | Gets the field name. |
| `FieldDescriptor::number` | Gets the field tag number. |
| `FieldDescriptor::type` | Gets the field type. |
| `FieldDescriptor::is_repeated` | Checks if the field is a repeated field. |

#### Arena Memory Management

| Function/Method | Description |
| -- | -- |
| `Arena::Create<T>` | Creates an object instance in the arena memory pool to reduce memory fragmentation. |
| `Arena::CreateArray<T>` | Creates an array in the arena memory pool. |
| `Arena::Reset` | Resets the arena memory pool, releasing all allocated objects. |
| `Arena::SpaceAllocated` | Gets the size of the memory allocated by the arena. |
| `Arena::Own` | Takes ownership of a heap object, allowing its lifecycle to be managed by the arena. |
| `MessageLite::GetArena` | Gets the pointer to the arena where the message resides. |

#### I/O Encoding and Decoding

| Function/Method | Description |
| -- | -- |
| `CodedInputStream::ReadVarint32` | Reads a 32-bit variable-length integer from the input stream. |
| `CodedInputStream::ReadVarint64` | Reads a 64-bit variable-length integer from the input stream. |
| `CodedInputStream::ReadString` | Reads a string from the input stream. |
| `CodedInputStream::ReadTag` | Reads a field tag. |
| `CodedInputStream::PushLimit` | Sets a read limit to prevent buffer overflow. |
| `CodedOutputStream::WriteVarint32` | Writes a 32-bit variable-length integer to the output stream. |
| `CodedOutputStream::WriteVarint64` | Writes a 64-bit variable-length integer to the output stream. |
| `CodedOutputStream::WriteString` | Writes a string to the output stream and encodes it. |
| `CodedOutputStream::WriteTag` | Writes a field tag. |

#### JSON Conversion

| Function/Method | Description |
| -- | -- |
| `util::MessageToJsonString` | Converts a Protobuf message into a JSON string. |
| `util::JsonStringToMessage` | Parses a JSON string into a Protobuf message. |
| `util::BinaryToJsonString` | Converts binary Protobuf data into a JSON string. |
| `util::JsonToBinaryString` | Converts a JSON string into binary Protobuf data. |

#### Message Comparison

| Function/Method | Description |
| -- | -- |
| `util::MessageDifferencer::Equals` | Compares whether the field values of two messages are completely identical. |
| `util::MessageDifferencer::Equivalent` | Compares whether two messages are equivalent (taking default values into account). |
| `util::MessageDifferencer::Compare` | Performs message comparison and returns the differences. |
| `MessageDifferencer::ReportDifferencesToString` | Outputs the message differences as a string. |

#### Dynamic Messages

| Function/Method | Description |
| -- | -- |
| `DynamicMessageFactory::GetPrototype` | Creates a prototype of a dynamic message based on a descriptor. |
| `DynamicMessage::New` | Creates an instance of a dynamic message. |

## Directory Structure

The project directory structure is as follows:

```text
# Document Directory
README_EN.md                          # Project description
LICENSE                            # Code license
docs/
├── LICENSE                        # Document license
└── en/
    ├── api_reference.md           # API Reference
    ├── installation_guide.md      # Installation Guide
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

For detailed information about feature changes in each Kunpeng-optimized Protobuf release, see [Release Notes](docs/en/release_notes.md).

## Environment Deployment

For details about the compilation environment, dependency obtaining, and installation procedure of Kunpeng-optimized Protobuf, see [Installation Guide](docs/en/installation_guide.md).

## Quick Start

To quickly get started with Protobuf, see [Quick Start](docs/en/quick_start.md).

## Documents

|Name|Description|
|--|--|
|[Release Notes](docs/en/release_notes.md)|Provides basic information and feature updates of each version of Kunpeng-optimized Protobuf.|
|[Quick Start](docs/en/quick_start.md)|Provides quick start examples and compilation instructions for Kunpeng-optimized Protobuf.|
|[API Reference](docs/en/api_reference.md)|Provides descriptions and definitions of APIs related to varint encoding and decoding optimization.|
|[Installation Guide](docs/en/installation_guide.md)|Provides detailed guidance on environment configuration, compilation, and installation of Kunpeng-optimized Protobuf code.|

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

Protobuf is licensed under BSD-3-Clause. For details, see [LICENSE](LICENSE).

The documents of Protobuf are licensed under CC-BY 4.0. For details, see [LICENSE](docs/LICENSE).

## Contribution Statement

We welcome your contributions to the community. If you have any questions/suggestions or want to provide feedback on feature requirements and bug reports, you can submit issues. For details, see the contribution guideline. You are also welcome to share insights in the [Discussions](https://gitcode.com/boostkit/community/discussions). Thank you for your support.

## Acknowledgments

Protobuf is jointly developed by the following Huawei department:

- Kunpeng Computing BoostKit Development Dept

Thank you to everyone in the community for your PRs. We warmly welcome contributions to Protobuf!
