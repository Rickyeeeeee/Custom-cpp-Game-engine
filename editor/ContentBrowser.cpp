#include "ContentBrowser.h"

#include "core/pch.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

// #include <filesystem> 

constexpr char s_AssetsDirectory[] = "asset";

void ContentBrowserPanel::OnImguiRender()
{
    ImGui::Begin("Content Browser");

    // for (auto& p : std::filesystem::directory_iterator(s_AssetsDirectory))
    // {
    //     // auto path = p.path().string();
    //     // ImGui::Text("%s", path.c_str());
    // }

    ImGui::End();
}
