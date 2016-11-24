cmake_minimum_required(VERSION 2.8)
#specify project
project( RenderAbstraction )
SET(CMAKE_CXX_FLAGS "-std=c++11")
# The following folder will be included
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(external)
include_directories(
	external/glfw/include/GLFW/
	external/glew/include/
        glfw
        RenderAbstraction
		
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
add_executable(hello ${PROJECT_SOURCE_DIR}/HelloWorld.cpp
                glfw/Display
                RenderAbstraction/Shader
                RenderAbstraction/VertexArray
                RenderAbstraction/RenderContext
                RenderAbstraction/Texture
                RenderAbstraction/RenderAbstraction)
target_link_libraries(hello
        ${ALL_LIBS} ${SDL2_LIBRARIES}
)
