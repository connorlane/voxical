cmake_minimum_required (VERSION 2.6)
project (Voxical)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)
include_directories(clipper)

add_subdirectory(clip2tri)

file(GLOB SOURCES "*.cpp")

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

add_executable(Voxical ${SOURCES})
target_link_libraries(Voxical ${GLFW_STATIC_LIBRARIES})
target_link_libraries(Voxical clip2tri)
target_compile_features(Voxical PRIVATE cxx_range_for)

file(COPY models/ DESTINATION models/)
