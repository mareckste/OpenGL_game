#include "mesh.h"
#include "object.h"

// This object renders the scene background
// It does not use the camera so it uses different shader program
// Background animation is achieved by passing an offset to the fragment shader that offsets texture mapping
class Flower_stem : public Object {
public:
    Flower_stem();
    ~Flower_stem();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;

private:

    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;

};
typedef std::shared_ptr< Flower_stem > Flower_stemPtr;