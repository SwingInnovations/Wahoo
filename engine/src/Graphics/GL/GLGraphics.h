#ifndef WAHOO_GLGRAPHICS_H
#define WAHOO_GLGRAPHICS_H

#include <regex>

#include "../../../include/GL/glew.h"
#include "../../Entity/STLight.h"
#include "../../Entity/STEntity.h"
#include "../../STSceneManager.h"
#include "../STGraphics.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "GLMesh.h"


class GLGraphics;

struct Character{
    GLuint texID;
    Vector2<int> size;
    Vector2<int> bearing;
    GLuint Advance;
};

struct GLRenderScene : public STRenderScene{
    GLRenderScene(){
        m_skybox = 0;
        m_skyboxShdr = nullptr;
        m_initiated = false;
        skyboxMesh = new GLMesh(new STCube(1000));
    }

    inline void initSkybox(const std::string& shdr, const std::string& map){
        m_skybox = GLTexture::loadCubemapTexture(map);
        m_skyboxShdr = new GLShader(shdr);
    }

    inline void drawSkybox(Camera& cam){
        glDisable(GL_CULL_FACE);
        glDepthFunc(GL_LEQUAL);
        m_skyboxShdr->bind();
        m_skyboxShdr->update(cam);
        glActiveTexture(GL_TEXTURE0);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox);

        skyboxMesh->draw();
        glDepthFunc(GL_LESS);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);
    }

    GLuint m_skybox;
    GLShader* m_skyboxShdr;
    GLMesh* skyboxMesh;
};

class GLGraphics : public STGraphics{
public:
    GLGraphics();
    GLGraphics(STGame*);

    std::string getVendor();

    void drawText(Vector2<stReal> pos, const std::string& text, stReal fontSize );
    void drawText(Vector2<stReal> pos, const std::string& text, stReal fontSize, Vector4<stReal>* color);
    void drawText(Vector2<stReal> pos, const std::string& text, stReal fontSize, stReal value);
    void drawText(Vector2<stReal> pos, const std::string& text, stReal fontSize, std::string& msg);
    void drawText(Vector2<stReal> pos, const std::string& text, stReal fontSize, stReal v1, stReal v2);
    void drawText(Vector2<stReal> pos, const std::string& text, stReal fontSize, Vector2<stReal> vector);
    void drawText(Vector2<stReal> pos, const std::string& text, stReal fontSize, stReal v1, stReal v2, stReal v3);
    void drawText(Vector2<stReal> pos, const std::string& text, stReal fontSize, Vector3<stReal> vector);
    Matrix4f getOrthographicProjection()const {
        return orthoProjection;
    }

    virtual void drawScene(STScene* scene);
    virtual void initScene(stUint index);

    inline void enableBlend(){
        glEnable(GL_BLEND);
    }

    inline void disableBlend(){
        glDisable(GL_BLEND);
    }
    static Vector3<stReal> TextColor;

protected:

private:
    std::map<GLchar, Character> characters;
    std::map<stUint, GLRenderScene> scenes;
    GLuint textVAO;
    GLuint textVBO;
    GLShader* textShader;
    Matrix4f orthoProjection;
    GLuint frameBuffer;
    GLuint frameTexBuffer;
    GLuint velocityBuffer;
    GLuint velocityTexture;
    GLuint rendBuffer;
    GLuint  bloomThresTex;
    GLuint bloomThresBuf;

    GLShader* screenShdr;
    GLShader* Bloom_Composite;
    GLShader* Bloom_Threshold;
    GLShader* Motion_Blur;

    GLMesh* screenQuad;
    STMaterial* m_directionalLightMat;
    STMaterial* m_pointLightMat;
    STMaterial* m_spotLightMat;
    STMaterial* m_albedoMat;
    STMaterial* m_IBLMat;
    STMaterial* m_velocityMat;
    GLuint Bloom();
    GLuint MotionBlur();
};


#endif //WAHOO_GLGRAPHICS_H
