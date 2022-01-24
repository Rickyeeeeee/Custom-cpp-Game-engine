cmake_minimum_required(VERSION 3.0.0)
project(Coolgame VERSION 0.1.1)

include(CTest)
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
enable_testing()

add_executable(Coolgame main.cpp                    
                        src/Application/Application.cpp
                        src/event/Event.cpp
                        src/event/types/key/KeyEvent.cpp
                        src/event/types/mouse/MouseEvent.cpp
                        src/event/types/window/WindowEvent.cpp
                        src/event/EventDispatcher.cpp
                        src/Window/Window.cpp
                        src/layer/Layer.cpp
                        src/layer/LayerStack.cpp
                        src/shader/Shader.cpp
                        src/texture/Texture.cpp
                        src/map/MapModel.cpp
                        src/map/Map.cpp
                        src/renderer/Renderer.cpp
                        src/model/Model.cpp
                        src/mesh/Mesh.cpp
                        src/player/Ship.cpp
                        src/imgui/imgui_draw.cpp
                        src/imgui/imgui_impl_glfw.cpp
                        src/imgui/imgui_impl_opengl3.cpp
                        src/imgui/imgui_tables.cpp
                        src/imgui/imgui_widgets.cpp
                        src/imgui/imgui_demo.cpp
                        src/imgui/imgui.cpp
                        src/layer/imguiLayer.cpp
                    )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_link_libraries(
    Coolgame
    glad
    glfw3
    )

include(CPack)
