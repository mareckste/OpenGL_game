//
// Created by stevu on 11.12.2016.
//

#include "bee.h"
#include "scene.h"
#include <GLFW/glfw3.h>
#include "flower_head.h"


bee::bee(float y, float z) {
    this->position.y = y;
    this->position.z = z;

    rotation = glm::vec3(4.8f, 0.0f, 0.0f);

    player = PlayerPtr(new Player{});
    wings = WingsPtr(new Wings{});

    player->position = position;
    wings->position = position;
}

bee::~bee() {
}

bool bee::Update(Scene &scene, float dt) {

    if (!isFlower(scene)) return  false;

    time += dt;

    int c = 1;

    if(scene.keyboard[GLFW_KEY_LEFT] && position.x <= 9 ) {
        position.x += 5 * dt;
        rotation.z = -PI/4.0f;
        scene.camera->position.x += 0.05f;
        c = 2;
    } else if(scene.keyboard[GLFW_KEY_RIGHT] && position.x >= -9) {
        position.x -= 5 * dt;
        rotation.z = PI/4.0f;
        scene.camera->position.x -= 0.05f;
        c = 2;
    } else if(scene.keyboard[GLFW_KEY_UP] && position.y <= 10) {
        rotation = glm::vec3(4.8f, 0.0f, 0.0f);
        position.y += 5 * dt;
        scene.camera->position.y += 0.05f;
    }else if(scene.keyboard[GLFW_KEY_DOWN] && position.y >= -10) {
        position.y -= 5 * dt;
        rotation = glm::vec3(14.0f, 3.0f, 0.0f);
        scene.camera->position.y -= 0.05f;
    } else {
        rotation.z = 0;
    }

    player->position = position;
    player->rotation = rotation;

    wings ->rotation = rotation;

    if (time > .05) {
        if (flip) {
            wings->rotation.z = -PI / 8.0f * c;
            flip = false;
        } else {
            wings->rotation.z = +PI / 8.0f * c;
            flip = true;
        }
        time = 0;
    }

    wings->position = position;

    player->Update(scene, dt);
    wings->Update(scene, dt);



    return true;
}

bool bee::isFlower(Scene &scene) {
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

    return true;
 }

 void bee::Render(Scene &scene) {
     player->Render(scene);
     wings->Render(scene);
 }

