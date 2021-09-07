#include "Renderer.h"

Renderer::Renderer(OrthoCamera* camera)
	:shader("res/shaders/DefaultVertex.shader", "res/shaders/DefaultFragment.shader"),
	vao(),vbo(),ibo(),
	camera(camera)
{
	vao.Bind();
	vbo.Bind();
	//vbo.SetData(vertices.size() * sizeof(Vertex), vertices.data());
	ibo.Bind();
	//ibo.SetData(indices);

	vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
	vao.LinkAttribute(vbo, 1, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	vao.LinkAttribute(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
	vao.LinkAttribute(vbo, 3, 1, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureIndex));

	vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();

	defaultTexture = TextureManager::LoadTexture("res/textures/white1x1.png");
	shader.Unbind();
	//// 1x1 white texture
	//uint32_t wpId;
	//glCreateTextures(GL_TEXTURE_2D, 1, &wpId);
	//glBindTexture(GL_TEXTURE_2D, wpId);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//uint32_t color = 0xffffffff;
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);
}

void Renderer::BeginFrame()
{
}

void Renderer::EndFrame()
{
}

void Renderer::DrawTexture(const Vec2f& pos, const Texture* texture)
{
	Quad q(pos, Vec2f(texture->GetWidth() / 2.f, texture->GetHeight() / 2.f), glm::vec4(1.f));
	Draw(q, texture);
}

void Renderer::Draw(Primitive& primitive, const Texture* texture)
{
	// set default shader
	this->shader.Bind();
	glm::mat4 viewProj = camera->GetViewProjectionMatrix();
	this->shader.SetUniform4fv("uViewProj", viewProj);
	this->shader.SetUniform4f("uColor", glm::vec4(1.f));
	// activate texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());
	// draw
	ImmediateDraw(primitive);
}

void Renderer::Draw(Primitive& primitive, const glm::vec4& color)
{
	// set default shader
	this->shader.Bind();
	glm::mat4 viewProj = camera->GetViewProjectionMatrix();
	this->shader.SetUniform4fv("uViewProj", viewProj);
	this->shader.SetUniform4f("uColor", color);
	// activate default texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, defaultTexture->GetId() );
	ImmediateDraw(primitive);
}
void Renderer::Draw(Primitive& primitive, const Texture* texture, glm::mat4& transform, Shader& shader)
{
	shader.Bind(); 
	glm::mat4 viewProj = camera->GetViewProjectionMatrix();
	shader.SetUniform4fv("uViewProj", viewProj);
	shader.SetUniform4f("uColor", glm::vec4(1.f));
	shader.SetUniform4fv("uTransform", transform);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());
	ImmediateDraw(primitive.GetVertices(), primitive.GetIndices());
	shader.Unbind();
}
void Renderer::Draw(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const Texture* texture)
{
	// set default shader
	this->shader.Bind();
	glm::mat4 viewProj = camera->GetViewProjectionMatrix();
	this->shader.SetUniform4fv("uViewProj", viewProj);
	this->shader.SetUniform4f("uColor", glm::vec4(1.f));
	// activate texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());
	// draw
	ImmediateDraw(vertices, indices);
}
void Renderer::Draw(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const glm::vec4& color)
{
	// set default shader
	this->shader.Bind();
	glm::mat4 viewProj = camera->GetViewProjectionMatrix();
	this->shader.SetUniform4fv("uViewProj", viewProj);
	this->shader.SetUniform4f("uColor", color);
	// activate default texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, defaultTexture->GetId());
	ImmediateDraw(vertices, indices);
}

void Renderer::Draw(Mesh& mesh)
{
	Draw(mesh.GetVertices(), mesh.GetIndices(), mesh.GetTexture().get() );
}

void Renderer::ImmediateDraw(Primitive& primitive)
{
	vao.Bind();
	vbo.Bind();				// todo: ei toimi jos eri *Vertex* struct
	vbo.SetData(primitive.GetVertexCount() * sizeof(Vertex), primitive.GetVertices().data());
	ibo.Bind();
	ibo.SetData(primitive.GetIndices());
	glDrawElements(GL_TRIANGLES, primitive.GetIndexCount(), GL_UNSIGNED_INT, 0);
	vao.Unbind();
}

void Renderer::ImmediateDraw(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
{
	vao.Bind();
	vbo.Bind();						// todo: ei toimi jos eri *Vertex* struct
	vbo.SetData(vertices.size() * sizeof(Vertex), vertices.data());
	ibo.Bind();
	ibo.SetData(indices);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	vao.Unbind();
}
