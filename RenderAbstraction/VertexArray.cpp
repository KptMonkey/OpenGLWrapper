#include "VertexArray.hpp"
int
map_to_gl( GlTypes t) {
    const int lut[] = {
            GL_FLOAT };
    return lut[ static_cast<int>( t )];
}

int
map_to_gl( GlBool b ) {
    const int lut[] = {
            GL_FALSE };
    return lut[ static_cast<int>( b )];
}

void
VertexArray::createVertexArrayObject( const std::vector<float> &vertices ) {
    glGenVertexArrays( 1, &m_VAO );
    glGenBuffers( 1, &m_VBO );
    glBindVertexArray( m_VAO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof( GLfloat ), &vertices[ 0 ], GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    m_NoV = vertices.size();
}
GLuint
VertexArray::getVertexArray() {
    return m_VAO;
}
void
VertexArray::describeVertex(int start, int number, GlTypes t, GlBool normalized ) {
    glBindVertexArray( m_VAO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glVertexAttribPointer( start, number, map_to_gl( t ), map_to_gl( normalized ), number * sizeof( GLfloat ), (GLvoid*)0 );
    glEnableVertexAttribArray( 0 );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}
void
VertexArray::bind() {
    glBindVertexArray(m_VAO);
}
