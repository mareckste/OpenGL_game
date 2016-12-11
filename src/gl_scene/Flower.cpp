//
// Created by stevu on 11.12.2016.
//

#include "Flower.h"
#include "scene.h"

#include "explosion.h"
#include "bee.h"

Flower::Flower(float x, float y, float z) {

    //  scale *= 10.0f;
    scale /=33.0f;
    rotation = glm::vec3(4.7f, 0.0f, 0.0f);

    position = glm::vec3(x,y,z);

    head = Flower_headPtr(new Flower_head{});
    stem = Flower_stemPtr(new Flower_stem{});

    head->position = position;
    head->rotation = rotation;
    head->scale = scale;

    stem->rotation = rotation;
    stem->position = position;
    stem->scale = scale;
}

Flower::~Flower() {
}

bool Flower::Update(Scene &scene, float dt) {
    // Count time alive
    age += dt;
    if (age < maxAge)
        scale += dt*0.02;


    for ( auto obj : scene.objects ) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;

        // We only need to collide with flowers, ignore other objects
        auto player = std::dynamic_pointer_cast<bee>(obj);
        if (!player) continue;

        if ((fabs(player->position.x - position.x) < .8) && (fabs(player->position.y - position.y) < .8)  ) {
            // Explode

            auto explosion = ExplosionPtr(new Explosion{});
            explosion->position = player->position;
            explosion->scale *=  1.5f;
            scene.objects.push_back(explosion);
            return false;
        }
    }


    head->scale = scale;
    stem->scale = scale;

    head->Update(scene,dt);
    stem->Update(scene,dt);

    return true;
}


void Flower::Render(Scene &scene) {
    head->Render(scene);
    stem->Render(scene);
}

// shared resources
MeshPtr Flower::mesh;
ShaderPtr Flower::shader;
TexturePtr Flower::texture;
