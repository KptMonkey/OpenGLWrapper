#include "HelloWorld.hpp"
#include <string>
#include <memory>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include "HeightMap.hpp"


#include "RenderAbstraction.hpp"
#include "Controller/Controller.hpp"
int main(int argc, char *argv[])
{
    RenderContext rctx;

    Shader shader;
    shader.bindShader( "shader/VertexShader.vert" );
    shader.bindShader( "shader/FragmentShader.frag" );

    Shader playerShader;
    playerShader.bindShader("shader/VertexShader.vert");
    playerShader.bindShader("shader/FragmentShader.frag");

    Shader depthShader;
    depthShader.bindShader("shader/SimpleDepth.vert");
    depthShader.bindShader("shader/SimpleDepth.frag");

    Shader shadowShader;
    shadowShader.bindShader("shader/shadowMap.vert");
    shadowShader.bindShader("shader/shadowMap.frag");

    Shader shadowShaderFloor;
    shadowShaderFloor.bindShader("shader/shadowMapFloor.vert");
    shadowShaderFloor.bindShader("shader/shadowMapFloor.frag");

    Shader debug;
    debug.bindShader("shader/DebugDepth.vert");
    debug.bindShader("shader/DebugDepth.frag");
    HeightMap hM;
    hM.LoadHeightMapFromImage("heightmap.r16");
    std::vector<float> quad = {
         // Positions        // Texture Coords
         -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
         -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
          1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
          1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     };

    std::vector<float> floor = {
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f
    };

    std::vector<glm::vec3>vertices = {
         glm::vec3(0.5f,  0.5f, 0.0f),  // Top Right
         glm::vec3(0.5f, -0.5f, 0.0f),  // Bottom Right
         glm::vec3(-0.5f, -0.5f, 0.0f),  // Bottom Left
         glm::vec3(-0.5f,  0.5f, 0.0f)   // Top Left
    };
    std::vector<unsigned int> indices = {  // Note that we start from 0!
        0, 1, 3,   // First Triangle
        1, 2, 3    // Second Triangle
    };
    std::vector<float> cube = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f

        };

    std::vector<Vertex> floorVertex;
    for ( int i = 0; i < 6; ++i ) {
        Vertex vert;
        vert.Position = glm::vec3(floor[i*8],floor[i*8+1],floor[i*8+2]);
        vert.Normal= glm::vec3(floor[i*8+3],floor[i*8+4],floor[i*8+5]);
        vert.TexPosition= glm::vec2(floor[i*8+6],floor[i*8+7]);
        floorVertex.push_back(vert);
    }

    for( int i = 0; i < floorVertex.size()/3; ++i ) {
        glm::vec3 edge1 = floorVertex[i*3 +1 ].Position - floorVertex[i*3].Position;
        glm::vec3 edge2 = floorVertex[i*3 +2 ].Position - floorVertex[i*3].Position;

        float dU1 = floorVertex[i*3 +1 ].TexPosition.x - floorVertex[i*3].TexPosition.x;
        float dU2 = floorVertex[i*3 +1 ].TexPosition.x - floorVertex[i*3].TexPosition.x;
        float dV1 = floorVertex[i*3 +1 ].TexPosition.y - floorVertex[i*3].TexPosition.y;
        float dV2 = floorVertex[i*3 +1 ].TexPosition.y - floorVertex[i*3].TexPosition.y;

        float f = 1.0f / (dU1 * dV2 - dU2 * dV1);

        glm::vec3 tangent;
//        tangent.x = f * (dV2 * edge1.x - dV1 * edge1.x);
//        tangent.y = f * (dV2 * edge1.x - dV1 * edge1.x);
//        tangent.z = f * (dV2 * edge1.x - dV1 * edge1.x);

        tangent = f * dV2*edge1 - dV1*edge2;
        floorVertex[i*3].Tangent += tangent;
        floorVertex[i*3+1].Tangent += tangent;
        floorVertex[i*3+2].Tangent += tangent;
    }

    for( auto & v : floorVertex ) {
        v.Tangent = glm::normalize(v.Tangent);
    }
    VertexArray ground;
    ground.createVertexArray(floorVertex);
    ground.describeVertexArray(0,3,GlTypes::Float, 11, GlBool::False,0);
    ground.describeVertexArray(1,3,GlTypes::Float, 11, GlBool::False,3);
    ground.describeVertexArray(2,2,GlTypes::Float, 11, GlBool::False,6);

    VertexArray cubeVA;
    cubeVA.createVertexArray(cube);
    cubeVA.describeVertexArray(0,3,GlTypes::Float, 6, GlBool::False,0);
    cubeVA.describeVertexArray(1,3,GlTypes::Float, 6, GlBool::False,3);

    VertexArray debugDepth;
    debugDepth.createVertexArray(quad);
    debugDepth.describeVertexArray(0,3,GlTypes::Float, 5, GlBool::False, 0);
    debugDepth.describeVertexArray(1,2,GlTypes::Float, 5, GlBool::False, 3);

    glm::mat4 projection = glm::perspective( 45.0f, 0.75f, 0.001f, 10000.0f );
    glm::mat4 model(1.0f);

    VertexArray terrainModel;
    terrainModel.createIndexBuffer(hM.vertices,hM.indices);
    terrainModel.describeVertexArray(0,3,GlTypes::Float, 5, GlBool::False,0);
    terrainModel.describeVertexArray(1,2,GlTypes::Float, 5, GlBool::False,3);


    /*Shadow Mapping*/
    //Create FBO
    GLuint depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    Texture depth;
    auto depthMap =depth.createDepthMap(1024, 1024);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap,0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER,0);

