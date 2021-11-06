#include "Renderer.h"


MapRenderer::MapRenderer() 
    : m_shader("../assets/shaders/basic_shader.vs",
               "../assets/shaders/basic_shader.fs")
{    
}

void MapRenderer::render(MapModel& model,Camera& camera) 
{
    m_shader.use();
    m_shader.setMat4("view", camera.getViewMatrix());
    model.draw();
}

Renderer::Renderer(float width, float height) 
    : m_shader("../assets/shaders/Mainshader.vs",
               "../assets/shaders/Mainshader.fs"),
      RenderWidth(width), RenderHeight(height)
{
    
}

void Renderer::render(std::tuple<Mesh*, Matrix4*> mesh_in) 
{
    Mesh* mesh;
    Matrix4* model;
    std::tie(mesh, model) = mesh_in;

    m_shader.use();
    m_shader.setMat4("model", *model);
    mesh->draw();
}

void Renderer::render(Model& model, Camera& camera) 
{
    m_shader.use();
    m_shader.setMat4("model", model.getModelMatrix());
    m_shader.setMat4("view", camera.getViewMatrix());
    m_shader.setMat4("screen", getScreenMatrix());
    model.draw();
}

Matrix4 Renderer::getScreenMatrix() 
{

    return glm::scale(Matrix4(1.0f), Vector3(1.0f, RenderHeight / RenderWidth, 1.0f));
}


