#pragma once 

#include <tuple>

#include "../map/MapModel.h"
#include "../shader/Shader.h"
#include "../mesh/Mesh.h"
#include "../glm/glm.h"
#include "../model/Model.h"
#include "../camera/Camera.h"

class MapRenderer
{
public:
    MapRenderer();
    void render(MapModel& model, Camera& camera);
private:
    Shader m_shader; 
};

class Renderer
{
public: 
    Renderer(float width, float height);
    void render(std::tuple<Mesh*, Matrix4*> mesh);
    void render(Model& model, Camera& camera);
private:
    Matrix4 getScreenMatrix();
private:
    float RenderWidth;
    float RenderHeight;
    Shader m_shader;
};