cmake_minimum_required(VERSION 2.8)
#specify project
project( RenderAbstraction )
SET(CMAKE_CXX_FLAGS "-std=c++14")
# The following folder will be included
include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(external)
add_subdirectory(glm)
add_subdirectory(assimp)
INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
include_directories(
        external/glew/include/
        RenderAbstraction
        glm
#        assimp/include/
	${SDL2_INCLUDE_DIRS}
	${SDL2IMAGE_INCLUDE_DIRS}
)
set(ALL_LIBS
	${OPENGL_LIBRARY}
        GLEW_1130
	soil
        assimp
)
#set(BUILD_ASSIMP_TOOLS ON)
#set(ASSIMP_BUILD_STATIC_LIB ON)
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
file(COPY  ${CMAKE_SOURCE_DIR}/texture.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(hello ${PROJECT_SOURCE_DIR}/HelloWorld.cpp
                RenderAbstraction/Shader
                RenderAbstraction/VertexArray
                RenderAbstraction/RenderContext
                RenderAbstraction/Texture
                RenderAbstraction/RenderAbstraction
                Controller/Controller
                HeightMap
                )
target_link_libraries(hello
        ${ALL_LIBS} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES}
)
