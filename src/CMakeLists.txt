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
imgui/imgui_draw.cpp
imgui/imgui_impl_glfw.cpp
imgui/imgui_impl_opengl3.cpp
imgui/imgui_tables.cpp
imgui/imgui_widgets.cpp
imgui/imgui_demo.cpp
imgui/imgui.cpp
layer/imguiLayer.cpp
Windowsinput.cpp
)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_link_libraries(
    engine
    glad
    glfw3
    )

include(CPack)
