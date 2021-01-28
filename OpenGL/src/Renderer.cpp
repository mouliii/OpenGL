#include "Renderer.h"

Renderer::Renderer()
{
	
	vao.Bind();
	vbo.GenerateBuffer();
	vbo.SetData(nullptr, data.maxVertexCount * sizeof(QuadVertex));
	BufferLayout layout(
		{
			{DataType::FLOAT, 3, "aPosition"},
			{DataType::FLOAT, 4, "aColor"}
		}
	);
	vbo.SetLayout(layout);
	vao.AddVertexBuffer(vbo);
	data.VertexBufferBase = new QuadVertex[data.maxVertexCount];
	
	unsigned int* quadIndices = new unsigned int[data.maxIndicesCount];

	uint32_t offset = 0;
	for (size_t i = 0; i < data.maxIndicesCount; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;


		quadIndices[i + 3] = offset + 3;
		quadIndices[i + 4] = offset + 2;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	ibo.SetData(quadIndices, data.maxIndicesCount);
	delete[] quadIndices;

	shader.LoadShaders("res/shaders/VertexShader.shader", "res/shaders/FragmentShader.shader");
	shader.Bind();
	// set texture 0 valk.
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "OpenGL warning/error at renderer: " << err << std::endl;
	}
	
}

Renderer::~Renderer()
{
}

void Renderer::DrawQuad(const glm::vec3& pos, const glm::vec2& size, const glm::vec4& color)
{
	data.VertexBufferPtr->position = { pos.x, pos.y, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr++;	

	data.VertexBufferPtr->position = {pos.x + size.x, pos.y, 0.0f};
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->position = { pos.x + size.x, pos.y + size.y, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr++;

	data.VertexBufferPtr->position = { pos.x, pos.y + size.y, 0.0f };
	data.VertexBufferPtr->color = color;
	data.VertexBufferPtr++;

	data.quadIndexCount += 6;
}

void Renderer::DrawQuad(const glm::vec3& pos, const glm::vec2& size, unsigned int textureID)
{
}

void Renderer::Clear()
{
	glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::BeginBatch()
{
	// shader set viewproj
	data.quadIndexCount = 0;
	data.VertexBufferPtr = data.VertexBufferBase;
}

void Renderer::Flush()
{
	glDrawElements(GL_TRIANGLES, data.quadIndexCount, GL_UNSIGNED_INT, nullptr);
	
}

void Renderer::EndBatch()
{
	uint32_t dataSize = (uint8_t*)data.VertexBufferPtr - (uint8_t*)data.VertexBufferBase;
	vbo.SetSubData(data.VertexBufferBase, dataSize);
	Flush();
}
