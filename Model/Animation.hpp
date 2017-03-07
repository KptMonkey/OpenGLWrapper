#pragma once

#include <map>
#include <vector>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <GL/glew.h>

class Animation
{
public:
    Animation();

    ~Animation();

    bool
    loadMesh(std::string const & filename);

    void
    render();

    uint
    numberOfBones() const
    {
        return m_NumBones;
    }

    void
    boneTransform(float timeInSeconds, std::vector<glm::mat4> & transforms, int i);
    void
    initBoneLocation(GLuint & shaderProgram);
    void
    setBoneTransform( std::vector<glm::mat4> & transforms );

private:
    #define NUM_BONES_PER_VEREX 4
#define ZERO_MEM(a) std::memset( a, 0, sizeof(a))

    struct BoneInfo
    {
        glm::mat4 BoneOffset;
        glm::mat4 FinalTransformation;

        BoneInfo()
        {
            BoneOffset = glm::mat4(0.0f);
            FinalTransformation = glm::mat4(0.0f);
        }
    };

    struct VertexBoneData
    {
        uint IDs[NUM_BONES_PER_VEREX];
        float Weights[NUM_BONES_PER_VEREX];

        VertexBoneData()
        {
            reset();
        }

        void reset()
        {
            ZERO_MEM(IDs);
            ZERO_MEM(Weights);
        }

        void addBoneData(uint boneID, float weight);
    };
    void
    copyMatrix(aiMatrix4x4 aMatrix, glm::mat4 &mat );
    void
    copyVector( aiVector3D & aVector, glm::vec3 & vec);
    void
    calcInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim);
    void
    calcInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* pNodeAnim);
    void
    calcInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim);
    uint
    findScaling(float animationTime, const aiNodeAnim* pNodeAnim);
    uint
    findRotation(float animationTime, const aiNodeAnim* pNodeAnim);
    uint
    findPosition(float animationTime, const aiNodeAnim* pNodeAnim);
    const aiNodeAnim*
    findNodeAnim(const aiAnimation* pAnimation, std::string const nodeName);
    void
    readNodeHeirarchy(float animationTime, const aiNode* pNode, glm::mat4 const & parentTransform, int num);
    bool
    initFromScene(const aiScene* pScene, std::string const & filename);
    void
    initMesh(uint meshIndex,
                  const aiMesh* paiMesh,
                  std::vector<glm::vec3> & positions,
                  std::vector<glm::vec3> & normals,
                  std::vector<glm::vec2> & texCoords,
                  std::vector<VertexBoneData> & bones,
                  std::vector<unsigned int> & indices);
    void
    loadBones(uint meshIndex, const aiMesh* paiMesh, std::vector<VertexBoneData> & bones);
    bool
    initMaterials(const aiScene* pScene, std::string const & filename);
    void
    clear();

#define INVALID_MATERIAL 0xFFFFFFFF

enum VB_TYPES {
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    BONE_VB,
    NUM_VBs
};

    GLuint m_Buffers[NUM_VBs];

    struct MeshEntry {
        MeshEntry()
        {
            NumIndices    = 0;
            BaseVertex    = 0;
            BaseIndex     = 0;
        }

        unsigned int NumIndices;
        unsigned int BaseVertex;
        unsigned int BaseIndex;
        unsigned int MaterialIndex;
    };

    std::vector<MeshEntry> m_Entries;

    std::map<std::string,uint> m_BoneMapping;
    uint m_NumBones;
    std::vector<BoneInfo> m_BoneInfo;
    glm::mat4 m_GlobalInverseTransform;
    GLuint m_BoneLocation[100];
    const aiScene* m_pScene;
    Assimp::Importer m_Importer;
public:
    GLuint m_VAO;
};
