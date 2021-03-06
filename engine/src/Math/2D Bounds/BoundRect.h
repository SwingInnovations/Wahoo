#ifndef WAHOO_BOUNDRECT_H
#define WAHOO_BOUNDRECT_H

#include "BoundShape.h"
#include "../Shape/Rect.h"

class BoundRect : public BoundShape{
public:
    BoundRect(){
        points.push_back(0);
        points.push_back(0);
        points.push_back(1);
        points.push_back(1);

        m_iterations = 1;
    }

    BoundRect(stReal x, stReal y, stReal width, stReal height){
        m_iterations = 1;

        points.push_back(x);
        points.push_back(y);
        points.push_back(width);
        points.push_back(height);
    }

    BoundRect(const STRect& rect){

    }

    void reset(const stReal x, const stReal y, const stReal w, const stReal h){
        points[0] = x;
        points[1] = y;
        points[2] = w;
        points[3] = h;
    }

    virtual bool contains(const BoundShape &other){
        return false;
    }

    std::string info(){
        std::ostringstream ss;
        for(unsigned int i = 0; i < 4; i++){
            if(i < 3) ss << points.at(i) << " : ";
            else ss << points.at(i) << "\n";
        }
        return ss.str();
    }

    virtual bool contains(const Vector2<stReal> &cursor){
        stReal x = points.at(0);
        stReal y = points.at(1);
        stReal w = points.at(2);
        stReal h = points.at(3);

        return cursor.getX() >= x &&
        cursor.getX() <= x + w &&
        cursor.getY() >= y &&
        cursor.getY() <= y + h;
    }

    stReal getWidth() const { return points.at(2); }
    stReal getHeight() const { return points.at(3); }
};

#endif //WAHOO_BOUNDRECT_H