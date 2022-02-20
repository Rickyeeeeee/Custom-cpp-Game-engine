#include "SceneHierarchyPanel.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "scene/Components.h"
SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
{
   SetContext(context); 
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
{
    m_Context = context;
}

void SceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");
    m_Context->m_Registry.each([&](auto entityID)
    {
        Entity entity( entityID, m_Context.get() );
        DrawEntityNode(entity);
    });
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_SelectionEntity = {};
    ImGui::End();

    ImGui::Begin("Properties");
    if (m_SelectionEntity)
        DrawComponents(m_SelectionEntity);
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
    auto& tag = entity.GetComponent<TagComponent>().Tag;
    ImGuiTreeNodeFlags flags = ((m_SelectionEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
    if (ImGui::IsItemClicked())
    {
        m_SelectionEntity =  entity;
    }
    if (opened)
    {
        ImGui::TreePop();
    }

}

void SceneHierarchyPanel::DrawComponents(Entity entity)
{
    if (entity.HasComponent<TagComponent>())
    {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strcpy_s(buffer, sizeof(buffer), tag.c_str());
        if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
        {
            tag = std::string(buffer);
        }
    }

    if (entity.HasComponent<TransformComponent>())
    {
        if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
        {
            auto& transform = entity.GetComponent<TransformComponent>().Transform;
            ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
            ImGui::TreePop();
        }
    }

    if (entity.HasComponent<CameraComponent>())
    {
        if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
        {
            auto& cameraComponent = entity.GetComponent<CameraComponent>();
            auto& camera = cameraComponent.Camera;
            const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
            const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
            if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
            {
                for (int i = 0; i < 2; i++)
                {
                    bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                    if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
                    {
                        currentProjectionTypeString = projectionTypeStrings[i];
                        camera.SetProjectiontype((SceneCamera::ProjectionType)i);
                    }

                    if (isSelected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();

            }
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
            {
                float perspectFOV = glm::degrees(camera.GetPerspectiveFOV());
                float perspectNear = camera.GetPerspectiveNearClip();
                float perspectFar = camera.GetPerspectiveFarClip();
                if (ImGui::DragFloat("FOV", &perspectFOV))
                    camera.SetPerspectiveFOV(glm::radians(perspectFOV));
                if (ImGui::DragFloat("Near", &perspectNear))
                    camera.SetPerspectiveNearClip(perspectNear);
                if (ImGui::DragFloat("Far", &perspectFar))
                    camera.SetPerspectiveFarClip(perspectFar);
            }
            if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
            {
                float orthoSize = camera.GetOrthographicSize();
                float orthoNear = camera.GetOrthographicNearClip();
                float orthoFar = camera.GetOrthographicFarClip();
                if (ImGui::DragFloat("Size", &orthoSize))
                    camera.SetOrthographicSize(orthoSize);
                if (ImGui::DragFloat("Near", &orthoNear))
                    camera.SetOrthographicNearClip(orthoNear);
                if (ImGui::DragFloat("Far", &orthoFar))
                    camera.SetOrthographicFarClip(orthoFar);
            }
            ImGui::TreePop();
        }


    }
}
