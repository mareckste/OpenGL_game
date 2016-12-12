#include "watering_can.h"
#include "scene.h"

#include "explosion.h"
#include "bee.h"
#include "object_frag.h"
#include "object_vert.h"
#include "generator.h"
#include "Flower.h"

watering_can::watering_can(glm::vec3 wp[4]) {
    walkPath[0] = wp[0]; walkPath[1] = wp[1]; walkPath[2] = wp[2]; walkPath[3] = wp[3];

    scale /=24.0f;
    rotation = glm::vec3(4.7f, 0.0f, 0.0f);

    // Initialize static resources if needed
    if (!shader) shader = ShaderPtr(new Shader{object_vert, object_frag});
    if (!texture) texture = TexturePtr(new Texture{"can.rgb", 2048, 2048});
    if (!mesh) mesh = MeshPtr(new Mesh{shader, "krhla2.obj"});
}

watering_can::~watering_can() {
}

bool watering_can::Update(Scene &scene, float dt) {
    if (position.y < -9) return false;

    position = positionManager(dt);

    bool flag = false;
    GeneratorPtr generator;
    for ( auto obj : scene.objects ) {

        if (obj.get() == this)
            continue;

        if (!flag)
            generator = std::dynamic_pointer_cast<Generator>(obj);

        if (generator) flag = true;

        auto player = std::dynamic_pointer_cast<bee>(obj);

        if (!player) continue;

        if ((fabs(player->position.x - position.x) < .8) && (fabs(player->position.y - position.y) < .8) ) {
            // Explode
            generator->flowers_history += 2;

            auto explosion = ExplosionPtr(new Explosion{});
            explosion->position = player->position;
            explosion->scale *=  1.2f;
            scene.objects.push_back(explosion);

            generateFlowers(scene);
            generateFlowers(scene);

            return false;
        }
    }



    GenerateModelMatrix();

    return true;
}


void watering_can::Render(Scene &scene) {
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

void watering_can::applyLight(glm::vec3 *pointLightPositions, int i, float linModifier, float quadModifier) {
    std::string pointLightParam = std::string("pointLights[") + std::to_string(i) + std::string("]");
    glUniform3f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".position")).c_str()), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
    glUniform3f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".ambient")).c_str()), 0.05f, 0.05f, 0.05f);
    glUniform3f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".diffuse")).c_str()), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".specular")).c_str()), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".constant")).c_str()), 1.0f);
    glUniform1f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".linear")).c_str()), linModifier);
    glUniform1f(glGetUniformLocation(shader->program, (pointLightParam + std::string(".quadratic")).c_str()), quadModifier);
}

void watering_can::generateFlowers(Scene &scene) {

    auto obj = FlowerPtr(new Flower(Rand(-9, 9),Rand(-6.5f,9), -7));
    scene.objects.push_back(obj);

}

glm::vec3 watering_can::positionManager(float dt) {
    walkTime += 0.55f*dt;
    if(walkTime >= 1){
        walkTime = 0.0f;
        walkPointer = (walkPointer + 1) % 4;
    }
    return linInt(walkPath[walkPointer], walkPath[walkPointer + 1], walkTime);
}

glm::vec3 watering_can::linInt(glm::vec3 a, glm::vec3 b, float t) {
    return a + (b - a) * t;
}

// shared resources
MeshPtr watering_can::mesh;
ShaderPtr watering_can::shader;
TexturePtr watering_can::texture;
