#ifndef MYTEXT_H
#define MYTEXT_H
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <iostream>

class MyText
{
    public:
        MyText(const char* t, TTF_Font* f, int x, int y, SDL_Color c, SDL_Renderer* renderer);
        SDL_Texture* getText();
        SDL_Rect* getTextRect();
        void setText(const char* t);
        void setColor(SDL_Color c);


    private:
        int w = 0;
        int h = 0;
        TTF_Font* font;
        SDL_Surface* message;
        SDL_Texture* text;
        SDL_Rect textRect;
        SDL_Color textColor;
        SDL_Renderer* renderer;

};

#endif // MYTEXT_H
