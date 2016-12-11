#include "body.h"
#include "scene.h"
#include "object_frag.h"
#include "object_vert.h"


Player::Player() {
  position.z = -9.8f;

  scale *= 65.0f;
  rotation = glm::vec3(4.8f, 0.0f, 0.0f);

  // Initialize static resources if needed
  if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
  if (!texture) texture = TexturePtr(new Texture{"bee_body_c.rgb", 1280, 960});
  if (!mesh) mesh = MeshPtr(new Mesh{shader, "bee_body.obj"});
}

Player::~Player() {
}

bool Player::Update(Scene &scene, float dt) {

  GenerateModelMatrix();
  return true;
}

void Player::Render(Scene &scene) {
  shader->Use();

  // use camera
  shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
  shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

  // render mesh
  shader->SetMatrix(modelMatrix, "ModelMatrix");
  shader->SetTexture(texture, "Texture");
  mesh->Render();
}

// shared resources
MeshPtr Player::mesh;
ShaderPtr Player::shader;
TexturePtr Player::texture;