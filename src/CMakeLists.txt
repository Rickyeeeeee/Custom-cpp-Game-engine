cmake_minimum_required(VERSION 3.0.0)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


file(GLOB CPP_SOURCE 
    *.cpp
    core/*.cpp
    event/*.cpp
    scene/*.cpp
    ImGui/*.cpp
    stb_image/*.cpp
    OpenGL/*.cpp
    Renderer/*.cpp
    Maths/*.cpp
    utils/*.cpp
    physic/*.cpp
)
add_library(engine 
    OBJECT
    ${CPP_SOURCE}
)

# target_include_directories(
#     engine 
#     PRIVATE ../external
#     # PRIVATE ../external/spdlog/include
#     PRIVATE ../src
#     )
target_link_libraries(
    engine
    # spdlog::spdlog
    # imgui
    glad
    glfw3
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libyaml-cpp.dll
    # ImGuizmo
    assimp
)

