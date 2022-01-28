cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(engine 
Application/Application.cpp
event/Event.cpp
event/types/key/KeyEvent.cpp
event/types/mouse/MouseEvent.cpp
event/types/window/WindowEvent.cpp
event/EventDispatcher.cpp
Window/Window.cpp
layer/Layer.cpp
layer/LayerStack.cpp
shader/Shader.cpp
texture/Texture.cpp
map/MapModel.cpp
map/Map.cpp
renderer/Renderer.cpp
model/Model.cpp
mesh/Mesh.cpp
player/Ship.cpp
Windowsinput.cpp
ImGui/ImGuiBuild.cpp
layer/imguiLayer.cpp
)

target_include_directories(engine PUBLIC ../external/imgui)

target_link_libraries(
    engine
    glad
    glfw3
    )

