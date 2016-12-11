#include "generator.h"
#include "asteroid.h"
#include "flower_head.h"
#include "flower_stem.h"
#include "watering_can.h"

bool Generator::Update(Scene &scene, float dt) {
  // Accumulate time
  time += dt;
    generateFlowers(scene);
  // Add object to scene when time reaches certain level

    flowers = 0;
    for ( auto obj : scene.objects ) {
        // Ignore self in scene
        if (obj.get() == this)
            continue;

        // We only need to collide with flowers, ignore other objects
        auto flower = std::dynamic_pointer_cast<Flower_head>(obj);
        if (!flower) continue;

        flowers++;
    }
    if (abs(flowers_history - flowers) > 1) {
        flowers_history = flowers;

        glm::vec3 points[4];
        points[0] = glm::vec3(Rand(-9, 9),6.63f,-12.3f);
        points[1] = glm::vec3(Rand(-9, 9),Rand(-4.0f,5.0f),-12.3f);
        points[2] = glm::vec3(Rand(-9, 9),Rand(-7.0f,-4.0f),-12.3f);
        points[3] = glm::vec3(Rand(-9, 9),-11,-12.3f);

        auto obj = CanPtr(new watering_can(points));
        obj->position = this->position;
        scene.objects.push_back(obj);
    }

    //flowers_history = flowers;

  return true;
}

void Generator::generateFlowers(Scene &scene) {
    if ( generated < 5) {
        generated++;

        auto obj = Flower_headPtr(new Flower_head());
        obj->position = this->position;
        obj->position.x += Rand(-9, 9); obj->position.y += Rand(-17,-0.75);
        obj->position.z = -7;
        scene.objects.push_back(obj);


        auto obj1 = Flower_stemPtr(new Flower_stem());
        obj1->position = this->position;
        obj1->position.x = obj->position.x;
        obj1->position.y = obj->position.y;
        obj1->position.z = -7;
        scene.objects.push_back(obj1);

        time = 0;
        flowers_history++;
    }
}

void Generator::Render(Scene &scene) {
  // Generator will not be rendered
}

Generator::~Generator() {
}

Generator::Generator() {
  time = 0;
}
