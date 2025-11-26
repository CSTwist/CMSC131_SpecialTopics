#include "komsai.h"

Komsai::Komsai() : komsaiY(0.0f), komsaiX(0.0f), komsaiSpeed(5.0f), komsaiType(Komsai::HEALER) {}

//Getter Implementations
int Komsai::get_KomsaiY() const{
    return static_cast<int>(komsaiY);
}
int Komsai::get_KomsaiX() const{
    return static_cast<int>(komsaiX);
}
float Komsai::get_KomsaiSpeed() const{
    return komsaiSpeed;
}

int Komsai::get_KomsaiType() const{
    return komsaiType;
}

//Setter Implementations
void Komsai::set_KomsaiY(float y) {
    this->komsaiY = y;
}
void Komsai::set_KomsaiX(float x) {
    this->komsaiX = x;
}
void Komsai::set_KomsaiSpeed(float speed) {
    this->komsaiSpeed = speed;
}
void Komsai::set_KomsaiType(KomsaiType type) {
    this->komsaiType = type;
}