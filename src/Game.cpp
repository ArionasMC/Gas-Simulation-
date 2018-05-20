#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Molecule.h"
#include "MyButton.h"
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <sstream>

#define MAX_TIME 120

using namespace std;

Game::Game()
{}

Game::~Game()
{}

//GameObject* asteroid;
//GameObject* asteroid2;

//Molecule* molecule1;
//Molecule* molecule2;
int N_MOLECULES = 20;
//Molecule* molecules[N_MOLECULES];
vector<Molecule*> molecules(N_MOLECULES);

MyText* energyText;
MyText* moleculesText;
MyText* copyrightText;
TTF_Font* font;
//MyButton* button1;
//MyButton* button2;
//int S = 0;
double t = 0.0;
double dt = 1.0;
double qx = 1.0; // 0.9995
double qy = 1.0; // 0.9995

MyButton* restartButton;
MyButton* lossButton;
MyButton* minusButton;
MyButton* plusButton;
MyButton* settingsButton;
MyButton* playButton;
MyButton* arrowsButton;

bool mainScreen = true;
bool energyLoss = false;
bool settingsOn = false;
bool pause = false;
bool showArrows = false;
int delay = 5;

int updateEnergy = MAX_TIME;

SDL_Rect settingsRect;

SDL_Surface* screen;
void Game::init(const char* title, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 213, 224, 242, 255);
		}
        screen = SDL_GetWindowSurface(window);

		isRunning = true;
	}

	if ( TTF_Init( ) == -1 )
    {
        std::cout << "Initialization of TTF failed : " << SDL_GetError() << std::endl;
    }

    font = TTF_OpenFont("OpenSans-Regular.ttf", 25);

    //S = 500;
    /*
    molecule1 = new Molecule("images/molecule.png", renderer, 50, 200, 2);
    molecule1->setSpeedX(2);
    molecule1->setSpeedY(2);
    molecule2 = new Molecule("images/molecule.png", renderer, 500, 200, 5);
    molecule2->setSpeedX(-2);
    molecule2->setSpeedY(2);
    */

    srand(time(NULL));
    for(int i = 0; i < N_MOLECULES; i++) {
        molecules[i] = new Molecule("images/molecule.png", renderer, rand() % 780, rand() % 580, 1);
        molecules[i]->setSpeedX(rand() % 5 + 1);
        molecules[i]->setSpeedY(rand() % 5 + 1);
    }

    //button1 = new MyButton("images/button1.png", renderer, 20, 100, 360, 140);
    //button2 = new MyButton("images/button2.png", renderer, 420, 100, 360, 140);
    restartButton = new MyButton("images/restart1.png", renderer, 740, 540, 60, 60);
    restartButton->setChangePos(false);
    lossButton = new MyButton("images/red1.png",renderer,740,480,60,60);
    lossButton->setChangePos(false);

    minusButton = new MyButton("images/minus1.png", renderer, 0, 0, 30, 30);
    plusButton = new MyButton("images/plus1.png", renderer, 70, 0, 30, 30);

    string mol = ""+to_string(N_MOLECULES);
    moleculesText = new MyText(mol.c_str(), font, 35, -2, {0,0,0}, renderer);

    settingsButton = new MyButton("images/settings1.png", renderer, 745, 0, 55, 55);
    playButton = new MyButton("images/play1.png", renderer, 745, 0, 55, 55);

    arrowsButton = new MyButton("images/arrows1red.png", renderer, 740, 420, 60, 60);
    arrowsButton->setChangePos(false);

    settingsRect.x = 0;
    settingsRect.y = 0;
    settingsRect.w = 800;
    settingsRect.h = 70;

    //printf("Molecule1\tMolecule2\n");
    //string eText = "E = "+to_string(molecule1->getEnergy()+molecule2->getEnergy());
    energyText = new MyText("E = null", font, 10, 10, {0,0,0}, renderer);
    copyrightText = new MyText("Nikolas Spyropoulos © 2017-2018", font, 10, 550, {255,255,255}, renderer);
}

