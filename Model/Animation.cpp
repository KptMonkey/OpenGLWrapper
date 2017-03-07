#include "Animation.hpp"
#include <iostream>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

void
Animation::copyMatrix(aiMatrix4x4 aMatrix, glm::mat4 & mat)
{
    assert(sizeof(mat) == sizeof(aMatrix));
    memcpy(&mat, &aMatrix, sizeof(aMatrix));
    mat = glm::transpose(mat);
}

void
Animation::copyVector( aiVector3D & aVector, glm::vec3 & vec){
    assert(sizeof(vec) == sizeof(aVector));
    memcpy(&vec, &aVector, sizeof(aVector));
}

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4
// Praktisch aber behindert zu lesen... ToDO
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

void
Animation::VertexBoneData::addBoneData(uint boneID, float weight)
{
    for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++) {
        if (Weights[i] == 0.0) {
            IDs[i]     = boneID;
            Weights[i] = weight;
            return;
        }
    }
}

Animation::Animation()
{
    m_VAO = 0;
    ZERO_MEM(m_Buffers);
    m_NumBones = 0;
    m_pScene = NULL;
}


Animation::~Animation()
{
    clear();
}


void Animation::clear()
{
    if (m_Buffers[0] != 0) {
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
    }

    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
}


bool
Animation::loadMesh(std::string const & filename)
{
    clear();

    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

    bool ret = false;

    m_pScene = m_Importer.ReadFile( filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                                                         aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if (m_pScene) {
        copyMatrix(m_pScene->mRootNode->mTransformation, m_GlobalInverseTransform);
        glm::inverse( m_GlobalInverseTransform );
        ret = initFromScene(m_pScene, filename);
    }
    else {
        printf("Error parsing '%s': '%s'\n", filename.c_str(), m_Importer.GetErrorString());
    }
    glBindVertexArray(0);

    return ret;
}

bool
Animation::initFromScene(const aiScene* pScene, const std::string& filename)
{
    m_Entries.resize(pScene->mNumMeshes);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    std::vector<VertexBoneData> bones;
    std::vector<uint> indices;

    uint numVertices = 0;
    uint numIndices = 0;

    for ( auto & entries : m_Entries ){

        auto pos = std::addressof(entries) - std::addressof(m_Entries[0]);
        entries.MaterialIndex = pScene->mMeshes[pos]->mMaterialIndex;
        entries.NumIndices    = pScene->mMeshes[pos]->mNumFaces * 3;
        entries.BaseVertex    = numVertices;
        entries.BaseIndex     = numIndices;

        numVertices += pScene->mMeshes[pos]->mNumVertices;
        numIndices  += entries.NumIndices;
    }

    positions.reserve(numVertices);
    normals.reserve(numVertices);
    texCoords.reserve(numVertices);
    bones.resize(numVertices);
    indices.reserve(numIndices);

    for (uint i = 0 ; i < m_Entries.size() ; i++) {
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i, paiMesh, positions, normals, texCoords, bones, indices);
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(BONE_ID_LOCATION);
    glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
    glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
    glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    return true;
}


void
Animation::initMesh(uint meshIndex,
                    const aiMesh* paiMesh,
                    std::vector<glm::vec3>& positions,
                    std::vector<glm::vec3>& normals,
                    std::vector<glm::vec2>& texCoords,
                    std::vector<VertexBoneData>& bones,
                    std::vector<uint>& indices)
{
    const aiVector3D zeroVec(0.0f, 0.0f, 0.0f);

    for (uint i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zeroVec;

        positions.emplace_back(pPos->x, pPos->y, pPos->z);
        normals.emplace_back(pNormal->x, pNormal->y, pNormal->z);
        texCoords.emplace_back(pTexCoord->x, pTexCoord->y);
    }
    loadBones(meshIndex, paiMesh, bones);

    for (uint i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        indices.push_back(Face.mIndices[0]);
        indices.push_back(Face.mIndices[1]);
        indices.push_back(Face.mIndices[2]);
    }

}

void
Animation::loadBones(uint meshIndex, aiMesh const * pMesh, std::vector<VertexBoneData>& bones)
{
    for (uint i = 0 ; i < pMesh->mNumBones ; i++) {
        uint boneIndex = 0;
        std::string boneName(pMesh->mBones[i]->mName.data);
        if (m_BoneMapping.find(boneName) == m_BoneMapping.end()) {
            boneIndex = m_NumBones;
            m_NumBones++;
            BoneInfo bi;
            m_BoneInfo.push_back(bi);
            copyMatrix( pMesh->mBones[i]->mOffsetMatrix, m_BoneInfo[boneIndex].BoneOffset );
            m_BoneMapping[boneName] = boneIndex;
        }
        else {
            boneIndex = m_BoneMapping[boneName];
        }
        m_BoneMapping[boneName] = boneIndex;
        copyMatrix( pMesh->mBones[i]->mOffsetMatrix, m_BoneInfo[boneIndex].BoneOffset );

        for (uint j = 0 ; j < pMesh->mBones[i]->mNumWeights ; j++) {
            uint vertexID = m_Entries[meshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float weight  = pMesh->mBones[i]->mWeights[j].mWeight;
            bones[vertexID].addBoneData(boneIndex, weight);
        }
    }
}

void Animation::render()
{
    glBindVertexArray(m_VAO);

    for (uint i = 0 ; i < m_Entries.size() ; i++) {

        glDrawElementsBaseVertex(GL_TRIANGLES,
                                 m_Entries[i].NumIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(uint) * m_Entries[i].BaseIndex),
                                 m_Entries[i].BaseVertex);
    }
    glBindVertexArray(0);
}

uint
Animation::findPosition(float animationTime, const aiNodeAnim* pNodeAnim)
{
    for (uint i = 0 ; i < (pNodeAnim->mNumPositionKeys - 1) ; i++) {
        if (animationTime <= (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    return 0;
}

uint
Animation::findRotation(float animationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    for (uint i = 0 ; i < (pNodeAnim->mNumRotationKeys - 1 ); i++) {
        if (animationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    return 0;
}

uint
Animation::findScaling(float animationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    for (uint i = 0 ; i < (pNodeAnim->mNumScalingKeys - 1) ; i++) {
        if (animationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }
    return 0;
}

void
Animation::calcInterpolatedPosition(aiVector3D &  out, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    uint positionIndex = findPosition(animationTime, pNodeAnim);
    uint nextPositionIndex = (positionIndex + 1);

    assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);
    float deltaTime = (float)(pNodeAnim->mPositionKeys[nextPositionIndex].mTime - pNodeAnim->mPositionKeys[positionIndex].mTime);
    float factor = (animationTime - (float)pNodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;

    assert(factor >= 0.0f && factor <= 1.0f);
    const aiVector3D& start = pNodeAnim->mPositionKeys[positionIndex].mValue;
    const aiVector3D& end = pNodeAnim->mPositionKeys[nextPositionIndex].mValue;
    aiVector3D delta = end - start;

    out = start + factor * delta;
}

void
Animation::calcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumRotationKeys == 1) {
        out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    uint rotationIndex = findRotation(animationTime, pNodeAnim);
    uint nextRotationIndex = (rotationIndex + 1);

    assert(nextRotationIndex < pNodeAnim->mNumRotationKeys);
    float deltaTime = static_cast<float>(pNodeAnim->mRotationKeys[nextRotationIndex].mTime - pNodeAnim->mRotationKeys[rotationIndex].mTime);
    float factor = ((animationTime - static_cast<float>(pNodeAnim->mRotationKeys[rotationIndex].mTime)) / deltaTime);

    assert(factor >= 0.0f && factor <= 1.0f);
    auto const & startRotationQ = pNodeAnim->mRotationKeys[rotationIndex].mValue;
    auto const & endRotationQ   = pNodeAnim->mRotationKeys[nextRotationIndex].mValue;
    aiQuaternion::Interpolate(out, startRotationQ, endRotationQ, factor);

    out = out.Normalize();
}

void
Animation::calcInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    uint scalingIndex = findScaling(animationTime, pNodeAnim);
    uint nextScalingIndex = (scalingIndex + 1);

    assert(nextScalingIndex < pNodeAnim->mNumScalingKeys);
    float deltaTime = (float)(pNodeAnim->mScalingKeys[nextScalingIndex].mTime - pNodeAnim->mScalingKeys[scalingIndex].mTime);
    float factor = (animationTime - (float)pNodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;

    assert(factor >= 0.0f && factor <= 1.0f);
    auto const & start = pNodeAnim->mScalingKeys[scalingIndex].mValue;
    auto const & end   = pNodeAnim->mScalingKeys[nextScalingIndex].mValue;
    auto delta = end - start;

    out = start + factor * delta;
}

void
Animation::readNodeHeirarchy(float animationTime, const aiNode* pNode, const glm::mat4 & parentTransform, int num)
{
    std::string nodeName(pNode->mName.data);
    const aiAnimation* pAnimation = m_pScene->mAnimations[num];
    for ( int i = 0; i < m_pScene->mNumAnimations; i++ ) {
    }
    glm::mat4 nodeTransformation;
    copyMatrix( pNode->mTransformation, nodeTransformation);
    const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, nodeName);
    if (pNodeAnim) {
        aiVector3D scaling;
        calcInterpolatedScaling(scaling, animationTime, pNodeAnim);
        glm::mat4 scalingM;
        scalingM = glm::scale( scalingM,glm::vec3((scaling.x, scaling.y, scaling.z) ) );

        aiQuaternion rotationQ;
        calcInterpolatedRotation(rotationQ, animationTime, pNodeAnim);
        glm::mat4 rotationM(1.0f);
        glm::quat myQuat(rotationQ.w, rotationQ.x, rotationQ.y, rotationQ.z);
        rotationM = glm::toMat4( myQuat );

        aiVector3D translation;
        calcInterpolatedPosition(translation, animationTime, pNodeAnim);
        glm::mat4 translationM(1.0f);
        glm::vec3 translate;
        copyVector(translation, translate );
        translationM = glm::translate( translationM, translate );

        nodeTransformation = translationM *rotationM * scalingM;
    }
    glm::mat4 globalTransformation = parentTransform * nodeTransformation;

    if (m_BoneMapping.find(nodeName) != m_BoneMapping.end()) {
        uint boneIndex = m_BoneMapping[nodeName];
        m_BoneInfo[boneIndex].FinalTransformation = m_GlobalInverseTransform * globalTransformation * m_BoneInfo[boneIndex].BoneOffset;
    }

    for (uint i = 0 ; i < pNode->mNumChildren ; i++) {

        readNodeHeirarchy(animationTime, pNode->mChildren[i], globalTransformation,num);
    }
}

void
Animation::boneTransform(float timeInSeconds, std::vector<glm::mat4> & transforms, int i)
{
    glm::mat4 identity(1.0f);
    if(!m_pScene->HasAnimations()){return;}
    float ticksPerSecond = (float)(m_pScene->mAnimations[i]->mTicksPerSecond != 0 ? m_pScene->mAnimations[i]->mTicksPerSecond : 25.0f);
    float timeInTicks = (timeInSeconds) * ticksPerSecond;
    float animationTime = fmod(timeInTicks, (float)m_pScene->mAnimations[i]->mDuration);
    readNodeHeirarchy(animationTime, m_pScene->mRootNode, identity,i);

    transforms.resize(m_NumBones);

    for (uint i = 0 ; i < m_NumBones ; i++) {
        transforms[i] = m_BoneInfo[i].FinalTransformation;
    }
}

const aiNodeAnim*
Animation::findNodeAnim(const aiAnimation* pAnimation, const std::string nodeName)
{
    for (uint i = 0 ; i < pAnimation->mNumChannels ; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (std::string(pNodeAnim->mNodeName.data) == nodeName) {
            return pNodeAnim;
        }
    }

    return nullptr;
}

void
Animation::initBoneLocation(GLuint & shaderProgram)
{
    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_BoneLocation) ; i++) {
         char name[128];
         memset(name, 0, sizeof(name));
         snprintf(name, sizeof(name), "gBones[%d]", i);
         m_BoneLocation[i] = glGetUniformLocation(shaderProgram, name);;
     }
}

void
Animation::setBoneTransform( std::vector<glm::mat4> & transforms )
{
    for( uint i = 0; i < transforms.size(); ++i ){
        glUniformMatrix4fv(m_BoneLocation[i], 1, GL_FALSE, glm::value_ptr(transforms[i] ) );
    }
}
