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
//-6, -12.75f



    shader->Use();

    // use camera
    shader->SetMatrix(scene.camera->projectionMatrix, "ProjectionMatrix");
    shader->SetMatrix(scene.camera->viewMatrix, "ViewMatrix");
    // render mesh
    shader->SetMatrix(modelMatrix, "ModelMatrix");
    shader->SetTexture(texture, "Texture");

    glUniform3f(glGetUniformLocation(shader->program, "viewPos"), scene.camera->position.x, scene.camera->position.y, scene.camera->position.z);
    applyLight(pointLightPositions, 0, 2 / 55, 1 / (55 * 55));
    applyLight((pointLightPositions + 1),1 , 2 / 55, 1 / (55 * 55));

    mesh->Render();
}

void Field::applyLight(glm::vec3 *pointLightPositions, int i, float linModifier, float quadModifier) {
    std::string pointLightParam = std::string("pointLights[") + std::to_string(i) + std::string("]");
    glUniform3f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".position")).c_str()), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    glUniform3f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".ambient")).c_str()), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".diffuse")).c_str()), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".specular")).c_str()), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".constant")).c_str()), 1.0f);
    glUniform1f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".linear")).c_str()), linModifier);
    glUniform1f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".quadratic")).c_str()), quadModifier);
}

// shared resources
MeshPtr Field::mesh;
ShaderPtr Field::shader;
TexturePtr Field::texture;
