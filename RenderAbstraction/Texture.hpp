#pragma once
#include <external/soil/inc/SOIL/SOIL.h>
#include <string>
#include <GL/glew.h>

class Texture {
public:
    Texture(std::string path, GLuint & program );
    ~Texture(){}
    void
    activate( int textureUnit, std::string variableName );
private:
    GLuint m_Texture;
    GLuint & m_Program;
};
