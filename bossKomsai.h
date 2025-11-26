#ifndef BOSSKOMSAI_H
#define BOSSKOMSAI_H

class BossKomsai {
private:
    float bossKomsaiX, bossKomsaiY;
    float speedX, speedY; 
    int bossHealth;

public:
    BossKomsai();

    void set_BossKomsaiX(float val);
    void set_BossKomsaiY(float val);
    int get_BossKomsaiX() const;
    int get_BossKomsaiY() const;

    void set_BossSpeedX(float val);
    void set_BossSpeedY(float val);
    float get_BossSpeedX() const;
    float get_BossSpeedY() const;

    void decrement_BossHealth(); 
    int get_BossHealth() const;
};

#endif