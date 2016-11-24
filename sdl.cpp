//
//  HelloWorld.cpp
//  CPP
//
//  Created by <author> on 13/08/2016.
//
//


#include "Shader.hpp"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <string>

#include <iostream>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>


//// OpenGL headers
//#define GLEW_STATIC
//#include <GL/glew.h>
//#include <GL/glu.h>
//#include <GL/gl.h>

//// SDL headers
//#include <SDL_main.h>
//#include <SDL.h>
//#include <SDL_opengl.h>

//bool quit;

//SDL_Window* window;
//SDL_GLContext glContext;
//SDL_Event event;

//int main(int argc, char *argv[])
//{
//    quit = false;

//    //Use OpenGL 3.1 core
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
//    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

//    // Initialize video subsystem
//    if(SDL_Init(SDL_INIT_VIDEO) < 0)
//    {
//        // Display error message
//        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
//        return false;
//    }
//    else
//    {
//        // Create window
//        window = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
//        if( window == NULL )
//        {
//            // Display error message
//            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
//            return false;
//        }
//        else
//        {
//            // Create OpenGL context
//            glContext = SDL_GL_CreateContext(window);

//            if( glContext == NULL )
//            {
//                // Display error message
//                printf( "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError() );
//                return false;
//            }
//            else
//            {
//                // Initialize glew
//                glewInit();
//            }
//        }
//    }

//    // Game loop
//    while (!quit)
//    {
//        while(SDL_PollEvent(&sdlEvent) != 0)
//        {
//            // Esc button is pressed
//            if(sdlEvent.type == SDL_QUIT)
//            {
//                quit = true;
//            }
//        }

//        // Set background color as cornflower blue
//        glClearColor(0.39f, 0.58f, 0.93f, 1.f);
//        // Clear color buffer
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        // Update window with OpenGL rendering
//        SDL_GL_SwapWindow(window);
//    }

//    //Destroy window
//    SDL_DestroyWindow(window);
//    window = NULL;

//    //Quit SDL subsystems
//    SDL_Quit();

//    return 0;
//}

// The MAIN function, from here we start the application and run the game loop
SDL_Window * window;
SDL_GLContext glContext;
SDL_Event event;
bool quit ;
int main()
{

    quit = false;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    if( SDL_Init( SDL_INIT_VIDEO ) < 0.0 );
    {
        std::cout << "penis";
    }
    std::string testi = "Penis";
    window  = SDL_CreateWindow(testi.c_str(),
                     SDL_WINDOWPOS_CENTERED,
                     SDL_WINDOWPOS_CENTERED,
                     800,
                     600,
                     SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if ( window == NULL ) {std::cout<<"penis";return -1;}

    glContext = SDL_GL_CreateContext( window );
    if (glContext == NULL){
        std::cout << "penis";
    }
    glewExperimental=GL_TRUE;
    glewInit();

    Shader shader;
    std::string penis= shader.readShader( "VertexShader.vert" );

    std::string busen = shader.readShader( "FragmentShader.frag");


    std::cout << penis;
    std::cout << busen;

    const GLchar * test = penis.c_str();
    std::cout << busen << penis;
    const GLchar * mest = busen.c_str();
    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &test, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &mest, NULL);
    glCompileShader(fragmentShader);
    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // Set up vertex data (and buffer(s)) and attribute pointers
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f, // Left
         0.5f, -0.5f, 0.0f, // Right
         0.0f,  0.5f, 0.0f  // Top
    };
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)



    // Game loop
//    while (!glfwWindowShouldClose(window))
//    bool bla = true;
//    while(bla){

//    while(SDL_PollEvent(&event))
//        {
//        if( event.type == SDL_QUIT != 0 )
//        {
//            bla = false;
//        }
//        }
//        glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//         SDL_GL_SwapWindow( window );
//    }
        while (!quit)
        {
            while(SDL_PollEvent(&event) != 0)
            {
                // Esc button is pressed
                if(event.type == SDL_QUIT)
                {
                    quit = true;
                }
            }

            // Set background color as cornflower blue
            glClearColor(0.39f, 0.58f, 0.93f, 1.f);
            // Clear color buffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Update window with OpenGL rendering
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            SDL_GL_SwapWindow(window);
        }
    SDL_GL_DeleteContext( glContext );
    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}


