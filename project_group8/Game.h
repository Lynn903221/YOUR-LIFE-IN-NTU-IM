#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "Item.h"
#include "Global.h"

using namespace std;

class Game {
private:
    int currentLevel;
    vector<string> maze;
    int HEIGHT, WIDTH;

    Player player;
    string nickname;
    vector<vector<bool>> credits;

    vector<Enemy*> enemies;
    vector<pair<int, int>> enemyPositions;
    int enemyCnt = 0;

    vector<Item*> items;

    bool isNight;
    int radius;

    bool isRunning;

public:
    Game();
    ~Game();

    void intro();
    void start(std::mt19937& gen, std::uniform_int_distribution<>& dis);
};

#endif