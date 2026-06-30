# Installation Guide

This document describes how to compile and install Kunpeng-optimized Protocol Buffers (Protobuf).

## Environment Requirements

| Item| Version Requirement| Description|
| ------ | ---------- | ------ |
| Hardware | Kunpeng 950 processor | Supports the NEON/SVE/SVE2 instruction set. |
| OS| openEuler 22.03 LTS SP3 | Linux Distribution|
| Compiler| Clang 16.0.6 <br>GCC 12.3.1 | Supports the C++23 standard.|
| CMake | 3.15 or later| Build tool|
| Bazel | 7.0+ (optional)| Alternative build system|

## Obtaining Code

Obtain the code.

```bash
git clone --recursive -b dev_forJD_v33.0 https://gitcode.com/boostkit/protobuf.git
cd protobuf
```

## Compilation and Installation

### Baseline Compilation

The default compilation supports the generic Arm architecture and is applicable to various Arm processors.

1. Set the installation directory.

   ```bash
   export PROTOBUF_INSTALL_DIR=$HOME/.local/protobuf
   ```

2. Configure the build.

   ```bash 
   cmake -B build \
     -Dprotobuf_FORCE_FETCH_DEPENDENCIES=ON \
     -Dprotobuf_BUILD_SHARED_LIBS=ON \
     -DCMAKE_CXX_STANDARD=17 \
     -DCMAKE_BUILD_TYPE=Release \
     -DCMAKE_INSTALL_PREFIX=${PROTOBUF_INSTALL_DIR}
   ```

3. Compile.

   ```bash
   cmake --build build -j
   ```

4. Install.

   ```bash
   cmake --install build
   ```

### SVE2 Optimized Compilation (Kunpeng 950 Processor)

Enable SVE2 compilation options to fully leverage the optimized performance of Kunpeng processors.

1. Set the installation directory.

   ```bash
   export PROTOBUF_INSTALL_DIR=$HOME/.local/protobuf-opt
   ```

2. Configure the build and enable the SVE2 support for C++23 optimization.

   ```bash 
   cmake -B build \
     -Dprotobuf_FORCE_FETCH_DEPENDENCIES=ON \
     -Dprotobuf_BUILD_SHARED_LIBS=ON \
     -DCMAKE_CXX_STANDARD=23 \
     -DCMAKE_BUILD_TYPE=Release \
     -DCMAKE_CXX_FLAGS="-O3 -march=armv8.5-a+crc+sve+sve2+sve2-bitperm -mtune=native" \
     -DCMAKE_INSTALL_PREFIX=${PROTOBUF_INSTALL_DIR}
   ```

   **Advanced optimization options** (Optional, applicable to scenarios pursuing ultimate performance)

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
   ```

3. Compile.

   ```bash
   cmake --build build -j
   ```

4. Install.

   ```bash
   cmake --install build
   ```

### Compilation Option Description

| Option | Description | Default Value |
| ------ | ------ | -------- |
| `CMAKE_BUILD_TYPE` | Build type (Debug/Release/RelWithDebInfo). | Release |
| `protobuf_FORCE_FETCH_DEPENDENCIES` | Forces dependencies (such as Abseil) to be fetched from the network. Enabling this is recommended. | OFF |
| `protobuf_BUILD_SHARED_LIBS` | Builds shared libraries instead of static libraries. | OFF |
| `CMAKE_CXX_STANDARD` | C++ standard version (17 or 23 is recommended). | 17 |
| `CMAKE_CXX_FLAGS` | Compiler flags (e.g., `-march` to enable SVE2 optimization). | - |
| `CMAKE_INSTALL_PREFIX` | Installation path. | /usr/local |
| `protobuf_BUILD_TESTS` | Builds tests. | OFF |
| `protobuf_BUILD_CONFORMANCE` | Builds conformance tests. | OFF |

## Installation Verification

After the installation is complete, run the following command to verify whether the dynamic libraries are successfully installed:

```bash
ls -la ${PROTOBUF_INSTALL_DIR}/lib*/libprotobuf*
```

The installation is successful if the expected output resembles the following:

```text
lrwxrwxrwx. 1 user user      26 Mar 20 12:19 libprotobuf-lite.so -> libprotobuf-lite.so.33.0.0
-rwxr-xr-x. 1 user user 1138776 Mar 20 12:16 libprotobuf-lite.so.33.0.0
lrwxrwxrwx. 1 user user      21 Mar 20 12:19 libprotobuf.so -> libprotobuf.so.33.0.0
-rwxr-xr-x. 1 user user 4996960 Mar 20 12:16 libprotobuf.so.33.0.0
```

## Running Tests

After compilation, you can run tests to verify the build results.

1. Run all tests.

   ```bash
   ctest --test-dir build --verbose
   ```

2. Run a specific test.

   ```bash
   ctest --test-dir build -R message_test
   ```

3. Run tests in parallel.

   ```bash
   ctest --test-dir build -j
   ```
