#include "Renderer.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

    // TODO - delete?
    GLErrorCheck check;
    check.GLLogError();
}

void Renderer::DrawLine(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    glDrawElements(GL_LINE, ib.GetCount(), GL_UNSIGNED_INT, nullptr);

    // TODO - delete?
    GLErrorCheck check;
    check.GLLogError();
}

void Renderer::Clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}
