#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "Character.h"
#include <random>
#include <vector>

using namespace std;

class Enemy : public Character {
    friend class Player;

private:
    std::string color;

public:
    Enemy(const int& e_posX, const int& e_posY);
    const std::string& getColor() const;
    // 敵人移動（隨機）
    void moveEnemy(const std::vector<std::string>& maze, std::mt19937& gen, std::uniform_int_distribution<>& dis);
};

#endif