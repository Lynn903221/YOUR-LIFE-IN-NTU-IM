#pragma once

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Item {
protected:
    int locationX_, locationY_;
    bool isCollected_;

public:
    Item(const int x = 0, const int y = 0);
    virtual ~Item();
    virtual const char getSymbol();
    const int getPosX() const;
    const int getPosY() const;
    // 檢查玩家是否撿到道具
    void checkAndCollect(const int playerLocX, const int playerLocY, const vector<string>& maze);
    const bool getIsCollected() const;
};

class IdCard : public Item {
public:
    IdCard(const int x, const int y);
    ~IdCard();
    const char getSymbol() override;
};

class Lover : public Item {
public:
    Lover(const int x, const int y);
    ~Lover();
    const char getSymbol() override;
};

class Bicycle : public Item {
public:
    Bicycle(const int x, const int y);
    ~Bicycle();
    const char getSymbol() override;
};

class Food : public Item {
public:
    Food(const int x, const int y);
    ~Food();
    const char getSymbol() override;
};