//#include "Control.hpp"
//#include "glm/gtx/transform.hpp"
//#include <iostream>
//glm::vec3 Control::camPos = glm::vec3( 0.0f, 0.0f, 3.0f );
//glm::vec3 Control::Direction = glm::vec3( 0.0f, 0.0f, -1.0f );
//glm::vec3 Control::Up = glm::vec3( 0.0f, 1.0f, 0.0f );
//bool _keys[1024];

//void
//Control::keyInput( GLFWwindow* window,  int key, int scancode, int action, int mods ){
//    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS )
//        glfwSetWindowShouldClose(window, GLFW_TRUE);
//    if ( action == GLFW_PRESS )
//        _keys[key] = true;
//    else if ( action == GLFW_RELEASE )
//        _keys[key] = false;
//}

//void
//Control::keyBoardMovement(){
//    if ( _keys[GLFW_KEY_W ] )
//        Control::camPos += m_Sensitivity * Control::Direction;

//    if ( _keys[GLFW_KEY_S ] )
//        Control::camPos -= m_Sensitivity * Control::Direction;

//    if ( _keys[GLFW_KEY_D] )
//        Control::camPos += m_Sensitivity * glm::cross( Control::Direction, Control::Up );

//    if ( _keys[GLFW_KEY_A] )
//        Control::camPos -= m_Sensitivity * glm::cross( Control::Direction, Control::Up );
//}

//void
//Control::mouseInput( GLFWwindow* window, float posX, float posY ){
//    glm::vec2 d;
//    if ( m_FirstMInput ){
//        Control::m_MousePos[0] = posX;
//        m_MousePos[1] = posY;
//    }
//    d[0] = m_MousePos[0] - posX;
//    d[1] = m_MousePos[1] - posY;

//    m_MousePos[0] = posX;
//    m_MousePos[1] = posY;

//    m_YawPitch += m_Sensitivity * d;

//    auto dir = glm::mat3( glm::rotate( d[0], Control::Up ) ) * Control::Direction;
//    auto right = glm::cross( dir, Control::Up );
//    dir = glm::mat3( glm::rotate( d[1],right) ) * Control::Direction;
//    Control::Direction = glm::normalize( dir );

//}


