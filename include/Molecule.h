#ifndef MOLECULE_H
#define MOLECULE_H

#include <GameObject.h>


class Molecule : public GameObject
{
    public:
        Molecule(const char* textureSheet, SDL_Renderer* ren, double x, double y, int mass);
        void update(double dt);
        void setMass(int m);
        int getMass();
        double getEnergy();
        double getEnergyY();
        double getEnergyX();
    private:
        int mass;
};

#endif // MOLECULE_H
