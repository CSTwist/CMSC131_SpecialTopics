#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include "komsai.h"
#include "bullet.h"
#include "bossKomsai.h"

using namespace std;

class Level {
public:
    enum KomsaiMovement {
        MOVE_LEFT,
        MOVE_RIGHT,
        MOVE_DOWN
    };

    Level();

    // Getters and Setters
    int get_LevelNumber() const;
    int get_KomsaiMovement() const;
    void set_LevelNumber(int currentLevel);
    void set_KomsaiMovement(KomsaiMovement movement);

    // Logic
    void komsaiGenerator(int screenWidth, int screenHeight, vector<Komsai>& komsais);
    void bossSpawner(int screenWidth, int screenHeight, vector<BossKomsai>& erylBoss);

    // Movement (Now with Delta Time)
    void player_movement(float& playerX, float& playerXMovement, int screenWidth, int SHIP_WIDTH, float dt);
    void komsai_movement(vector<Komsai>& komsais, vector<Bullet>& bullets, int& score, int& playerLife, int screenHeight, int screenWidth, float dt);
    void boss_movement(vector<BossKomsai>& erylBoss, float& playerX, vector<Bullet>& bullets, int& score, int& playerLife, int screenHeight, int screenWidth, float dt);
    void bullet_movement(vector<Bullet>& bullets, float dt);

private:
    int levelNumber;
    KomsaiMovement komsaiMovement;
};

#endif