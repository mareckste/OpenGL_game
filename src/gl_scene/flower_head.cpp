#include "flower_head.h"
#include "scene.h"
#include "object_frag.h"
#include "object_vert.h"


Flower_head::Flower_head() {

    if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
    if (!texture) texture = TexturePtr(new Texture{"flower_head.rgb", 1200, 800});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "kvet.obj"});
}

Flower_head::~Flower_head() {
}

bool Flower_head::Update(Scene &scene, float dt) {
    GenerateModelMatrix();

    return true;
}


void Flower_head::Render(Scene &scene) {
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
MeshPtr Flower_head::mesh;
ShaderPtr Flower_head::shader;
TexturePtr Flower_head::texture;
