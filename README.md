# OpenGLWrapper

OpenGlWrapper to avoid some of the boilerplate code
```
#include "RenderAbstraction.hpp"
int main() {
   RenderContext rctx;
   Camera cam = Camera(glm::vec3(0.0f, 5.0f,30.0f), glm::vec3(0.0f, 1.0f, -1.0f));
   
   // Can be float, int, glm::vec, etc
   std::vector<float> triangle { .... }

   // Vertex, Fragment and Geometry shader are supported
   Shader shader;
   shader.bindShader(shaderPath);
   shader.bindShader(shaderPath);
   
   // Indexed and not indexed is supported
   VertexArray va;
   va.createVertexArray<float>(triangle);
   va.describeVertexArray(0, 3, GlTypes::Float, 3, GlBool::False, 0);
   
   glm::mat4 model(1.f);
   
   while(render) {
      rctx.clearColor(0.0f, 0.0f, 0.1f, 1.0f);
      rctx.clearColorBuffer();
      
      // In this example we init a uniform variable to the shader
      shader.activate();
      shader["mvp"] = cam.Projection * cam.View * model;
      va.bindVertexArray();
      rctx.drawIndex(PrimitiveType::Triangle, va);
   }
}

```