//    glViewport(0,0,shadow_width, shadow_height);
    float near_plane = 1.0f;
    float far_plane = 7.5f;
    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt( glm::vec3(1.0f,1.0f, 3.0f),
                                       glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f) );

    glm::mat4 lightSpace = lightProjection * lightView;

    Texture floorTexture("colormap.bmp");
    floorTexture.load();
    Texture floorTextureNormal("177_norm.JPG");
    floorTextureNormal.load();

    bool running = true;
    glm::vec3 moveClick(0.0f);
    glm::vec3 playerPos(0.0f, 0.0f, hM.getHeight(0.0f,0.0f));
    Controller controller;
    rctx.enableDepthTest();
    glm::vec2 mousePosition(0.0f);
    float yaw = 0.0f;
    float pitch = 0.0f;
    int i = 0;
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);

    while( running){
        controller.cameraIsometric( moveClick, projection, running );
//        controller.cameraFPS(mousePosition,yaw,pitch,running);
        glm::vec3 lightPos(std::cos(i*0.01), 0.0f, 0.3f);
        ++i;
        glViewport(0,0,1024,1024);
        depthShader.activate();
        depthShader["lightSpace"] =  lightSpace;
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        rctx.clearDepthBuffer();

        depthShader["model"]= model;
        ground.bindVertexArray();
        glDrawArrays(GL_TRIANGLES,0,6);


        glm::mat4 trans(1.0f);

        playerPos += (moveClick-playerPos)* (0.01f);
        playerPos.z = hM.getHeight(playerPos.x, playerPos.y);
        trans = glm::translate(trans, playerPos);
//        trans = glm::scale(trans, glm::vec3(0.2f));
        depthShader["model"] = trans;
        cubeVA.bindVertexArray();
        glDrawArrays(GL_TRIANGLES,0,36);

        rctx.draw(cubeVA, PrimitiveType::Triangles);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0,0,800,600);
        rctx.clearColor( 0.1f, 0.4f, 0.2f, 1.0f );
        rctx.clearColorBuffer();
        rctx.clearDepthBuffer();
        glm::vec3 eyePosition(1.0f, 1.0f, 1.0f);

        shadowShader.activate();
        shadowShader["model"] = trans;
        shadowShader["Color"] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        shadowShader["projection"] = projection;
        shadowShader["viewPos"] = glm::vec4( eyePosition.x, eyePosition.y, eyePosition.z, 1.0f);
        shadowShader["lightSpace"] = lightSpace;
        shadowShader["view"] = controller.getView();
        shadowShader["lightPos"] = lightPos;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        cubeVA.bindVertexArray();
        rctx.draw( cubeVA, PrimitiveType::Triangles );

        shadowShaderFloor.activate();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        floorTexture.activate(0);
        shadowShaderFloor["ground"] = 1;
        floorTextureNormal.activate(2);
        shadowShaderFloor["groundNormal"] = 2;

        shadowShaderFloor["projection"] = projection;
        shadowShaderFloor["viewPos"] = glm::vec4( eyePosition.x, eyePosition.y, eyePosition.z, 1.0f);
        shadowShaderFloor["lightSpace"] = lightSpace;
        shadowShaderFloor["view"] = controller.getView();
        shadowShaderFloor["model"] = model;
        shadowShaderFloor["Color"] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
        shadowShaderFloor["lightPos"] = lightPos;
        ground.bindVertexArray();
        rctx.draw( ground, PrimitiveType::Triangles );

         glViewport(0,0,800,600);
         shader.activate();
         floorTexture.activate(0);
         shader["terrain"]=0;
         shader["view"] = controller.getView();
         shader["projection"] = projection;
         glm::mat4 model(1.0f);
//         model = glm::scale(model,glm::vec3(0.3f));
         shader["model"] = model;
         terrainModel.bindVertexArray();
         rctx.drawIndex(shader,hM.indices.size()-2);


        rctx.swapBuffers();


    } // running
    return 0;
}
