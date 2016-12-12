#include "mesh.h"
#include "object.h"


class Flower_head : public Object {
public:
    Flower_head();
    ~Flower_head();

    bool Update(Scene &scene, float dt) override;
    void Render(Scene &scene) override;

private:

    static MeshPtr mesh;
    static ShaderPtr shader;
    static TexturePtr texture;

};
typedef std::shared_ptr< Flower_head > Flower_headPtr;