void Game::handleEvents()
{
	SDL_Event event;

	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT :
		isRunning = false;
		break;
    case SDL_MOUSEBUTTONDOWN:

        int x, y;
        SDL_GetMouseState( &x, &y );
        if(restartButton->isTriggered(x, y) && !pause) {


            for(int i = 0; i < N_MOLECULES; i++) {
                molecules[i] = NULL;
                delete molecules[i];
            }

            t = 0.0;
            dt = 1.0;
            updateEnergy = MAX_TIME;

            for(int i = 0; i < N_MOLECULES; i++) {
                molecules[i] = new Molecule("images/molecule.png", renderer, rand() % 780, rand() % 580, 1);
                molecules[i]->setSpeedX(rand() % 5 + 1);
                molecules[i]->setSpeedY(rand() % 5 + 1);
            }

        }
        if(lossButton->isTriggered(x,y) && !pause) {
            energyLoss = !energyLoss;
            if(energyLoss) {
                qx = 0.9995;
                qy = 0.9995;
                lossButton->setTexture("images/green2.png");
            } else {
                qx = 1.0;
                qy = 1.0;
                lossButton->setTexture("images/red2.png");
            }
        }
        if(minusButton->isTriggered(x,y)) {
            if(N_MOLECULES > 1 && settingsOn  && pause) {

                N_MOLECULES--;
                //molecules.resize(N_MOLECULES);

                string mol = ""+to_string(N_MOLECULES);
                //moleculesText->setText(mol.c_str());
                delete moleculesText;
                moleculesText = new MyText(mol.c_str(), font, 35, -2, {0,0,0}, renderer);

            }
        }
        if(plusButton->isTriggered(x,y) ) {
            if(N_MOLECULES < 50 && settingsOn && pause) {
                //printf("OKKK!\n");
                N_MOLECULES++;
                //printf("OKKK2!\n");
                /*
                molecules.resize(N_MOLECULES);

                int i = molecules.size()-1;
                molecules[i] = new Molecule("images/molecule.png", renderer, rand() % 780, rand() % 580, 1);
                molecules[i]->setSpeedX(rand() % 5 + 1);
                molecules[i]->setSpeedY(rand() % 5 + 1);
                */
                string mol = ""+to_string(N_MOLECULES);
                //printf("OKKK3!\n");
                delete moleculesText;
                //printf("OKKK4!\n");
                moleculesText = new MyText(mol.c_str(), font, 35, -2, {0,0,0}, renderer);
                //printf("OKKK5!\n");
            }
        }
        if(settingsButton->isTriggered(x,y) && !pause) {
            if(!pause) {
                settingsOn = !settingsOn;
            }
            pause = true;
            delay = 5;
        }
        if(playButton->isTriggered(x,y) && pause) {
            //printf("%d", delay);
            if(delay <= 0) {
                pause = false;
                settingsOn = !settingsOn;
                delay = 5;
                for(int i = 0; i < N_MOLECULES; i++) {
                    molecules[i] = NULL;
                    delete molecules[i];
                }

                t = 0.0;
                dt = 1.0;
                updateEnergy = MAX_TIME;

                for(int i = 0; i < N_MOLECULES; i++) {
                    molecules[i] = new Molecule("images/molecule.png", renderer, rand() % 780, rand() % 580, 1);
                    molecules[i]->setSpeedX(rand() % 5 + 1);
                    molecules[i]->setSpeedY(rand() % 5 + 1);
                }
            }
        }

        if(arrowsButton->isTriggered(x,y) && !pause) {
            showArrows = !showArrows;
            if(showArrows) arrowsButton->setTexture("images/arrows2green.png");
            else arrowsButton->setTexture("images/arrows2red.png");
        }

        break;
	default:
		break;
	}
}

int sumX = 0;
int sumY = 0;
double totalEnergy = 0.0;

