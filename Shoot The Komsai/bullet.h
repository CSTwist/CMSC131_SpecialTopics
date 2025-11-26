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
    //Getters - return int for rendering compatibility
    int get_bulletY() const;
    int get_bulletX() const;
    float get_bulletSpeed() const;
    int get_bulletType() const;

    //Setters
    void set_bulletY(float y);
    void set_bulletX(float x);
    void set_bulletSpeed(float speed);
    void set_bulletType(BulletType type);

private:
    //Attributes
    float bulletY;
    float bulletX;
    float bulletSpeed;
    BulletType bulletType;
};

#endif