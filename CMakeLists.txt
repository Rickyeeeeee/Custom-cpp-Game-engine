cmake_minimum_required(VERSION 3.0.0)
project(Coolgame VERSION 0.1.1)

add_executable(${PROJECT_NAME} sandbox.cpp)

add_subdirectory(src)

target_include_directories(${PROJECT_NAME} PUBLIC src)

target_link_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} engine)