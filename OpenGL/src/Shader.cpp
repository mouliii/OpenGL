#include "Shader.h"

Shader::Shader(const std::string& filepath)
	:
	filepath(filepath),
	rendererID(0)
{
    ShaderPrograms sp = ParseShader(filepath);
    rendererID = CreateShader(sp.vertexShader, sp.fragmentShader);

    // TODO - delete?
    GLErrorCheck check;
    check.GLLogError();
}

Shader::~Shader()
{
    glDeleteProgram(rendererID);
}

void Shader::Bind() const
{
    glUseProgram(rendererID);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& u_name, int value)
{
    glUniform1i(GetUniformLocation(u_name), value);
}

void Shader::SetUniform1f(const std::string& u_name, float value)
{
    glUniform1f(GetUniformLocation(u_name), value);
}

void Shader::SetUniform4f(const std::string& u_name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(u_name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& u_name, const glm::mat4& mat)
{
    glUniformMatrix4fv(GetUniformLocation(u_name), 1, GL_FALSE, &mat[0][0]);
}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (uniformLocationCache.find(name) != uniformLocationCache.end())
    {
        return uniformLocationCache[name];
    }
    int location = glGetUniformLocation(rendererID, name.c_str());
    if (location == -1)
    {
        std::cout << "Warning: uniform " << name << " does not exist!" << std::endl;
    }
    else
    {
        uniformLocationCache[name] = location;
    }
    // TODO - delete?
    GLErrorCheck check;
    check.GLLogError();

    return location;
}

bool Shader::CompileShader()
{
	return false;
}

ShaderPrograms Shader::ParseShader(const std::string& filepath)
{
    enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };
    std::stringstream ss[2];

    std::ifstream stream(filepath);
    if (stream.fail())
    {
        std::cout << "could not open shader file!" << std::endl;
    }
    std::string line;
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
            else
            {
                std::cout << "shader parse error" << std::endl;
            }
        }
        else
        {
            // lue shader
            ss[(int)type] << line << "\n";
        }
    }
    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
        std::cout << message << std::endl;
        delete message;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
