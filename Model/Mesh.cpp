#include "Mesh.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace animation {



Mesh::Mesh( std::vector<Vertex> vertices, std::vector<GLuint> indices) :
    m_vertices(vertices),
    m_indices(indices)
{
    setupMesh();
}

void
Mesh::setupMesh() {
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT, GL_TRUE, m_vertices.size()*sizeof(Vertex),(GLvoid*)0 );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int),&m_indices[0], GL_STATIC_DRAW);
    glBindVertexArray(0);

}

void
Mesh::draw() {
    glBindVertexArray( VAO );
    glDrawElements( GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

} // animation
