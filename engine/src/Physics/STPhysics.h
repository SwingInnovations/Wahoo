#ifndef SWINGTECH1_STPHYSICS_H
#define SWINGTECH1_STPHYSICS_H

#include "../Math/STCore.h"
#include "../Math/Vector.h"
#include "../Application/Util/Data_Structure/STList.h"

class STScene;
class STEntity;
/**
 * Base class for initializing physics.
 */
class STPhysics{
public:
    enum PhysicsEngine : unsigned char{
        NONE = 0,
        BULLET = 1
    };

    explicit STPhysics();

    virtual void init() = 0;
    virtual void init(PhysicsEngine engine);
    virtual void initScene(STScene* scene) = 0;
    virtual void update(stUint delta)  = 0;
    virtual void setGravity(stReal gravity) = 0;
    virtual void dispose() = 0;
    inline PhysicsEngine getPhysicsEngineMode() const{ return m_physicsEngine; }
    STList<STEntity*> Raycast(Vector3D start, Vector3D end)const;
    virtual STList<STEntity*> RaycaseHelper(Vector3D start, Vector3D end) = 0;
protected:
    STScene* m_scene;
    PhysicsEngine  m_physicsEngine;

};

#endif //SWINGTECH1_STPHYSICS_H
