#include "bossKomsai.h"

// Constructor
BossKomsai::BossKomsai() : bossKomsaiX(0.0f), bossKomsaiY(0.0f), speedX(3.0f), speedY(2.0f), bossHealth(20){}

// Getter Implementations
int BossKomsai::get_BossKomsaiX() const {
    return static_cast<int>(bossKomsaiX);
}

int BossKomsai::get_BossKomsaiY() const {
    return static_cast<int>(bossKomsaiY);
}

float BossKomsai::get_BossSpeedX() const {
    return speedX;
}

float BossKomsai::get_BossSpeedY() const {
    return speedY;
}

int BossKomsai::get_BossHealth() const {
    return bossHealth;
}

// Setter Implementations
void BossKomsai::set_BossKomsaiX(float x) {
    bossKomsaiX = x;
}

void BossKomsai::set_BossKomsaiY(float y) {
    bossKomsaiY = y;
}

void BossKomsai::set_BossSpeedX(float speed) {
    speedX = speed;
}

void BossKomsai::set_BossSpeedY(float speed) {
    speedY = speed;
}

void BossKomsai::decrement_BossHealth() {
    bossHealth--; 
}