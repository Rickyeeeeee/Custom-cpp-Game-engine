#pragma once

#include "core/core.h"
#include "core/pch.h"
#include "scene/Scene.h"

class SceneSerializer
{
public:
    SceneSerializer(const Ref<Scene>& scene);

    void SerializeText(const std::string& filepath);
    void SerielizeRuntime(const std::string& filepath);

    std::tuple<bool, Ref<Scene>> DeserializeText(const std::string& filepath);
    bool DeserielizeRuntime(const std::string& filepath);
private:
    Ref<Scene> m_Scene;
};