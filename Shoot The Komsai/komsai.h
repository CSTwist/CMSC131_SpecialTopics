#ifndef KOMSAI_H
#define KOMSAI_H

class Komsai {
public:
    enum KomsaiType {
        HEALER,
        TARGET
    };

    Komsai();

    //Getter
    int get_KomsaiY() const;
    int get_KomsaiX() const;
    float get_KomsaiSpeed() const;
    int get_KomsaiType() const;

    //Setter
    void set_KomsaiY(float y);
    void set_KomsaiX(float x);
    void set_KomsaiSpeed(float speed);
    void set_KomsaiType(KomsaiType type);

private:
    float komsaiY;
    float komsaiX;
    float komsaiSpeed;
    KomsaiType komsaiType;
};

#endif