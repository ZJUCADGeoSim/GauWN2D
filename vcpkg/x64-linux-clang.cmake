set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE static)
set(VCPKG_CMAKE_SYSTEM_NAME Linux)
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/clang-toolchain.cmake)
set(VCPKG_DISABLE_COMPILER_TRACKING ON)

message("clang triplet CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
message("clang triplet CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")