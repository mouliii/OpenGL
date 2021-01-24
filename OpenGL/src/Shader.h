#pragma once

#include <glad.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include "glm/glm.hpp"

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	void Bind() const;
	void Unbind() const;
	void SetUniform1f(const std::string& name, float value);
	void SetUniform1i(const std::string& name, int value);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform3f(const std::string& name, glm::vec3 vec3);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, glm::vec4 vec4);
	void SetUniform4fv(const std::string& name, glm::mat4 mat4);
	void CheckCompileErrors(unsigned int shader, std::string type);
	unsigned int GetID() { return id; }
private:
	unsigned int GetUniformLocation(const std::string& name);
private:
	unsigned int id;
	std::unordered_map<std::string, unsigned int> uniformCache;
};