#include <emscripten/emscripten.h>
#include <iostream>
#include <vector>
#include "bullet.h"
#include "komsai.h"
#include "level.h"
#include "bossKomsai.h"
using namespace std;

constexpr int SHIP_WIDTH = 56;

class Game {
public:
    static Game& instance() {
        srand(static_cast<unsigned int>(time(0)));
        static Game instance;
        return instance;
    }

    void update();
    int getScreenWidth();
    int getScreenHeight();

    int getPlayerX() const { return playerX; }
    const std::vector<Bullet>& getBullets() const { return bullets; }
    const std::vector<Komsai>& getKomsais() const { return komsais; }
    const std::vector<BossKomsai>& getBoss() const { return erylBoss; }

    int moveRight();
    int moveLeft();
    void shootBulletPlayer();
    void shootBulletBoss();
    void komsaiGenerator();
    int get_score();
    int getPlayerLife() const;
    int getcurrentLevel() const;
    void spawnBoss();

private:
    Game() = default; // private constructor for singleton

    int currentLevel = 0;
    int playerLife = 5;
    int score = 0;
    int playerXMovement = 0;
    int screenWidth = 0;
    int screenHeight = 0;
    int playerX = 0;
    vector<Bullet> bullets;
    vector<Komsai> komsais;
    vector<BossKomsai> erylBoss;
    bool bossActive = false;
    Level gameLevel;
    int lastMilestone = 0;
};

// ---------------- Implementation ----------------

void Game::update() {
    screenWidth = getScreenWidth();
    screenHeight = getScreenHeight();

    gameLevel.set_LevelNumber(currentLevel);
    if (score >= lastMilestone + 100) {
        lastMilestone += 100;
        currentLevel++;

        gameLevel.set_LevelNumber(currentLevel);
        gameLevel.set_KomsaiMovement(static_cast<Level::KomsaiMovement>(rand() % 3));

        komsais.clear();
        bullets.clear();
    }


    // Player movement
    gameLevel.player_movement(playerX, playerXMovement, screenWidth, SHIP_WIDTH);

    //Komsai and Boss Movement
    if ((gameLevel.get_LevelNumber()+1) % 3 == 0 && gameLevel.get_LevelNumber() != 0) {
        gameLevel.boss_movement(erylBoss, playerX, bullets, score, playerLife, getScreenHeight(), getScreenWidth());
    } else {
        gameLevel.komsai_movement(komsais, bullets, score, playerLife, getScreenHeight(), getScreenWidth());
    }

    // Bullet movement
    gameLevel.bullet_movement(bullets);

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
    playerXMovement = 10;
    return playerXMovement;
}

int Game::moveLeft() {
    playerXMovement = -10;
    return playerXMovement;
}

void Game::shootBulletPlayer() {
    Bullet bullet;
    bullet.set_bulletX(playerX + SHIP_WIDTH / 2);
    bullet.set_bulletY(getScreenHeight()-80);
    bullet.set_bulletSpeed(-10);
    bullets.push_back(bullet);
}

void Game::shootBulletBoss() {
    Bullet bullet;
    bullet.set_bulletX(getBoss()[0].get_BossKomsaiX() + rand() % 200);
    bullet.set_bulletY(getBoss()[0].get_BossKomsaiY()+150);
    bullet.set_bulletSpeed(10);
    bullet.set_bulletType(static_cast<Bullet::BulletType>(1));
    bullets.push_back(bullet);
}

void Game::komsaiGenerator() {
    gameLevel.komsaiGenerator(screenWidth, screenHeight, komsais);
}

void Game::spawnBoss() {
    erylBoss.clear();
    gameLevel.bossSpawner(screenWidth, screenHeight, erylBoss);
}

int Game::get_score() {
    return score;
}

int Game::getPlayerLife() const {
    return playerLife;
}

int Game::getcurrentLevel() const {
    return currentLevel;
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
    void shoot_bullet_player() {
        Game::instance().shootBulletPlayer();
    }

    EMSCRIPTEN_KEEPALIVE
    void shoot_bullet_boss() {
        Game::instance().shootBulletBoss();
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
    int get_komsai_type(int index) {
        const auto& komsais = Game::instance().getKomsais();
        if (index < 0 || index >= komsais.size()) return -1;
        return komsais[index].get_KomsaiType();
    }

    EMSCRIPTEN_KEEPALIVE
    void generate_komsai() {
        Game::instance().komsaiGenerator();
    }

    EMSCRIPTEN_KEEPALIVE
    int get_score() {
        return Game::instance().get_score();
    }

    EMSCRIPTEN_KEEPALIVE
    int get_player_life() {
        return Game::instance().getPlayerLife();
    } // extern "C"

    EMSCRIPTEN_KEEPALIVE
    int get_game_level() {
        return Game::instance().getcurrentLevel();
    }

    EMSCRIPTEN_KEEPALIVE
    void spawn_boss() {
        Game::instance().spawnBoss();
    }

    EMSCRIPTEN_KEEPALIVE
    int get_boss_x(int index) {
        const auto& boss = Game::instance().getBoss();
        if (index < 0 || index >= boss.size()) return -1;
        return boss[index].get_BossKomsaiX();
    }

    EMSCRIPTEN_KEEPALIVE
    int get_boss_y(int index) {
        const auto& boss = Game::instance().getBoss();
        if (index < 0 || index >= boss.size()) return -1;
        return boss[index].get_BossKomsaiY();
    }

    EMSCRIPTEN_KEEPALIVE
    int get_boss_health(int index) {
        const auto& boss = Game::instance().getBoss();
        if (index < 0 || index >= boss.size()) return -1;
        return boss[index].get_BossHealth();
    }
}