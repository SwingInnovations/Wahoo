#ifndef WAHOO_STMATERIAL_H
#define WAHOO_STMATERIAL_H

#include "Texture.h"
#include "Shader.h"

class STMaterial{

private:
    Texture* texture;
    Shader* shader;
    bool useTexture;
};

#endif //WAHOO_STMATERIAL_H