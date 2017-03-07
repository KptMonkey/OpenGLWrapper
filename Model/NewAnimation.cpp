#include "NewAnimation.hpp"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

bool
NewAnimation::loadMesh(std::string path, GLuint & vao, int & point_count, int & bone_count, std::vector<glm::mat4> & boneOffset , SkeletonNode *&skeletonNode, double &animDuration)
{
    const aiScene * scene = importer.ReadFile(path, aiProcess_Triangulate);
    if(!scene){
        std::cout << "error\n";
    }

    const aiMesh * mesh = scene->mMeshes[0];
    std::vector<int>       boneIds;
    int numWeights;
    point_count = mesh->mNumVertices;
    std::vector<std::string> boneNames;
    if( mesh->HasBones() )
    {
        bone_count = mesh->mNumBones;
        boneIds.resize(point_count);
        for ( int i = 0; i < bone_count; ++i ){
            auto bones = mesh->mBones[i];

            boneNames.emplace_back( bones->mName.C_Str() );
            glm::mat4 offSet;
            copyMatrix( bones->mOffsetMatrix, offSet);
            boneOffset.push_back(offSet);

            numWeights = bones->mNumWeights;

            for ( int j=0; j<numWeights;++j){
                auto weight = bones->mWeights[j];
                int vertexId = weight.mVertexId;
                if( weight.mWeight >= 0.5){
                    boneIds[vertexId] = i;
                }
            }
        }
        aiNode * assimpNode = scene->mRootNode;
        if ( !importSkeletonNode( assimpNode, skeletonNode, bone_count, boneNames ))
            std::cout << "Skeleton not found \n";

        if ( scene->mNumAnimations > 0 ){
            aiAnimation * anim = scene->mAnimations[0];
            std::cout << "NodeChannels: " << anim->mNumChannels << "\n";
            std::cout << "MeshChannels:"  << anim->mNumMeshChannels << "\n";
            std::cout << "Ticks per Second: "<< anim->mTicksPerSecond << "\n";
            std::cout << "Duration: " << anim->mDuration << "\n";

            auto duration = anim->mDuration;

            for ( int i = 0; i < anim->mNumChannels; ++i ){
                auto chan = anim->mChannels[i];
                auto sNode = findNodeInSkeleton( skeletonNode, std::string(chan->mNodeName.C_Str()) );
                sNode->numTranslations = chan->mNumPositionKeys;
                sNode->numRotation = chan->mNumRotationKeys;

                sNode->translation.emplace_back( (float)chan->mPositionKeys->mValue.x, (float)chan->mPositionKeys->mValue.y, (float)chan->mPositionKeys->mValue.z );
                sNode->posKeyTimes.emplace_back( chan->mPositionKeys->mTime );
            }
        }
    }

    glGenVertexArrays(1,&vao);
    glBindVertexArray( vao );

    std::vector<glm::vec3> position;
    std::vector<glm::vec3> normals;

    for ( int i = 0; i<point_count ;++i )
    {
        auto & aVertex = mesh->mVertices[i];
        position.emplace_back( glm::vec3(aVertex.x, aVertex.y, aVertex.z) );
        auto & aNormal = mesh->mNormals[i];
        normals.emplace_back( glm::vec3( aNormal.x, aNormal.y, aNormal.z) );
    }

    GLuint vbo;
    glGenBuffers( 1, &vbo);
    glBindBuffer( GL_ARRAY_BUFFER, vbo);
    glBufferData( GL_ARRAY_BUFFER, position.size() * sizeof(position.front()),&position.front(),GL_STATIC_DRAW);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    GLuint vbo2;
    glGenBuffers( 1, &vbo2);
    glBindBuffer( GL_ARRAY_BUFFER, vbo2);
    glBufferData( GL_ARRAY_BUFFER, normals.size() * sizeof(normals.front()),&normals.front(),GL_STATIC_DRAW);
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    if ( mesh->HasBones()){
        GLuint vbo;
        glGenBuffers( 1, & vbo);
        glBindBuffer( GL_ARRAY_BUFFER, vbo );
        glBufferData( GL_ARRAY_BUFFER, boneIds.size() * sizeof( boneIds.front() ), &boneIds.front(), GL_STATIC_DRAW );
        glVertexAttribIPointer( 2,1,GL_INT, 0, nullptr);
        glEnableVertexAttribArray(2);
    }

    glBindVertexArray(0);
    return true;

}

