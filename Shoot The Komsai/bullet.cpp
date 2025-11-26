#include "bullet.h"

Bullet::Bullet() : bulletY(0.0f), bulletX(0.0f), bulletSpeed(0.0f), bulletType(PLAYER) {}

//Getter Implementations
int Bullet::get_bulletY() const{
    return static_cast<int>(bulletY);
}
int Bullet::get_bulletX() const{
    return static_cast<int>(bulletX);
}
float Bullet::get_bulletSpeed() const{
    return bulletSpeed;
}

int Bullet::get_bulletType() const{
    return bulletType;
}

//Setter Implementations
void Bullet::set_bulletY(float y) {
    bulletY = y;
}
void Bullet::set_bulletX(float x) {
    bulletX = x;
}
void Bullet::set_bulletSpeed(float speed) {
    bulletSpeed = speed;
}
void Bullet::set_bulletType(BulletType type){
    bulletType = type;
}