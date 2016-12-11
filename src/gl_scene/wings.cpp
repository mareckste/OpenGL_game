#include "wings.h"
#include "scene.h"
#include "asteroid.h"
#include "flower_head.h"
#include "projectile.h"
#include "explosion.h"

#include "object_frag.h"
#include "object_vert.h"

#include <GLFW/glfw3.h>

Wings::Wings() {
    position.z = -9.8f;
    // Reset fire delay

    // Scale the default model
    scale *= 65.0f;
    rotation = glm::vec3(4.8f, 0.0f, 0.0f);

    // Initialize static resources if needed
    if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
    if (!texture) texture = TexturePtr(new Texture{"wings_c.rgb", 600, 600});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "bee_wing.obj"});
}

Wings::~Wings() {
}

bool Wings::Update(Scene &scene, float dt) {
    // Fire delay increment
    time += dt;


    // Keyboard controls
    if(scene.keyboard[GLFW_KEY_LEFT] && position.x <= 9 ) {
        position.x += 5 * dt;
        rotation.z = -PI/4.0f;
    } else if(scene.keyboard[GLFW_KEY_RIGHT] && position.x >= -9) {
        position.x -= 5 * dt;
        rotation.z = PI/4.0f;
    } else if(scene.keyboard[GLFW_KEY_UP]&& position.y <= 10) {
        position.y += 5 * dt;
    }else if(scene.keyboard[GLFW_KEY_DOWN]&& position.y >= -10) {
        position.y -= 5 * dt;
    } else {
        rotation.z = 0;
    }

    if (time > .05) {
        //const int offst = 2;
        if (flip) {
            rotation.z = -PI / 8.0f;
            flip = false;
        } else {
            rotation.z = +PI / 8.0f;
            flip = true;
        }
        time = 0;
    }

    GenerateModelMatrix();
    return true;
}

void Wings::Render(Scene &scene) {
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
MeshPtr Wings::mesh;
ShaderPtr Wings::shader;
TexturePtr Wings::texture;