cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE)

file(GLOB CPP_SOURCE 
    *.cpp

    Application/*.cpp

    core/EntryPoint.cpp

    Window/*.cpp

    event/*.cpp
    event/types/key/*.cpp
    event/types/mouse/*.cpp
    event/types/window/*.cpp
    
    layer/*.cpp
    
    ImGui/*.cpp
    
    stb_image/*.cpp
    
    OpenGL/*.cpp
    
    MainRenderer/*.cpp
    
    )
add_library(engine 
    ${CPP_SOURCE}
)

target_include_directories(
    engine 
    PRIVATE ../external/imgui
    PRIVATE ../external/stb_image
    # PRIVATE ../external/spdlog/include
    PRIVATE ../src
    )

target_link_libraries(
    engine
    # spdlog::spdlog
    imgui
    glad
    glfw3
    )

