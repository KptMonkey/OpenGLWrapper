#pragma once
#include "VertexArray.hpp"
#include "Shader.hpp"
#include <glm/glm.hpp>
#include "glfw/Display.hpp"
#include <memory>
enum class PrimitiveType { Points, TiangleStrip, Triangles };
enum class StateType { Depth, Stencil, Blend, Culling };
class RenderContext {
public:
    RenderContext(){}
    ~RenderContext(){}

    void
    draw( VertexArray &VertexArray,
          Shader const & shader ,
          PrimitiveType p );

    Display*
    createDisplay( int x, int y );

    void
    enableState( StateType s );

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
private:
    Display* m_Display;
};
