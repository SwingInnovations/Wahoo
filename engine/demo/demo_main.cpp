
#include "../src/Math/STCore.h"
#include "../src/STGame.h"
#include "../src/Entity/STEntity.h"
#include "../src/Graphics/GL/GLGraphics.h"
#include "../src/Entity/Components/STEventComponent.h"
#include "../src/Entity/STActor.h"
#include "../src/Graphics/Interface/STLabel.h"
#include "../src/Graphics/Interface/STButton.h"

class InputMap;
class STGame;


using namespace std;

class STGraphicsComponent;

class TestState : public STGameState {
public:
    TestState(int id){ this->m_id = id; }

    void init(STGame * window) {
        //window->getCamera()->setHAngle(90.0f);
        counter = 0;
        drawMode = STMesh::TRIANGLES;
        auto scene = STSceneManager::Get()->initScene((stUint) getID());


        auto resManager = window->getResourceManager();
        resManager->addMaterial("default", new STMaterial(new GLShader("standard", "standard_directional_forward")));
        GLGraphics::GlobalAmbient = Vector3<stReal>(.1,.1,.1);
        int count=0;

//        for (int i = 0; i< 5; i++) {
//            for(int j = 0; j< 5; j++) {
//
//                _testActors.push_back(new STActor("monkey.obj", STMesh::OBJ, resManager->getMaterial("default")));
//                _testActors.at(count)->setTranslateY(i *3 - 5.0f);
//                _testActors.at(count)->setTranslateX(j * 3 - 5.0f );
//                _testActors.at(count)->setShdrUniform("_Metallic", ((float)i)/4.0f);
//                _testActors.at(count)->setShdrUniform("_Roughness", ((float)j)/4.0f);
//                _testActors.at(count)->setScale(1);
//                scene->addActor(_testActors.at(count));
//                count++;
//            }
//         }
        STGame::Get()->getCamera()->setSpeed(0.005f);

        _testActor2 = new STActor("monkey.obj", STMesh::OBJ, resManager->getMaterial("default"));
        //_testActor2->setTranslateX(1);
        _testActor2->setShdrUniform("_Metallic", 1.0f);
        _testActor2->setShdrUniform("_Roughness",0.0f);
        _testActor2->setScale(1);

         roughnessTex = new GLTexture("roughness.png");

        //_testActor2->setShdrUniform_Texture("_RoughnessTex",roughnessTex->genTex("roughness.png"));
        //_testActor2->setTranslateY(-4);
        _testLight = new STLight(4,Vector3<stReal>(1,0,0));

        _testLight->setTranslateZ(2);


        _testLight2 = new STLight(Vector3<stReal>(-1,-1,-1),Vector3<stReal>(1,0,1));
       // _testLight2->intensity =1;
       // _testLight->setTranslateZ(1.2f);
       // _testLight->setTranslateX(1.2f);

        scene->addSkybox("lycksele", "skybox");


        scene->addLight(_testLight);

        scene->addActor(_testActor2);
        scene->addLight(_testLight2);
                STGraphics::ClearColor = Vector4<stReal>(0.0, 0.0, 0.168, 1.0);
    }

    void handleInput(STGame * win, Uint32 delta){
        Input* input = win->getInput();
        auto cam = win->getCamera();
        if(input->isKeyPressed(KEY::KEY_ESC)){
            input->requestClose();
        }

        if(input->isKeyPressed(KEY::KEY_Q)){
            bool state = input->isCursorBound();
            input->setCursorBound(!state);
            input->setCursorVisible(state);
        }

        if(input->isKeyPressed(KEY::KEY_6)) currObject = 0;

        if(input->isKeyPressed(KEY::KEY_7)) currObject = 1;

        if(input->isKeyPressed(KEY::KEY_0)){
            win->getCamera()->centerCam(input);
        }
    }

    void handleLogic(STGame * win, Uint32 delta){
//        int count=0;
//        for (int i = 0; i< 5; i++) {
//            for(int j = 0; j< 5; j++) {
//                _testActors.at(count)->update();
//                count++;
//            }
//        }
        counter += 0.025f * delta;
        _testLight->setTranslateY(3.0f*std::sin(counter*.1f));
      //  _testLight2->setTranslateY(3.0f*std::sin(counter*.02f+3));
    }

    void render(STGame * win){
        auto grphx = win->getGraphics();
        win->getGraphics()->drawScene(STSceneManager::Get()->getScene((stUint)getID()));

    }

    ~TestState(){
        delete _testActor;
        delete _testActor2;
        delete _testLight;
        delete _testLight2;
        delete _testLight3;
    }
private:
    STSceneManager* sceneManager;
    int drawMode;
    int currObject;
    float counter = 0;
    STButton* btn;
    STActor* _testActor;
    vector<STActor*> _testActors;
    STActor* _testActor2;
    STLight* _testLight;
    STLight* _testLight2;
    STLight* _testLight3;
    GLTexture* roughnessTex;
    STLabel* lbl;
    Vector3<stReal> lightPos;
    int width = 0, height = 0;
};


int main(int argc, char** argv){
    InputMap* inputMap = new InputMap;
    inputMap->addMapping(MOVEMENT::FORWARD, KEY::KEY_W);
    inputMap->addMapping(MOVEMENT::BACKWARD, KEY::KEY_S);
    inputMap->addMapping(MOVEMENT::STRAFE_LEFT, KEY::KEY_A);
    inputMap->addMapping(MOVEMENT::STRAFE_RIGHT, KEY::KEY_D);


    auto win = STGame::Init("Swing Tech 1", 1440, 720);
    win->setOpenGLVersion(4,0);
    win->setClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    win->setTargetFPS(120);
    STGraphics::YUp = false;
    win->getInput()->setInputMap(inputMap);
    Vector3<stReal> campos(0.0, 0, -3.0f);
    win->addCamera(new Camera(*win, campos, 0));
    win->addState(new TestState(0));
    win->enterState(0);

    win->start();

    return 0;
}
