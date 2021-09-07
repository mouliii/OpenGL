#include "Batch.h"

Batch::Batch(std::string batchName, Primitive batchType, uint32_t maxBatchCount)
	:
	name(batchName), shader("res/shaders/DefaultBatchVertex.shader", "res/shaders/DefaultBatchFragment.shader"), 
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
	for (size_t i = 0; i < maxNumVertices; i++)
	{
		vertices.push_back({ Vec3f(Vec2f(), 0.0f), glm::vec4(), Vec2f(), 0.0f });
	}
	vao.Bind();
	vbo.Bind();
	vbo.SetData(maxNumVertices * sizeof(Vertex), nullptr);
	ibo.Bind();
	ibo.SetData(indices);

	vao.LinkAttribute(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, pos));
	vao.LinkAttribute(vbo, 1, 4, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
	vao.LinkAttribute(vbo, 2, 2, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoord));
	vao.LinkAttribute(vbo, 3, 1, GL_FLOAT, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textureIndex));

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
		samplers[i] = i;
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
	// limit indices ? todo samoiten tex slotit
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

void Batch::Update(Mesh& mesh)
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
		if (mesh.GetTexture()->GetId() == textureSlots[i])
		{
			textureIndex = textureSlots[i];
			mesh.SetTextureIndex((float)i);
		}
	}
	if (textureIndex == 0)
	{
		textureSlots[textureSlotIndex] = mesh.GetTexture()->GetId();
		mesh.SetTextureIndex((float)textureSlotIndex);
		textureSlotIndex++;
	}
	for (size_t i = 0; i < mesh.GetVertices().size(); i++)
	{
		this->vertices[curVertexCount] = mesh.GetVertices()[i];
		curVertexCount++;
	}
}

void Batch::Update(Primitive& primi, const Texture* texture)
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
			for (size_t j = 0; j < primi.vertices.size(); j++)
			{
				primi.vertices[j].textureIndex = (float)i;
			}
		}
	}
	if (textureIndex == 0)
	{
		textureSlots[textureSlotIndex] = texture->GetId();
		for (size_t i = 0; i < primi.vertices.size(); i++)
		{
			primi.vertices[i].textureIndex = (float)textureSlotIndex;
		}
		textureSlotIndex++;
	}
	for (size_t i = 0; i < primi.GetVertices().size(); i++)
	{
		this->vertices[curVertexCount] = primi.GetVertices()[i];
		curVertexCount++;
	}
}

void Batch::SetSubData(uint32_t offsetCount, uint32_t count, const void* data)
{
	// binds buffer
	vbo.SetSubData(offsetCount, count, data);
}