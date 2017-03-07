#pragma once
#include <string>
#include <GL/glew.h>

class Texture {
public:
    Texture(){}
    Texture( std::string path );
    ~Texture(){}
    void
    activate( int textureUnit );
    void
    load();

    GLuint
    createDepthMap(int width, int height);
private:
    GLuint m_Texture;
    std::string m_Path;
};
