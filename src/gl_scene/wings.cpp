#include "wings.h"
#include "scene.h"
#include "object_frag.h"
#include "object_vert.h"

Wings::Wings() {
    position.z = -9.8f;


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