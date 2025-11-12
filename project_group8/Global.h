#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <utility> // for std::pair
#include "Player.h"
#include "Enemy.h"
#include "Item.h"

using namespace std;

// 清除終端原有輸出
void clearScreen();

// 讀入新關卡名字、地圖、敵人、道具
void loadEnemies(const string& filename, vector<pair<int, int>>& enemyPositions, int& enemyCnt);
void loadItems(const string& filename, vector<Item*>& items, const int& WIDTH, const int& HEIGHT, int& itemCnt);
void loadMaze(const string& filename, vector<string>& maze);

// 釋放敵人與道具記憶體，方便讀入新關卡的敵人與道具資訊
void cleanupEnemies(vector<Enemy*>& enemies);
void cleanupItems(vector<Item*>& items);

// 輸出地圖
void displayMaze(const string& mazename, const vector<string>& maze, Player& player, const vector<Enemy*>& enemies, 
       const vector<Item*>& items, const vector<vector<bool>>& credit, const bool& isNight, const float& radius, 
       const string& playerName, const int& currentLevel);
// 輸出地圖上各個元素
void displayCell(int x, int y, const vector<string>& maze, Player& player, const vector<Enemy*>& enemies,
                 const vector<Item*>& items, const vector<vector<bool>>& credit);

#endif
