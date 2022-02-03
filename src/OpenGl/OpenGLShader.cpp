#include "pch.h"
#include "OpenGLShader.h"
#include <fstream>
#include <sstream>
#include <string>

OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc) 
    : m_Name(name)
{
    std::unordered_map<GLenum, std::string> shaderSources;
    shaderSources[GL_VERTEX_SHADER] = vertexSrc;
    shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;
    Complie(shaderSources);
}

OpenGLShader::OpenGLShader(const std::string& filepath) 
{
    std::string source = ReadFile(filepath);
    auto shaderSources = PreProcess(source);
    Complie(shaderSources);
    // take out the name
    auto lastSlash = filepath.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = filepath.rfind('.');
    auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
    m_Name = filepath.substr(lastSlash, count);
}

OpenGLShader::~OpenGLShader() 
{
    glDeleteProgram(m_RendererID);
}

void OpenGLShader::Bind() const
{
    glUseProgram(m_RendererID);
}

void OpenGLShader::UnBind() const
{
    glUseProgram(m_RendererID);
}

const std::string OpenGLShader::GetName() const 
{
    return m_Name;
}

void OpenGLShader::UploadUniformInt(const std::string& name, const int& value) 
{
    GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1i(location, value);
}

void OpenGLShader::UploadUniformFloat(const std::string& name, const float& value) 
{
    GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform1f(location, value);
}

void OpenGLShader::UploadUniformVector2(const std::string& name, const Vector2& vector) 
{
    GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform2f(location, vector.x, vector.y);
}

void OpenGLShader::UploadUniformVector3(const std::string& name, const Vector3& vector) 
{
    GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform3f(location, vector.x, vector.y, vector.z);
}

void OpenGLShader::UploadUniformVector4(const std::string& name, const Vector4& vector4) 
{
    GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniform4f(location, vector4.x, vector4.y, vector4.z, vector4.a);
}

void OpenGLShader::UploadUniformMat3(const std::string& name, const Matrix3& matrix) 
{
    GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void OpenGLShader::UploadUniformMat4(const std::string& name, const Matrix4& matrix) 
{
    GLuint location = glGetUniformLocation(m_RendererID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string OpenGLShader::ReadFile(const std::string& filepath) 
{
    std::stringstream ss;
    std::ifstream in;
    in.open(filepath);
    if (!in.fail())
    {
        ss << in.rdbuf();
        in.close();
    }
    else
    {
        std::cout << "could not open file" << "\n";
    }
    return ss.str();
}

static GLenum ShaderTypeFromString(const std::string& type)
{
    if (type == "vertex")
        return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel")
        return GL_FRAGMENT_SHADER;
    std::cout << "Unknown shader type!" << "\n";
    return 0;
}

static std::string StringFromShaderType(const GLenum type)
{
    if (type == GL_VERTEX_SHADER)
        return "vertex ";
    if (type == GL_FRAGMENT_SHADER)
        return "fragment/pixel ";
    std::cout << "Unknown shader type!" << "\n";
    return 0;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source) 
{
    std::unordered_map<GLenum, std::string> shaderSources;

    const char* typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);
        if (!(eol != std::string::npos)) std::cout <<  "Syntax error" << std::endl;
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);
        if (!ShaderTypeFromString(type))
            std::cout << "Invalid shader type specification!" << "\n";

        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);
        shaderSources[ShaderTypeFromString(type)] = 
            source.substr(nextLinePos, 
            pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
    }
    return shaderSources;
}

void OpenGLShader::Complie(const std::unordered_map<GLenum, std::string>& shaderSources) 
{
    GLuint program = glCreateProgram();
    
    std::vector<GLenum> glShaderIDs;
    glShaderIDs.reserve(shaderSources.size());
    for (auto&& [type, source] : shaderSources)
    {
        GLuint shader = glCreateShader(type);
        const GLchar *sourceCStr = (const GLchar *)source.c_str();
        glShaderSource(shader, 1, &sourceCStr, 0);

        // Compile the vertex shader
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if(isCompiled == GL_FALSE)
        {
        	GLint maxLength = 0;
        	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        	std::vector<GLchar> infoLog(maxLength);
        	glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

        	glDeleteShader(shader);
            for (auto& c : infoLog)
                std::cout << c;
            std::cout << "\n";
            std::cout << StringFromShaderType(type) << "shader compilation failed!" << std::endl;
        	break;
        }
        glAttachShader(program, shader);
        glShaderIDs.emplace_back(type);
    }
    glLinkProgram(program);

    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
    	GLint maxLength = 0;
    	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    	std::vector<GLchar> infoLog(maxLength);
    	glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
    
    	glDeleteProgram(program);
        for (auto id : glShaderIDs)
    	    glDeleteShader(id);
        std::cout << "Linking error occur!" << std::endl;
    	return;
    }
    for (auto id : glShaderIDs)
       glDetachShader(program, id);

    m_RendererID = program;
}
