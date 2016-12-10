#include "watering_can.h"
#include "scene.h"

#include "explosion.h"
#include "player.h"
#include "object_frag.h"
#include "object_vert.h"

watering_can::watering_can(glm::vec3 wp[4]) {
    walkPath[0] = wp[0];walkPath[1] = wp[1];walkPath[2] = wp[2];walkPath[3] = wp[3];

    //  scale *= 10.0f;
    scale /=24.0f;
    rotation = glm::vec3(4.7f, 0.0f, 0.0f);

    // Initialize static resources if needed
    if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
    if (!texture) texture = TexturePtr(new Texture{"can.rgb", 2048, 2048});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "krhla.obj"});
}

watering_can::~watering_can() {
}

bool watering_can::Update(Scene &scene, float dt) {

    //position = positionManager(dt);
    // Count time alive
    /*age += dt;
    if (age < maxAge)
        scale += dt*0.02;*/
    // Generate modelMatrix from position, rotation and scale


    for ( auto obj : scene.objects ) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;

        // We only need to collide with flowers, ignore other objects
        auto player = std::dynamic_pointer_cast<Player>(obj);
        if (!player) continue;

        if ((fabs(player->position.x - position.x) < .8) && (fabs(player->position.y - position.y) < .8) ) {
            // Explode

            auto explosion = ExplosionPtr(new Explosion{});
            explosion->position = this->position;
            explosion->scale *=  1.5f;
            scene.objects.push_back(explosion);
            return false;
        }
    }



    GenerateModelMatrix();

    return true;
}


void watering_can::Render(Scene &scene) {
    shader->Use();

    // use camera
    shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
    shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");

    // render mesh
    shader->SetMatrix(modelMatrix, "ModelMatrix");
    shader->SetTexture(texture, "Texture");
    mesh->Render();
}

glm::vec3 watering_can::positionManager(float dt) {
    walkTime += 0.1*dt;
    if(walkTime >= 1){
        walkTime = 0.0f;
        walkPointer = (walkPointer + 1) % 4;
    }
    return linInt(walkPath[walkPointer], walkPath[walkPointer + 1], walkTime);
}

glm::vec3 watering_can::linInt(glm::vec3 a, glm::vec3 b, float t) {
    return a + (b - a) * t;
}

// shared resources
MeshPtr watering_can::mesh;
ShaderPtr watering_can::shader;
TexturePtr watering_can::texture;
