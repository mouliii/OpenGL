#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    LoadShaders(vertexPath, fragmentPath);
}

void Shader::LoadShaders(const std::string& vertexPath, const std::string& fragmentPath)
{
    // check shader compilation
    GLint success;
    char infoLog[512];

    std::string temp = "";
    std::string src = "";

    std::ifstream in_file;

    // VERTEX
    in_file.open(vertexPath);
    if (in_file.is_open() )
    {
        while (std::getline(in_file, temp))
        {
            src += temp + "\n";
        }
    }
    else
    {
        std::cout << "Unable to open Vertex shader file \n";
    }
    in_file.close();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // vertex string to c-string
    const GLchar* vertexSrc = src.c_str();
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);
    // error check
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << "Vertex shader compilation error:\n";
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << infoLog << "\n";
    }
    //clear
    temp = "";
    src = "";
    // FRAGMENT
    in_file.open(fragmentPath);
    if (in_file.is_open())
    {
        while (std::getline(in_file, temp))
        {
            src += temp + "\n";
        }
    }
    else
    {
        std::cout << "Unable to open Fragment shader file \n";
    }
    in_file.close();

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // vertex string to c-string
    const GLchar* fragmentSrc = src.c_str();
    glShaderSource(fragmentShader, 1, &fragmentSrc, NULL);
    glCompileShader(fragmentShader);
    // error check
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::cout << "Fragment shader compilation error:\n";
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << infoLog << "\n";
    }

    // program
    id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::cout << "Could not link shader program\n";
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        std::cout << infoLog << "\n";
    }

    // delete - already in shader program memory
    glUseProgram(0);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}



void Shader::Bind() const
{
    glUseProgram(id);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform3f(const std::string& name, glm::vec3 vec3)
{
    glUniform3f(GetUniformLocation(name), vec3.x, vec3.y, vec3.z);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform4f(const std::string& name, glm::vec4 vec4)
{
 
    glUniform4f(GetUniformLocation(name), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::SetUniform4fv(const std::string& name, glm::mat4 mat4)
{
    glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &mat4[0][0]);
}

void Shader::CheckCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (uniformCache.find(name) != uniformCache.end())
    {
        return uniformCache[name];
    }
    else
    {
        unsigned int location = glGetUniformLocation(id, name.c_str());
        uniformCache[name] = location;
        return location;
    }
}
