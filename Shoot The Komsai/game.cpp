#include <emscripten/emscripten.h>
#include <iostream>
#include <vector>
#include "bullet.h"
#include "komsai.h"
using namespace std;

constexpr int SHIP_WIDTH = 56;

class Game {
public:
    static Game& instance() {
        static Game instance;
        return instance;
    }

    void update();
    int getScreenWidth();
    int getScreenHeight();

    int getPlayerX() const { return playerX; }
    const std::vector<Bullet>& getBullets() const { return bullets; }
    const std::vector<Komsai>& getKomsais() const { return komsais; }

    int moveRight();
    int moveLeft();
    void shootBullet();
    void komsaiGenerator();

private:
    Game() = default; // private constructor for singleton

    int playerXMovement = 0;
    int screenWidth = 0;
    int screenHeight = 0;
    int playerX = 0;
    vector<Bullet> bullets;
    vector<Komsai> komsais;

};

// ---------------- Implementation ----------------

void Game::update() {
    screenWidth = getScreenWidth();
    screenHeight = getScreenHeight();

    // Player movement
    playerX += playerXMovement;
    if (playerX < 0 || playerX > screenWidth - SHIP_WIDTH) {
        playerXMovement = 0;
        playerX = (playerX < 0) ? 0 : screenWidth - SHIP_WIDTH;
    }

    // Falling Komsai movement
    for (auto it = komsais.begin(); it != komsais.end(); ) {
        it->set_KomsaiY(it->get_KomsaiY() + it->get_KomsaiSpeed());

        if (it->get_KomsaiY() > getScreenHeight()) {
            it = komsais.erase(it);
            continue;
        }

        bool destroyed = false;
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
            if (bulletIt->get_bulletY() < it->get_KomsaiY() + 56 &&
                bulletIt->get_bulletY() + 16 > it->get_KomsaiY() &&
                bulletIt->get_bulletX() < it->get_KomsaiX() + 56 &&
                bulletIt->get_bulletX() + 8 > it->get_KomsaiX()) {

                bulletIt = bullets.erase(bulletIt);
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

    // Bullet movement
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->set_bulletY(it->get_bulletY() + it->get_bulletSpeed());

        if (it->get_bulletY() < 0) {
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }
}

int Game::getScreenWidth() {
    return EM_ASM_INT({
        return window.innerWidth;
    });
}

int Game::getScreenHeight() {
    return EM_ASM_INT({
        return window.innerHeight;
    });
}

int Game::moveRight() {
    playerXMovement = 5;
    return playerXMovement;
}

int Game::moveLeft() {
    playerXMovement = -5;
    return playerXMovement;
}

void Game::shootBullet() {
    Bullet bullet;
    bullet.set_bulletX(playerX + SHIP_WIDTH / 2);
    bullet.set_bulletY(getScreenHeight()-160);
    bullet.set_bulletSpeed(-10);
    bullet.set_BulletHitLine(Bullet::BulletHitLine{bullet.get_bulletX(), bullet.get_bulletX() + 8, bullet.get_bulletY()});
    bullets.push_back(bullet);
}

void Game::komsaiGenerator() {
    Komsai komsai;
    komsai.set_KomsaiX(rand() % (getScreenWidth() - 56));
    komsai.set_KomsaiY(0);
    komsai.set_KomsaiSpeed(2);
    komsai.set_KomsaiHitLine(Komsai::KomsaiHitLine{komsai.get_KomsaiX(), komsai.get_KomsaiX() + 56, komsai.get_KomsaiY()});
    komsais.push_back(komsai);
}

// ---------------- Exposed to JavaScript ----------------
extern "C" {

EMSCRIPTEN_KEEPALIVE
void update() {
    Game::instance().update();
}

EMSCRIPTEN_KEEPALIVE
int get_x() {
    return Game::instance().getPlayerX();
}

EMSCRIPTEN_KEEPALIVE
int right_movement() {
    return Game::instance().moveRight();
}

EMSCRIPTEN_KEEPALIVE
int left_movement() {
    return Game::instance().moveLeft();
}

EMSCRIPTEN_KEEPALIVE
void shoot_bullet() {
    Game::instance().shootBullet();
}

EMSCRIPTEN_KEEPALIVE
int get_bullet_x(int index) {
    const auto& bullets = Game::instance().getBullets();
    if (index < 0 || index >= bullets.size()) return -1;
    return bullets[index].get_bulletX();
}

EMSCRIPTEN_KEEPALIVE
int get_bullet_y(int index) {
    const auto& bullets = Game::instance().getBullets();
    if (index < 0 || index >= bullets.size()) return -1;
    return bullets[index].get_bulletY();
}

EMSCRIPTEN_KEEPALIVE
int get_bullet_count() {
    return Game::instance().getBullets().size();
}

EMSCRIPTEN_KEEPALIVE
int get_komsai_x(int index) {
    const auto& komsais = Game::instance().getKomsais();
    if (index < 0 || index >= komsais.size()) return -1;
    return komsais[index].get_KomsaiX();
}

EMSCRIPTEN_KEEPALIVE
int get_komsai_y(int index) {
    const auto& komsais = Game::instance().getKomsais();
    if (index < 0 || index >= komsais.size()) return -1;
    return komsais[index].get_KomsaiY();
}

EMSCRIPTEN_KEEPALIVE
int get_komsai_count() {
    return Game::instance().getKomsais().size();
}

EMSCRIPTEN_KEEPALIVE
void generate_komsai() {
    Game::instance().komsaiGenerator();
}
} // extern "C"