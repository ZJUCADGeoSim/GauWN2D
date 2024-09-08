add_library(GauWN)

target_link_libraries(GauWN PUBLIC
    Eigen3::Eigen fmt::fmt
    absl::inlined_vector
    absl::flat_hash_map
    absl::flat_hash_set
    OpenMP::OpenMP_CXX
    igl::igl_core)
target_compile_features(GauWN PUBLIC cxx_std_23)

add_subdirectory(piecewise2d)
add_subdirectory(numerics)
add_subdirectory(io)
add_subdirectory(meshwise2d)
add_subdirectory(utils)
add_subdirectory(bvh2d)
