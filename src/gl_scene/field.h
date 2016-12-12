#include "mesh.h"
#include "object.h"

// This object renders the scene background
// It does not use the camera so it uses different shader program
// Background animation is achieved by passing an offset to the fragment shader that offsets texture mapping
class Field : public Object {
public:
  Field();
  ~Field();

  bool Update(Scene &scene, float dt) override;
  void Render(Scene &scene) override;
  void applyLight(glm::vec3 *pointLightPositions, int i, float linModifier, float quadModifier);
  static ShaderPtr shader;

private:
 // float offset;
  // Static resources (Shared between instances)
  static MeshPtr mesh;

  static TexturePtr texture;
};
typedef std::shared_ptr< Field > FieldPtr;


