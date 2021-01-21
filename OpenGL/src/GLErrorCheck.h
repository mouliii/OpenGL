#pragma once
#include <GL/glew.h>
#include <assert.h>
#include <iostream>



class GLErrorCheck
{
public:
    bool errors = false;
    void GLLogError()
    {
        while (GLenum error = glGetError())
        {
            errors = true;
            std::cout << "[OpenGL error] (" << error << ")" << std::endl;
        }
        if (errors)
            assert(!"opengl error(s)!");
    }
    void ClearErrors()
    {
        while (GLenum error = glGetError())
        {
            std::cout << "[OpenGL error] (" << error << ")" << " clearing.." <<  std::endl;
        }
    }
};