void Game::update()
{
    int x, y;
    SDL_GetMouseState( &x, &y );




   // t+=0.01;
    //S = abs(molecule1->getX()+molecule1->getRect().w-molecule2->getX());



    /*
    molecule1->update(dt);
    molecule2->update(dt);

    if(collide(molecule1->getRect(), molecule2->getRect())) {
        //printf("Heyyy!!!");
        int m1 = molecule1->getMass();
        int m2 = molecule2->getMass();
        double uX1 = molecule1->getSpeedX();
        double uX2 = molecule2->getSpeedX();
        double uY1 = molecule1->getSpeedY();
        double uY2 = molecule2->getSpeedY();

        molecule1->setSpeedX(q * (2*m2*uX2/(m1+m2) + (m1-m2)*uX1/(m1+m2)));
        molecule1->setSpeedY(q * (2*m2*uY2/(m1+m2) + (m1-m2)*uY1/(m1+m2)));

        molecule2->setSpeedX(q * (2*m1*uX1/(m1+m2) + (m2-m1)*uX2/(m1+m2)));
        molecule2->setSpeedY(q * (2*m1*uY1/(m1+m2) + (m2-m1)*uY2/(m1+m2)));

        printf("vX:%.2f\t\tvX:%.2f\n", molecule1->getSpeedX(), molecule2->getSpeedX());
        //printf("\tE:%.2f\n", (molecule1->getEnergy()+molecule2->getEnergy()));
    }
    */
    /*
    button1->isTriggered(x,y);
    button1->update("images/button1.png", "images/button1_2.png");
    button2->isTriggered(x,y);
    button2->update("images/button2.png", "images/button2_2.png");
    */
    if(!pause) {

        for(int i = 0; i < N_MOLECULES; i++) totalEnergy+=molecules[i]->getEnergy();

        stringstream stream;
        stream << fixed << setprecision(3) << totalEnergy;
        string s = stream.str();

        string et = "E = "+s;
        energyText->setText(et.c_str());
        energyText->setColor({0,0,0});
        totalEnergy = 0;

        if(updateEnergy > 0) {
            updateEnergy--;
        } else {
            updateEnergy = MAX_TIME;
            for(int i = 0; i < N_MOLECULES; i++) {
                sumX+=molecules[i]->getEnergyX();
                sumY+=molecules[i]->getEnergyY();
            }
            printf("Ex = %d\tEy = %d\n", sumX, sumY);
            sumX = 0;
            sumY = 0;
        }
        //printf(".");
        restartButton->isTriggered(x,y);
        restartButton->update("images/restart1.png", "images/restart2.png");
        lossButton->isTriggered(x,y);
        if(energyLoss) {
            lossButton->update("images/green1.png", "images/green2.png");
        } else {
            lossButton->update("images/red1.png", "images/red2.png");
        }

        arrowsButton->isTriggered(x,y);
        if(showArrows)arrowsButton->update("images/arrows1green.png", "images/arrows2green.png");
        else arrowsButton->update("images/arrows1red.png", "images/arrows2red.png");

        settingsButton->isTriggered(x,y);
        settingsButton->update("images/settings1.png", "images/settings2.png");
        //if(!mainScreen) {
            t = t + dt;
            for(int i = 0; i < N_MOLECULES; i++) {

                molecules[i]->update(dt);

                for(int j = 0; j < N_MOLECULES; j++) {
                    if(i != j) {
                        if(collide(molecules[i]->getRect(), molecules[j]->getRect())) {
                            //printf("Collision: %d with %d", i, j);

                            int m1 = molecules[i]->getMass();
                            double uX1 = molecules[i]->getSpeedX();
                            double uY1 = molecules[i]->getSpeedY();
                            int m2 = molecules[j]->getMass();
                            double uX2 = molecules[j]->getSpeedX();
                            double uY2 = molecules[j]->getSpeedY();

                            molecules[i]->setSpeedX(qx * (2*m2*uX2/(m1+m2) + (m1-m2)*uX1/(m1+m2)));
                            molecules[i]->setSpeedY(qy * (2*m2*uY2/(m1+m2) + (m1-m2)*uY1/(m1+m2)));

                            molecules[j]->setSpeedX(qx * (2*m1*uX1/(m1+m2) + (m2-m1)*uX2/(m1+m2)));
                            molecules[j]->setSpeedY(qy * (2*m1*uY1/(m1+m2) + (m2-m1)*uY2/(m1+m2)));

                        }
                    }

                }

            }
    } else {
        if(delay > 0) {
            delay--;
        }
        playButton->isTriggered(x,y);
        playButton->update("images/play1.png", "images/play2.png");
        if(settingsOn) {
            minusButton->isTriggered(x,y);
            minusButton->update("images/minus1.png", "images/minus2.png");
            plusButton->isTriggered(x,y);
            plusButton->update("images/plus1.png", "images/plus2.png");
        }
    }



    //}
    //string eText = "E = "+to_string(molecule1->getEnergy()+molecule2->getEnergy());
    //energyText->setText(eText.c_str());

}

