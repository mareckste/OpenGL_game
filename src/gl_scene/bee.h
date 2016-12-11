//
// Created by stevu on 11.12.2016.
//

#ifndef PPGSO_BEE_H
#define PPGSO_BEE_H


#include <texture.h>
#include <shader.h>
#include <mesh.h>
#include "wings.h"
#include "body.h"

#include "object.h"

// Simple object representing the player
// Reads keyboard status and manipulates its own position
// On Update checks collisions with Asteroid objects in the scene
class bee : public Object {
public:
    bee(float y, float z);

    ~bee();

    bool Update(Scene &scene, float dt) override;

    void Render(Scene &scene) override;

private:

    PlayerPtr player;
    WingsPtr wings;
    float time = 0;
    bool flip = false;

    bool isFlower(Scene &scene);
};

typedef std::shared_ptr<bee> BeePtr;



#endif //PPGSO_BEE_H
