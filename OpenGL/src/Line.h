#pragma once
#include "Renderer.h"
#include "Vec2.h"
#include "OrthoCamera.h"

class Line
{
public:
	Line(Vec2f start, Vec2f end)
	{
		positions[0] = Vec2f(start.x, start.y);
		positions[1] = Vec2f(end.x, end.y);

		vao.Bind();
		VertexBuffer vbo((void*)&positions, sizeof(positions));
		IndexBuffer ibo((void*)&indices,2);
		BufferLayout layout(
			{
				{DataType::FLOAT, 2, "aPos"},
				{DataType::FLOAT, 4, "aColor"}
			}
		);
		vbo.SetLayout(layout);
		vao.AddVertexBuffer(vbo);
		// ??? tämä ???

		vbo.SetSubData((void*)&positions, sizeof(positions));

		vao.Unbind();
		vbo.Unbind();
		ibo.Unbind();
	}
	void Draw(OrthoCamera* cam)
	{
		Shader s("res/shaders/LineVertexShader.shader", "res/shaders/LineFragmentShader.shader");
		glm::mat4 viewProj = cam->GetViewProjectionMatrix();
		s.SetUniform4fv("uViewProj", viewProj);
		vao.Bind();
		glDrawArrays(GL_LINES, 0, 2);
		s.Unbind();
	}
private:
	Vec2f positions[2];
	int indices[2] = {0,1};
	VertexArray vao;
};