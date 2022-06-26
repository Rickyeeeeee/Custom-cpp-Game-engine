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
    m_SelectionEntity = Entity(entt::null, nullptr);
}

void SceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Setting");
    if (m_Context->m_SceneType == SceneType::_3D)
    {
        ImGui::Text("Physic World");
        auto scene = (Scene3D*)(m_Context.get());
        static bool phy_running;
        ImGui::Checkbox("Simulating", &phy_running);
        if (phy_running) scene->m_PhysicWorld.Start();
        else             scene->m_PhysicWorld.Stop();
    }

    ImGui::End();

    ImGui::Begin("Scene Hierarchy");
    m_Context->m_Registry.each([&](auto entityID)
    {
        Entity entity( entityID, m_Context.get() );
        DrawEntityNode(entity);
    });
    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
        m_SelectionEntity = {entt::null, nullptr};

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Create Empty Entity"))
            m_Context->CreateEntity("Empty Entity");
        ImGui::EndPopup();
    }
    ImGui::End();

    ImGui::Begin("Properties");
    if ((bool)m_SelectionEntity)
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
            if (ImGui::MenuItem("MeshRenderer"))
            {
                if (!m_SelectionEntity.HasComponent<MeshRendererComponent>())
                    m_SelectionEntity.AddComponent<MeshRendererComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Light"))
            {
                if (!m_SelectionEntity.HasComponent<LightComponent>())
                    m_SelectionEntity.AddComponent<LightComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("RigidBody"))
            {
                if (!m_SelectionEntity.HasComponent<RigidBodyComponent>())
                    m_SelectionEntity.AddComponent<RigidBodyComponent>();
                ImGui::CloseCurrentPopup();
            }
            if (ImGui::MenuItem("Collider"))
            {
                if (!m_SelectionEntity.HasComponent<ColliderComponent>())
                    m_SelectionEntity.AddComponent<ColliderComponent>();
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
        m_SelectionEntity = entity;
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
                ImGui::Checkbox("hasShadow", &lc.HasShadow);
                if (lc.hasShadow() && !lc.DepthMap)
                    lc.CreateShadow();
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
            const char* items[] = { "none", "cube", "sphere", "other" };
            int lastShape = src.meshSource;;
            ImGui::Combo("Mesh Shape", &src.meshSource, items, IM_ARRAYSIZE(items));
            if (lastShape != src.meshSource)
            {
                src.mesh.Reset();
                src.filepath.clear();
                if (src.meshSource == 1)
                    src.filepath = "C:/Users/ricky/Dev/coolgame/run/asset/models/cube.obj";
                if (src.meshSource == 2)
                    src.filepath = "C:/Users/ricky/Dev/coolgame/run/asset/models/icoSphere.obj";
                else if (src.meshSource == 3)
                    src.filepath = FileDialogs::OpenFile("Model (*.obj)\0*.obj\0") ;
                src.Load();
                if (entity.HasComponent<MeshRendererComponent>() && src.meshSource != 0)
                {
                    auto& meshRendererComponent = entity.GetComponent<MeshRendererComponent>();
                    ComponentUtils::UnSubmitMesh(meshRendererComponent);
                    ComponentUtils::SubmitMesh(src, meshRendererComponent);
                }
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

    if (entity.HasComponent<MeshRendererComponent>())
    {
        bool open = ImGui::TreeNodeEx((void*)typeid(MeshRendererComponent).hash_code(), treeNodeFlag, "MeshRenderer");
        if (open)
        {
            auto& mrc = entity.GetComponent<MeshRendererComponent>();
            ImGui::ColorEdit3("Ambient", glm::value_ptr(mrc.Material.Ambient));
            ImGui::DragFloat("Diffuse", &mrc.Material.Diffuse, 0.05f, 0.0f, 1.0f);
            ImGui::DragFloat("Specular", &mrc.Material.Specular, 0.05f, 0.0f, 1.0f);
            if (ImGui::Button("Load Texture"))
            {
                std::string path = FileDialogs::OpenFile("Texture (*.jpg;*.png)\0*.jpg;*png\0");
                mrc.Material.AmbientTexture = Texture2D::Create(path);
                mrc.Material.HasTexture = true;
            }
            if (mrc.Material.HasTexture)
            {
                ImGui::DragFloat2("Tiling", glm::value_ptr(mrc.Tiling), 0.1f, 0.1f, 100.f);
                ImGui::DragFloat2("Offset", glm::value_ptr(mrc.Offset), 0.01f, 0.0f, 1.0f);
                auto texture = mrc.Material.AmbientTexture;
                ImGui::Image((void*)texture->GetRendererID(), { 50.0f, 50.0f * texture->GetHeight() / texture->GetWidth() });
            }
            ImGui::Checkbox("Normal Map", &mrc.Material.HasNormalMap);

        }
        ImGui::TreePop();
    }

    if (entity.HasComponent<RigidBodyComponent>())
    {
        bool open = ImGui::TreeNodeEx((void*)typeid(RigidBodyComponent).hash_code(), treeNodeFlag, "RigidBody");
        if (open)
        {
            auto& rbc = entity.GetComponent<RigidBodyComponent>();
            const char* items[] = { "Static", "Dynamic", "Kinematic" };
            int type = rbc.body->GetType();
            ImGui::Combo("Type", &type, items, IM_ARRAYSIZE(items));
            rbc.body->SetType((RIGIDBODY_TYPE)type);
            if (type != RIGIDBODY_TYPE::STATIC)
            {
                float mass = rbc.body->GetMass();
                ImGui::DragFloat("Mass", &mass);
                rbc.body->SetMass(mass);
            }
            ImGui::DragFloat("restitution", &rbc.body->restitution, 0.01f, 0.0f, 1.0f);

            ImGui::TreePop();
        }
    }

    if (entity.HasComponent<ColliderComponent>())
    {
        bool open = ImGui::TreeNodeEx((void*)typeid(ColliderComponent).hash_code(), treeNodeFlag, "Collider");
        if (open)
        {
            auto& cc = entity.GetComponent<ColliderComponent>();
            const char* items[] = { "SPHERE", "BOX", "PLANE", "MESH" };

            int type = cc.type;
            ImGui::Combo("Type", &type, items, IM_ARRAYSIZE(items));
            if (type != cc.type)
            {
                auto oc = cc.collider;
                cc.collider = Collider::CreateCollidser((ColliderType)type, oc);
                ((Scene3D*)m_Context.get())->m_PhysicWorld.RemoveCollider(oc);
                ((Scene3D*)m_Context.get())->m_PhysicWorld.AddCollider(cc.collider);
                delete oc;
                cc.type = (ColliderType)type;
            }

            switch (type)
            {
            case 0:
                ImGui::DragFloat("Radius", &((SphereCollider*)cc.collider)->Radius);
                break;
            case 1:
                ImGui::DragFloat3("Scale", glm::value_ptr(((BoxCollider*)cc.collider)->Width));
                break;
            case 2:
                ImGui::DragFloat("Width", &((PlaneCollider*)cc.collider)->Width);
                ImGui::DragFloat("Height", &((PlaneCollider*)cc.collider)->Height);
                break;
            default:
                break;
            }

            ImGui::TreePop();
        }
    }
}
