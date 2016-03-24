#ifndef WAHOO_STECHGRAPHICS_H
#define WAHOO_STECHGRAPHICS_H

#include "../STGlobal.h"

class Camera;
class STGame;

enum ST_YUpState{
    YPos_Down = false,
    YPos_Up = true
};

struct STRenderPass{

};

class STGraphics {
public:
    STGraphics();
    STGraphics(STGame *);
    ~STGraphics();

    void setCamera(Camera* cam){
        m_Cam = cam;
    }

    static void setYUp(bool val){ m_YUp = val; }
    static bool getYUpSetting(){ return m_YUp; }

    Camera* camera(){
        return m_Cam;
    }
protected:
    unsigned int WIDTH, HEIGHT;
    Camera* m_Cam;
    static bool m_YUp;
};


#endif //WAHOO_STECHGRAPHICS_H
