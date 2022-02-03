#include <Engine.h>

class ExampleLayer : public Layer
{
public:
    ExampleLayer()
        : Layer(), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
    {
        m_VertexArray.reset(VertexArray::Create());
        float vertices[ 4 * 5 ] = {
            // -0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.4f, 1.0f,
            //  0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.4f, 1.0f,
            //  0.5f,  0.5f, 0.0f, 0.2f, 0.3f, 0.4f, 1.0f,
            // -0.5f,  0.5f, 0.0f, 0.2f, 0.3f, 0.4f, 1.0f
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f

        };
        unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };

        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        {
            BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float2, "a_TexCoord"}
            };
            m_VertexBuffer->SetLayout(layout);
        }
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int)));
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);
        auto shader = m_ShaderLibrary.Load("asset/shaders/example.glsl");
        m_Texture = Texture2D::Create("asset/picture/n.png");

        std::dynamic_pointer_cast<OpenGLShader>(shader)->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformInt("u_Texture", 0);
    }

    void OnUpdate(Timestep ts) override
    {


        if (Input::IsKeyPressed(KEY_LEFT))
            m_CameraPosition.x -= m_CameraSpeed * ts;

        if (Input::IsKeyPressed(KEY_RIGHT))
            m_CameraPosition.x += m_CameraSpeed * ts;
        
        if (Input::IsKeyPressed(KEY_UP))
            m_CameraPosition.y -= m_CameraSpeed * ts;

        if (Input::IsKeyPressed(KEY_DOWN))
            m_CameraPosition.y += m_CameraSpeed * ts;
        

        RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1.0 });
        RenderCommand::Clear();

        m_Camera.SetPosition(m_CameraPosition);

        Renderer::BeginScene(m_Camera);
        m_Texture->Bind();
        auto shader = m_ShaderLibrary.Get("example");
        Renderer::Submit(m_VertexArray, shader);            
        Renderer::EndScene();
    }

    virtual void OnImGuiRender() override
    {}

    void OnEvent(Event& event) override
    {
    }

private:
    ShaderLibrary m_ShaderLibrary;
    Ref<Shader> m_Shader;
    Ref<VertexBuffer> m_VertexBuffer;
    Ref<IndexBuffer> m_IndexBuffer; 
    Ref<VertexArray> m_VertexArray;
    Ref<Texture> m_Texture;
    Orthographic2DCamera m_Camera;
    Vector3 m_CameraPosition = {0.0f, 0.0f, 0.0f,};
    float m_CameraSpeed = 0.1f;
};

class Sandbox : public Application
{
public:
    Sandbox()
        : Application()
    {
        PushOverLayer(new ExampleLayer());
    }

};

int main()
{
    auto* sandboxApplication = new Sandbox();
    sandboxApplication->Run();   
}