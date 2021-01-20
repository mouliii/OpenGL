#pragma once
#include <GL/glew.h>
#include <assert.h>
#include <iostream>



class GLErrorCheck
{
public:
    void GLLogError()
    {
        while (GLenum error = glGetError())
        {
            std::cout << "[OpenGL error] (" << error << ")" << std::endl;
            assert(!error != GL_NO_ERROR);
        }
    }
};