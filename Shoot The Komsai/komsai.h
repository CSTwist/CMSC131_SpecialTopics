#ifndef KOMSAI_H
#define KOMSAI_H

class Komsai{
public:
    //Constructor
    Komsai();

    typedef struct {
        int KomsaiHitLineStart;
        int KomsaiHitLineEnd;
        int currentKomsaiY;
    } KomsaiHitLine;

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
    KomsaiHitLine get_KomsaiHitLine() const;

    //Setters
    void set_KomsaiY(int y);
    void set_KomsaiX(int x);
    void set_KomsaiSpeed(int speed);
    void set_KomsaiType(KomsaiType type);
    void set_KomsaiHitLine(KomsaiHitLine komsaiHitLine);

private:
    //Attributes
    int komsaiY;
    int komsaiX;
    int komsaiSpeed;
    KomsaiType komsaiType;
    KomsaiHitLine komsaiHitLine;
};

#endif