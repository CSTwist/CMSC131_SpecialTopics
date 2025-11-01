#include "komsai.h"

Komsai::Komsai() : komsaiY(0), komsaiX(0), komsaiSpeed(5), komsaiHitLine{0,0,0} {}

//Getter Implementations
int Komsai::get_KomsaiY() const{
    return komsaiY;
}
int Komsai::get_KomsaiX() const{
    return komsaiX;
}
int Komsai::get_KomsaiSpeed() const{
    return komsaiSpeed;
}

Komsai::KomsaiHitLine Komsai::get_KomsaiHitLine() const{
    return komsaiHitLine;
}

//Setter Implementations
void Komsai::set_KomsaiY(int y) {
    this->komsaiY = y;
}
void Komsai::set_KomsaiX(int x) {
    this->komsaiX = x;
}
void Komsai::set_KomsaiSpeed(int speed) {
    this->komsaiSpeed = speed;
}

void Komsai::set_KomsaiHitLine(KomsaiHitLine komsaiHitLine) {
    this->komsaiHitLine = komsaiHitLine;
}