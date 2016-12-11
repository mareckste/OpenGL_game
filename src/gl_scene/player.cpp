#include "player.h"
#include "scene.h"
#include "object_frag.h"
#include "object_vert.h"
#include "flower_head.h"

#include <GLFW/glfw3.h>

Player::Player() {
  position.z = -9.8f;
  // Reset fire delay


  // Scale the default model
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
  int flowers = 0;
  for ( auto obj : scene.objects ) {
    // Ignore self in scene
    if (obj.get() == this)
      continue;

    // We only need to collide with flowers, ignore other objects
    auto flower = std::dynamic_pointer_cast<Flower_head>(obj);
    if (!flower) continue;

    flowers++;
  }

  if (!flowers) return false;


  // Keyboard controls
  if(scene.keyboard[GLFW_KEY_LEFT] && position.x <= 9 ) {
    position.x += 5 * dt;
    rotation.z = -PI/4.0f;
      scene.camera->position.x += 0.05;
  } else if(scene.keyboard[GLFW_KEY_RIGHT] && position.x >= -9) {

    position.x -= 5 * dt;
    rotation.z = PI/4.0f;
      scene.camera->position.x -= 0.05;
  } else if(scene.keyboard[GLFW_KEY_UP] && position.y <= 10) {
      rotation = glm::vec3(4.8f, 0.0f, 0.0f);
    position.y += 5 * dt;
      scene.camera->position.y += 0.05;
  }else if(scene.keyboard[GLFW_KEY_DOWN] && position.y >= -10) {
    position.y -= 5 * dt;
      rotation = glm::vec3(14.0f, 3.0f, 0.0f);
      scene.camera->position.y -= 0.05;
  } else {
    rotation.z = 0;
  }



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