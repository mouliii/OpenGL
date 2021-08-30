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

void Renderer::Draw(Primitive& primitive, const OrthoCamera& cam, Texture* texture, Shader* shader)
{
	// todo cache vai ?
	const auto defaultTexture = TextureManager::GetTexture("res/textures/white1x1.png");
	
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
	if (texture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->GetId() );
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, defaultTexture->GetId());
	}
	vao.Bind();
	vbo.Bind();
	vbo.SetData(primitive.GetVertexCount() * sizeof(Vertex), primitive.GetVertices().data());
	ibo.Bind();
	ibo.SetData(primitive.GetIndices());
	glDrawElements(GL_TRIANGLES, primitive.GetIndexCount(), GL_UNSIGNED_INT, 0);
	vao.Unbind();
}