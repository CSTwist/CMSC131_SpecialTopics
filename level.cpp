#include "level.h"
#include "komsai.h"
#include "bullet.h"
#include "bossKomsai.h"
#include <vector>
#include <cstdlib>

using namespace std;

Level::Level() : levelNumber(0), komsaiMovement(static_cast<Level::KomsaiMovement>(rand() % 3)) {}

int Level::get_LevelNumber() const{
    return levelNumber;
}

int Level::get_KomsaiMovement() const{
    return komsaiMovement;
}

void Level::set_LevelNumber(int currentLevel) {
    this->levelNumber = currentLevel;
}

void Level::set_KomsaiMovement(KomsaiMovement movement) {
    this->komsaiMovement = movement;
}

void Level::komsaiGenerator(int screenWidth, int screenHeight, vector<Komsai>& komsais) {
    Komsai komsai;

    if (komsaiMovement == MOVE_LEFT) {
        komsai.set_KomsaiY(rand() % (screenHeight - 500));
        komsai.set_KomsaiX(screenWidth);
        komsai.set_KomsaiSpeed(4.0f);
    } else if (komsaiMovement == MOVE_RIGHT) {
        komsai.set_KomsaiY(rand() % (screenHeight - 500));
        komsai.set_KomsaiX(100);
        komsai.set_KomsaiSpeed(4.0f);
    }
    else {
        komsai.set_KomsaiX(rand() % (screenWidth - 56));
        komsai.set_KomsaiY(0);
        komsai.set_KomsaiSpeed(4.0f);
    }

    int r = rand() % 8;
    Komsai::KomsaiType type;

    if (r == 0) type = Komsai::HEALER;
    else type = Komsai::TARGET;

    komsai.set_KomsaiType(type);
    komsais.push_back(komsai);
}

void Level::bossSpawner(int screenWidth, int screenHeight, vector<BossKomsai>& erylBoss){
    BossKomsai eryl;

    eryl.set_BossKomsaiX(rand() % (screenWidth - 200));
    eryl.set_BossKomsaiY(0);

    float baseHor = 3.0f + (rand() % 3);
    if (rand() % 2 == 0) baseHor = -baseHor;
    eryl.set_BossSpeedX(baseHor);

    float baseVer = 1.0f + (rand() % 2);
    if (rand() % 2 == 0) baseVer = -baseVer;
    eryl.set_BossSpeedY(baseVer);

    erylBoss.push_back(eryl);
}

void Level::player_movement(float& playerX, float& playerXMovement,
                            int screenWidth, int SHIP_WIDTH, float dt) {

    playerX += playerXMovement * dt;

    if (playerX < 0 || playerX > screenWidth - SHIP_WIDTH) {
        // Clamp
        playerX = (playerX < 0) ? 0.0f : (float)(screenWidth - SHIP_WIDTH);
    }
}

void Level::komsai_movement(vector<Komsai>& komsais, vector<Bullet>& bullets,
    int& score, int& playerLife, int screenHeight, int screenWidth, float dt) {
    for (auto it = komsais.begin(); it != komsais.end(); ) {
        if (komsaiMovement == MOVE_LEFT) {
            it->set_KomsaiX(it->get_KomsaiX() - (it->get_KomsaiSpeed() * dt));

            if (it->get_KomsaiX() <= 0) {
                if (it->get_KomsaiType() == Komsai::TARGET && playerLife > 0) {
                    playerLife--;
                }
                it = komsais.erase(it);
                continue;
            }
        } else if (komsaiMovement == MOVE_RIGHT) {
            it->set_KomsaiX(it->get_KomsaiX() + (it->get_KomsaiSpeed() * dt));

            if (it->get_KomsaiX() > screenWidth) {
                if (it->get_KomsaiType() == Komsai::TARGET && playerLife > 0) {
                    playerLife--;
                }
                it = komsais.erase(it);
                continue;
            }
        } else {
            it->set_KomsaiY(it->get_KomsaiY() + (it->get_KomsaiSpeed() * dt));

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
            // Collision logic uses integers (get_X returns int), safe for bounding box checks
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
                break;
            } else {
                ++bulletIt;
            }
        }

        if (!destroyed) {
            ++it;
        }
    }
}

void Level::boss_movement(vector<BossKomsai>& erylBoss, float& playerX, vector<Bullet>& bullets,
    int& score, int& playerLife, int screenHeight, int screenWidth, float dt) {
    for (auto it = erylBoss.begin(); it != erylBoss.end(); ) {
        it->set_BossKomsaiX(it->get_BossKomsaiX() + (it->get_BossSpeedX() * dt));
        it->set_BossKomsaiY(it->get_BossKomsaiY() + (it->get_BossSpeedY() * dt));

        if (it->get_BossKomsaiX() <= 0 || it->get_BossKomsaiX() >= screenWidth - 200)
            it->set_BossSpeedX(-it->get_BossSpeedX());

        if (it->get_BossKomsaiY() <= 0 || it->get_BossKomsaiY() >= screenHeight - 600)
            it->set_BossSpeedY(-it->get_BossSpeedY());

        bool destroyed = false;
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
            if (bulletIt->get_bulletY() < it->get_BossKomsaiY() + 200 &&
                bulletIt->get_bulletY() + 16 > it->get_BossKomsaiY() &&
                bulletIt->get_bulletX() < it->get_BossKomsaiX() + 200 &&
                bulletIt->get_bulletX() + 8 > it->get_BossKomsaiX() && bulletIt->get_bulletType() == 0) {

                bulletIt = bullets.erase(bulletIt);
                it->decrement_BossHealth();
                if (it->get_BossHealth() == 0){
                    it = erylBoss.erase(it);
                    destroyed = true;
                    score += 100;
                    break;
                }
            } else if (bulletIt->get_bulletType() == Bullet::BOSS) {
                constexpr int PLAYER_WIDTH = 56;
                constexpr int PLAYER_HEIGHT = 56;

                int playerY = screenHeight - 80;
                if (bulletIt->get_bulletX() < playerX + PLAYER_WIDTH &&
                    bulletIt->get_bulletX() + 8 > playerX &&
                    bulletIt->get_bulletY() < playerY + PLAYER_HEIGHT &&
                    bulletIt->get_bulletY() + 16 > playerY) {
                    playerLife--;
                    bulletIt = bullets.erase(bulletIt);
                }
                else {
                    ++bulletIt;
                }
            } else {
                ++bulletIt;
            }
        }

        if (!destroyed) {
            ++it;
        }
    }
}

void Level::bullet_movement(vector<Bullet>& bullets, float dt) {
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->set_bulletY(it->get_bulletY() + (it->get_bulletSpeed() * dt));

        if (it->get_bulletY() < 0 || it->get_bulletY() > 2000) { // Cleanup boundaries
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}