#ifndef BOSSKOMSAI_H
#define BOSSKOMSAI_H

class BossKomsai {
private:
    int bossKomsaiX, bossKomsaiY;
    int speedX, speedY;  // horizontal & vertical speed
    int bossHealth;

public:
    BossKomsai();

    void set_BossKomsaiX(int val);
    void set_BossKomsaiY(int val);
    int get_BossKomsaiX() const;
    int get_BossKomsaiY() const;

    void set_BossSpeedX(int val);
    void set_BossSpeedY(int val);
    int get_BossSpeedX() const;
    int get_BossSpeedY() const;

    void decrement_BossHealth(); 
    int get_BossHealth() const;
};

#endif