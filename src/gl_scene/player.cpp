#include "player.h"
#include "scene.h"
#include "flower_head.h"
#include "projectile.h"
#include "explosion.h"

#include "object_frag.h"
#include "object_vert.h"

#include <GLFW/glfw3.h>

Player::Player() {
  position.z = -9.8f;
  // Reset fire delay
  fireDelay = 0;
  // Set the rate of fire
  fireRate = 0.3f;
  // Fire offset;
  fireOffset = glm::vec3(0.7f,0.0f,0.0f);

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



  // Fire delay increment
  fireDelay += dt;

  // Hit detection


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

  // Firing projectiles
  if(scene.keyboard[GLFW_KEY_SPACE] && fireDelay > fireRate) {
    // Reset fire delay
    fireDelay = 0;
    // Invert file offset
    fireOffset = -fireOffset;

    auto projectile = ProjectilePtr(new Projectile{});
    projectile->position = position + glm::vec3(0.0f, 0.0f, 0.3f) + fireOffset;
    scene.objects.push_back(projectile);
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