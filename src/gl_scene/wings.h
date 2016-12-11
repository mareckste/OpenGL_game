//
// Created by stevu on 10.12.2016.
//

#ifndef PPGSO_WINGS_H
#define PPGSO_WINGS_H


#include <texture.h>
#include <shader.h>
#include <mesh.h>

#include "object.h"

// Simple object representing the player
// Reads keyboard status and manipulates its own position
// On Update checks collisions with Asteroid objects in the scene
class Wings : public Object {
public:
    Wings();
    ~Wings();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;

private:
    // Delay fire and fire rate
    bool flip = false;
    float time = 0;


    // Static resources (Shared between instances)
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};
typedef std::shared_ptr< Wings > WingsPtr;

#endif //PPGSO_WINGS_H
