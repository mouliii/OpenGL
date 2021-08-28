#include "Batch.h"

Batch::Batch(GLenum drawMode, std::string batchName, Shader shader, Primitive primitive, uint32_t maxBatchCount)
	:
	drawMode(drawMode), name(batchName), primitive(primitive), shader(shader),
	maxBatchCount(maxBatchCount), maxNumVertices(maxBatchCount* primitive.GetVertices().size()), maxNumIndices(maxBatchCount* primitive.GetIndexCount()),
	vao(),vbo(),ibo()
{
	uint32_t offset = 0;
	for (size_t i = 0; i < maxNumIndices; i+= primitive.GetVertices().size())
	{
		for (size_t j = 0; j < primitive.GetIndexCount(); j++)
		{
			indices.emplace_back(offset + primitive.GetIndices()[j]);
		}
		offset += primitive.GetVertices().size();
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

void Batch::Draw(const OrthoCamera& cam)
{
	curVertex = 0;
	numOfDrawCalls = 0;

	shader.Bind();
	glm::mat4 viewProj = cam.GetViewProjectionMatrix();
	shader.SetUniform4fv("uViewProj", viewProj);
	shader.SetUniform1f("textureIndex", 0);
	// todo
	auto id = TextureManager::GetTexture("res/textures/white1x1.png");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, *id);
	// TÄHÄN TEXTURE ID | päivitä updatessa, vector ?
	vao.Bind();

	uint32_t leftToDraw = vertices.size();
	uint32_t vertPointer = 0;
	glPolygonMode(GL_FRONT_AND_BACK, drawMode);
	while (leftToDraw >= maxNumVertices)
	{
		SetSubData(0, maxNumVertices, &vertices[vertPointer]);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		leftToDraw -= maxNumVertices;
		vertPointer += maxNumVertices;
		numOfDrawCalls++;
	}
	if (leftToDraw != 0)
	{
		SetSubData(leftToDraw, maxNumVertices - leftToDraw, nullptr);
		SetSubData(0, leftToDraw, &vertices[vertPointer]);
		glDrawElements(GL_TRIANGLES, leftToDraw / primitive.GetVertexCount() * primitive.GetIndexCount(), GL_UNSIGNED_INT, 0);
		numOfDrawCalls++;
	}
	vao.Unbind();
	textureIds.clear();
}


void Batch::Update(Primitive& primitive, const std::shared_ptr<uint32_t>& textureId)
{
	if (textureId != nullptr)
	{
		textureIds.emplace_back(*textureId);
	}
	for (size_t i = 0; i < primitive.GetVertexCount(); i++)
	{
		vertices[curVertex] = primitive.GetVertices()[i];
		curVertex++;
	}
}

void Batch::SetSubData()
{
	vbo.SetSubData(vertices);
}

void Batch::SetSubData(uint32_t offsetCount, uint32_t count, const void* data)
{
	vbo.SetSubData(offsetCount, count, data);
}

void Batch::Add(uint32_t count, const Primitive& primitive)
{
	for (size_t i = 0; i < count; i++)
	{
		for (size_t k = 0; k < primitive.GetVertexCount(); k++)
		{
			vertices.push_back({ Vec3f(Vec2f(), 0.0f), glm::vec4(), Vec2f() });
		}
	}
}

// ei toiminnasssa atm
void Batch::Remove()
{
	vertices.pop_back();
}