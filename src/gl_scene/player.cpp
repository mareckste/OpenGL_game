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
  for ( auto obj : scene.objects ) {
    // Ignore self in scene
    if (obj.get() == this)
      continue;

    // We only need to collide with asteroids, ignore other objects
      auto flower = std::dynamic_pointer_cast<Flower_head>(obj);
      if (!flower) continue;

      if ((fabs(flower->position.x - position.x) < .8) && (fabs(flower->position.y - position.y) < .8) ) {
          // Explode

          auto explosion = ExplosionPtr(new Explosion{});
          explosion->position = this->position;
          explosion->scale *=  1.5f;
          scene.objects.push_back(explosion);
    }
  }

  // Keyboard controls
  if(scene.keyboard[GLFW_KEY_LEFT] && position.x <= 9 ) {
    position.x += 5 * dt;
    rotation.z = -PI/4.0f;
  } else if(scene.keyboard[GLFW_KEY_RIGHT] && position.x >= -9) {
    position.x -= 5 * dt;
    rotation.z = PI/4.0f;
  } else if(scene.keyboard[GLFW_KEY_UP] && position.y <= 10) {
    position.y += 5 * dt;
  }else if(scene.keyboard[GLFW_KEY_DOWN] && position.y >= -10) {
    position.y -= 5 * dt;
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