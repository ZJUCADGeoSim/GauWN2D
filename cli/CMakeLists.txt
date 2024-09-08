add_executable(gauwn-cli entrypoint.cpp ${BACKWARD_ENABLE})

add_backward(gauwn-cli)

target_compile_features(gauwn-cli PUBLIC cxx_std_23)
target_link_libraries(gauwn-cli PRIVATE GauWN CLI11::CLI11)

target_sources(gauwn-cli PUBLIC
    FILE_SET cxx_modules TYPE CXX_MODULES FILES

    main.ixx
    value.ixx
    spgrad.ixx
    veclocgrad.ixx
    cli_subcmd.ixx

)

target_sources(gauwn-cli PRIVATE
    value.cxx
    spgrad.cxx
    veclocgrad.cxx
)