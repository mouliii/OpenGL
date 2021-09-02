#include "Batch.h"

Batch::Batch(std::string batchName, Primitive batchType, uint32_t maxBatchCount)
	:
	name(batchName), shader("res/shaders/DefaultBatchVertex.shader", "res/shaders/DefaultBatchFragment.shader"), primitive(batchType),
	maxBatchCount(maxBatchCount), maxNumVertices(maxBatchCount * batchType.GetVertexCount()), maxNumIndices(maxBatchCount* batchType.GetIndexCount()),
	vao(),vbo(),ibo()
{
	uint32_t offset = 0;
	for (size_t i = 0; i < maxNumIndices; i+= batchType.GetVertexCount())
	{
		for (size_t j = 0; j < batchType.GetIndexCount(); j++)
		{
			indices.emplace_back(offset + batchType.GetIndices()[j]);
		}
		offset += batchType.GetVertexCount();
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
	for (size_t i = 0; i < 16; i++)
	{
		textureSlots.emplace_back(0);
	}
	// toimiiko?
	textureSlots[0] = TextureManager::LoadTexture("res/textures/white1x1.png")->GetId();
	// setup samplers
	int samplers[16];
	for (size_t i = 0; i < 16; i++)
	{
		samplers[i] = 1;
	}
	shader.Bind();
	auto loc = shader.GetUniformLocation("uTextures");
	glUniform1iv(loc, 16, samplers);
	shader.Unbind();
}

void Batch::BeginBatch()
{
	curVertexCount = 0;
	numOfDrawCalls = 0;
	textureSlotIndex = 1;
}

void Batch::EndBatch()
{
	SetSubData(0, curVertexCount, &vertices[0]);
}

void Batch::Flush()
{
	shader.Bind();
	shader.SetUniform4fv("uViewProj", cam->GetViewProjectionMatrix() );

	for (size_t i = 0; i < textureSlots.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureSlots[i]);
	}
	vao.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Batch::Update(const std::vector<Vertex>& vertices, const std::shared_ptr<Texture>& texture)
{
	if (textureSlotIndex > textureSlots.size() || curVertexCount >= maxNumVertices)
	{
		EndBatch();
		Flush();
		BeginBatch();
	}
	int textureIndex = 0;
	for (size_t i = 1; i < textureSlotIndex; i++)
	{
		if (texture->GetId() == textureSlots[i])
		{
			textureIndex = textureSlots[i];
		}
	}
	if (textureIndex == 0)
	{
		textureSlots[textureSlotIndex] = texture->GetId();
		textureSlotIndex++;
	}
	for (size_t i = 0; i < vertices.size(); i++)
	{
		this->vertices[curVertexCount] = vertices[i];
		curVertexCount++;
	}
}

void Batch::SetSubData(uint32_t offsetCount, uint32_t count, const void* data)
{
	// binds buffer
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