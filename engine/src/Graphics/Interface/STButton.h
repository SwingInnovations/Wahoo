#ifndef SWINGTECH1_STBUTTON_H
#define SWINGTECH1_STBUTTON_H

#include <c++/functional>
#include "STInterWidget.h"
#include "../STGraphics.h"
#include "STButtonGroup.h"

class STButton : public STInterWidget{
public:
    enum ButtonType{
        PUSH = 0,
        TOOL = 1,
        RADIO = 2
    };

    STButton();
    STButton(const std::string& text);
    STButton(stReal x, stReal y, std::string text);
    STButton(stReal x, stReal y, std::string text, ButtonType type);

    void update(STGame* window) override;
    void draw(STGraphics* grphx) override;
    void inputEvent(std::function<void(STEntity*, STGame*)>);

    void setToggle(bool val){
        toggled = val;
        parent->toggleGroup(index);
    }
    bool isToggled(){ return toggled; }
    void setIndex(unsigned int);
    void toggle(){ toggled =! toggled; }
private:
    void invokeInputEvent(STEntity*, STGame*);
    std::function<void(STEntity*, STGame*)> inputEvents = 0;
    ButtonType buttonType;
    bool toggled;
    STButtonGroup* parent = NULL;
    unsigned int index;
};


#endif //SWINGTECH1_STBUTTON_H
