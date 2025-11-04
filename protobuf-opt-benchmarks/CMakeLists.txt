cmake_minimum_required(VERSION 3.10...3.24)
project(ProtobufMatBenchmark)

# 获取PB环境变量
if(NOT DEFINED ENV{PBLIB_HOME})
	message(FATAL_ERROR "Environment variable PBLIB_HOME is not set")
endif()

# 生成PB序列化头文件
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/arr_data32.pb.h")
    message(STATUS "arr_data32.pb.h not found. Generating using protoc.")
    execute_process(
        COMMAND $ENV{PBLIB_HOME}/bin/protoc --cpp_out=. ./arr_data32.proto
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error
    )
	if(NOT result EQUAL 0)
        message(FATAL_ERROR "protoc command failed with error: ${error}")
    else()
        message(STATUS "protoc command output: ${output}")
    endif()
else()
    message(STATUS "arr_data32.pb.h already exists.")
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/arr_data64.pb.h")
    message(STATUS "arr_data64.pb.h not found. Generating using protoc.")
    execute_process(
        COMMAND $ENV{PBLIB_HOME}/bin/protoc --cpp_out=. ./arr_data64.proto
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error
    )
	if(NOT result EQUAL 0)
        message(FATAL_ERROR "protoc command failed with error: ${error}")
    else()
        message(STATUS "protoc command output: ${output}")
    endif()
else()
    message(STATUS "arr_data64.pb.h already exists.")
endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/test.pb.h")
    message(STATUS "test.pb.h not found. Generating using protoc.")
    # 执行protoc命令生成test.pb.h
    execute_process(
        COMMAND $ENV{PBLIB_HOME}/bin/protoc --cpp_out=. ./test.proto
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        RESULT_VARIABLE result
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error
    )
        if(NOT result EQUAL 0)
        message(FATAL_ERROR "protoc command failed with error: ${error}")
    else()
        message(STATUS "protoc command output: ${output}")
    endif()
else()
    message(STATUS "test.pb.h already exists.")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-L$ENV{PBLIB_HOME}/lib64")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -march=native")

include_directories("$ENV{PBLIB_HOME}/include")

add_executable(bm benchmark.cpp arr_data32.pb.cc arr_data64.pb.cc test.pb.cc)
target_link_libraries(bm benchmark pthread protobuf absl_log_internal_check_op absl_log_internal_message)
