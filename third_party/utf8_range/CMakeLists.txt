cmake_minimum_required (VERSION 3.5)
project (utf8_range C CXX)

# option() honor variables
if (POLICY CMP0077)
  cmake_policy(SET CMP0077 NEW)
endif (POLICY CMP0077)

option (utf8_range_ENABLE_TESTS "Build test suite" ON)
option (utf8_range_ENABLE_INSTALL "Configure installation" ON)

set_source_files_properties(simdutf-wrapper.c PROPERTIES LANGUAGE CXX)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

##
# Create the lightweight C library
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
  add_library (utf8_range STATIC range-neon.c naive.c range2-neon.c range2-sse.c simdutf-wrapper.c simdutf.cpp)
else()
  add_library (utf8_range STATIC naive.c range2-neon.c range2-sse.c)
endif()

##
# A heavier-weight C++ wrapper that supports Abseil.
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
  add_library (utf8_validity STATIC utf8_validity.cc range-neon.c naive.c simdutf-wrapper.c simdutf.cpp)
else()
  add_library (utf8_validity STATIC utf8_validity.cc)
endif()

# Load Abseil dependency.
if (NOT TARGET absl::strings)
  if (NOT ABSL_ROOT_DIR)
    find_package(absl REQUIRED CONFIG)
  else ()
    set(ABSL_ENABLE_INSTALL ${utf8_range_ENABLE_INSTALL})
    set(ABSL_PROPAGATE_CXX_STD ON)
    add_subdirectory(${ABSL_ROOT_DIR} third_party/abseil-cpp)
  endif ()
endif ()
target_link_libraries(utf8_validity PUBLIC absl::strings)

# Configure tests.
if (utf8_range_ENABLE_TESTS)
  enable_testing()

  find_package(GTest REQUIRED)

  add_executable(tests utf8_validity_test.cc)
  target_link_libraries(tests utf8_validity GTest::gmock_main)

  add_test(NAME utf8_validity_test COMMAND tests)

  add_custom_target(check
    COMMAND tests
    DEPENDS tests
  )
endif ()

# Configure installation.
if (utf8_range_ENABLE_INSTALL)
  include(CMakePackageConfigHelpers)
  include(GNUInstallDirs)

  install(EXPORT ${PROJECT_NAME}-targets
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    NAMESPACE utf8_range3::
  )
  install(TARGETS utf8_validity utf8_range EXPORT ${PROJECT_NAME}-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

  configure_package_config_file(
    cmake/${PROJECT_NAME}-config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )
  install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
  )
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/utf8_range.pc.cmake
                 ${CMAKE_CURRENT_BINARY_DIR}/utf8_range.pc @ONLY)
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/utf8_range.pc
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

  # Install public headers explicitly.
  install(FILES utf8_range.h utf8_validity.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )
endif ()
