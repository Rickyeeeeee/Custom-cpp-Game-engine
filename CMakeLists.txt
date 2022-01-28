cmake_minimum_required(VERSION 3.0.0)
project(Coolgame VERSION 0.1.1)

add_executable(${PROJECT_NAME} 
    sandbox/sandbox.cpp)

add_subdirectory(external/imgui)
add_subdirectory(src)
include_directories(${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_SOURCE_DIR}/external/imgui
                    sandbox)

# target_include_directories(${PROJECT_NAME} PUBLIC src
#                                            PUBLIC external/imgui)


target_link_directories(${PROJECT_NAME}
    PRIVATE src
    PRIVATE external/imgui)

target_link_libraries(${PROJECT_NAME} 
    engine
    imgui)