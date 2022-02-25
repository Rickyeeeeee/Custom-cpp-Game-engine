#include "SceneHierarchyPanel.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"
SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
{
   SetContext(context); 
}

void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
{
    m_Context = context;
    m_SelectionEntity = {};
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

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
            m_Context->CreateEntity("Empty Entity");
        ImGui::EndPopup();
    }
    ImGui::End();

    ImGui::Begin("Properties");
    if (m_SelectionEntity)
    {
        DrawComponents(m_SelectionEntity);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Camera"))
            {
                if (!m_SelectionEntity.HasComponent<CameraComponent>())
                    m_SelectionEntity.AddComponent<CameraComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Sprite"))
            {
                if (!m_SelectionEntity.HasComponent<SpriteComponent>())
                    m_SelectionEntity.AddComponent<SpriteComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("StaticMesh"))
            {
                if (!m_SelectionEntity.HasComponent<MeshComponent>())
                    m_SelectionEntity.AddComponent<MeshComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Light"))
            {
                if (!m_SelectionEntity.HasComponent<LightComponent>())
                    m_SelectionEntity.AddComponent<LightComponent>();
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        
    }
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
    bool entityDeleted = false;
    if (ImGui::BeginPopupContextItem())
    {
        if (ImGui::MenuItem("Delete Entity"))
            entityDeleted = true;
        ImGui::EndPopup();
    }
    if (opened)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
        bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
        if (opened)
            ImGui::TreePop();
        ImGui::TreePop();
    }
    if (entityDeleted)
    {
        if (m_SelectionEntity == entity)
            m_SelectionEntity = {};
        m_Context->DestroyEntity(entity);
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
    
    const ImGuiTreeNodeFlags treeNodeFlag = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

    if (entity.HasComponent<TransformComponent>())
    {
        bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlag, "Transform");
        
        if (open)
        {
            auto& tc = entity.GetComponent<TransformComponent>();
            ImGui::DragFloat3("Position", glm::value_ptr(tc.Translation), 0.1f);
            ImGui::DragFloat3("Rotation", glm::value_ptr(tc.Rotation), 0.1f);
            ImGui::DragFloat3("Scale", glm::value_ptr(tc.Scale), 0.1f);
            ImGui::TreePop();
        }

        
    }

    if (entity.HasComponent<CameraComponent>())
    {
        if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlag, "Camera"))
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

    if (entity.HasComponent<LightComponent>())
    {
        bool open = ImGui::TreeNodeEx((void*)typeid(LightComponent).hash_code(), treeNodeFlag, "Light");

        if (open)
        {
            auto& lc = entity.GetComponent<LightComponent>();
            const char* items[] = { "PointLight", "DirectionalLight", "SpotLight" };
            ImGui::Combo("Type", (int*)&lc.Type, items, IM_ARRAYSIZE(items));
            if (lc.Type == LightType::POINT)
            {
                ImGui::DragFloat("ambient", &lc.Light.ambient, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("diffuse", &lc.Light.diffuse, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("specular", &lc.Light.specular, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("constant", &lc.Light.constant, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("linear", &lc.Light.linear, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("quadratic", &lc.Light.quadratic, 0.01f, 0.0f, 1.0f);
                ImGui::ColorEdit3("Color", glm::value_ptr(lc.Light.color));
            }
            else if (lc.Type == LightType::DIRECTIONAL)
            {
                ImGui::DragFloat("ambient", &lc.Light.ambient, 0.01f, 0.0f, 1.0f);
                ImGui::ColorEdit3("Color", glm::value_ptr(lc.Light.color));
            }

            ImGui::TreePop();
        }
    }

    if (entity.HasComponent<SpriteComponent>())
    {
        bool open = ImGui::TreeNodeEx((void*)typeid(SpriteComponent).hash_code(), treeNodeFlag, "Sprite Renderer");
        
        ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
        if(ImGui::Button("+", ImVec2(20, 20)))
        {
            ImGui::OpenPopup("ComponentSettings");
        }
        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings"))
        {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;

            ImGui::EndPopup();
        }
        if (open)
        {
            auto& src = entity.GetComponent<SpriteComponent>();
            ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));

            ImGui::TreePop();
        }

        if (removeComponent)
            entity.RemoveComponent<SpriteComponent>();
    }

    if (entity.HasComponent<MeshComponent>())
    {
        bool open = ImGui::TreeNodeEx((void*)typeid(MeshComponent).hash_code(), treeNodeFlag, "Mesh Renderer");

        ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);
        if (ImGui::Button("+", ImVec2(20, 20)))
        {
            ImGui::OpenPopup("Component Settings");
        }
        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings"))
        {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;

            ImGui::EndPopup();
        }
        if (open)
        {
            auto& src = entity.GetComponent<MeshComponent>();
            const char* items[] = { "none", "cube" };
            int lastShape = src.meshSource;;
            ImGui::Combo("Mesh Shape", &src.meshSource, items, IM_ARRAYSIZE(items));
            // if (item_current == 0)
            if (lastShape != src.meshSource)
            {
                if (src.meshSource == 1)
                    src.mesh.SetCube();
                else if (src.meshSource == 0)
                    src.mesh.Reset();
            }
            else 
            {
                ImGui::ColorEdit4("Color", glm::value_ptr(src.mesh.color));
            }
            ImGui::TreePop();
        }

        if (removeComponent)
            entity.RemoveComponent<MeshComponent>();
    }
}
