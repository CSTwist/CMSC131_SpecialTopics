#ifndef BULLET_H
#define BULLET_H

class Bullet{
public:
    //Constructor
    Bullet();

    enum BulletType{
        PLAYER,
        BOSS
    };

    //Methods
    //Getters
    int get_bulletY() const;
    int get_bulletX() const;
    int get_bulletSpeed() const;
    int get_bulletType() const;

    //Setters
    void set_bulletY(int y);
    void set_bulletX(int x);
    void set_bulletSpeed(int speed);
    void set_bulletType(BulletType type);

private:
    //Attributes
    int bulletY;
    int bulletX;
    int bulletSpeed;
    BulletType bulletType;
};

#endif