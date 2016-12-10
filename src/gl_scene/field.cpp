#include "field.h"
#include "scene.h"


#include "object_frag.h"
#include "object_vert.h"

Field::Field() {

    scale *= 10.0f;
    position.z = 1;

    // Initialize static resources if needed
    if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
    if (!texture) texture = TexturePtr(new Texture{"grass.rgb", 2048, 2048});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "boxx.obj"});
}

Field::~Field() {
}

bool Field::Update(Scene &scene, float dt) {
    // Count time alive


    // Generate modelMatrix from position, rotation and scale
    GenerateModelMatrix();

    return true;
}


void Field::Render(Scene &scene) {
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
MeshPtr Field::mesh;
ShaderPtr Field::shader;
TexturePtr Field::texture;
