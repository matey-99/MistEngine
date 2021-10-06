#include "Shader.h"

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

    GL_CALL(unsigned int shaderProgram = glCreateProgram());
    GL_CALL(glAttachShader(shaderProgram, vertexShader));
    GL_CALL(glAttachShader(shaderProgram, fragmentShader));
    GL_CALL(glLinkProgram(shaderProgram));

    int result;
    char infoLog[512];
    GL_CALL(glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result));
    if (!result)
    {
        GL_CALL(glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog));
        std::cout << "Shader program linking failed: " << infoLog << std::endl;
    }

    GL_CALL(glDeleteShader(vertexShader));
    GL_CALL(glDeleteShader(fragmentShader));

    id = shaderProgram;
}

void Shader::Use() const
{
    GL_CALL(glUseProgram(id));
}

void Shader::SetBool(const std::string& name, bool value) const
{
    GL_CALL(glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value));
}

void Shader::SetInt(const std::string& name, int value) const
{
    GL_CALL(glUniform1i(glGetUniformLocation(id, name.c_str()), value));
}

void Shader::SetFloat(const std::string& name, float value) const
{
    GL_CALL(glUniform1f(glGetUniformLocation(id, name.c_str()), value));
}

void Shader::SetMat4(const std::string& name, glm::mat4& mat) const
{
    GL_CALL(glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

unsigned int Shader::CompileShader(unsigned int type, const char* source)
{
    GL_CALL(unsigned int shader = glCreateShader(type));
    GL_CALL(glShaderSource(shader, 1, &source, nullptr));
    GL_CALL(glCompileShader(shader));

    int result;
    char infoLog[512];
    GL_CALL(glGetShaderiv(shader, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        GL_CALL(glGetShaderInfoLog(shader, 512, NULL, infoLog));
        std::cout << ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment") << " Shader compilation failed: ";
        std::cout << infoLog << std::endl;
        GL_CALL(glDeleteShader(shader));
        return 0;
    }

    return shader;
}
