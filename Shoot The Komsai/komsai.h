#ifndef KOMSAI_H
#define KOMSAI_H

class Komsai{
public:
    //Constructor
    Komsai();

    enum KomsaiType {
        HEALER,
        TARGET,
    };

    //Methods
    //Getters
    int get_KomsaiY() const;
    int get_KomsaiX() const;
    int get_KomsaiSpeed() const;
    int get_KomsaiType() const;

    //Setters
    void set_KomsaiY(int y);
    void set_KomsaiX(int x);
    void set_KomsaiSpeed(int speed);
    void set_KomsaiType(KomsaiType type);

private:
    //Attributes
    int komsaiY;
    int komsaiX;
    int komsaiSpeed;
    KomsaiType komsaiType;
};

#endif