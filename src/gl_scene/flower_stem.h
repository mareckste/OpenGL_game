#include "mesh.h"
#include "object.h"


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