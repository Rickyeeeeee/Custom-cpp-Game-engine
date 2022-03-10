#pragma once
#include <string>
#include "core/pch.h"
#include "Renderer/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
class MeshLoader
{
private:
    std::string m_Filepath;
    Mesh& m_Mesh;
    uint32_t m_VertexOffset = 0;
public:
    MeshLoader(const std::string& filepath, Mesh& mesh);
    ~MeshLoader();
    bool LoadMeshTo();
private:
    void processNode(aiNode* node, const aiScene *scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
};
