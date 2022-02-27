#pragma once
#include <string>
#include "Renderer/Mesh.h"

class MeshLoader
{
private:
    std::string m_Filepath;
public:
    MeshLoader(const std::string& filepath);
    ~MeshLoader();
    bool LoadMeshTo(Mesh& mesh);
private:
    
};
