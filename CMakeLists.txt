cmake_minimum_required(VERSION 3.0.0)
project(project1 VERSION 0.1.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../run)

include_directories(
    PUBLIC external/entt/include
    PUBLIC external
    PUBLIC external/std_image
)
 
add_subdirectory(external/imgui) 
add_subdirectory(external/spdlog)
add_subdirectory(src) 
add_subdirectory(sandbox)
add_subdirectory(editor)


target_include_directories(
    engine 
    # PRIVATE external
    # PRIVATE ../external/spdlog/include
    PRIVATE src
)

