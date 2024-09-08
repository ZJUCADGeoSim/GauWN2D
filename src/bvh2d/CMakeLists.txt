target_sources(GauWN PUBLIC
    FILE_SET cxx_modules_bvh2d TYPE CXX_MODULES FILES

    aabb.ixx
    approx_polygon.ixx
)

target_sources(GauWN PUBLIC approx_polygon.cxx)