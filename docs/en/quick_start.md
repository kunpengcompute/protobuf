# Quick Start

Before performing operations, ensure that the compilation and installation process described in [Installation Guide](installation_guide.md) has been completed.
After the installation is successful, the generated files are stored in the installation directory, for example, `/path/to/install/pb-bin`.

```text
/path/to/install/pb-bin/
├── bin/
│   └── protoc                    # Protocol Buffers compiler
├── include/
│   └── google/protobuf/          # C++ header file
├── lib/ or lib64/
│   ├── libprotobuf.so            # Complete runtime library
│   ├── libprotobuf-lite.so       # Lightweight runtime library
│   ├── libprotoc.so              # Compiler library
│   └── pkgconfig/                # pkg-config
```

## Creating .proto Data

Create a `.proto` file to define your data structure. For example, the file name is `addressbook.proto`, and the file content is as follows:

```protobuf
syntax = "proto3";

package tutorial;

// Personnel information
message Person {
  string name = 1;
  int32 id = 2;
  string email = 3;
  repeated uint32 scores = 5 [packed = true];

  enum PhoneType {
    PHONE_TYPE_UNSPECIFIED = 0;
    PHONE_TYPE_MOBILE = 1;
    PHONE_TYPE_HOME = 2;
    PHONE_TYPE_WORK = 3;
  }

  message PhoneNumber {
    string number = 1;
    PhoneType type = 2;
  }

  repeated PhoneNumber phones = 4;
}

// Address book
message AddressBook {
  repeated Person people = 1;
}
```

## Compiling the .proto File

Use the `protoc` compiler to generate code in the target language.

Set the environment variables as follows:

```bash
# Set the path for protoc (assuming it is installed in /path/to/install/pb-bin).
export PATH=/path/to/install/pb-bin/bin:$PATH
# Modify lib or lib64 according to the actual path.
export LD_LIBRARY_PATH=/path/to/install/pb-bin/lib:$LD_LIBRARY_PATH
```

### Generating C++ Code

```bash
protoc --cpp_out=. addressbook.proto
```

The preceding command generates the following two files:

- `addressbook.pb.h`: header file, containing class declarations
- `addressbook.pb.cc`: implementation file, containing class definitions

### Generating Code in Other Languages

- Java

  ```bash
  protoc --java_out=. addressbook.proto
  ```

- Python

  ```bash
  protoc --python_out=. addressbook.proto
  ```

- Go (The protoc-gen-go plugin must be installed.)

  ```bash
  protoc --go_out=. addressbook.proto
  ```

- C#

  ```bash
  protoc --csharp_out=. addressbook.proto
  ```
  
## Code Generated Using protoc

### C++ Encoding and Decoding Sample Code

```cpp
#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include "addressbook.pb.h"

static void DumpHex(const std::string& data) {
    std::cout << "addressbook.bin (hex):" << std::endl;
    for (size_t i = 0; i < data.size(); ++i) {
        if (i % 16 == 0) {
            std::cout << std::setw(4) << std::setfill('0') << std::hex << i << ": ";
        }
        std::cout << std::setw(2) << std::setfill('0')
                  << static_cast<int>(static_cast<unsigned char>(data[i])) << " ";
        if (i % 16 == 15 || i + 1 == data.size()) {
            std::cout << std::endl;
        }
    }
    std::cout << std::dec;
}

int main() {
    // Construct data.
    tutorial::AddressBook address_book;
    tutorial::Person* person = address_book.add_people();
    person->set_name("Jack");
    person->set_id(1234);
    person->set_email("zhangsan@example.com");
    tutorial::Person::PhoneNumber* phone = person->add_phones();
    phone->set_number("138-1234-5678");
    phone->set_type(tutorial::Person::PHONE_TYPE_MOBILE);
    person->add_scores(1);
    person->add_scores(150);
    person->add_scores(300);

    // Serialize to a file.
    std::fstream output("addressbook.bin",
                        std::ios::out | std::ios::binary);
    if (!address_book.SerializeToOstream(&output)) {
        std::cerr << "Serialization failed" << std::endl;
        return -1;
    }
    output.close();

    // Read the file content and print it in hexadecimal format.
    std::ifstream bin_in("addressbook.bin", std::ios::in | std::ios::binary);
    std::string data((std::istreambuf_iterator<char>(bin_in)),
                     std::istreambuf_iterator<char>());
    DumpHex(data);

    // Deserialize from the file.
    tutorial::AddressBook parsed;
    std::fstream input("addressbook.bin",
                       std::ios::in | std::ios::binary);
    if (!parsed.ParseFromIstream(&input)) {
        std::cerr << "Deserialization failed" << std::endl;
        return -1;
    }

    // Read data.
    for (int i = 0; i < parsed.people_size(); i++) {
        const tutorial::Person& p = parsed.people(i);
        std::cout << "Name: " << p.name() << std::endl;
        std::cout << "ID: " << p.id() << std::endl;
        std::cout << "Email: " << p.email() << std::endl;
        for (int j = 0; j < p.phones_size(); j++) {
            const tutorial::Person::PhoneNumber& pn = p.phones(j);
            std::cout << "Phone_number: " << pn.number()
                      << " (Type: " << pn.type() << ")" << std::endl;
        }
    }

    return 0;
}
```

### Compiling and Running

1. Compile the code (assuming it is installed in /path/to/install/pb-bin).

   ```bash
   export PKG_CONFIG_PATH=/path/to/install/pb-bin/lib64/pkgconfig:$PKG_CONFIG_PATH
   g++ -std=c++17 example.cc addressbook.pb.cc \
       -I/path/to/install/pb-bin/include \
       $(pkg-config --libs protobuf) \
       -o example
   ```

2. Run the code.

   ```bash
   ./example
   ```

## Common Operations

### JSON Conversion

Protobuf supports conversion to and from JSON.

```cpp
#include <google/protobuf/util/json_util.h>

// Convert Protobuf to JSON.
std::string json_string;
google::protobuf::util::MessageToJsonString(address_book, &json_string);
std::cout << json_string << std::endl;

// Convert JSON to Protobuf.
tutorial::AddressBook address_book2;
google::protobuf::util::JsonStringToMessage(json_string, &address_book2);
```

### Using the Arena Memory Pool

Arena can improve performance and reduce memory fragments. The usage is as follows:

```cpp
// All objects are automatically released when the arena memory pool is destroyed.
#include <google/protobuf/arena.h>

google::protobuf::Arena arena;

// Create a message in the arena memory pool.
tutorial::Person* person =
    google::protobuf::Arena::CreateMessage<tutorial::Person>(&arena);
person->set_name("Jerry");
person->set_id(5678);
```
