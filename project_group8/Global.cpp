#include "Global.h"

using namespace std;

void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

void loadEnemies(const string& filename, vector<pair<int, int>>& enemyPositions, int& enemyCnt) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Unable to open enemy file " << filename << endl;
        exit(1);
    }

    enemyCnt = 0; // 初始化敵人數量
    std::string line;
    while (std::getline(file, line)) { // 一次讀取一行
        if (line.empty()) {
            continue; // 跳過空行
        }

        std::istringstream ss(line); // 將每行內容放入字符串流
        int x, y;                   // 解析座標
        char comma;                 // 用於解析逗號
        if (ss >> x >> comma >> y && comma == ',') { // 驗證格式為 "數字, 數字"
            enemyPositions.emplace_back(x, y); // 加入座標到 vector
            enemyCnt++;                        // 增加敵人數量
        } else {
            std::cerr << "Error: Invalid line format in " << filename << ": " << line << std::endl;
        }
    }

    file.close(); // 關閉檔案
}

void loadItems(const string& filename, vector<Item*>& items, const int& WIDTH, const int& HEIGHT, int& itemCnt) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        exit(1);
    }

    int x, y;
    string itemType;
    while (file >> x >> y >> itemType) {
        itemCnt++;
        if (x >= WIDTH || y >= HEIGHT) {
            cerr << "Error: Item position out of bounds in " << filename << " at line " << itemCnt << endl;
            exit(1);
        }
        if (itemType == "%") {
            items.push_back(new IdCard(x, y));
        } else if (itemType == "!") {
            items.push_back(new Lover(x, y));
        } else if (itemType == "~") {
            items.push_back(new Bicycle(x, y));
        } else if (itemType == "*") {
            items.push_back(new Food(x, y));
        } else {    
            cerr << "Error: Unknown item type \"" << itemType << "\" in " << filename << " at line " << itemCnt << endl;
            exit(1);
        }
    }
    file.close();
}


void loadMaze(const string& filename, vector<string>& maze) {
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Unable to open maze file " << filename << endl;
        exit(1);
    }

    string line;

    while (getline(file, line)) {
            maze.push_back(line); // 確保每一行地圖都被加入
    }

    file.close();
}

void cleanupEnemies(vector<Enemy*>& enemies) {
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
    enemies.clear();
}

void cleanupItems(vector<Item*>& items) {
    for (Item* item : items) {
        delete item;
    }
    items.clear();
}

void displayMaze(const string& mazename, const vector<string>& maze, Player& player, const vector<Enemy*>& enemies, 
       const vector<Item*>& items, const vector<vector<bool>>& credit, const bool& isNight, const float& radius, 
       const string& playerName, const int& currentLevel) {

    const int HEIGHT = maze.size();
    const int WIDTH = maze[0].size();

    // 輸出地圖與玩家資訊
    cout << mazename << "\n";
    cout << "-----------------------------\n";
    cout << playerName << "'s GPA:     " << player.getHP() << ".0\n";
    cout << playerName << "'s Credits: " << player.getCredit() << "\n"; 
    cout << "-----------------------------\n";

    // 檢查每個座標
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // 夜間模式檢查距離
            if (isNight) {
                float distanceSquared = (player.getPosX() - x) * (player.getPosX() - x) / 4 + (player.getPosY() - y) * (player.getPosY() - y);
                if (distanceSquared > radius * radius) {
                    cout << "\033[30m \033[0m"; // 黑色輸出
                    continue;
                }
            }

            // 顯示當前格子內容
            displayCell(x, y, maze, player, enemies, items, credit);
        }
        cout << endl;
    }

    // 不同關卡的提示
    switch (currentLevel)
    {
    case 0:
        cout << "\nPress (W) (A) (S) (D) to move.";
        break;
    case 1:
        cout << "\nBE CAREFUL! There is a dangerous enemy.";
        break;
    case 2:
        cout << "\nNight comes...";
        break;
    default:
        break;
    }
}

void displayCell(int x, int y, const vector<string>& maze, Player& player, const vector<Enemy*>& enemies,
        const vector<Item*>& items, const vector<vector<bool>>& credit) {

    // 敵人為優先顯示
    for (Enemy* enemy : enemies) {
        if (x == enemy->getPosX() && y == enemy->getPosY()) {
            cout << enemy->getColor() << enemy->getSymbol() << "\033[0m";
            return;
        }
    }

    // 道具為次要顯示
    for(Item* item : items) {
        if (x == item->getPosX() && y == item->getPosY() && !item->getIsCollected()) {
            cout << "\033[32m" << item->getSymbol() << "\033[0m";
            return;
        }
    }

    // 其餘元素
    if (std::string(1, maze[y][x]) == "#") {
        cout << "#";
    }
    else if (x == player.getPosX() && y == player.getPosY()) {
        cout << player.getColor() << player.getSymbol() << "\033[0m";
    } 
    else if (std::string(1, maze[y][x]) == "=") {
        cout << "=";
    }
    else if (maze[y][x] == 'E') {
        cout << "\033[32mE\033[0m";
    } 
    else if (('A' <= maze[y][x] && maze[y][x] <= 'Z') || ('a' <= maze[y][x] && maze[y][x] <= 'z')) {
        cout << "\033[36m" << maze[y][x] << "\033[0m";
    }
    else if (credit[y][x]){
        cout << "\033[33m.\033[0m";
    } 
    else {
        cout << maze[y][x];
    }
}