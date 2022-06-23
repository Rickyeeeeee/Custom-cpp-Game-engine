cmake_minimum_required(VERSION 3.0.0)
project(project1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(STATUS "Is the C++ compiler loaded? ${CMAKE_CXX_COMPILER_LOADED}")
if(CMAKE_CXX_COMPILER_LOADED)
message(STATUS "The C++ compiler ID is: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Is the C++ from GNU? ${CMAKE_COMPILER_IS_GNUCXX}")
message(STATUS "The C++ compiler version is: ${CMAKE_CXX_COMPILER_VERSION}")
endif()


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../run)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include_directories(
    PUBLIC external/entt/include
    PUBLIC external/yaml-cpp/include
    PUBLIC external/std_image
    PUBLIC external
    PUBLIC external/assimp/include
    PUBLIC external/ImGuizmo
    PUBLIC src
)
 
# add_subdirectory(external/yaml-cpp)
add_subdirectory(external/imgui)
add_subdirectory(external/ImGuizmo)
add_subdirectory(external/spdlog)
add_subdirectory(external/assimp)
add_subdirectory(src) 
add_subdirectory(sandbox)
add_subdirectory(editor)
add_subdirectory(examples)


target_include_directories(
    engine 
    # PRIVATE external
    # PRIVATE ../external/spdlog/include
    PRIVATE src
)