void Game::render()
{
	SDL_RenderClear(renderer);

	//molecule1->render();
	//molecule2->render();
	//drawText(energyText);
	/*
	button1->render();
	button2->render();
    */

    drawText(copyrightText);
    if(!pause) {

        drawText(energyText);
        restartButton->render();
        lossButton->render();
        settingsButton->render();
        arrowsButton->render();

        for(int i = 0; i < N_MOLECULES; i++) {
            molecules[i]->render();
            double x = molecules[i]->getX()+molecules[i]->getRect().w/2;
            double y = molecules[i]->getY()+molecules[i]->getRect().h/2;
            double uX = molecules[i]->getSpeedX();
            double uY = molecules[i]->getSpeedY();
            int multiplier = 15;
            if(showArrows) {
                SDL_SetRenderDrawColor(renderer, 9, 99, 244, 255);
                SDL_RenderDrawLine(renderer, x, y, x+uX*multiplier, y); // x line
                SDL_SetRenderDrawColor(renderer, 255, 2, 2, 255);
                SDL_RenderDrawLine(renderer, x, y, x, y+uY*multiplier); // y line
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawLine(renderer, x, y, x+uX*multiplier, y+uY*multiplier); // sunistamenh line
            }
            //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            //SDL_Rect r = {molecules[i]->getRect().x, molecules[i]->getRect().y, molecules[i]->getRect().w, molecules[i]->getRect().h};
            //SDL_RenderDrawRect(renderer, &r);
        }
        SDL_SetRenderDrawColor(renderer, 213, 224, 242, 255);
    } else {
        if(settingsOn) {
            SDL_SetRenderDrawColor(renderer, 219,219,219, 255);
            SDL_RenderFillRect(renderer, &settingsRect);
            SDL_SetRenderDrawColor(renderer, 213, 224, 242, 255);
            //SDL_FillRect(screen, &settingsRect, SDL_MapRGB(screen->format, 219, 219, 219));
            minusButton->render();
            plusButton->render();
            drawText(moleculesText);
        }
        playButton->render();
    }
	//if(!mainScreen) {

	//}

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::drawText(MyText* text) {
    SDL_RenderCopy(renderer, text->getText(), NULL, text->getTextRect());
}

bool Game::collide(SDL_Rect a, SDL_Rect b) {

        float xA = a.x+a.w/2;
        float yA = a.y+a.h/2;
        float xB = b.x+b.w/2;
        float yB = b.y+b.h/2;
        float rA = a.h/2;
        float rB = b.h/2;
        double dist = pow(xA-xB, 2) + pow(yA-yB, 2);
        dist = sqrt(dist);
        if(dist<=1.0*(rA+rB)) {
            //printf("Collision!\t");
            return true;
        }
        return false;

}
