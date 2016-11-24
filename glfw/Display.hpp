#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Display{
public:
    Display();
    ~Display(){ glfwTerminate(); }

    bool
    windowClosed();

    void
    createWindow( int width, int height );

    void
    updateDisplay();
    void
    mouseInput(GLFWwindow* window, double posX, double posY );

    void
    keyInput( GLFWwindow* window , int key, int scancode, int action, int mods );

    void
    keyBoardMovement();



private:
    GLFWwindow* m_Window;
    glm::vec2       m_MousePos;
    glm::vec2       m_YawPitch;
    bool            m_FirstMInput;
    float           m_Sensitivity;

public:
    // ToDo: Remove static variable. Non static cause segmentation fault at the mouseinput :(
    static glm::vec3       Up;
    static glm::vec3       camPos;
    static glm::vec3       Direction;

};
