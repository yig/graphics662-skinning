cmake_minimum_required(VERSION 3.14...3.30)
project(
    bbw
    LANGUAGES CXX C
)

## Set a global C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


## Load packages
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

FetchContent_Declare(
    Eigen3
    # URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip
    # URL_HASH SHA256=1ccaabbfe870f60af3d6a519c53e09f3dcf630207321dffa553564a8e75c4fc8
    GIT_REPOSITORY https://gitlab.com/libeigen/eigen
    # GIT_TAG 3.4.0
    ## We want a newer tag to avoid Eigen's testing polluting our testing.
    GIT_TAG 99ffad1971fb03de65f36e5401add8f872550e5f
    ## We can't use `GIT_SHALLOW TRUE` with an unnamed tag.
    GIT_SHALLOW FALSE
    GIT_PROGRESS TRUE
    OVERRIDE_FIND_PACKAGE
    )
FetchContent_MakeAvailable( Eigen3 )

FetchContent_Declare(
    igl
    GIT_REPOSITORY https://github.com/libigl/libigl
    GIT_TAG v2.5.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    )
FetchContent_MakeAvailable( igl )

FetchContent_Declare(
    osqp
    # URL https://github.com/osqp/osqp/archive/refs/tags/v0.6.3.zip
    # URL_HASH SHA256=c554f3458164d6a7a5c3d2f1714f32ead3473b7e06d33ab4a93063639ccf8f93
    GIT_REPOSITORY https://github.com/osqp/osqp
    ## GIT_TAG v0.6.3
    ## GIT_TAG 25b6b39247954b74bba17667863fe19e9a8b1ade
    GIT_TAG c2c96983bdd7ef21f7982d8777274e3a13a5e4a4
    GIT_SHALLOW FALSE
    GIT_PROGRESS TRUE
    OVERRIDE_FIND_PACKAGE
    )
set(OSQP_BUILD_SHARED_LIB OFF)
set(OSQP_BUILD_DEMO_EXE OFF)
set(OSQP_ENABLE_INTERRUPT OFF)
FetchContent_MakeAvailable( osqp )
add_library(osqp::osqp ALIAS osqpstatic)

# FetchContent_Declare(
#     nasoq
#     GIT_REPOSITORY https://github.com/sympiler/nasoq
#     GIT_TAG fc2051dfa991160cd6dd326d0fb1580ffb77b93b
#     GIT_SHALLOW FALSE
#     GIT_PROGRESS TRUE
#     )
# set(NASOQ_USE_BLAS_MKL OFF)
# set(NASOQ_USE_OPENMP OFF)
# set(NASOQ_USE_BLAS_OpenBLAS OFF)
# set(NASOQ_USE_CLAPACK OFF)
# FetchContent_MakeAvailable( nasoq )


## Declare the library with code used by every front end
add_library(bbwlib OBJECT
    src/bbw.cpp
    )
target_include_directories( bbwlib PUBLIC src )
target_link_libraries( bbwlib
    PUBLIC
        Eigen3::Eigen
    PRIVATE
        osqp::osqp
        # nasoq::nasoqeigen
        igl::core
)

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
    ## Declare the Web Assembly wrapper
    add_executable( bbwbridge src/bbwbridge.cpp )
    target_link_libraries( bbwbridge
        PRIVATE
            bbwlib
            embind
    )
    
    ## Copy the executable to the `serve` directory
    set_target_properties( bbwbridge PROPERTIES LINK_FLAGS "-s SINGLE_FILE=1 -s ALLOW_MEMORY_GROWTH=1" )
    add_custom_command( TARGET bbwbridge POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:bbwbridge> ${CMAKE_SOURCE_DIR}/serve/
    )
    ## Without the `-s SINGLE_FILE=1` flag above, we would generate both a `.js` and `.wasm` file. We could copy both with:
#     add_custom_command( TARGET bbwbridge POST_BUILD
#       COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:bbwbridge>/$<TARGET_FILE_BASE_NAME:bbwbridge>.js $<TARGET_FILE_DIR:bbwbridge>/$<TARGET_FILE_BASE_NAME:bbwbridge>.wasm ${CMAKE_SOURCE_DIR}/serve/
#     )
else()
    
    ## Add tests
    # enable_testing()
    # add_subdirectory( test )

endif()
