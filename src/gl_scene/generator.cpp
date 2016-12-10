#include "generator.h"
#include "asteroid.h"
#include "flower_head.h"
#include "flower_stem.h"

bool Generator::Update(Scene &scene, float dt) {
  // Accumulate time
  time += dt;

  // Add object to scene when time reaches certain level
  if ( time > 2 && generated < 5) {
      generated++;
    auto obj = Flower_headPtr(new Flower_head());
    obj->position = this->position;
    obj->position.x += Rand(-9, 9); obj->position.y += Rand(-17,-0.75);
      obj->position.z = -7;
    //obj1->position.y += Rand(-20, 20);
    scene.objects.push_back(obj);
    auto obj1 = Flower_stemPtr(new Flower_stem());
    obj1->position = this->position;
    obj1->position.x = obj->position.x;
      obj1->position.y = obj->position.y;
      obj1->position.z = -7;
    //obj1->position.y += Rand(-20, 20);
    scene.objects.push_back(obj1);
    time = 0;

  }

  return true;
}

void Generator::Render(Scene &scene) {
  // Generator will not be rendered
}

Generator::~Generator() {
}

Generator::Generator() {
  time = 0;
}
