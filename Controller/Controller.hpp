#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class Controller {
public:
    Controller();

    void
    cameraIsometric(glm::vec3 & moveClick, glm::mat4 & projection, bool & running );

    void
    cameraFPS( glm::vec2 mousePosition, float yaw, float pitch, bool & running);

    void
    updateView();

    glm::mat4
    getView() { return m_View;}

private:

    glm::vec3 m_CameraPosition;
    glm::vec3 m_Forward;
    glm::vec3 m_Sideward;
    float     m_Pitch;
    float     m_Yaw;
    glm::mat4 m_View;
    SDL_Event m_Event;
};
