cmake_minimum_required(VERSION 3.0.0)
project(Coolgame VERSION 0.1.1)

set(project1 engine2d)
set(project2 engine3d)

add_executable(
    project1
    sandbox/sandbox.cpp
)

add_executable(
    project2
    sandbox/demo3d.cpp
)

add_subdirectory(external/imgui)

add_subdirectory(src)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/external/imgui
    )

target_link_directories(
    project1
    PRIVATE src
    PRIVATE external/imgui)
target_link_directories(
    project2
    PRIVATE src
    PRIVATE external/imgui)

target_link_libraries(
    project1 
    engine
    imgui)
target_link_libraries(
    project2 
    engine
    imgui)