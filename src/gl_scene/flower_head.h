#include "mesh.h"
#include "object.h"

// This object renders the scene background
// It does not use the camera so it uses different shader program
// Background animation is achieved by passing an offset to the fragment shader that offsets texture mapping
class Flower_head : public Object {
public:
    Flower_head();
    ~Flower_head();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;

private:
    // float offset;
    // Static resources (Shared between instances)
    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;
};
typedef std::shared_ptr< Flower_head > Flower_headPtr;