#include "komsai.h"

Komsai::Komsai() : komsaiY(0), komsaiX(0), komsaiSpeed(5), komsaiType(Komsai::HEALER) {}

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

int Komsai::get_KomsaiType() const{
    return komsaiType;
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
void Komsai::set_KomsaiType(KomsaiType type) {
    this->komsaiType = type;
}