void
NewAnimation::copyMatrix(aiMatrix4x4 aMatrix, glm::mat4 & mat)
{
    assert(sizeof(mat) == sizeof(aMatrix));
    memcpy(&mat, &aMatrix, sizeof(aMatrix));
    mat = glm::transpose(mat);
}

bool
NewAnimation::importSkeletonNode( aiNode * assimpNode,
                         SkeletonNode *& skeletonNode,
                         int boneCount,
                         std::vector<std::string> boneName ) {
    skeletonNode = new SkeletonNode();

    skeletonNode->name = std::string( assimpNode->mName.C_Str() );
    skeletonNode->numChildren = 0;

    std::cout << "Number of Children: " << assimpNode->mNumChildren << "\n";
    skeletonNode->boneIndex = -1;

    for ( int i = 0 ; i<3; ++i ){
        skeletonNode->children.push_back( nullptr);
    }
    bool hasBone = false;
    for ( int i=0; i<boneCount;++i ) {
        std::cout << "skNode: " << skeletonNode->name << std::endl;
        std::cout << boneName[i]<<std::endl;
        if( skeletonNode->name == boneName[i]){
            std::cout << "Node uses bone " << i << std::endl;
        skeletonNode->boneIndex = i;
        hasBone = true;
        break;}
    }
    if(!hasBone) std::cout << "No Bones found \n";

    bool hasUsefulChild = false;
    for ( int i = 0; i < static_cast<int>( assimpNode->mNumChildren ); ++i ){
        std::cout << "skeletonNote numChildren: " << skeletonNode->numChildren << std::endl;
        if( importSkeletonNode( assimpNode->mChildren[i], skeletonNode->children[skeletonNode->numChildren], boneCount, boneName)){
            hasUsefulChild = true;
            ++skeletonNode->numChildren;
            std::cout << "hakuna \n";

        }else{
            std::cout << "Useless node \n";
        }
    }
    if ( hasUsefulChild || hasBone ) {
        return true;
    }
    return false;
}

void
NewAnimation::skeletonAnimate(SkeletonNode * node,
        glm::mat4 & parent,
        std::vector<glm::mat4> boneOffSet,
        std::vector<glm::mat4> bAnimations , double time)
{

    auto ourMat = parent;
    glm::mat4 localAnim(1.0f);
    glm::mat4 nodeT(1.0f);
    if( node ->numTranslations > 0 ){
        int prevkey = 0;
        int nextkey = 0;
        for ( int i = 0; i < node->numTranslations-1; i++ ){
            prevkey = i;
            nextkey = i+1;
            if ( node->posKeyTimes[i] >= time ) break;

        }
        auto totalTime = node->posKeyTimes[nextkey] - node->posKeyTimes[prevkey];
        float t = ( time - node->posKeyTimes[prevkey]) / totalTime;

        glm::vec3 vi = node->translation[prevkey];
        glm::vec3 vf = node->translation[nextkey];

        glm::vec3 lerped = vi * ((float)1.0 - (float)t) + vf * (float)t;
        nodeT = glm::translate( nodeT, lerped);
    }
    localAnim = nodeT;
}
SkeletonNode *
NewAnimation::findNodeInSkeleton( SkeletonNode * root, std::string nodeName ){
    if ( nodeName == root->name ) return root;

    for ( int i = 0; i < root->numChildren; ++i ){
        auto child = findNodeInSkeleton( root->children[i], nodeName );
                if( child != nullptr )
                    return child;
    }
    return nullptr;
}
