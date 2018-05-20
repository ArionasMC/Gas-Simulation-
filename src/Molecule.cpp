#include "Molecule.h"
#include <cmath>

Molecule::Molecule(const char* textureSheet, SDL_Renderer* ren, double x, double y, int mass) : GameObject(textureSheet, ren, x, y)
{
    this->show = true;
    this->mass = mass;
    this->width = 20;
    this->height = 20;
    this->speedX = 0;
    this->speedY = 0;

}

void Molecule::update(double dt) {
    GameObject::update();
    if(this->show) {

        x = x + speedX * dt;
        y = y + speedY * dt;


        /*
        if(x <= 0) speedX = -speedX;
        if(x+width >= 800) speedX = -speedX;
        if()
        */

        if(x+width >= 800) {
            x=779;
            speedX = -speedX;
        }
        if(y+height >= 600) {
            y=579;
            speedY = -speedY;
        }
        if(x <= 0) {
            x=1;
            speedX = -speedX;
        }
        if(y <= 0) {
            y=1;
            speedY = -speedY;
        }

        this->destR.x = this->x;
        this->destR.y = this->y;
        this->destR.w = width;
        this->destR.h = height;
    }
}

void Molecule::setMass(int m) {
    this->mass = m;
}

int Molecule::getMass() {
    return this->mass;
}

double Molecule::getEnergy() {
    double speed = sqrt(pow(speedX, 2) + pow(speedY, 2));
    double energy = mass * pow(speed, 2) / 2;
    return energy;
}

double Molecule::getEnergyX() {
    double speed = speedX;
    double energy = mass * pow(speed, 2) / 2;
    return energy;
}

double Molecule::getEnergyY() {
    double speed = speedY;
    double energy = mass * pow(speed, 2) / 2;
    return energy;
}
