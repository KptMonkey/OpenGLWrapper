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
VertexArray::createVertexArray( const std::vector<float> &vertices ) {
    glGenVertexArrays( 1, &m_VAO );
    glGenBuffers( 1, &m_VBO );
    glBindVertexArray( m_VAO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glBufferData( GL_ARRAY_BUFFER, vertices.size()*sizeof( GLfloat ), &vertices[ 0 ], GL_STATIC_DRAW );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
    m_NoV = vertices.size();
}
GLuint
VertexArray::getVertexArray() {
    return m_VAO;
}
void
VertexArray::describeVertexArray(int shaderPos, int number, GlTypes t, int nov, GlBool normalized, int stride ) {
    glBindVertexArray( m_VAO );
    glBindBuffer( GL_ARRAY_BUFFER, m_VBO );
    glVertexAttribPointer( shaderPos, number, map_to_gl( t ), map_to_gl( normalized ), nov * sizeof( GLfloat ), (GLvoid*)(stride * sizeof(GLfloat) ) );
    glEnableVertexAttribArray( shaderPos );
    glBindBuffer( GL_ARRAY_BUFFER, 0 );
    glBindVertexArray( 0 );
}
void
VertexArray::bindVertexArray() {
    glBindVertexArray(m_VAO);
}
