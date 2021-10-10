#include "Shader.h"

#include <glad/glad.h>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexSource, fragmentSource;
    std::ifstream filestream;

    filestream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        std::stringstream buffer;

        filestream.open(vertexPath);
        buffer << filestream.rdbuf();
        vertexSource = buffer.str();
        filestream.close();

        buffer.str(std::string());

        filestream.open(fragmentPath);
        buffer << filestream.rdbuf();
        fragmentSource = buffer.str();
        filestream.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Reading shader failed." << std::endl;
    }

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource.c_str());
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int result;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    id = shaderProgram;
}

void Shader::Use() const
{
    glUseProgram(id);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetVec3(const std::string& name, glm::vec3& vec) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::SetVec4(const std::string& name, glm::vec4& vec) const
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetMat4(const std::string& name, glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

unsigned int Shader::CompileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int result;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment") << " Shader compilation failed: ";
        std::cout << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}
