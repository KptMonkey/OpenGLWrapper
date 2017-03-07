#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.hpp"
#include "Display.hpp"
#include <glm/glm.hpp>

namespace animation
{
class Model {
public:
    Model( std::string path ){
        loadModel( path );
    }
    void draw ( );
    std::vector<float> vertim;
    std::vector<unsigned int> indem;
    std::vector< std::vector<float> > vertime;
    std::vector< std::vector<unsigned int> > indeme;
    std::vector<Mesh> m_meshes;
    Assimp::Importer importer;
    const aiScene * scene;
    glm::mat4 globalInverseTransorm;
    void
    loadBones();
private:
    std::string directory;

    void loadModel( std::string path );
    void processNode( aiNode* node, const aiScene* scene );
    Mesh processMesh( aiMesh* mesh, const aiScene* scene );
    std::vector<Texture> loadMaterialTextures( aiMaterial* mat, aiTextureType type, std::string typeName );
};
}
