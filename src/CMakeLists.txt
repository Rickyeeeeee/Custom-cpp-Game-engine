cmake_minimum_required(VERSION 3.0.0)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(engine 
    Application/Application.cpp
    Window/Window.cpp
    event/Event.cpp
    event/types/key/KeyEvent.cpp
    event/types/mouse/MouseEvent.cpp
    event/types/window/WindowEvent.cpp
    event/EventDispatcher.cpp
    layer/Layer.cpp
    layer/LayerStack.cpp
    layer/imguiLayer.cpp
    ImGui/ImGuiBuild.cpp
    OpenGL/OpenGLContext.cpp
    OpenGL/OpenGLBuffer.cpp
    OpenGL/OpenGLVertexArray.cpp
    MainRenderer/Buffer.cpp
    MainRenderer/GraphicsContext.cpp
    MainRenderer/Shader.cpp
    MainRenderer/Renderer.cpp
    MainRenderer/VertexArray.cpp
    Windowsinput.cpp
)

target_include_directories(engine PUBLIC ../external/imgui)

target_link_libraries(
    engine
    glad
    glfw3
    )

