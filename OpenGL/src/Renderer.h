#pragma once

#include "GLErrorCheck.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"


class Renderer
{
public:

	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void DrawLine(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
	void Clear() const;
private:

};