#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "Character.h"
#include "Enemy.h"

class Player : public Character {
private:
    int hp;
    float credit;
    std::string color;
    bool hasIdCard;
    bool isSingle;
    bool isRidingBike;

public:
    Player();
    // 進入新關卡重製玩家位置和是否帶著學生證
    void resetPos();
    // 玩家移動
    void move(std::vector<std::string>& maze, const char& move_key);
    // 檢查是否血量歸零而輸
    const bool isLose() const;
    // 檢查是否抵達出口
    const bool isWin(const std::vector<std::string>& maze, const char& move_key) const;
    // 檢查是否遇到敵人
    const bool isHurt(const std::vector<Enemy*> enemies, const int& enemyCnt) const;
    // 遇到敵人後受傷且重製位置
    void playerHurt();

    const int getHP() const;
    const float getCredit() const;
    void increaseCredit();
    void resetCredit();
    const std::string& getColor() const;
    void setIdCard();
    const bool getHasIdCard() const;
    void setIsSingle();
    const bool getIsSingle() const;
    void increaseHP();
    void setIsRidingBike(const bool _isRidingBike);
    const bool getIsRidingBike() const;
    void setSymbol(const char _symbol);
};

#endif
