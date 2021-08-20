#include "Batch.h"

Batch::Batch(GLenum drawMode, std::string batchName, Shader shader, uint32_t maxQuadCount)
	:
	drawMode(drawMode), name(batchName), maxQuadCount(maxQuadCount), maxNumVertices(maxQuadCount * 4), maxNumIndices(maxQuadCount * 6),
	vao(),vbo(),ibo()
{
	uint32_t offset = 0;
	for (size_t i = 0; i < maxQuadCount; i+= 6)
	{
		// triangle 1
		indices.emplace_back(offset + 0);
		indices.emplace_back(offset + 1);
		indices.emplace_back(offset + 2);
		// triangle 2
		indices.emplace_back(offset + 2);
		indices.emplace_back(offset + 3);
		indices.emplace_back(offset + 0);
		offset += 4;
	}
	vao.Bind();
	vbo.Bind();
	vbo.SetData(maxNumVertices * sizeof(Vertex), nullptr);
	ibo.Bind();
	ibo.SetData(indices);

	vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
	vao.LinkAttribute(vbo, 1, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	vao.LinkAttribute(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));

	vao.Unbind();
	vbo.Unbind();
	ibo.Unbind();

}

void Batch::BeginFrame()
{
	curVertex = 0;
}

void Batch::Draw(Shader* shader, const OrthoCamera& cam)
{
	SetSubData();
	shader->Bind();
	glm::mat4 viewProj = cam.GetViewProjectionMatrix();
	shader->SetUniform4fv("uViewProj", viewProj);
	vao.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	vao.Unbind();
}

void Batch::Add(uint32_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		for (size_t k = 0; k < 4; k++)
		{
			vertices.push_back({ Vec3f(Vec2f(), 0.0f), glm::vec4(), Vec2f() });
		}
	}
}

void Batch::Remove()
{
	vertices.pop_back();
}

void Batch::Update(const std::vector<Vertex>& vertices)
{
	this->vertices[curVertex] = vertices[0];
	this->vertices[curVertex+1] = vertices[1];
	this->vertices[curVertex+2] = vertices[2];
	this->vertices[curVertex+3] = vertices[3];
	curVertex += 4;
	// if curQuad > maxQuads -> flush
}

void Batch::SetSubData()
{
	vbo.SetSubData(vertices);
}