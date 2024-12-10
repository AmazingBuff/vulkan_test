cmake_minimum_required(VERSION 3.25)

project(
    vulkan_test
    LANGUAGES CXX
)

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Eigen3 REQUIRED)

include(FetchContent)

FetchContent_Declare(
    yaml-cpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_TAG da82fd982c260e7f335ce5acbceff24b270544d1
)
FetchContent_MakeAvailable(yaml-cpp)

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 527e98e3f873562b640377ad8f3c6088ac34e01d
)
FetchContent_MakeAvailable(fmt)

file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

set(THIRD_PARTY_INCLUDE_DIRS 
    ext/traits
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
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    ${Vulkan_LIBRARIES}
    ${SDL2_LIBRARIES}
    Eigen3::Eigen
    yaml-cpp::yaml-cpp
    fmt::fmt
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