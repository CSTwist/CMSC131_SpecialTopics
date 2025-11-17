#ifndef LEVEL_H
#define LEVEL_H
#include <iostream>
#include <vector>
#include "bullet.h"
#include "komsai.h"
#include "level.h"
using namespace std;

class Level{
public:
    //Constructor
    Level();

    enum KomsaiMovement{
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
    };

    //Methods
    //Getters
    int get_LevelNumber() const;
    int get_KomsaiMovement() const;

    //Setters
    void set_LevelNumber(int currentLevel);
    void set_KomsaiMovement(KomsaiMovement movement);

    //Movements
    void player_movement(int& playerX, int& playerXMovement, int screenWidth, int SHIP_WIDTH);
    void komsai_movement(vector<Komsai>& komsais, vector<Bullet>& bullets, int& score, int& playerLife, int screenHeight, int screenWidth);
    void bullet_movement(vector<Bullet>& bullets);

    //Komsai Generation
    void komsaiGenerator(int screenWidth, int screenHeight,vector<Komsai>& komsais);

private:
    //Attributes
    int levelNumber;
    KomsaiMovement komsaiMovement;
};

#endif