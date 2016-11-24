#pragma once
#include <external/soil/inc/SOIL/SOIL.h>
#include <string>
#include <GL/glew.h>

class Texture {
public:
    Texture(std::string path );
    ~Texture(){}
    void
    activate( int textureUnit );
private:
    GLuint m_Texture;
};
