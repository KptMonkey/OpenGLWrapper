#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct SkeletonNode {
    std::vector<SkeletonNode *> children;

    std::vector<glm::vec3> translation;
    std::vector<glm::vec3> rotation;
    std::vector<glm::vec3> scale;
    std::vector<double> posKeyTimes;
    std::vector<double> rotationKeyTimes;
    std::vector<double> scaleKeyTimes;
    int numTranslations;
    int numRotation;
    int numScale;

    std::string name;
    int numChildren;
    int boneIndex;
};
class NewAnimation{
public:
    bool loadMesh( std::string path, GLuint & vao, int & point_count, int & bone_count , std::vector<glm::mat4> & boneOffset, SkeletonNode *& skeletonNode,
                   double & animDuration);
    bool importSkeletonNode(aiNode * assimpNode, SkeletonNode *& skeletonNode, int boneCount, std::vector<std::string> boneName );
private:
    Assimp::Importer importer;
    void
    copyMatrix( aiMatrix4x4 aMatrix, glm::mat4 & mat );
    void
    skeletonAnimate(
            SkeletonNode * node,
            glm::mat4 & parent,
            std::vector<glm::mat4> boneOffSet,
            std::vector<glm::mat4> bAnimations,
            double time);
    SkeletonNode *
    findNodeInSkeleton( SkeletonNode * root, std::string nodeName );


};

