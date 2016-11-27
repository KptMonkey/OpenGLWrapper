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
    createVertexArray( const std::vector<float> &vertices );
    GLuint
    getVertexArray();
    int
    getSizeOfArray(){ return m_NoV; }
    void
    describeVertexArray( int shaderPos, int number, GlTypes t, int nov, GlBool normalized, int stride );
    void
    bindVertexArray();

private:
    GLuint m_VBO;
    GLuint m_VAO;
    int m_NoV;
};
