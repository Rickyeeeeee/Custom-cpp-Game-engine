#include "core/pch.h"
#include "MeshLoader.h"

#include "Renderer/Vertex3D.h"

MeshLoader::MeshLoader(const std::string& filepath, Mesh& mesh)
    : m_Filepath(filepath), m_Mesh(mesh)
{
}

MeshLoader::~MeshLoader()
{
    
}

void MeshLoader::processNode(aiNode* node, const aiScene *scene)
{
    for (uint32_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }

    for (uint32_t i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

void MeshLoader::processMesh(aiMesh* mesh, const aiScene* scene)
{
    for (uint32_t i = 0; i < mesh->mNumVertices; i++)
    {
        m_Mesh.Vertices.push_back({
           Vector3{ mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z },
           Vector3{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z },
           Vector2{ mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y }}
        );
    }
    for (uint32_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++)
            m_Mesh.Indices.push_back(face.mIndices[j] + m_VertexOffset);
    }
    m_VertexOffset += mesh->mNumVertices;

    
}

bool MeshLoader::LoadMeshTo()
{
    Assimp::Importer importer;
    auto Ascene = importer.ReadFile(m_Filepath, aiProcess_Triangulate);

    if (!Ascene)
    {
        std::cout << "Error::Assimp: " << importer.GetErrorString() << std::endl;
        return false;
    }
    m_Mesh.Vertices.clear();
    m_Mesh.Indices.clear();
    
    processNode(Ascene->mRootNode,Ascene);
    return true;
}
