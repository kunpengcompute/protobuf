# Installation Guide

This document describes how to compile and install optimized Kunpeng Protocol Buffers (Protobuf).

## Environment Requirements

| Software| Version Requirement| Description|
|------|----------|------|
| OS| openEuler 22.03 LTS SP3 | Linux Distribution|
| Compiler| clang 16.0.6+<br>GCC 9.0+ | Supports the C++17 standard.|
| CMake | 3.15 or later| Build tool|
| Git | 2.0 or later| Version control|
| Bazel | 7.0+ (optional)| Alternative build system|

## Obtaining Code

Obtain the code.

```bash
git clone --recursive -b dev_forBD_v4.25.8_SVE2 https://gitcode.com/boostkit/protobuf.git
cd protobuf
```

If the code was not fetched using `git clone --recursive` to pull all dependencies, you need to download the Abseil C++ library to the specified directory and ensure that the `third_party/abseil-cpp` directory contains all the source files of Abseil.

- Method 1: Using Git Submodules (Recommended)

  ```bash
  cd protobuf
  git submodule update --init --recursive
  ```

- Method 2: Manual Download

  ```bash
  cd protobuf
  mkdir -p third_party/abseil-cpp
  cd third_party/abseil-cpp
  wget https://github.com/abseil/abseil-cpp/archive/refs/tags/20240722.1.tar.gz
  tar -xzf 20240722.1.tar.gz --strip-components=1
  ```

## Compilation and Installation

1. Create a user-defined installation directory. You can change the directory as required.

   ```bash
   mkdir -p /path/to/install/pb-bin
   export PROTOBUF_INSTALL_DIR=/path/to/install/pb-bin
   ```

2. Configure the build.

   ```bash
   mkdir build
   cd build

   # Basic configuration (generating a shared library)
   cmake -Dprotobuf_BUILD_SHARED_LIBS=ON \
         -DCMAKE_CXX_STANDARD=17 \
         -DCMAKE_BUILD_TYPE=Release \
         -DCMAKE_CXX_FLAGS="-O3" \
         -DCMAKE_INSTALL_PREFIX=${PROTOBUF_INSTALL_DIR} \
         ..
   ```

3. Perform the compilation.

   - Use all available CPU cores for parallel compilation.

    ```bash 
    cmake --build . --parallel $(nproc)
    ```

   - Or specify the number of cores (for example, 32 cores).

       ```bash 
      cmake --build . --parallel 32
      ```

4. Perform the installation.

   ```bash
   cmake --install .
   ```

5. Verify the installation directory.

    ```bash
    cmake --install .
    ```

    Command output:

    ```text
    total 28
    drwxr-xr-x.  5 user user  4096 Mar 20 12:19 .
    drwxr-xr-x. 33 user user  4096 Mar 20 16:39 ..
    drwxr-xr-x.  2 user user  4096 Mar 20 12:19 bin
    drwxr-xr-x.  5 user user  4096 Mar 20 12:19 include
    drwxr-xr-x.  4 user user 12288 Mar 20 12:19 lib64
    ```

6. View the dynamic library.

   ```bash
   ls -la ${PROTOBUF_INSTALL_DIR}/lib*/libprotobuf*
   ```

   Command output:

   ```text
   lrwxrwxrwx. 1 user user      26 Mar 20 12:19 /home/user/pb/pb-bin/lib64/libprotobuf-lite.so -> libprotobuf-lite.so.25.8.0
   -rwxr-xr-x. 1 user user 1138776 Mar 20 12:16 /home/user/pb/pb-bin/lib64/libprotobuf-lite.so.25.8.0
   lrwxrwxrwx. 1 user user      21 Mar 20 12:19 /home/user/pb/pb-bin/lib64/libprotobuf.so -> libprotobuf.so.25.8.0
   -rwxr-xr-x. 1 user user 4996960 Mar 20 12:16 /home/user/pb/pb-bin/lib64/libprotobuf.so.25.8.0
   ```

## Compilation Option Description

### Common CMake Options

| Option| Default Value| Description|
|------|--------|------|
| `-Dprotobuf_BUILD_TESTS` | OFF | Whether to build for testing|
| `-Dprotobuf_BUILD_SHARED_LIBS` | OFF | Whether to build a shared library (**ON** is recommended)|
| `-DCMAKE_CXX_STANDARD` | 17 | C++ standard (**17** is recommended)|
| `-DCMAKE_BUILD_TYPE` | Release | Build type (Release/Debug)|
| `-DCMAKE_CXX_FLAGS` | - | Additional compilation flag (**-O3** is recommended)|
| `-Dprotobuf_BUILD_CONFORMANCE` | OFF | Whether to build a consistency test|

### Performance Optimization Compilation Flag

Recommended compilation flags for the Arm architecture (Kunpeng processor)

```bash
cmake -DCMAKE_CXX_FLAGS="-O3 -march=armv9.2-a+crc+sve+sve2+sve2-bitperm -mtune=native" ..

# (Optional) Advanced optimization example
cmake -DCMAKE_CXX_FLAGS="-O3 -march=armv9.2-a+crc+sve+sve2+sve2-bitperm -flto=thin -fno-plt -fstrict-vtable-pointers" \
      -DCMAKE_EXE_LINKER_FLAGS="-fuse-ld=lld -Wl,-mllvm,-inline-threshold=1350" \
      -DCMAKE_SHARED_LINKER_FLAGS="-fuse-ld=lld -Wl,-mllvm,-inline-threshold=1350" \
      ..
```

## Running a Test

Go to the test directory.

```bash
cd build
```

Select any of the following methods:

- Run all tests.

  ```bash
  ctest --verbose
  ```

- Run a specific test.

  ```bash
  ctest -R message_test
  ```

- Run tests in parallel.

  ```bash
  ctest --parallel $(nproc)
  ```
