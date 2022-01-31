#include "../pch.h"
#include "OpenGLShader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc) 
{
    // Read our shaders into the appropriate buffers
    std::string vertexSource = vertexSrc;// Get source code for vertex shader.
    std::string fragmentSource = fragmentSrc;// Get source code for fragment shader.

    // Create an empty vertex shader handle
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Send the vertex shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    const GLchar *source = (const GLchar *)vertexSource.c_str();
    glShaderSource(vertexShader, 1, &source, 0);

    // Compile the vertex shader
    glCompileShader(vertexShader);

    GLint isCompiled = 0;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
    if(isCompiled == GL_FALSE)
    {
    	GLint maxLength = 0;
    	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

    	// The maxLength includes the NULL character
    	std::vector<GLchar> infoLog(maxLength);
    	glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
    
    	// We don't need the shader anymore.
    	glDeleteShader(vertexShader);

    	// Use the infoLog as you see fit.
        std::cout << "Vertex shader compilation failed!" << std::endl;
    	// In this simple program, we'll just leave
    	return;
    }

    // Create an empty fragment shader handle
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Send the fragment shader source code to GL
    // Note that std::string's .c_str is NULL character terminated.
    source = (const GLchar *)fragmentSource.c_str();
    glShaderSource(fragmentShader, 1, &source, 0);

    // Compile the fragment shader
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
    	GLint maxLength = 0;
    	glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

    	// The maxLength includes the NULL character
    	std::vector<GLchar> infoLog(maxLength);
    	glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
    
    	// We don't need the shader anymore.
    	glDeleteShader(fragmentShader);
    	// Either of them. Don't leak shaders.
    	glDeleteShader(vertexShader);

    	// Use the infoLog as you see fit.
        std::cout << "Fragment shader compilation failed!" << std::endl;
    
    	// In this simple program, we'll just leave
    	return;
    }

    // Vertex and fragment shaders are successfully compiled.
    // Now time to link them together into a program.
    // Get a program object.
    GLuint program = glCreateProgram();
    m_RendererID = program;

    // Attach our shaders to our program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    // Link our program
    glLinkProgram(program);

    // Note the different functions here: glGetProgram* instead of glGetShader*.
    GLint isLinked = 0;
    glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE)
    {
    	GLint maxLength = 0;
    	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

    	// The maxLength includes the NULL character
    	std::vector<GLchar> infoLog(maxLength);
    	glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
    
    	// We don't need the program anymore.
    	glDeleteProgram(program);
    	// Don't leak shaders either.
    	glDeleteShader(vertexShader);
    	glDeleteShader(fragmentShader);

    	// Use the infoLog as you see fit.
        std::cout << "Linking error occur!" << std::endl;
    	// In this simple program, we'll just leave
    	return;
    }

    // Always detach shaders after a successful link.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);        
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
