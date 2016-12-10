#include "flower_stem.h"
#include "scene.h"


#include "object_frag.h"
#include "object_vert.h"

Flower_stem::Flower_stem() {

  //  scale *= 10.0f;
      scale /=11.0f;
      rotation = glm::vec3(4.7f, 0.0f, 0.0f);

    // Initialize static resources if needed
    if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
    if (!texture) texture = TexturePtr(new Texture{"g_stem.rgb", 600, 400});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "stonka.obj"});
}

Flower_stem::~Flower_stem() {
}

bool Flower_stem::Update(Scene &scene, float dt) {
    // Count time alive


    // Generate modelMatrix from position, rotation and scale
    GenerateModelMatrix();

    return true;
}


void Flower_stem::Render(Scene &scene) {
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
MeshPtr Flower_stem::mesh;
ShaderPtr Flower_stem::shader;
TexturePtr Flower_stem::texture;
