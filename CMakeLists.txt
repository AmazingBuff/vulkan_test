cmake_minimum_required(VERSION 3.25)

project(
    vulkan_test
    LANGUAGES CXX
)

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Eigen3 REQUIRED)

#include(FetchContent)
#
#FetchContent_Declare(
#    yaml-cpp
#    GIT_REPOSITORY https://bgithub.xyz/jbeder/yaml-cpp.git
#    GIT_TAG da82fd982c260e7f335ce5acbceff24b270544d1
#)
#FetchContent_MakeAvailable(yaml-cpp)
#
#FetchContent_Declare(
#    fmt
#    GIT_REPOSITORY https://bgithub.xyz/fmtlib/fmt
#    GIT_TAG 527e98e3f873562b640377ad8f3c6088ac34e01d
#)
#FetchContent_MakeAvailable(fmt)

file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

set(THIRD_PARTY_INCLUDE_DIRS 
    ext/traits/include
    ext/reflect-cpp/include
    ext/yaml-cpp/include
    ext/fmt/include
    ext/spirv_cross/include
    ext/assimp/include
)

set(THIRD_PARTY_LIB_DIRS
    ext/reflect-cpp/lib/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
    ext/yaml-cpp/lib/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
    ext/fmt/lib/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
    ext/spirv_cross/lib/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
    ext/assimp/lib/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
)

set(THIRD_PARTY_LIBS
    $<$<CONFIG:Debug>:
    reflectcppd
    yaml-cppd
    fmtd
    spirv-cross-cored
    spirv-cross-glsld
    assimpd
    >
    $<$<CONFIG:Release>:
    reflectcpp
    yaml-cpp
    fmt
    spirv-cross-core
    spirv-cross-glsl
    assimp
    >
)

add_executable(
    ${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${Vulkan_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${THIRD_PARTY_INCLUDE_DIRS}
    src/
)

target_precompile_headers(
    ${PROJECT_NAME}
    PRIVATE
    src/pch.h
)

target_link_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${SDL2_LIBDIR}
    ${THIRD_PARTY_LIB_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ${Vulkan_LIBRARIES}
    ${SDL2_LIBRARIES}
    Eigen3::Eigen
    ${THIRD_PARTY_LIBS} 
)

target_compile_features(
    ${PROJECT_NAME}
    PRIVATE
    cxx_std_23
)

target_compile_definitions(
    ${PROJECT_NAME}
    PRIVATE
    VULKAN_RHI
    SOURCES_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
)

if (MSVC)
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        # make __VA_OPT__ valid in msvc
        /Zc:preprocessor
        # disable alternative tokens
        /Za
        # utf-8 source and execution character sets
        /utf-8
    )
endif ()