cmake_minimum_required(VERSION 3.0.0)
project(Coolgame VERSION 0.1.1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../sandbox)

add_executable(
    project1
    sandbox/sandbox.cpp
    )
    
add_executable(
    project2
    sandbox/demo3d.cpp
    )
    
add_subdirectory(external/imgui) 
add_subdirectory(external/spdlog)
add_subdirectory(src)


target_include_directories(
    project1
    PRIVATE src
    )
target_include_directories(
    project2
    PRIVATE src
    )
                    
target_link_libraries(
    project1 
    engine
    )
    
target_link_libraries(
    project2  
    engine
    )
