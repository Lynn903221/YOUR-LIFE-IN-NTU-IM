#include "Player.h"
#include <iostream>

using namespace std;

Player::Player() {
    posX = 1;
    posY = 1;
    symbol = 'Q';
    hp = 3;
    this->credit = 0;
    color = "\033[34m"; // 藍色
    this->hasIdCard = false;
    this->isSingle = true;
    this->isRidingBike = true;
}

void Player::resetPos() {
    this->posX = 1;
    this->posY = 1;
    this->hasIdCard = false;
}

void Player::move(std::vector<std::string>& maze, const char& move_key) {
    // 計算目標位置
    int targetX = posX, targetY = posY;

    switch (move_key) {
        case 'w': targetY = posY - 1; break;
        case 's': targetY = posY + 1; break;
        case 'a': targetX = posX - 1; break;
        case 'd': targetX = posX + 1; break;
        default: return; // 無效輸入
    }

    // 檢查是否可移動
    char targetCell = maze[targetY][targetX];

    if (targetCell == '#') {
        // 撞到牆，無法移動
        return;
    }

    if (targetCell == '=' && !isRidingBike) {
        // 撞到特殊牆，無法移動（未騎車）
        return;
    }

    if (targetCell == 'E') {
        return;
    }

    // 進行移動
    if (isRidingBike && targetCell == '=') {
        // 騎車時穿過特殊牆
        posY = targetY;
        posX = targetX;
        maze[posY][posX] = '='; // 保留牆體符號
    } else {
        // 普通移動
        posY = targetY;
        posX = targetX;
    }
}


const bool Player::isLose() const { return hp <= 0; }

const bool Player::isWin(const std::vector<std::string>& maze, const char& move_key) const {
    switch (move_key) {
        case 'w' : 
            if(maze[posY - 1][posX] == 'E' or maze[posY][posX] == 'E') 
                return true; 
            else
                return false;    
            break;
        case 's' : 
            if(maze[posY + 1][posX] == 'E' or maze[posY][posX] == 'E') 
                return true;
            else
                return false; 
            break;
        case 'a' : 
            if(maze[posY][posX - 1] == 'E' or maze[posY][posX] == 'E') 
                return true; 
            else
                return false;
            break;
        case 'd' : 
            if(maze[posY][posX + 1] == 'E' or maze[posY][posX] == 'E') 
                return true; 
            else
                return false;
            break;
        default : return false; break;
    }
}

const bool Player::isHurt(std::vector<Enemy*> enemies, const int& enemyCnt) const {
    for (int i = 0; i < enemyCnt; i++) {
        if (posX == enemies[i]->getPosX() && posY == enemies[i]->getPosY()) return true;
    }
    return false;
}

void Player::playerHurt() {
    posX = 1;
    posY = 1;
    hp--;
    cout << "\nYou were attacked by Little Chieh !!!";
}

void Player::increaseCredit() {
    this->credit += 0.25;
}

void Player::resetCredit() {
    this->credit = 0;
}

const int Player::getHP() const { return hp; }
const float Player::getCredit() const { return this->credit; }
const std::string& Player::getColor() const  { return color; }
void Player::setIdCard() { this->hasIdCard = true; };
const bool Player::getHasIdCard() const { return hasIdCard; };
void Player::setIsSingle() { isSingle = false; };
const bool Player::getIsSingle() const { return isSingle; };
void Player::increaseHP() { this->hp++; };
void Player::setIsRidingBike(const bool _isRidingBike) { this->isRidingBike = _isRidingBike; };
const bool Player::getIsRidingBike() const { return this->isRidingBike; };
void Player::setSymbol(const char _symbol) { this->symbol = _symbol; };