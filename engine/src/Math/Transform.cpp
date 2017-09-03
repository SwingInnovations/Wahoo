#include "Transform.h"
#include "../Entity/STEntity.h"

Transform::Transform(STEntity *parent) {
    this->parent = parent;
    this->translate = Vector3<stReal>(0.f, 0.f, 0.f);
    rotate = Vector3<stReal>(0.0f, 0.0f, 0.0f);
    scale = Vector3<stReal>(1.0f, 1.0f, 1.0f);
    rotateMode = Local;
}

void Transform::setTranslate(Vector3<stReal> &vec) {
    this->translate = vec;
    if(parent != nullptr){
        if(parent->hasChildren()){
            for(auto child : parent->getChildren()){
                auto childPos = child->transform()->getTranslate();
                childPos = childPos + vec;
                child->transform()->setTranslate(childPos);
            }
        }
    }
}

void Transform::setTranslateX(stReal _x) {
    this->translate.setX(_x);
    if(parent != nullptr && parent->hasChildren()){
        for(auto child : parent->getChildren()){
            stReal cX = child->transform()->getTranslate().getX();
            cX += _x;
            child->transform()->setTranslateX(cX);
        }
    }
}

void Transform::setTranslateY(stReal _y) {
    this->translate.setY(_y);
    if(parent != nullptr && parent->hasChildren()){
        for(auto child : parent->getChildren()){
            stReal cY = child->transform()->getTranslate().getY() + _y;
            child->transform()->setTranslateY(cY);
        }
    }
}

void Transform::setTranslateZ(stReal _z) {
    this->translate.setZ(_z);
    if(parent != nullptr && parent->hasChildren()){
        for(auto child : parent->getChildren()){
            child->transform()->setTranslateZ(child->transform()->getTranslate().getZ() + _z);
        }
    }
}

const Matrix4f Transform::getModel() const {
    Matrix4f trans, rot, scaleMat, rotX, rotY, rotZ;
    trans.initTranslation(translate);
    if(rotateMode == Global){
        rot.initRotate(rotate);
    }else{
        Matrix4f negTransMat;
        negTransMat.initTranslation(-translate.getX(), -translate.getY(), -translate.getZ());
        rot.initRotate(rotate);
        rot = trans * rot * negTransMat;
    }
    scaleMat.initScale(scale);

    Matrix4f ret;
    ret = scaleMat * rot * trans;
    return ret;
}

void Transform::setTranslate(stReal _value) {
    setTranslateX(_value);
    setTranslateY(_value);
    setTranslateZ(_value);
}

void Transform::setRotate(Vector3<stReal> &vec) {
    this->rotate = vec;
}

void Transform::setRotateX(stReal _x) {
    rotate.setX(_x);

}

void Transform::setRotateZ(stReal _z) {
    rotate.setZ(_z);
}

void Transform::setRotateY(stReal _y) {
    rotate.setY(_y);
}

std::string Transform::getInfo() {
    Matrix4f mat = getModel();
    return mat.getInfo();
}

Json Transform::to_json() const {
    return Json::object{
            {"translate", translate},
            {"rotate", rotate},
            {"scale", scale}
    };
}
