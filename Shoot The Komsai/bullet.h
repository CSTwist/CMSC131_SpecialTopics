#ifndef BULLET_H
#define BULLET_H

class Bullet{
public:
    //Constructor
    Bullet();

    //Methods
    //Getters
    int get_bulletY() const;
    int get_bulletX() const;
    int get_bulletSpeed() const;

    //Setters
    void set_bulletY(int y);
    void set_bulletX(int x);
    void set_bulletSpeed(int speed);

private:
    //Attributes
    int bulletY;
    int bulletX;
    int bulletSpeed;
};

#endif