#ifndef PPGSO_PLAYER_H
#define PPGSO_PLAYER_H

#include <texture.h>
#include <shader.h>
#include <mesh.h>

#include "object.h"

class Player : public Object {
public:
  Player();
  ~Player();

  bool Update(Scene &scene, float dt) override;
  void Render(Scene &scene) override;
  void applyLight(glm::vec3 *pointLightPositions, int i, float linModifier, float quadModifier);

  static ShaderPtr shader;
private:
  // Static resources (Shared between instances)
  static MeshPtr mesh;

  static TexturePtr texture;
};
typedef std::shared_ptr< Player > PlayerPtr;

#endif //PPGSO_PLAYER_H
