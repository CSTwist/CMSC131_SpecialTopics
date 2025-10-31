#include <emscripten/emscripten.h>
#include <iostream>
#include <vector>
#include "bullet.h"
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

    int moveRight();
    int moveLeft();
    void shootBullet();

private:
    Game() = default; // private constructor for singleton

    int playerXMovement = 0;
    int objectSpeed = 2;
    int objectY = 0;
    int screenWidth = 0;
    int playerX = 0;
    vector<Bullet> bullets;

};

// ---------------- Implementation ----------------

void Game::update() {
    screenWidth = getScreenWidth();

    // Player movement
    playerX += playerXMovement;
    if (playerX < 0 || playerX > screenWidth - SHIP_WIDTH) {
        playerXMovement = 0;
        playerX = (playerX < 0) ? 0 : screenWidth - SHIP_WIDTH;
    }

    // Falling object movement
    objectY += objectSpeed;
    if (objectY > 600) {
        objectY = 0;
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
    bullets.push_back(bullet);
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

} // extern "C"