#ifndef WAHOO_STGRAPHICSCOMPONENT_H
#define WAHOO_STGRAPHICSCOMPONENT_H

#include "../../Graphics/Shader.h"
#include "../../Graphics/Texture.h"
#include "STComponent.h"

#include "../../Math/Vector.h"
#include "../../Graphics/STMaterial.h"


class STComponent;

class STGraphicsComponent : public STComponent{
public:
    explicit STGraphicsComponent(Shader* shdr);
    STGraphicsComponent(const STGraphicsComponent& copy);
    void initScriptingFunctions(sol::state& state) override;

    explicit STGraphicsComponent(STMaterial* mat);

    explicit STGraphicsComponent(const std::string& shdr);
    ~STGraphicsComponent() override {
        delete m_material;
        for(auto uniform : m_Uniforms){
            if(uniform.second.type == STShader::TEX) {
                auto texHandle = (stUint)STShader::toVector2(uniform.second.value).getX();
                glDeleteTextures(1, &texHandle);
            }
        }
    }

    void addShdrUniform(const std::string& name, int value);
    void addShdrUniform(const std::string& name, float value);
    void addShdrUniform(const std::string& name, Vector2<stReal> value);
    void addShdrUniform(const std::string& name, Vector3<stReal> value);
    void addShdrUniform(const std::string& name, Vector4<stReal> value);
    void addShdrUniform(const std::string& name, Matrix4f value);
    void addShdrUniform_Texture(const std::string& name, stUint value);
    void addShdrUniform_Texture2DArray(const std::string& name, stUint value);
    void addShdrUniform_CubeMap(const std::string& name, stUint value);
    void setDiffuseTexture(const std::string& fileName);
    void setNormalTexture(const std::string& fileName);
    void setShdrUniform_Texture(const std::string& name, stUint id, stUint index);
    void setShdrUniform_Texture2DArray(const std::string& name, stUint id, stUint index);

    void setShdrUniform(const std::string& name, int value);
    void setShdrUniform(const std::string& name, float value);
    void setShdrUniform(const std::string& name, Vector2<stReal> value);
    void setShdrUniform(const std::string& name, Vector3<stReal> value);
    void setShdrUniform(const std::string& name, Vector4<stReal> value);
    void setShdrUniform(const std::string& name, Matrix4f value);
    void setShdrUniform_Texture(const std::string& name, stUint value);
    void setShdrUniform_CubeMap(const std::string& name, stUint value);

    inline void setShader(Shader* shdr){ m_shdr = shdr; }

    inline STMaterial* getMaterial(){ return m_material; }

    inline Shader* shdr(){
        if(m_material != nullptr){
            return m_material->shdr();
        }
        return m_shdr;

    }

    void update() override;
    void draw() override;
    void dispose() override;

    std::map<std::string, STShader::ShaderAttrib> &GetUniforms();

    void draw(Transform& T, Camera& C);
private:
    Shader* m_shdr;
    STMaterial* m_material;
    bool useTexture;
    bool useMaterial;
    std::map<std::string, STShader::ShaderAttrib> m_Uniforms;
};


#endif //WAHOO_STGRAPHICSCOMPONENT_H
