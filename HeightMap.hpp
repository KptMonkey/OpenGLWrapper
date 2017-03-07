#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "VertexArray.hpp"

class HeightMap
{
public:

   bool LoadHeightMapFromImage(std::string path);
   void ReleaseHeightmap();

   HeightMap(){}
   std::vector<VertexT> vertices;
   std::vector<unsigned int> indices;

   float
   getHeight(float x, float y);
   float
   barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

private:
   int iRows;
   int iCols;
   std::vector< std::vector<float> > heightTable;
};
