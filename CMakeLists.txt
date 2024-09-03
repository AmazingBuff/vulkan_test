cmake_minimum_required(VERSION 3.25)

project(
    vulkan_test
    LANGUAGES CXX
)

find_package(Vulkan REQUIRED)
find_package(SDL2 REQUIRED)

file(GLOB_RECURSE HEADER_FILES src/*.h)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

set(THIRD_PARTY_INCLUDE_DIRS ext)

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