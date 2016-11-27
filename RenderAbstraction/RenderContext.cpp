#define GLM_FORCE_RADIANS
#include "RenderContext.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


int
map_to_gl( PrimitiveType p) {
    const int lut[] = {
        GL_POINTS, GL_TRIANGLE_STRIP, GL_TRIANGLES
    };
    return lut[ static_cast<int>( p )];
}

int
map_to_gl( StateType s) {
    const int lut[] = {
            GL_DEPTH_TEST, GL_STENCIL, GL_BLEND, GL_CULL_FACE };
    return lut[ static_cast<int>( s )];
}
int map_to_gl ( DepthFunction d ) {
    const int lut[] = {
        GL_ALWAYS, GL_NEVER, GL_LESS, GL_EQUAL, GL_LEQUAL,
        GL_GREATER, GL_NOTEQUAL, GL_GEQUAL
    };
    return lut[ static_cast<int>( d )];
}

Display*
RenderContext::createDisplay( int x, int y )
{   m_Display = new Display() ;
    m_Display->createWindow( x, y );
    glewInit();
    glewExperimental = GL_TRUE;
    return m_Display;
}
void
RenderContext::enableDepthTest() {
    glEnable( GL_DEPTH_TEST );
}
void
RenderContext::writeToDepthBuffer() {
    glDepthMask( GL_TRUE );
    glDepthFunc( GL_ALWAYS );
}

void
RenderContext::readOnlyDepthBuffer() {
    glDepthMask( GL_FALSE );
}

void
RenderContext::setDepthFunction( DepthFunction d ) {
    glDepthFunc ( map_to_gl( d  ) );
}

void
RenderContext::clearColor( float r, float g, float b, float a ) {
    glClearColor( r, g, b, a );
}
void
RenderContext::clearColorBuffer() {
    glClear( GL_COLOR_BUFFER_BIT );
}
void
RenderContext::clearDepthBuffer() {
    glClear( GL_DEPTH_BUFFER_BIT );
}

void
RenderContext::draw( VertexArray & VertexArray,
                     const Shader & shader,
                     PrimitiveType p) {

    glm::mat4 view;
    // ToDo
    view = glm::lookAt( m_Display->camPos, m_Display->camPos + m_Display->Direction, m_Display->Up  );
    GLuint viewLoc = glGetUniformLocation( shader.m_Program, "view");
    glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );

    glDrawArrays( map_to_gl( p ), 0, VertexArray.getSizeOfArray() );
    glBindVertexArray( 0 );

}

