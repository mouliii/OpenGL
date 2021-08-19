#include "QuadRenderer.h"

QuadRenderer::QuadRenderer(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
	:
	vao(), vbo(vertices), ibo(indices)
{
	this->vertices = vertices;
	this->indices = indices;
	quadShader.LoadShaders("res/shaders/QuadVertex.shader", "res/shaders/QuadFragment.shader");
	SetBuffers();
}

QuadRenderer::~QuadRenderer()
{
	glDeleteVertexArrays(1, &vao.id);
	glDeleteBuffers(1, &vao.id);
	glDeleteBuffers(1, &ibo.id);
}

void QuadRenderer::Draw(const OrthoCamera& camera)
{
	this->quadShader.Bind();
	glm::mat4 viewProj = camera.GetViewProjectionMatrix();
	this->quadShader.SetUniform4fv("uViewProj", viewProj);
	vao.Bind();

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void QuadRenderer::SetBuffers()
{
	vao.Bind();
	vbo.Bind();
	ibo.Bind();

	vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
	vao.LinkAttribute(vbo, 1, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	vao.LinkAttribute(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

	vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();
}
