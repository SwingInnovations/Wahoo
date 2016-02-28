#ifndef WAHOO_STGRAPHICSCOMPONENT_H
#define WAHOO_STGRAPHICSCOMPONENT_H

#include <vector>

#include "Graphics/Shader.h"
#include "Graphics/Texture.h"
#include "STComponent.h"

#include "Math/Vector.h"

class STGraphicsComponent : public STComponent{
public:
    STGraphicsComponent(Shader* shdr);
    STGraphicsComponent(Shader* shdr, Texture* tex);
    STGraphicsComponent(const std::string& shdr);
    STGraphicsComponent(const std::string& shdrPath, const std::string& texPath);
    ~STGraphicsComponent(){
        delete m_shdr;
        if(useTexture) delete m_tex;
    }

    void addShdrAttrib(const std::string& name, int value);
    void addShdrAttrib(const std::string& name, float value);
    void addShdrAttrib(const std::string& name, Vector3<stReal> value);
    void addShdrAttrib(const std::string& name, Vector4<stReal> value);

    void setShdrAttrib(const std::string& name, int value);
    void setShdrAttrib(const std::string& name, float value);
    void setShdrAttrib(const std::string& name, Vector3<stReal> value);
    void setShdrAttrib(const std::string& name, Vector4<stReal> value);

    void setShader(Shader* shdr){
        m_shdr = shdr;
    }

    Shader* shdr(){ return m_shdr; }

    virtual void update(STGame * window, int delta){
        //Do Nothing

    }

    void draw(){
        //TODO Include shader handling and other stuff.
        m_shdr->bind();
        for(unsigned int i = 0, S = m_uniforms.size(); i < S; i++) {
            if (m_uniforms[i].type == STShader::INT) {
                m_shdr->update(m_uniforms[i].name, STShader::toInt(m_uniforms[i].value));
            } else if (m_uniforms[i].type == STShader::FLOAT) {
                m_shdr->update(m_uniforms[i].name, STShader::toFloat(m_uniforms[i].value));
            } else if (m_uniforms[i].type == STShader::VEC3) {
                m_shdr->update(m_uniforms[i].name, STShader::toVector3(m_uniforms[i].value));
            } else {
                m_shdr->update(m_uniforms[i].name, STShader::toVector4(m_uniforms[i].value));
            }
        }
        if(useTexture) m_tex->bind(0);
    }
private:
    Shader* m_shdr;
    Texture* m_tex;
    bool useTexture;
    std::vector<STShader::ShaderAttrib> m_uniforms;
};


#endif //WAHOO_STGRAPHICSCOMPONENT_H
