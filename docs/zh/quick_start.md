# 快速入门

执行本文档操作前确保已安装基于鲲鹏优化的Protocol Buffers，详细安装步骤请参见《[安装指南](./installation_guide.md)》。安装成功后，主要生成产物位于安装目录中（例如`/path/to/install/pb-bin`）。

```text
/path/to/install/pb-bin/
├── bin/
│   └── protoc                    # Protocol Buffers编译器
├── include/
│   └── google/protobuf/          # C++头文件
├── lib/ 或 lib64/
│   ├── libprotobuf.so            # 完整运行时库
│   ├── libprotobuf-lite.so       # 轻量运行时库
│   ├── libprotoc.so              # 编译器库
│   └── pkgconfig/                # pkg-config配置
```

## 创建.proto数据

创建一个`.proto`文件来定义数据结构。示例文件名称`addressbook.proto`，文件内容如下。

```protobuf
syntax = "proto3";

package tutorial;

// 人员信息
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

// 地址簿
message AddressBook {
  repeated Person people = 1;
}
```

## 编译.proto文件

使用`protoc`编译器生成目标语言的代码。

### 设置环境变量

设置protoc的bin和lib路径（假设安装在`/path/to/install/pb-bin`，请根据实际路径修改）。

```bash
export PATH=/path/to/install/pb-bin/bin:$PATH
export LD_LIBRARY_PATH=/path/to/install/pb-bin/lib:$LD_LIBRARY_PATH
```

### 生成C++代码

使用如下目录生成C++代码。

```bash
protoc --cpp_out=. addressbook.proto
```

上述命令会生成以下两个文件：

- `addressbook.pb.h`：头文件，包含类声明
- `addressbook.pb.cc`：实现文件，包含类定义

### 生成其他语言代码

支持以下语言的代码生成。

- Java

  ```bash
  protoc --java_out=. addressbook.proto
  ```

- Python

  ```bash
  protoc --python_out=. addressbook.proto
  ```

- Go（需要安装protoc-gen-go插件）

  ```bash
  protoc --go_out=. addressbook.proto
  ```

- C#

  ```bash
  protoc --csharp_out=. addressbook.proto
  ```
  
## 使用protoc生成的代码

### C++编解码示例代码

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
    // 构造数据
    tutorial::AddressBook address_book;
    tutorial::Person* person = address_book.add_people();
    person->set_name("张三");
    person->set_id(1234);
    person->set_email("zhangsan@example.com");
    tutorial::Person::PhoneNumber* phone = person->add_phones();
    phone->set_number("138-1234-5678");
    phone->set_type(tutorial::Person::PHONE_TYPE_MOBILE);
    person->add_scores(1);
    person->add_scores(150);
    person->add_scores(300);

    // 序列化到文件
    std::fstream output("addressbook.bin",
                        std::ios::out | std::ios::binary);
    if (!address_book.SerializeToOstream(&output)) {
        std::cerr << "序列化失败" << std::endl;
        return -1;
    }
    output.close();

    // 读取文件内容并打印十六进制
    std::ifstream bin_in("addressbook.bin", std::ios::in | std::ios::binary);
    std::string data((std::istreambuf_iterator<char>(bin_in)),
                     std::istreambuf_iterator<char>());
    DumpHex(data);

    // 从文件反序列化
    tutorial::AddressBook parsed;
    std::fstream input("addressbook.bin",
                       std::ios::in | std::ios::binary);
    if (!parsed.ParseFromIstream(&input)) {
        std::cerr << "反序列化失败" << std::endl;
        return -1;
    }

    // 读取数据
    for (int i = 0; i < parsed.people_size(); i++) {
        const tutorial::Person& p = parsed.people(i);
        std::cout << "姓名: " << p.name() << std::endl;
        std::cout << "ID: " << p.id() << std::endl;
        std::cout << "邮箱: " << p.email() << std::endl;
        for (int j = 0; j < p.phones_size(); j++) {
            const tutorial::Person::PhoneNumber& pn = p.phones(j);
            std::cout << "电话: " << pn.number()
                      << " (类型: " << pn.type() << ")" << std::endl;
        }
    }

    return 0;
}
```

### 编译和运行

1. 编译（假设安装在/path/to/install/pb-bin目录）。

   ```bash
   export PKG_CONFIG_PATH=/path/to/install/pb-bin/lib64/pkgconfig:$PKG_CONFIG_PATH
   g++ -std=c++20 example.cc addressbook.pb.cc \
       -I/path/to/install/pb-bin/include \
       $(pkg-config --libs protobuf) \
       -o example
   ```

2. 运行。

   ```bash
   ./example
   ```
