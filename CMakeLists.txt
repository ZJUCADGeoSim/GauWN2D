cmake_minimum_required(VERSION 3.28)

project(GauWN)

option(GAUWN_OPTIMIZE_FOR_NATIVE "Build with -march=native if possible" ON)

include(cmake/settings.cmake)

if(EXISTS "${PROJECT_SOURCE_DIR}/local_config.cmake")
    include(local_config.cmake)
endif()


find_package(OpenMP REQUIRED)
find_package(Eigen3 3.4 CONFIG REQUIRED)
find_package(absl  CONFIG REQUIRED)
find_package(fmt 10.2 CONFIG REQUIRED)
find_package(CLI11 CONFIG REQUIRED)


add_subdirectory(src)
add_subdirectory(cli)

