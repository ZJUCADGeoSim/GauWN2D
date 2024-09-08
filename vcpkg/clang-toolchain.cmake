find_program(
    CLANG_EXECUTABLE
    NAMES clang clang-16 clang-17 clang-18 clang-19)

find_program(
    CLANGXX_EXECUTABLE
    NAMES clang++ clang++-16 clang++-17 clang++-18 clang++-19)

find_program(
    LLD_EXECUTABLE
    NAMES lld lld-16 lld-17 lld-18)

if(NOT CLANG_EXECUTABLE OR NOT CLANGXX_EXECUTABLE OR NOT LLD_EXECUTABLE)
    message(FATAL_ERROR "Clang and LLD not found")
endif()

set(CMAKE_C_COMPILER ${CLANG_EXECUTABLE})
set(CMAKE_CXX_COMPILER ${CLANGXX_EXECUTABLE})
set(CMAKE_CXX_FLAGS "-stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "-lc++abi -fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS_INIT "-lc++abi -fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS_INIT "-lc++abi -fuse-ld=lld")
message("clang toolchain CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
message("clang toolchain CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")

set(VCPKG_ROOT_DIR ${Z_VCPKG_ROOT_DIR})

if(NOT DEFINED VCPKG_ROOT_DIR)
    set(VCPKG_ROOT_DIR ${_VCPKG_ROOT_DIR})
endif()

if(NOT DEFINED VCPKG_ROOT_DIR)
    set(VCPKG_ROOT_DIR $ENV{VCPKG_ROOT})
endif()

if(NOT DEFINED VCPKG_ROOT_DIR)
    message(FATAL_ERROR "VCPKG_ROOT_DIR not defined, please set env VCPKG_ROOT")
endif()

include(${VCPKG_ROOT_DIR}/scripts/toolchains/linux.cmake)
