target_sources(GauWN PUBLIC
    FILE_SET cxx_modules_numerics TYPE CXX_MODULES FILES

    gaussian.ixx
    gaussian_integrals.ixx
)

target_sources(GauWN PUBLIC 
    gaussian.cxx 
)

