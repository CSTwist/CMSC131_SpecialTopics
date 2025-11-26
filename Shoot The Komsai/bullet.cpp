#include "bullet.h"

Bullet::Bullet() : bulletY(0), bulletX(0), bulletSpeed(0), bulletType(PLAYER) {}

//Getter Implementations
int Bullet::get_bulletY() const{
    return bulletY;
}
int Bullet::get_bulletX() const{
    return bulletX;
}
int Bullet::get_bulletSpeed() const{
    return bulletSpeed;
}

int Bullet::get_bulletType() const{
    return bulletType;
}

//Setter Implementations
void Bullet::set_bulletY(int y) {
    bulletY = y;
}
void Bullet::set_bulletX(int x) {
    bulletX = x;
}
void Bullet::set_bulletSpeed(int speed) {
    bulletSpeed = speed;
}
void Bullet::set_bulletType(BulletType type){
    bulletType = type;
}