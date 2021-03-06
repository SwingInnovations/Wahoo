#ifndef WAHOO_STRECTBOUNDSCOMPONENT_H
#define WAHOO_STRECTBOUNDSCOMPONENT_H

#include "STComponent.h"
#include "../../Math/2D Bounds/BoundRect.h"

class STRectBoundsComponent : public STComponent {
public:
    STRectBoundsComponent(stReal _x, stReal _y, stReal _width, stReal _height);
    STRectBoundsComponent(stReal _x, stReal _y, stReal _width, stReal _height, bool YUpFlag);

    void update();
    inline BoundRect* bounds(){ return rect; }
    void reset(const stReal x, const stReal y, const stReal w, const stReal h);
    std::string info();

    void save(std::ofstream& out){

    }

    void load(std::ifstream& in){

    }

private:
    BoundRect* rect;
};



#endif //WAHOO_STRECTBOUNDSCOMPONENT_H
