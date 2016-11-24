#pragma once
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader{
public:
    Shader();
    ~Shader(){}
    void
    bindShader( std::string path );

//    private:

    std::string
    readShader( std::string path );

    void
    setVariable( std::string name, glm::mat4 matrix );
    void
    setTexture( std::string name, int number );
    void
    activate();

    GLuint m_Program;
};
