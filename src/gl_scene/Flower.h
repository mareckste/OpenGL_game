//
// Created by stevu on 11.12.2016.
//

#ifndef PPGSO_FLOWER_H
#define PPGSO_FLOWER_H


#include "mesh.h"
#include "object.h"
#include "flower_head.h"
#include "flower_stem.h"

// This object renders the scene background
// It does not use the camera so it uses different shader program
// Background animation is achieved by passing an offset to the fragment shader that offsets texture mapping
class Flower : public Object {
public:
    Flower(float x, float y, float z);
    ~Flower();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;

private:
    Flower_headPtr head;
    Flower_stemPtr stem;


    // Static resources (Shared between instances)
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
    float age = 0;
    const float maxAge = 2.3f;
};
typedef std::shared_ptr< Flower > FlowerPtr;

#endif //PPGSO_FLOWER_H
