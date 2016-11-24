#define GLM_FORCE_RADIANS
#include "RenderAbstraction/RenderContext.hpp"
#include "Display.hpp"
#include <math.h>
#include <iostream>
#include <glm/gtx/transform.hpp>
#include <functional>
glm::vec3 Display::Direction = glm::vec3( 0.0, 0.0, -1.0f );
glm::vec3 Display::camPos = glm::vec3( 0.0f, 0.0f, 3.0f );
glm::vec3 Display::Up = glm::vec3( 0.0f, 1.0f, 0.0f );
bool keys[1024];
Display::Display() : m_Sensitivity( 0.05f), m_FirstMInput( true )
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

}

bool
Display::windowClosed() {
   return glfwWindowShouldClose( m_Window );
}

void
Display::keyInput( GLFWwindow *window, int key, int scancode, int action, int mods ){
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if ( action == GLFW_PRESS )
        keys[key] = true;
    else if ( action == GLFW_RELEASE )
        keys[key] = false;
}
void
Display::keyBoardMovement(){
    if ( keys[GLFW_KEY_W ] )
        Display::camPos += m_Sensitivity * Display::Direction;

    if ( keys[GLFW_KEY_S ] )
        Display::camPos -= m_Sensitivity * Display::Direction;

    if ( keys[GLFW_KEY_D] )
        Display::camPos += m_Sensitivity * glm::cross( Display::Direction, Display::Up );

    if ( keys[GLFW_KEY_A] )
        Display::camPos -= m_Sensitivity * glm::cross( Display::Direction, Display::Up );
}
void
Display::createWindow( int width, int height ) {
    m_Window = glfwCreateWindow( width, height, "Door",nullptr, nullptr );
    glfwMakeContextCurrent( m_Window );
    Display * display;
    glfwSetWindowUserPointer( m_Window, display );

    auto keyFunc = []( GLFWwindow* window, int key, int scancode, int action, int mode){
        auto hakuna = static_cast< Display * >( glfwGetWindowUserPointer( window ) );
        hakuna->keyInput( window, key, scancode, action,mode );
    };
    auto mouseFunc = []( GLFWwindow* window, double posX, double posY ){
        auto matata = static_cast< Display* >( glfwGetWindowUserPointer( window ) );
        matata->mouseInput( window, posX, posY );
    };
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback( m_Window, keyFunc );
    glfwSetCursorPosCallback( m_Window, mouseFunc );
}
void
Display::mouseInput( GLFWwindow* window, double posX, double posY ){
    glm::vec2 d;
    if ( m_FirstMInput ){
        m_MousePos[0] = posX;
        m_MousePos[1] = posY;
        m_FirstMInput = false;
    }
    d[0] = m_MousePos[0] - posX;
    d[1] = m_MousePos[1] - posY;
    d *= 0.001;
    m_MousePos[0] = posX;
    m_MousePos[1] = posY;
    auto horz = glm::mat3( glm::rotate( d[0], Display::Up ) ) * Display::Direction;
    auto right = glm::cross( horz, Display::Up );
    auto vert = glm::mat3( glm::rotate( d[1],right) )*glm::mat3( glm::rotate( d[0], Display::Up ) ) * Display::Direction;
    Display::Direction = glm::normalize( vert );
}

void
Display::updateDisplay() {
    glfwPollEvents();
    keyBoardMovement();
    glfwSwapBuffers( m_Window );
}
