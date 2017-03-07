#ifndef WAHOO_STLIGHT_H
#define WAHOO_STLIGHT_H


#include "../Math/STCore.h"
#include "../Math/Vector.h"
#include "../Math/Transform.h"
#include "../Graphics/STMaterial.h"
#include "../Graphics/GL/GLShader.h"
#include "Components/STGraphicsComponent.h"
#include "Components/STMeshComponent.h"
#include "Components/STEventComponent.h"
#include "Components/STLightComponent.h"


class STLight :public STEntity{
public:

    static STLight* InitDirectionalLight(Vector3<stReal> position, Vector3<stReal> direction, Vector3<stReal> color){
        STLight* ret = new STLight;
        ret->addComponent(typeid(STLightComponent), new STLightComponent);
        ret->setTranslate(position);
        ret->get<STLightComponent>()->setType(STLightComponent::DIRECTIONAL_LIGHT);
        auto lightProps = ret->get<STLightComponent>()->getProperties();
        lightProps->color = color;
        lightProps->spotLightAtribs = Vector2<stReal>(0.f, 0.f);
        lightProps->direction = Vector4<stReal>(direction, 0.0f);
        lightProps->intensity = 1.f;
        return ret;
    }

    static STLight* InitSpotLight(Vector3<stReal> position, Vector3<stReal> direction, Vector3<stReal> color, stReal coneAngle, stReal coneDistance)
    {
        STLight* ret = new STLight;
        ret->setTranslate(position);
        ret->addComponent(typeid(STLightComponent), new STLightComponent);
        ret->get<STLightComponent>()->setType(STLightComponent::SPOT_LIGHT);
        auto lightProps = ret->get<STLightComponent>()->getProperties();
        lightProps->color = color;
        lightProps->direction = Vector4<stReal>(direction, 1.f);
        lightProps->spotLightAtribs = Vector2<stReal>(coneAngle, coneDistance);
        lightProps->intensity = 1.f;
        return ret;
    }

    static STLight* InitPointLight(Vector3<stReal> position, Vector3<stReal> color, stReal radius){
        STLight* ret = new STLight;
        ret->setTranslate(position);
        ret->addComponent(typeid(STLightComponent), new STLightComponent);
        ret->get<STLightComponent>()->setType(STLightComponent::POINT_LIGHT);
        auto lightProps = ret->get<STLightComponent>()->getProperties();
        lightProps->radius = radius;
        lightProps->direction = Vector4<stReal>(0.f, 0.f, 0.f, -1.f);
        lightProps->intensity = 1.f;
        return ret;
    }

    STLight(){
        m_transform = new Transform();
    }

    ~STLight(){
       delete m_material;
    }

    stUint shadowMapID[6];
    stUint shadowFrameBuffer[6];
    Matrix4f projections[6];
    Vector3<stReal> target;
private :
    STMaterial* m_material;
};




#endif //WAHOO_STLIGHT_H
