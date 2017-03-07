#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <string>
#include <Texture.hpp>
#include "RenderAbstraction/Shader.hpp"
#include "Display.hpp"
namespace animation {


struct Vertex {
    glm::vec3 Pos;
};

struct BoneData {
    std::vector< unsigned int > ID;
    std::vector< float > Weights;
};

class Mesh {
public:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    Mesh( std::vector<Vertex> vertices, std::vector<GLuint> indices);
    void draw( );

private:
    GLuint VAO, VBO, EBO;

    void setupMesh();
};

} // animation
