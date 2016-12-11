#include "watering_can.h"
#include "scene.h"

#include "explosion.h"
#include "player.h"
#include "object_frag.h"
#include "object_vert.h"
#include "flower_stem.h"
#include "flower_head.h"
#include "generator.h"

watering_can::watering_can(glm::vec3 wp[4]) {
    walkPath[0] = wp[0]; walkPath[1] = wp[1]; walkPath[2] = wp[2]; walkPath[3] = wp[3];

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
    if (position.y < -9)
        return false;
    position = positionManager(dt);


    std::cout << position.y << std::endl;
    bool flag = false;
    GeneratorPtr generator;
    for ( auto obj : scene.objects ) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;
        if (!flag)
            generator = std::dynamic_pointer_cast<Generator>(obj);
        if (generator) flag = true;
        // We only need to collide with flowers, ignore other objects
        auto player = std::dynamic_pointer_cast<Player>(obj);
        if (!player) continue;

        if ((fabs(player->position.x - position.x) < .8) && (fabs(player->position.y - position.y) < .8) ) {
            // Explode
            generator->flowers_history += 2;

            auto explosion = ExplosionPtr(new Explosion{});
            explosion->position = player->position;
            explosion->scale *=  1.2f;
            scene.objects.push_back(explosion);

            generateFlowers(scene);
            generateFlowers(scene);

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

void watering_can::generateFlowers(Scene &scene) {

        auto obj = Flower_headPtr(new Flower_head());
        obj->position = glm::vec3(0,0,0);
        obj->position.x += Rand(-9, 9); obj->position.y += Rand(-6.5f,9);
        obj->position.z = -7;
        scene.objects.push_back(obj);

        auto obj1 = Flower_stemPtr(new Flower_stem());
        obj1->position = glm::vec3(0,0,0);
        obj1->position.x = obj->position.x;
        obj1->position.y = obj->position.y;
        obj1->position.z = -7;
        scene.objects.push_back(obj1);

}

glm::vec3 watering_can::positionManager(float dt) {
    walkTime += 0.55f*dt;
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
