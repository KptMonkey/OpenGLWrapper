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
            GL_DEPTH, GL_STENCIL, GL_BLEND, GL_CULL_FACE };
    return lut[ static_cast<int>( s )];
}

Display*
RenderContext::createDisplay( int x, int y )
{   m_Display = new Display() ;
    m_Display->createWindow( x, y );
    return m_Display;
}
void
RenderContext::enableState( StateType s ) {
    glEnable( map_to_gl( s ) );
}

void
RenderContext::disableState( StateType s ) {
    glDisable( map_to_gl( s ) );
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

