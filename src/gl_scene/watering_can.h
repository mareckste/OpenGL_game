#include "mesh.h"
#include "object.h"

// This object renders the scene background
// It does not use the camera so it uses different shader program
// Background animation is achieved by passing an offset to the fragment shader that offsets texture mapping
class watering_can : public Object {
public:
    watering_can(glm::vec3 wp[4]);
    ~watering_can();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;
    glm::vec3 positionManager(float dt);
    glm::vec3 linInt(glm::vec3 a, glm::vec3 b, float t);


private:
    // float offset;
    // Static resources (Shared between instances)
    float walkTime = 0.0f;
    int walkPointer = 0;
    glm::vec3 walkPath[4];
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
    float age = 0;
    const float maxAge = 2.3f;
};
typedef std::shared_ptr< watering_can > CanPtr;