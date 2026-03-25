# 安装指南

本文档提供 Protocol Buffers 优化版本的详细编译安装步骤。

## 环境要求

| 软件 | 版本要求 | 说明 |
|------|----------|------|
| 操作系统 | openEuler 22.03 LTS SP3 | Linux 发行版 |
| 编译器 | clang 16.0.6+<br>GCC 9.0+ | 支持 C++17 标准 |
| CMake | 3.15 或更高版本 | 构建工具 |
| Git | 2.0 或更高版本 | 版本控制 |
| Bazel | 7.0+（可选） | 替代构建系统 |

## 获取代码

运行以下命令获取代码。

```bash
git clone --recursive -b dev_forBD_v4.25.8_SVE2 https://gitcode.com/boostkit/protobuf.git
cd protobuf
```

>说明：如果代码获取时未使用 `git clone --recursive` 拉取依赖，则需要下载Abseil C++库到指定目录，确保 `third_party/abseil-cpp` 目录包含 Abseil 的所有源文件。
>
>- 方法一：使用git子模块（推荐）
>```bash
>cd protobuf
>git submodule update --init --recursive
>```
>- 方法二：手动下载
>```bash
>cd protobuf
>mkdir -p third_party/abseil-cpp
>cd third_party/abseil-cpp
>wget https://github.com/abseil/abseil-cpp/archive/refs/tags/20240722.1.tar.gz
>tar -xzf 20240722.1.tar.gz --strip-components=1
>```



## 编译安装

1. 创建自定义安装目录（可根据实际需求修改路径）。

   ```bash
   mkdir -p /path/to/install/pb-bin
   export PROTOBUF_INSTALL_DIR=/path/to/install/pb-bin
   ```

2. 配置构建。

   ```bash
   mkdir build
   cd build

   # 基础配置（生成共享库）
   cmake -Dprotobuf_BUILD_SHARED_LIBS=ON \
         -DCMAKE_CXX_STANDARD=17 \
         -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_CXX_FLAGS="-O3" \
         -DCMAKE_INSTALL_PREFIX=${PROTOBUF_INSTALL_DIR} \
         ..
   ```

3. 编译。

  - 使用所有可用 CPU 核心并行编译
    ```bash 
    cmake --build . --parallel $(nproc)
    ```
   - 或指定核心数（例如 32 核）
       ```bash 
      cmake --build . --parallel 32
      ```

4. 安装。

   ```bash
   cmake --install .
   ```

5.  验证安装目录。

    ```bash
    cmake --install .
    ```

    回显结果如下：

    ```text
    total 28
    drwxr-xr-x.  5 user user  4096 Mar 20 12:19 .
    drwxr-xr-x. 33 user user  4096 Mar 20 16:39 ..
    drwxr-xr-x.  2 user user  4096 Mar 20 12:19 bin
    drwxr-xr-x.  5 user user  4096 Mar 20 12:19 include
    drwxr-xr-x.  4 user user 12288 Mar 20 12:19 lib64
    ```
6. 查看动态库。

   ```bash
   ls -la ${PROTOBUF_INSTALL_DIR}/lib*/libprotobuf*
   ```

   回显结果如下：

   ```text
   lrwxrwxrwx. 1 user user      26 Mar 20 12:19 /home/user/pb/pb-bin/lib64/libprotobuf-lite.so -> libprotobuf-lite.so.25.8.0
   -rwxr-xr-x. 1 user user 1138776 Mar 20 12:16 /home/user/pb/pb-bin/lib64/libprotobuf-lite.so.25.8.0
   lrwxrwxrwx. 1 user user      21 Mar 20 12:19 /home/user/pb/pb-bin/lib64/libprotobuf.so -> libprotobuf.so.25.8.0
   -rwxr-xr-x. 1 user user 4996960 Mar 20 12:16 /home/user/pb/pb-bin/lib64/libprotobuf.so.25.8.0
   ```

## 编译选项说明

### CMake 常用选项

| 选项 | 默认值 | 说明 |
|------|--------|------|
| `-Dprotobuf_BUILD_TESTS` | OFF | 是否编译测试 |
| `-Dprotobuf_BUILD_SHARED_LIBS` | OFF | 是否构建共享库（推荐 ON） |
| `-DCMAKE_CXX_STANDARD` | 17 | C++ 标准（推荐 17） |
| `-DCMAKE_BUILD_TYPE` | Release | 构建类型（Release/Debug） |
| `-DCMAKE_CXX_FLAGS` | - | 额外编译标志（推荐 -O3） |
| `-Dprotobuf_BUILD_CONFORMANCE` | OFF | 是否构建一致性测试 |

### 性能优化编译标志

针对 ARM 架构（鲲鹏处理器）的推荐编译标志。

```bash
cmake -DCMAKE_CXX_FLAGS="-O3 -march=armv9.2-a+crc+sve+sve2+sve2-bitperm -mtune=native" ..

# 进阶优化示例（可选）
cmake -DCMAKE_CXX_FLAGS="-O3 -march=armv9.2-a+crc+sve+sve2+sve2-bitperm -flto=thin -fno-plt -fstrict-vtable-pointers" \
      -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld -Wl,-mllvm,-inline-threshold=1350" \
      -DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=lld -Wl,-mllvm,-inline-threshold=1350" \
      ..
```


## 运行测试

进入测试目录。

```
cd build
```
以下三种方式任选其一执行即可。
- 运行所有测试

  ```
  ctest --verbose
  ```

- 运行特定测试

  ```
  ctest -R message_test
  ```

- 并行运行测试

  ```
  ctest --parallel $(nproc)
  ```
