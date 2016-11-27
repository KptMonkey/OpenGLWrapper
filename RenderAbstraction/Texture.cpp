#include "Texture.hpp"
int
map_to_gl( int textureUnit ) {
    const int lut[] = {
        GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2
    };
    return lut[ textureUnit ];
}

Texture::Texture( std::string path, GLuint & program ) : m_Program( program ) {
    int width, height;
    unsigned char * image = SOIL_load_image( path.c_str(), &width, &height, 0, SOIL_LOAD_RGB );
    glGenTextures( 1, &m_Texture );
    glBindTexture( GL_TEXTURE_2D, m_Texture );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, 0);
    SOIL_free_image_data( image );
}
void
Texture::activate( int textureUnit, std::string variableName ) {
    glActiveTexture( map_to_gl( textureUnit ) );
    glBindTexture( GL_TEXTURE_2D, m_Texture );
    glUniform1i(glGetUniformLocation( m_Program, variableName.c_str() ), textureUnit );
}
