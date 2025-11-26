#include "level.h"
#include "komsai.h"
#include "bullet.h"
#include "bossKomsai.h"
#include <vector>
using namespace std;

Level::Level() : levelNumber(0), komsaiMovement(static_cast<Level::KomsaiMovement>(rand() % 3)) {}

//Getter Implementations
int Level::get_LevelNumber() const{
    return levelNumber;
}

int Level::get_KomsaiMovement() const{
    return komsaiMovement;
}

//Setter Implementations
void Level::set_LevelNumber(int currentLevel) {
    this->levelNumber = currentLevel;
}

void Level::set_KomsaiMovement(KomsaiMovement movement) {
    this->komsaiMovement = movement;
}

//Komsai Generation
void Level::komsaiGenerator(int screenWidth, int screenHeight, vector<Komsai>& komsais) {
    Komsai komsai;

    if (komsaiMovement == MOVE_LEFT) {
        komsai.set_KomsaiY(rand() % (screenHeight - 500));
        komsai.set_KomsaiX(screenWidth);
        komsai.set_KomsaiSpeed(6);
    } else if (komsaiMovement == MOVE_RIGHT) {
        komsai.set_KomsaiY(rand() % (screenHeight - 500));
        komsai.set_KomsaiX(100);
        komsai.set_KomsaiSpeed(6);
    }
    else {
        komsai.set_KomsaiX(rand() % (screenWidth - 56));
        komsai.set_KomsaiY(0);
        komsai.set_KomsaiSpeed(2);
    }

    int r = rand() % 8;
    Komsai::KomsaiType type;

    if (r == 0) type = Komsai::HEALER;
    else type = Komsai::TARGET; 

    komsai.set_KomsaiType(type);
    komsais.push_back(komsai);
}

//Boss Spawner
void Level::bossSpawner(int screenWidth, int screenHeight, vector<BossKomsai>& erylBoss){
    BossKomsai eryl;
    
    eryl.set_BossKomsaiX(rand() % (screenWidth - 56));
    eryl.set_BossKomsaiY(0);

    // Random initial speeds: horizontal can be left or right, vertical small
    int baseHor = 3 + (rand() % 3); // 3..5 px/frame
    if (rand() % 2 == 0) baseHor = -baseHor; // random direction
    eryl.set_BossSpeedX(baseHor);

    int baseVer = 1 + (rand() % 2); // 1..2 px/frame vertical
    if (rand() % 2 == 0) baseVer = -baseVer;
    eryl.set_BossSpeedY(baseVer);

    erylBoss.push_back(eryl);
}

//Movement Implementations
void Level::player_movement(int& playerX, int& playerXMovement,
                            int screenWidth, int SHIP_WIDTH) {

    playerX += playerXMovement;

    if (playerX < 0 || playerX > screenWidth - SHIP_WIDTH) {
        playerXMovement = 0;
        playerX = (playerX < 0) ? 0 : screenWidth - SHIP_WIDTH;
    }
}

void Level::komsai_movement(vector<Komsai>& komsais, vector<Bullet>& bullets,
                           int& score, int& playerLife, int screenHeight, int screenWidth) {
    for (auto it = komsais.begin(); it != komsais.end(); ) {
        if (komsaiMovement == MOVE_LEFT) {
            it->set_KomsaiX(it->get_KomsaiX() - it->get_KomsaiSpeed());

            if (it->get_KomsaiX() <= 0) {
                if (it->get_KomsaiType() == Komsai::TARGET && playerLife > 0) {
                    playerLife--;
                }
                it = komsais.erase(it);
                continue;
            }
        } else if (komsaiMovement == MOVE_RIGHT) {
            it->set_KomsaiX(it->get_KomsaiX() + it->get_KomsaiSpeed());

            if (it->get_KomsaiX() > screenWidth) {
                if (it->get_KomsaiType() == Komsai::TARGET && playerLife > 0) {
                    playerLife--;
                }
                it = komsais.erase(it);
                continue;
            }
        } else {
            // Default movement is down
            it->set_KomsaiY(it->get_KomsaiY() + it->get_KomsaiSpeed());

            if (it->get_KomsaiY() > screenHeight) {
                if (it->get_KomsaiType() == Komsai::TARGET && playerLife > 0) {
                    playerLife--;
                }
                it = komsais.erase(it);
                continue;
            }
        }
        

        bool destroyed = false;
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
            if (bulletIt->get_bulletY() < it->get_KomsaiY() + 100 &&
                bulletIt->get_bulletY() + 16 > it->get_KomsaiY() &&
                bulletIt->get_bulletX() < it->get_KomsaiX() + 100 &&
                bulletIt->get_bulletX() + 8 > it->get_KomsaiX()) {

                bulletIt = bullets.erase(bulletIt);
                if (it ->get_KomsaiType() == Komsai::TARGET) {
                    score += 10;
                } else if (it ->get_KomsaiType() == Komsai::HEALER && playerLife < 5) {
                    playerLife++;
                }
                it = komsais.erase(it);
                destroyed = true;
                break; // exit inner loop safely
            } else {
                ++bulletIt;
            }
        }

        if (!destroyed) {
            ++it; // only increment if we didn’t erase
        }
    }
}

void Level::boss_movement(vector<BossKomsai>& erylBoss, vector<Bullet>& bullets,
                           int& score, int& playerLife, int screenHeight, int screenWidth) {
    for (auto it = erylBoss.begin(); it != erylBoss.end(); ) {
        it->set_BossKomsaiX(it->get_BossKomsaiX() + it->get_BossSpeedX());
        it->set_BossKomsaiY(it->get_BossKomsaiY() + it->get_BossSpeedY());

        
        if (it->get_BossKomsaiX() <= 0 || it->get_BossKomsaiX() >= screenWidth - 200)
            it->set_BossSpeedX(-it->get_BossSpeedX());

        if (it->get_BossKomsaiY() <= 0 || it->get_BossKomsaiY() >= screenHeight - 200)
            it->set_BossSpeedY(-it->get_BossSpeedY());


        bool destroyed = false;
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
            if (bulletIt->get_bulletY() < it->get_BossKomsaiY() + 300 &&
                bulletIt->get_bulletY() + 16 > it->get_BossKomsaiY() &&
                bulletIt->get_bulletX() < it->get_BossKomsaiX() + 300 &&
                bulletIt->get_bulletX() + 8 > it->get_BossKomsaiX()) {

                bulletIt = bullets.erase(bulletIt);
                it->decrement_BossHealth();
                if (it->get_BossHealth() == 0){
                    it = erylBoss.erase(it);
                    destroyed = true;
                    score += 100;
                    break; // exit inner loop safely
                }
            } else {
                ++bulletIt;
            }
        }

        if (!destroyed) {
            ++it; // only increment if we didn’t erase
        }
    }
}

void Level::bullet_movement(vector<Bullet>& bullets) {
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->set_bulletY(it->get_bulletY() + it->get_bulletSpeed());

        if (it->get_bulletY() < 0) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}