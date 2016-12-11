#include "generator.h"
#include "watering_can.h"
#include "Flower.h"

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
        auto flow = std::dynamic_pointer_cast<Flower>(obj);
        if (!flow) continue;

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


  return true;
}

void Generator::generateFlowers(Scene &scene) {
    if ( generated < 5) {
        generated++;

        auto obj = FlowerPtr(new Flower(position.x + Rand(-9,9), position.y + Rand(-17,-0.75), -7));
        scene.objects.push_back(obj);

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
