#ifndef BULLET_H
#define BULLET_H

class Bullet{
public:
    //Constructor
    Bullet();
    typedef struct {
        int bulletHitLineStart;
        int bulletHitLineEnd;
        int currentBulletY;
    } BulletHitLine;

    //Methods
    //Getters
    int get_bulletY() const;
    int get_bulletX() const;
    int get_bulletSpeed() const;
    BulletHitLine get_BulletHitLine() const;

    //Setters
    void set_bulletY(int y);
    void set_bulletX(int x);
    void set_bulletSpeed(int speed);
    void set_BulletHitLine(BulletHitLine bulletHitLine);

private:
    //Attributes
    int bulletY;
    int bulletX;
    int bulletSpeed;
    BulletHitLine bulletHitLine;
};

#endif