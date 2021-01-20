#pragma once

#include <GL/glew.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include "GLErrorCheck.h"
#include "glm/glm.hpp"

struct ShaderPrograms
{
	std::string vertexShader;
	std::string fragmentShader;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& u_name, int value);
	void SetUniform1f(const std::string& u_name, float value);
	void SetUniform4f(const std::string& u_name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& u_name, const glm::mat4& mat);
private:
	unsigned int GetUniformLocation(const std::string& name);
	bool CompileShader();
	ShaderPrograms ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
private:
	unsigned int rendererID;
	std::unordered_map<std::string, int> uniformLocationCache;
	std::string filepath; // debugaamisen
};