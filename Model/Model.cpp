#include "Model.h"
#include <iostream>
namespace animation {

void
Model::draw() {
    for ( auto  & mesh : m_meshes ) {
        mesh.draw( );
    }
}
void
Model::loadModel(std::string path) {

     scene = importer.ReadFile( path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if ( !scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode ) {
        std::cout << "ERROR::ASSIMP:" << importer.GetErrorString() << "\n";
    }
    directory = path.substr(0, path.find_last_of('/') );
    processNode( scene->mRootNode, scene );

    auto temp = scene->mRootNode->mTransformation;
    std::cout << sizeof(temp) << "\n";

    for( int i = 0; i < 4; i++){
        for ( int j = 0; j <4; j++)
        {
            globalInverseTransorm[j][i] = temp[i][j];
        }
    }
    globalInverseTransorm = glm::inverse(globalInverseTransorm);
}

void
Model::processNode(aiNode *node, const aiScene *scene) {
    for ( GLuint i = 0; i< node->mNumMeshes; i++ ) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.emplace_back( processMesh( mesh, scene) );
    }
    for ( GLuint i = 0; i< node->mNumChildren; i++ ) {
        processNode( node->mChildren[i], scene );
    }
}

Mesh
Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vert;
    std::vector<GLuint> ind;
    vertim.clear();
    indem.clear();
    for ( GLuint i = 0; i<mesh->mNumVertices; i++){
        Vertex vertex;
        glm::vec3 temp;

        temp.x = mesh->mVertices[i].x;
        temp.y = mesh->mVertices[i].y;
        temp.z = mesh->mVertices[i].z;
        vertim.push_back( mesh->mVertices[i].x );
        vertim.push_back( mesh->mVertices[i].y );
        vertim.push_back( mesh->mVertices[i].z );
        vertex.Pos = temp;

        vert.push_back(vertex);
    }
    for ( GLuint i = 0; i < mesh->mNumFaces; i++ ) {
        aiFace face = mesh->mFaces[i];
        for ( GLuint j = 0; j < face.mNumIndices; j++ ) {
            ind.push_back( face.mIndices[j]);
            indem.push_back(face.mIndices[j]);
        }
    }
    vertime.push_back(vertim);
    indeme.push_back(indem);
    return Mesh( vert, ind);
}

} //animations
