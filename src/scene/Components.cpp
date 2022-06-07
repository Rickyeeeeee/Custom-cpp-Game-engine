#include "Components.h"

#include "Renderer/Renderer3D.h"

namespace ComponentUtils {
    
    void SubmitMesh(MeshComponent& mesh, MeshRendererComponent& meshRenderer)
    {
        if (!meshRenderer.Submitted)
        {
            meshRenderer.Renderer3Did = Renderer3D::SubmitStaticMesh(mesh.mesh);
            meshRenderer.Submitted = true;
        }
    }

    void UnSubmitMesh(MeshRendererComponent& meshRenderer)
    {
        Renderer3D::DestroyStaticMesh(meshRenderer.Renderer3Did);
        meshRenderer.Submitted = false;
    }
    
}