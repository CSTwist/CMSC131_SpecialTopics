#include "bossKomsai.h"

// Constructor
BossKomsai::BossKomsai() : bossKomsaiX(0), bossKomsaiY(0), speedX(3), speedY(2), bossHealth(20){}

// Getter Implementations
int BossKomsai::get_BossKomsaiX() const {
    return bossKomsaiX;
}

int BossKomsai::get_BossKomsaiY() const {
    return bossKomsaiY;
}

int BossKomsai::get_BossSpeedX() const {
    return speedX;
}

int BossKomsai::get_BossSpeedY() const {
    return speedY;
}

int BossKomsai::get_BossHealth() const {
    return bossHealth;
}

// Setter Implementations
void BossKomsai::set_BossKomsaiX(int x) {
    bossKomsaiX = x;
}

void BossKomsai::set_BossKomsaiY(int y) {
    bossKomsaiY = y;
}

void BossKomsai::set_BossSpeedX(int speed) {
    speedX = speed;
}

void BossKomsai::set_BossSpeedY(int speed) {
    speedY = speed;
}

void BossKomsai::decrement_BossHealth() {
    bossHealth--; 
}