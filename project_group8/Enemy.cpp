#include "Enemy.h"
#include <random> 

using namespace std;

Enemy::Enemy(const int& e_posX, const int& e_posY) {
    this->posX = e_posX;
    this->posY = e_posY;
    this->symbol = '@';
    this->color = "\033[31m"; // 紅色
}

const string& Enemy::getColor() const {
    return color;
}

void Enemy::moveEnemy(const std::vector<std::string>& maze, std::mt19937& gen, std::uniform_int_distribution<>& dis) {
    int newX = posX;
    int newY = posY;

    int direction = dis(gen); // 隨機生成方向
    switch (direction) {
        case 1: // 上
            newY -= 1;
            break;
        case 2: // 下
            newY += 1;
            break;
        case 3: // 左
            newX -= 1;
            break;
        case 4: // 右
            newX += 1;
            break;
        default:
            break;
    }

    // 確認新位置是否為空地
    if (maze[newY][newX] != '#' && maze[newY][newX] != '@' && maze[newY][newX] != 'E' && maze[newY][newX] != '=') {
        posX = newX;
        posY = newY;
    }
}
