# 安装指南

本文档提供基于鲲鹏优化的Protocol Buffers的详细编译安装步骤。

## 环境要求

| 软件 | 版本要求 | 说明 |
| ------ | ---------- | ------ |
| 硬件配置 | 鲲鹏950处理器 | 支持NEON/SVE/SVE2指令集 |
| 操作系统 | openEuler 22.03 LTS SP3 | Linux发行版 |
| 编译器 | clang 16.0.6 <br>GCC 12.3.1 | 支持 C++23 标准 |
| CMake | 3.15或更高版本 | 构建工具 |
| Bazel | 7.0+（可选） | 替代构建系统 |

## 获取代码

运行以下命令获取代码。

```bash
git clone --recursive -b dev_forJD_v33.0 https://gitcode.com/boostkit/protobuf.git
cd protobuf
```

## 编译安装

### 基础编译（基线版本）

默认编译会支持通用的 ARM 架构，适用于各种 ARM 处理器：

```bash
# 设置安装目录
export PROTOBUF_INSTALL_DIR=$HOME/.local/protobuf

# 配置构建
cmake -B build \
  -Dprotobuf_FORCE_FETCH_DEPENDENCIES=ON \
  -Dprotobuf_BUILD_SHARED_LIBS=ON \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=${PROTOBUF_INSTALL_DIR}

# 编译
cmake --build build -j

# 安装
cmake --install build
```

### SVE2 优化编译（鲲鹏 950）

启用 SVE2 编译选项以充分发挥鲲鹏处理器的优化性能：

```bash
# 设置安装目录
export PROTOBUF_INSTALL_DIR=$HOME/.local/protobuf-opt

# 配置构建并启用 SVE2 支持，开启C++23优化
cmake -B build \
  -Dprotobuf_FORCE_FETCH_DEPENDENCIES=ON \
  -Dprotobuf_BUILD_SHARED_LIBS=ON \
  -DCMAKE_CXX_STANDARD=23 \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-O3 -march=armv8.5-a+crc+sve+sve2+sve2-bitperm -mtune=native" \
  -DCMAKE_INSTALL_PREFIX=${PROTOBUF_INSTALL_DIR}

# 编译
cmake --build build -j

# 安装
cmake --install build
```

**进阶优化选项**（可选，适用于追求极致性能的场景）：

```bash
cmake -B build \
  -Dprotobuf_FORCE_FETCH_DEPENDENCIES=ON \
  -Dprotobuf_BUILD_SHARED_LIBS=ON \
  -DCMAKE_CXX_STANDARD=23 \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS="-O3 -march=armv8.5-a+crc+sve+sve2+sve2-bitperm -flto=thin -fno-plt -fstrict-vtable-pointers" \
  -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld -Wl,-mllvm,-inline-threshold=1350" \
  -DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=lld -Wl,-mllvm,-inline-threshold=1350" \
  -DCMAKE_INSTALL_PREFIX=${PROTOBUF_INSTALL_DIR}

cmake --build build -j
cmake --install build
```

### 验证安装

安装完成后，通过以下命令查看动态库是否安装：

```bash
ls -la ${PROTOBUF_INSTALL_DIR}/lib*/libprotobuf*
```

预期输出：

```text
lrwxrwxrwx. 1 user user      26 Mar 20 12:19 libprotobuf-lite.so -> libprotobuf-lite.so.33.0.0
-rwxr-xr-x. 1 user user 1138776 Mar 20 12:16 libprotobuf-lite.so.33.0.0
lrwxrwxrwx. 1 user user      21 Mar 20 12:19 libprotobuf.so -> libprotobuf.so.33.0.0
-rwxr-xr-x. 1 user user 4996960 Mar 20 12:16 libprotobuf.so.33.0.0
```

## 编译选项说明

| 选项 | 说明 | 默认值 |
| ------ | ------ | -------- |
| `CMAKE_BUILD_TYPE` | 构建类型（Debug/Release/RelWithDebInfo） | Release |
| `protobuf_FORCE_FETCH_DEPENDENCIES` | 强制从网络获取依赖项（Abseil等），推荐开启 | OFF |
| `protobuf_BUILD_SHARED_LIBS` | 构建共享库而非静态库 | OFF |
| `CMAKE_CXX_STANDARD` | C++ 标准版本（推荐17或23） | 17 |
| `CMAKE_CXX_FLAGS` | 编译器标志（-march 启用 SVE2 优化） | - |
| `CMAKE_INSTALL_PREFIX` | 安装路径 | /usr/local |
| `protobuf_BUILD_TESTS` | 编译测试 | OFF |
| `protobuf_BUILD_CONFORMANCE` | 构建一致性测试 | OFF |

## 运行测试

编译完成后，可以运行测试验证构建结果：

```bash
# 运行所有测试
ctest --test-dir build --verbose

# 运行特定测试
ctest --test-dir build -R message_test

# 并行运行测试
ctest --test-dir build -j
```
