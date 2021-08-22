#include "Renderer.h"

Renderer::Renderer()
	:shader("res/shaders/DefaultVertex.shader", "res/shaders/DefaultFragment.shader"),
	vao(),vbo(),ibo()
{
	vao.Bind();
	vbo.Bind();
	//vbo.SetData(vertices.size() * sizeof(Vertex), vertices.data());
	ibo.Bind();
	//ibo.SetData(indices);

	vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
	vao.LinkAttribute(vbo, 1, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	vao.LinkAttribute(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

	vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();
}

void Renderer::Draw(const Primitive& primitive, const OrthoCamera& cam, Shader* shader)
{
	if (shader != nullptr)
	{
		shader->Bind();
		glm::mat4 viewProj = cam.GetViewProjectionMatrix();
		shader->SetUniform4fv("uViewProj", viewProj);
	}
	else
	{
		this->shader.Bind();
		glm::mat4 viewProj = cam.GetViewProjectionMatrix();
		this->shader.SetUniform4fv("uViewProj", viewProj);
	}
	vao.Bind();
	vbo.Bind();
	vbo.SetData(primitive.GetVertexCount() * sizeof(Vertex), primitive.GetVertices().data());
	ibo.Bind();
	ibo.SetData(primitive.GetIndices());
	glDrawElements(GL_TRIANGLES, primitive.GetIndexCount(), GL_UNSIGNED_INT, 0);
	vao.Unbind();
}
