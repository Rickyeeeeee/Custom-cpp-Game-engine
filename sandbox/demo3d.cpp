#include <Engine.h>

class ExampleLayer : public Layer
{
public:
    ExampleLayer()
        : Layer(), m_model(1.0f), m_view(1.0f), m_projection(1.0f)
    {
        m_VertexArray.reset(VertexArray::Create());
        float vertices[3 * 8] = {
             0.5f,  0.5f,  0.5f,
             0.5f,  0.5f, -0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f, -0.5f, -0.5f,
            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f, -0.5f
        };

        unsigned int indices[3 * 2 * 6] = {
            0, 4, 6,    6, 2, 0,
            0, 2, 3,    3, 1, 2,
            4, 0, 1,    1, 5, 4,
            2, 6, 7,    7, 3, 2,
            6, 4, 5,    5, 7, 6,
            1, 3, 7,    7, 5, 1
        };
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        {
            BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" }
            };
            m_VertexBuffer->SetLayout(layout);
        }

        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);    
        auto shader = m_ShaderLibrary.Load("asset/shaders/simple3d.glsl");

        float width = Application::Get().GetWindow().GetWidth();
        float height = Application::Get().GetWindow().GetHeight();

        
        m_view = glm::translate(m_view, Vector3(1.0f, -1.0f, -10.0f));
        m_projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.f);
    }

    void OnUpdate(Timestep ts) override
    {
        RenderCommand::SetClearColor( { 0.1f, 0.1f, 0.1f, 1.0f });
        RenderCommand::Clear();
        auto shader = m_ShaderLibrary.Get("simple3d");
        std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Model", m_model);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_View", m_view);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Projection", m_projection);
        m_VertexArray->Bind();
        RenderCommand::DrawIndexed(m_VertexArray);
    }

    void OnImGuiRender() override
    {}

    void OnEvent(Event& event) override
    {}
private:
    ShaderLibrary m_ShaderLibrary;
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<IndexBuffer> m_IndexBuffer;
    Ref<VertexArray> m_VertexArray;
    Matrix4 m_model, m_view, m_projection;
};

class App : public Application
{
public:
    App()
        : Application()
    {
        PushOverLayer(new ExampleLayer());
    }
};

int main()
{
    auto app = new App();
    app->Run();    
}