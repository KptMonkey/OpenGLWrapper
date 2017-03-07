#pragma once
#include "VertexArray.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include <memory>
#include <SDL2/SDL.h>

enum class PrimitiveType { Points, TiangleStrip, Triangles };
enum class StateType { Depth, Stencil, Blend, Culling };
enum class DepthFunction { Always, Never, Less, Equal, LessEqual, Greater, NotEqual, GreaterEqual };
class RenderContext {
public:
    RenderContext();
    ~RenderContext(){}

    void
    draw( VertexArray &VertexArray,
          PrimitiveType p );
    void
    drawIndex( Shader const & shader, int size);

    void
    enableDepthTest();

    void
    writeToDepthBuffer();
    void
    readOnlyDepthBuffer();
    void
    setDepthFunction( DepthFunction d );

    void
    disableState( StateType s );

    void
    clearColor ( float r, float g, float b, float a );

    void
    clearColorBuffer();
    void
    clearDepthBuffer();

    VertexArray
    createVertexArray();

    void
    swapBuffers();

private:
    SDL_Window * m_Window;

};
