cmake_minimum_required(VERSION 3.25)

project(
    vulkan_test
    LANGUAGES CXX
)

add_subdirectory(ext/yaml-cpp)
add_subdirectory(ext/reflect-cpp)
add_subdirectory(ext/eigen)
add_subdirectory(ext/assimp)

file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

set(THIRD_PARTY_INCLUDE_DIRS 
    3rd/traits/include
    3rd/spirv_cross/include
    3rd/sdl2/include
    3rd/vulkan/include
    ext/reflect-cpp/include
)

set(THIRD_PARTY_LIB_DIRS
    3rd/spirv_cross/lib/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
    3rd/sdl2/lib/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>
    3rd/vulkan/lib
)

set(THIRD_PARTY_LIBS
    $<$<CONFIG:Debug>:
    spirv-cross-cored
    spirv-cross-glsld
    SDL2d
    SDL2maind
    vulkan-1
    >
    $<$<CONFIG:Release>:
    spirv-cross-core
    spirv-cross-glsl
    SDL2
    SDL2main
    vulkan-1
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
    ${THIRD_PARTY_LIB_DIRS}
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    Eigen3::Eigen
    yaml-cpp::yaml-cpp
    assimp::assimp
    reflectcpp
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