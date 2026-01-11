
#include <GL/glew.h>

#include <assert.h>
#include <stdio.h>

namespace Engine::RenderUtils
{
    static void checkForGLError()
    {
#ifdef DEBUG
        GLenum err = glGetError();
        if(err != GL_NO_ERROR)
        {
            fprintf(stderr, "openGL error occured, code: %d\n", err);
            assert(false);
        }
#endif
    }
} // namespace Engine::RenderUtils