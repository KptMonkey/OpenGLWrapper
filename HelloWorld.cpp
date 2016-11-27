#include "HelloWorld.hpp"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>
#include <memory>
#include <vector>
#include "Display.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "RenderAbstraction.hpp"
int main(int argc, char *argv[])
{
    RenderContext rctx;

    Display * Display = rctx.createDisplay( 800, 600 );
//    glewInit();
//    glewExperimental = GL_TRUE;

    Shader shader;

    shader.bindShader( "shader/VertexShader.vert" );
    shader.bindShader( "shader/FragmentShader.frag" );


    std::vector<GLfloat> cube =  {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
               0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
               0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
               0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
              -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
              -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

              -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
               0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
               0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
               0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
              -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
              -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

              -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
              -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
              -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
              -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
              -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
              -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

               0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
               0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
               0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
               0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
               0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
               0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

              -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
               0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
               0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
               0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
              -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
              -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

              -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
               0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
               0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
               0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
              -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
              -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    VertexArray triangle;
    triangle.createVertexArray( cube );
    triangle.describeVertexArray( 0, 3, GlTypes::Float, 5, GlBool::True, 0 );
    triangle.describeVertexArray( 1, 2, GlTypes::Float, 5, GlBool::True, 3 );
    glm::mat4 projection = glm::perspective( 45.0f, 0.75f, 0.01f, 100.0f );
    glm::mat4 model;
    shader.activate();
    Texture texture("texture.png",shader.m_Program );
    shader.setVariable( "model", model );
    shader.setVariable( "projection", projection);
    texture.activate(0,"lambda");
    rctx.enableDepthTest();
    //    Game loop
    while (!Display->windowClosed()) {
        rctx.clearColor( 0.1f, 0.4f, 0.2f, 1.0f );
        rctx.clearColorBuffer();
        rctx.clearDepthBuffer();
        triangle.bindVertexArray();
        rctx.draw( triangle, shader, PrimitiveType::Triangles );
        Display->updateDisplay();
    }
    // Properly de-allocate all resources once they've outlived their purpose
    return 0;
}

