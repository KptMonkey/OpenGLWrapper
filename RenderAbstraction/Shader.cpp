#include "Shader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <glm/gtc/type_ptr.hpp>


Shader::Shader():m_Program( glCreateProgram() ){}

std::string
Shader::readShader( std::string path ){

    std::ifstream file( path );
    std::string line;
    std::ostringstream oss;
    if ( !file )
    {
        std::cerr << "Couldn't read shader file! \n";
        return "Fuck";
    }
    else{
        while( std::getline( file, line ) ){
        oss << line << "\n";
        }
        return oss.str();
    }
}
void
Shader::bindShader( std::string path ) {

    auto shaderString = readShader( path );
    const GLchar * shaderSource = shaderString.c_str();
    auto shaderType = path.substr( (path.size() - 4),path.size() );

    GLuint shader;
    if ( shaderType == "frag") shader = glCreateShader( GL_FRAGMENT_SHADER );
    if ( shaderType == "vert") shader = glCreateShader( GL_VERTEX_SHADER );
    if ( shaderType == "geom") shader = glCreateShader( GL_GEOMETRY_SHADER );

    glShaderSource( shader, 1, &shaderSource, nullptr );
    glCompileShader( shader );
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv( shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog( shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader( m_Program, shader);
    glLinkProgram( m_Program );
    glGetProgramiv( m_Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog( m_Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader( shader );
}

void
Shader::setVariable( std::string name, glm::mat4 matrix ) {
    GLuint modelLoc = glGetUniformLocation( m_Program, name.c_str() );
    glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( matrix ) );
}
void
Shader::setTexture( std::string name, int number ) {
    glUniform1i( glGetUniformLocation( m_Program, name.c_str() ), number );
}

void
Shader::activate() {
    glUseProgram( m_Program );
}
