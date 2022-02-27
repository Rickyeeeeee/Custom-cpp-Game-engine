#include "core/pch.h"
#include "MeshLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Renderer/Vertex3D.h"

MeshLoader::MeshLoader(const std::string& filepath)
    : m_Filepath(filepath)
{
}

MeshLoader::~MeshLoader()
{
    
}

bool MeshLoader::LoadMeshTo(Mesh& mesh)
{
    Assimp::Importer importer;
    auto Ascene = importer.ReadFile(m_Filepath, aiProcess_Triangulate);

    if (!Ascene)
    {
        std::cout << "Error::Assimp: " << importer.GetErrorString() << std::endl;
        return false;
    }
    auto Anode = Ascene->mRootNode;
    auto Amesh = Ascene->mMeshes[Anode->mChildren[0]->mMeshes[0]];
    mesh.Vertices.clear();
    mesh.Indices.clear();
    for (uint32_t i = 0; i < Amesh->mNumVertices; i++)
    {
        Vertex3DSimple vertex = {
           { Amesh->mVertices[i].x, Amesh->mVertices[i].y, Amesh->mVertices[i].z },
           { Amesh->mNormals[i].x, Amesh->mNormals[i].y, Amesh->mNormals[i].z },
           0
        };

        mesh.Vertices.push_back(vertex);
    }

    for (uint32_t i = 0; i < Amesh->mNumFaces; i++)
    {
        aiFace face = Amesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++)
            mesh.Indices.push_back(face.mIndices[j]);
    }
    return true;
}
