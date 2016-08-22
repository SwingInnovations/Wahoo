#ifndef WAHOO_STSCENEMANAGER_H
#define WAHOO_STSCENEMANAGER_H

#include <vector>
#include <map>
#include "Entity/STEntity.h"

class STActor;
class STLight;
class STInterWidget;

template<typename G>
class Node{
protected:
    Node* parent;
    G* data;
    virtual void split() = 0;
};

template<typename GameObject>
class QuadNode : public Node<GameObject>{
public:
    QuadNode(){
        this->parent = nullptr;
        this->data = nullptr;
        childCount = 0;
    }

    QuadNode(GameObject* data){
        this->parent = nullptr;
        this->data = data;
        childCount = 0;
    }

    QuadNode(QuadNode* parent, GameObject* data){
        this->parent = parent;
        this->data = data;
        childCount = 0;
    }

    inline void setData(GameObject* data){
        this->data = data;
    }

    inline void addChild(GameObject* childData){
        if(childCount < 4){
            if(this->data != nullptr){
                children[childCount] = new QuadNode(this, this->data);
                delete this->data;
                childCount++;
            }
            children[childCount] = new QuadNode(this, childData);
            childCount++;
        }
        //Do something when all children are filled;
    }

    GameObject* getData(){ return this->data; }

protected:
    inline void split() override {
        //Do something to split.
    }
private:
    QuadNode* children[4] = {nullptr, nullptr, nullptr, nullptr};
    stUint childCount;
};

template<typename GameObject>
class OctoNode : public Node<GameObject> {
public:
    OctoNode(){
        this->parent = nullptr;
        this->data = nullptr;
        this->childCount = 0;
    }

    OctoNode(GameObject* data){
        this->parent = nullptr;
        this->data = data;
        this->childCount = 0;
    }

    OctoNode(OctoNode* parent, GameObject* data){
        this->parent = parent;
        this->data = data;
        this->childCount = 0;
    }

    inline void setParent(OctoNode* parent){ this->parent = parent; }
    inline void setData(GameObject* data){ this->data = data; }

    inline void addChild(GameObject* data){
        if(this->childCount < 8){
            if(this->data != nullptr){
                children[this->childCount] = new OctoNode(this, this->data);
                delete this->data;
                this->data = nullptr;
                this->childCount++;
            }
            children[this->childCount] = new OctoNode(this, data);
            this->childCount++;
        }

    }
protected:
    inline void split() override {
        // Do something to split

    }

private:
    OctoNode* children[8] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    stUint childCount;
};

class STScene{
public:
    STScene(){;}

    inline void addActor(STActor* actor){
        actors.push_back(actor);
    }

    inline void addLight(STLight* light){
        lights.push_back(light);
    }

    inline void addUIElement(STInterWidget* ui){
        uiElements.push_back(ui);
    }

    const std::vector<STActor*> &getActors()const{ return actors; }
    const std::vector<STLight*> &getLights()const{ return lights; }
    const std::vector<STInterWidget*> &getUIElements()const{ return uiElements; }

private:
    std::vector<STActor*> actors;
    std::vector<STLight*> lights;
    std::vector<STInterWidget*> uiElements;
};

class STSceneManager{
public:
    static STSceneManager* m_instance;

    static STSceneManager* Get(){
        if(m_instance == nullptr) m_instance = new STSceneManager;
        return m_instance;
    }

    STSceneManager(){ m_NumLights = 0; }

    inline void initScene(){
        scenes.push_back(new STScene());
    }

    inline STScene* getScene(stUint index){
        if(index < scenes.size()){
            return scenes.at(index);
        }
    }

    void addEntity(STEntity* entity){
        m_Entities.push_back(entity);
    }

    void addLight(STLight* light){

    }

    void addSkyBox(const std::string& file, const std::string& shader){
        m_skyboxName = file;
        m_skyboxShader = shader;
    }

    void addSkyCube(const std::string& file){
        m_skyboxName = file;
    }

    void addSkyboxShader(const std::string& shader){
        m_skyboxShader = shader;
    }

    const std::vector<STEntity *> &getEntities() const {
        return m_Entities;
    }

    const std::vector<STLight *> &getLights() const {
        return m_Lights;
    }

    const std::string getSkyboxName()const{ return m_skyboxName; }
    const std::string getSkyboxShader()const{return m_skyboxShader;}
private:
    std::vector<STEntity*> m_Entities;
    std::vector<STLight*> m_Lights;

    std::string m_skyboxName;
    std::string m_skyboxShader;

    int m_NumLights;

    std::vector<STScene*> scenes;
};

#endif //WAHOO_STSCENEMANAGER_H
