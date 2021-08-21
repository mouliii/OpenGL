#include "Batch.h"

Batch::Batch(GLenum drawMode, std::string batchName, Shader shader, Primitive primitive, uint32_t maxBatchCount)
	:
	drawMode(drawMode), name(batchName),
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

void Batch::BeginFrame()
{
	// siirrä draw?
	curVertex = 0;
	numOfDrawCalls = 0;
}

void Batch::Draw(Shader* shader, const OrthoCamera& cam)
{
	shader->Bind();
	glm::mat4 viewProj = cam.GetViewProjectionMatrix();
	shader->SetUniform4fv("uViewProj", viewProj);
	vao.Bind();

	uint32_t leftToDraw = vertices.size();
	uint32_t vertPointer = 0;
	while (leftToDraw >= maxNumVertices)
	{
		SetSubData(0, maxNumVertices, &vertices[vertPointer]);
		glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);
		leftToDraw -= maxNumVertices;
		vertPointer += maxNumVertices;
		numOfDrawCalls++;
	}
	SetSubData(0, leftToDraw, &vertices[vertPointer]);
	glDrawElements(drawMode, indices.size(), GL_UNSIGNED_INT, 0);
	vao.Unbind();

	numOfDrawCalls++;
}


void Batch::Update(const Primitive& primitive)
{
	for (size_t i = 0; i < primitive.GetVertices().size(); i++)
	{
		vertices[curVertex] = primitive.GetVertices()[i];
		curVertex++;
	}
}

void Batch::SetSubData()
{
	vbo.SetSubData(vertices);
}

void Batch::SetSubData(uint32_t offset, uint32_t count, const void* data)
{
	vbo.SetSubData(offset, count, data);
}

void Batch::Add(Primitive primitive, uint32_t count)
{
	for (size_t i = 0; i < count; i++)
	{
		for (size_t k = 0; k < primitive.GetVertices().size(); k++)
		{
			vertices.push_back({ Vec3f(Vec2f(), 0.0f), glm::vec4(), Vec2f() });
		}
	}
}

void Batch::Remove()
{
	vertices.pop_back();
}




/*
Batch::Batch(GLenum drawMode, std::string batchName, Shader shader, Primitive primitive, uint32_t maxQuadCount)
	:
	drawMode(drawMode), name(batchName), maxQuadCount(maxQuadCount), maxNumVertices(maxQuadCount * 4), maxNumIndices(maxQuadCount * 6),
	vao(),vbo(),ibo()
{
	uint32_t offset = 0;
	for (size_t i = 0; i < maxNumIndices; i++)
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

	// testi vain
	//priv = Quad(Vec2f(0.0f,0.0f),Vec2f(0.0f,0.0f),glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}
*/