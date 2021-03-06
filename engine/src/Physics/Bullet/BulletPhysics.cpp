#include "BulletPhysics.h"
#include "../../Application/STSceneManager.h"
#include "../../Entity/Components/ST3DPhysicsComponent.h"
#include "BulletRigidBody.h"
#include "../../Entity/Components/STEventComponent.h"
#include <iostream>

BulletPhysics::BulletPhysics() {
    m_collisionConfiguration = nullptr;
    m_dispatcher = nullptr;
    m_broadphase = nullptr;
    m_solver = nullptr;
    m_dynamicsWorld = nullptr;
}

void BulletPhysics::init() {
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_broadphase = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
    m_dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void BulletPhysics::update(stUint delta) {
    if(m_dynamicsWorld){
        m_dynamicsWorld->stepSimulation(delta);

        for(stUint i = 0; i < m_dynamicsWorld->getDispatcher()->getNumManifolds(); i++){
            auto collision = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
            auto gameObject = ((STEntity*)collision->getBody0()->getUserPointer());
            auto otherObject = ((STEntity*)collision->getBody1()->getUserPointer());
            auto ev1 = gameObject->get<STEventComponent>();
            auto ev2 = otherObject->get<STEventComponent>();
            ev1->setOther(otherObject);
            ev2->setOther(gameObject);

            ev1->setEvent("onCollision");
            ev2->setEvent("onCollision");
        }

        for(int i = 0, L = m_dynamicsWorld->getNumCollisionObjects(); i < L; i++){
            auto collisionObj = m_dynamicsWorld->getCollisionObjectArray()[i];
            btRigidBody* rigidBody = btRigidBody::upcast(collisionObj);
            btTransform transform;
            btScalar  rX, rY, rZ;
            if(rigidBody->getMotionState() != nullptr){
                rigidBody->getMotionState()->getWorldTransform(transform);
            }else{
                transform = rigidBody->getWorldTransform();
            }
            transform = rigidBody->getWorldTransform();
            transform.getRotation().getEulerZYX(rZ, rY, rX);

            rX = toDegree(rX);
            rY = toDegree(rY);
            rZ = toDegree(rZ);

            auto entityTransform = ((STEntity*)rigidBody->getUserPointer())->transform();
            entityTransform->setTranslate(Vector3D(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ()));
            entityTransform->setRotationMode(Transform::Local);
            entityTransform->setRotate({rX, rY, rZ});
        }
    }
}


void BulletPhysics::setGravity(stReal gravity) {
    if(m_dynamicsWorld){
        m_dynamicsWorld->setGravity(btVector3(0, -gravity, 0));
    }
}

void BulletPhysics::dispose() {
//    delete m_collisionConfiguration;
//    delete m_dispatcher;
//    delete m_broadphase;
//    delete m_solver;
//    delete m_dynamicsWorld;
}



void BulletPhysics::init(STPhysics::PhysicsEngine engineMode) {
    m_physicsEngine = engineMode;
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_broadphase = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver;
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
    if(m_dynamicsWorld){
        std::cout << "Using Bullet Physics Engine" << std::endl;
    }
}

void BulletPhysics::clearScene() {
    m_dynamicsWorld->getCollisionObjectArray().clear();
}

void BulletPhysics::initScene(STScene *scene) {
    m_scene = scene;
    for(auto actor : scene->getActors()){
        auto physComponent = actor->get<ST3DPhysicsComponent>();
        if(physComponent != nullptr){
            auto rigidBody = ((BulletRigidBody*)physComponent->getRigidBody())->getRigidBody();
            m_dynamicsWorld->addRigidBody(rigidBody);
        }
    }
}

BulletPhysics::~BulletPhysics() {
    delete m_collisionConfiguration;
    delete m_dispatcher;
    delete m_broadphase;
    delete m_solver;
    delete m_dynamicsWorld;
}

STList<STEntity *> BulletPhysics::RaycaseHelper(Vector3D start, Vector3D end) {
    auto btStart = btVector3(start.getX(), start.getY(), start.getZ());
    auto btEnd = btVector3(end.getX(), end.getY(), end.getZ());
    auto ret = STList<STEntity *>();

    btCollisionWorld::AllHitsRayResultCallback RayCallback(btStart, btEnd);
    m_dynamicsWorld->rayTest(btStart, btEnd, RayCallback);
    if(RayCallback.hasHit()){
        for(stUint i = 0, L = (stUint)RayCallback.m_collisionObjects.size(); i < L; i++)
        {
            ret.addLast((STEntity*)RayCallback.m_collisionObjects[i]->getUserPointer());
        }
    }
    return ret;
}
