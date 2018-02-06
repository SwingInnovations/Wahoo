#include "../src/Math/STCore.h"
#include "../src/Application/STGame.h"
#include "../src/Application/STSceneManager.h"
#include "../src/Application/Util/STJson.h"
#include "../src/Entity/Components/ST3DAnimationComponent.h"
#include "../src/Entity/Components/ST3DPhysicsComponent.h"
#include "../src/Application/Util/File/STFileManager.h"
class STFileManager;

/**
 * This is an example class for demonstrating How a typical game state would be setup.
 */
class SampleState : public STGameState{
public:
    SampleState(stInt id){ m_id = id; }
    void init(STGame* game) override {
        m_scene->addSkybox("green");
        game->getCamera()->setSpeed(0.025f);

        auto mainLight = STLight::InitDirectionalLight(Vector3D(4.f, 5.f, 3.f), Vector3D(-.5f, -.5f, -.5f), Vector3D(0.25f, 0.25f, 0.85f));
        mainLight->get<STLightComponent>()->setTarget(Vector3D(0.f, 0.f, 0.f));

        auto accentLight = STLight::InitDirectionalLight(Vector3D(2.f, 4.f, -3.f), Vector3D(), Vector3D(1.f, 1.f, 1.f));
        accentLight->get<STLightComponent>()->setTarget(Vector3D());

        auto accentLight2 = STLight::InitDirectionalLight(Vector3D(-2.f, 4.f, -2.f), Vector3D(), Vector3D(0.f, 0.2f, 1.f));
        accentLight2->get<STLightComponent>()->setTarget(Vector3D());
        accentLight2->get<STLightComponent>()->getProperties()->intensity = 0.9f;

        auto sphere = STActor::Create("smooth_Sphere.obj");
        sphere->transform()->setTranslate( {1.f, 5.f, 1.f} );
        auto physHandle = sphere->addComponent<ST3DPhysicsComponent>(new ST3DPhysicsComponent(STRigidBody::SPHERE, {0.9f}));
        physHandle->updateTransform();
        sphere->setName("Sphere");
        sphere->setTag("object");

        STFileManager::Write("testEntity.bin", sphere);
        auto sp2 = STFileManager::Read<STActor>("testEntity.bin");

        STFileManager::Write("testLight", accentLight2);

        auto l = STFileManager::Read<STLight>("testLight");

        auto p = STActor::Create("plane.obj");
        p->get<STRendererComponent>()->getMaterial()->setDiffuseTexture("grid.png");
        p->transform()->setTranslateY(-2.f);
        auto pHandle = p->addComponent<ST3DPhysicsComponent>(new ST3DPhysicsComponent(STRigidBody::RigidBodyShape::BOX, {5.f, 0.1f, 5.f}));
        pHandle->toggleFreeze(true);
        pHandle->updateTransform();

        m_scene->addLight(mainLight);
        m_scene->addLight(accentLight);
        m_scene->addLight(l);
        m_scene->addActor(p);
        m_scene->addActor(sp2);
    }

    void update(STGame* game) override{
        auto input = game->getInput();
        if(input->isKeyPressed(KEY::KEY_ESC)) input->requestClose();
        if(input->isKeyPressed(KEY::KEY_Q)){
            input->setCursorBound(!input->isCursorBound());
        }

        if(input->isKeyPressed(KEY::KEY_F)){
            counter++;
            game->setFullScreen(counter % 2);
        }

//		if (input->isKeyDown(KEY::KEY_L)) {
//			auto rZ = plane->transform()->getRotate().getZ();
//			plane->transform()->setRotateZ(rZ + 0.025f * game->getDelta());
//			plane->get<ST3DPhysicsComponent>()->updateTransform();
//		}
//
//		if (input->isKeyDown(KEY::KEY_J)) {
//			auto rZ = plane->transform()->getRotate().getZ();
//			plane->transform()->setRotateZ(rZ - 0.025f * game->getDelta());
//			plane->get<ST3DPhysicsComponent>()->updateTransform();
//		}
//
//        if(input->isKeyDown(KEY::KEY_I)){
//            auto rX = plane->transform()->getRotate().getX();
//            plane->transform()->setRotateX(rX + 0.025f * game->getDelta());
//            plane->get<ST3DPhysicsComponent>()->updateTransform();
//        }
//
//        if(input->isKeyDown(KEY::KEY_K)){
//            auto rX = plane->transform()->getRotate().getX();
//            plane->transform()->setRotateX(rX - 0.025f * game->getDelta());
//            plane->get<ST3DPhysicsComponent>()->updateTransform();
//        }

        m_scene->update();
    }

    void render(STGame* game) override {
        game->getGraphics()->drawScene(m_scene);
    }

    ~SampleState(){
        std::cout << "Clearing Game State" << std::endl;
        m_scene->dispose();
        delete m_scene;
    }

private:
    stUint counter;
};

int main(int argc, char** argv){
    auto inputMapping = new InputMap("Input.json");

    auto win = STGame::Init("Swing Tech", 1440, 720, STPhysics::PhysicsEngine::BULLET);
    win->setOpenGLVersion(4, 0);
    win->setTargetFPS(60);
    STGraphics::YUp = false;
    win->getInput()->setInputMap(inputMapping);
    win->addCamera(new Camera(*win, Vector3D(0.f, 1.f, -1.f), 0));
    win->addState(new SampleState(0));
    win->enterState(0);
    win->getGraphics()->enableShadow(true);
    win->getGraphics()->setRenderMode(STGraphics::DEFERRED);
    win->start();

    return 0;
}