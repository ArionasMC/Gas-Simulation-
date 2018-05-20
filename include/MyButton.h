#ifndef MYBUTTON_H
#define MYBUTTON_H

#include "GameObject.h"

class MyButton : public GameObject
{
    public:
        MyButton(const char* textureSheet, SDL_Renderer* ren, int x, int y, int w, int h);
        bool isTriggered(int mouseX, int mouseY);
        void update(const char* normal, const char* pushed);
        void setChangePos(bool pos);
    private:
        bool pushedState = false;
        bool changed = false;
        bool changePos = false;
};

#endif // MYBUTTON_H
