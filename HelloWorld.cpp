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
    glewInit();
    glewExperimental = GL_TRUE;

    Shader shader;

    shader.bindShader( "shader/VertexShader.vert" );
    shader.bindShader( "shader/FragmentShader.frag" );

    std::vector<GLfloat> vertices1 = {
        -0.5f, -0.5f, 0.0f, // Left
         0.5f, -0.5f, 0.0f, // Right
         0.0f,  0.5f, 0.0f  // Top
    };

    std::vector<GLfloat> cube = {
                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,

                -0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,

                 0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,

                -0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f, -0.5f,
                 0.5f, -0.5f,  0.5f,
                 0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f,  0.5f,
                -0.5f, -0.5f, -0.5f,

                -0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f, -0.5f,
                 0.5f,  0.5f,  0.5f,
                 0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f,  0.5f,
                -0.5f,  0.5f, -0.5f
    };
    std::vector<float> strip = {
        0.0f, 1.0f, 0.0f
        ,0.0f, 0.0f, 0.0f
        ,1.0f, 1.0f, 0.0f
        ,1.0f, 0.0f, 0.0f
        ,1.0f, 1.0f, 1.0f
        ,1.0f, 0.0f, 1.0f

    };
    VertexArray triangle;
    triangle.createVertexArrayObject( cube );
    triangle.describeVertex( 0, 3, GlTypes::Float, GlBool::True );
    glm::mat4 projection = glm::perspective( 45.0f, 0.75f, 0.01f, 100.0f );
    glm::mat4 model;

    rctx.enableState( StateType::Depth );
    //    Game loop
    while (!Display->windowClosed()) {
        triangle.bind();
        shader.setVariable( "model", model );
        shader.setVariable( "projection", projection);
        shader.activate();
        rctx.clearColor( 0.1f, 0.4f, 0.2f, 1.0f );
        rctx.clearColorBuffer();
        rctx.clearDepthBuffer();
        rctx.draw( triangle, shader, PrimitiveType::TiangleStrip );
        Display->updateDisplay();
    }
    // Properly de-allocate all resources once they've outlived their purpose
    return 0;
}

