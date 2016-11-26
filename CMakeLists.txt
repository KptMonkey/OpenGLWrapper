cmake_minimum_required(VERSION 2.8)
#specify project
project( RenderAbstraction )
SET(CMAKE_CXX_FLAGS "-std=c++14")
# The following folder will be included
include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(external)
add_subdirectory(glm)
include_directories(
	external/glfw/include/GLFW/
	external/glew/include/
        glfw
        RenderAbstraction
        glm
)
set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW_1130
	soil
)
# I copied this part from somewhere... This is still magic for me
add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)
# End magic

file(COPY  ${CMAKE_SOURCE_DIR}/shader DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_executable(hello ${PROJECT_SOURCE_DIR}/HelloWorld.cpp
                glfw/Display
                RenderAbstraction/Shader
                RenderAbstraction/VertexArray
                RenderAbstraction/RenderContext
                RenderAbstraction/Texture
                RenderAbstraction/RenderAbstraction
                shader/VertexShader.vert
                shader/FragmentShader.frag
                )
#Doesn't build anymore without SDL2 oO
target_link_libraries(hello
        ${ALL_LIBS} ${SDL2_LIBRARIES}
)
