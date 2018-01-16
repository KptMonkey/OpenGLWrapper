#define GLM_FORCE_RADIANS
#include "RenderContext.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <GL/glew.h>


RenderContext::RenderContext() {


    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);

    m_Window = SDL_CreateWindow("penis",
            0, 0,
            800, 600,
            SDL_WINDOW_OPENGL /*| SDL_WINDOW_RESIZABLE*/);

    SDL_GL_SetSwapInterval(1); // v-sync
    auto gl_context = SDL_GL_CreateContext(m_Window);

    glewExperimental = true;
    glewInit();

    m_Quad.createVertexArray(quad);
    m_Quad.describeVertexArray(0,2,GlTypes::Float, 4, GlBool::False, 0);
    m_Quad.describeVertexArray(1,2,GlTypes::Float, 4, GlBool::False, 2);
}

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

void
RenderContext::enableDepthTest() {
    glEnable( GL_DEPTH_TEST );
}

void
RenderContext::writeToDepthBuffer() {
    glDepthMask( GL_TRUE );
    glDepthFunc( GL_LESS );
}

void
RenderContext::readOnlyDepthBuffer() {
    glDepthMask( GL_FALSE ); //wrong
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
                     PrimitiveType p) {
    glDrawArrays( map_to_gl( p ), 0, VertexArray.getSizeOfArray() );
    glBindVertexArray( 0 );

}
void
RenderContext::drawIndex(PrimitiveType p, int size) {
    glDrawElements(map_to_gl( p ), size, GL_UNSIGNED_INT, 0);
    glBindVertexArray( 0 );

}

void
RenderContext::drawDepthMap(Shader & shader, Texture & texture) {
    glViewport(0,0,800,600);
    shader.activate();
    texture.activate(0);
    clearDepthBuffer();
    m_Quad.bindVertexArray();
    draw(m_Quad,PrimitiveType::Triangles);
}

void
RenderContext::swapBuffers() {
    SDL_GL_SwapWindow(m_Window);
}
