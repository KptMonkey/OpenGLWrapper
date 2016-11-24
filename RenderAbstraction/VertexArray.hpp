#pragma once
#include <GL/glew.h>
#include <vector>
enum class GlTypes { Float };
enum class GlBool { True, False };
class VertexArray{
public:
    VertexArray(){}
    ~VertexArray(){}
    void
    createVertexArrayObject(const std::vector<float> &vertices );
    GLuint
    getVertexArray();
    int
    getSizeOfArray(){ return m_NoV; }
    void
    describeVertex( int start, int number, GlTypes t, GlBool normalized );
    void
    bind();

private:
    GLuint m_VBO;
    GLuint m_VAO;
    int m_NoV;
};
