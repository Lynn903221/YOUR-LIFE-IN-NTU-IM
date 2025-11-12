#include "Game.h"
#include "conio.h"  // for windows

/*#include <termios.h>    // for mac
#include <unistd.h>*/ 

using namespace std;

// mac 用於即時輸入的函數
/*char getch() {
    struct termios oldt, newt;
    char ch;
    
    tcgetattr(STDIN_FILENO, &oldt);            // 獲取當前終端設置
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);           // 禁用緩衝區和回顯
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);   // 設置新終端設置

    ch = getchar();                            // 讀取字符

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);   // 恢復原始終端設置
    return ch;
}*/

Game::Game() {
    // 設置遊戲初始狀態
    this->currentLevel = 0;
    isNight = false;
    radius = 6;
    isRunning = true;
}

Game::~Game() {
    for (auto enemy : enemies) delete enemy;
    for (auto item : items) delete item;
}

void Game::intro() {
    string filename1 = "betas/beta4/txts/cutscene/gameintro.txt";

    ifstream inFile(filename1); 
    if (!inFile) {
        cerr << "無法開啟檔案: " << filename1 << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) { 
        cout << line << endl;
    }
    inFile.close();
    cout << "\nPress any button except (Q) and ENTER to cuntinue.\n";
    cout << "Press (Q) and ENTER to quit.\n";
}

void Game::start(std::mt19937& gen, std::uniform_int_distribution<>& dis) {
    if (this->currentLevel == 0) {
        // 遊戲開始選擇是否遊玩
        char startChoice;
        cin >> startChoice;
        if (startChoice == 'q' || startChoice == 'Q') {
            clearScreen();
            cout << endl << "Goodbye..." << endl;
            return;
        }

        clearScreen();
        // 玩家輸入姓名
        cout << "\nEnter your nickname: ";
        cin >> nickname;
        clearScreen();
        cout << "\nYour nickname is: " << nickname << endl;
        cout << "Please press any button except (Q) to start the game.";
    }

    // 給予各地圖名字
    vector<string> mazeNames = {
        "Royal Plam Blvd.",
        "College of Manegemant",
        "Gongguan Night Market",
        "IM's space",
        "Library",
    };

    // 給予各關卡地圖、敵人、過場畫面、道具檔案路徑
    vector<string> mazeFiles = {
        "betas/beta4/txts/maze/maze_RoyalPlamBlvd.txt",
        "betas/beta4/txts/maze/maze_CoM.txt",
        "betas/beta4/txts/maze/maze_gg.txt",
        "betas/beta4/txts/maze/maze_imspace.txt",
        "betas/beta4/txts/maze/maze_library.txt",
    };
    vector<string> enemyFiles = {
        "betas/beta4/txts/enemy/enemy_RoyalPlamBlvd.txt",
        "betas/beta4/txts/enemy/enemy_CoM.txt",
        "betas/beta4/txts/enemy/enemy_gg.txt",
        "betas/beta4/txts/enemy/enemy_imspace.txt",
        "betas/beta4/txts/enemy/enemy_library.txt",
    };
    vector<string> cutsceneFiles = {
        "betas/beta4/txts/cutscene/map_intro_to_1.txt",
        "betas/beta4/txts/cutscene/map_1_to_2.txt",
        "betas/beta4/txts/cutscene/map_2_to_3.txt",
        "betas/beta4/txts/cutscene/map_3_to_4.txt",
        "betas/beta4/txts/cutscene/map_4_to_5.txt",
        "betas/beta4/txts/cutscene/ending.txt",
    };
    vector<string> itemFiles = {
        "betas/beta4/txts/item/item_RoyalPlamBlvd.txt",
        "betas/beta4/txts/item/item_CoM.txt",
        "betas/beta4/txts/item/item_gg.txt",
        "betas/beta4/txts/item/item_imspace.txt",
        "betas/beta4/txts/item/item_library.txt",
    };

    // 判定遊戲是否繼續進行
    while (this->currentLevel < 5 && this->currentLevel != -1 && isRunning && player.getHP() > 0) {
        // 輸出過場畫面
        ifstream inFile(cutsceneFiles[currentLevel]); 
        if (!inFile) {
            cerr << "無法開啟檔案: " << cutsceneFiles[currentLevel] << endl;
            return;
        }

        clearScreen();
        string line;
        while (getline(inFile, line)) { 
            cout << line << endl;
        }
        inFile.close();

        player.resetPos();

        // 第二關時腳踏車被偷
        if(currentLevel == 1) {
            player.setSymbol('O');
            player.setIsRidingBike(false);
        }

        // 前兩關為白天，後面變成黑暗模式
        if(currentLevel < 2)
            isNight = false;
        else 
            isNight = true;

        // 第三關之後可是範圍逐漸縮小
        if(currentLevel > 2)
            radius -= 2;

        vector<string> maze;
        vector<pair<int, int>> enemyPositions;
        int enemyCnt = 0;

        // 敵人隨機移動所需
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1,4);

        loadMaze(mazeFiles[currentLevel], maze);
        loadEnemies(enemyFiles[currentLevel], enemyPositions, enemyCnt);

        const int HEIGHT = maze.size(); // 地圖高度
        const int WIDTH = maze[0].size(); // 地圖寬度

        // 學分判定的二微陣列
        vector<vector<bool>> credits(HEIGHT, vector<bool>(WIDTH, true));
        
        vector<Enemy*> enemies;
        for (const auto& pos : enemyPositions) {
            enemies.push_back(new Enemy(pos.first, pos.second));
        }

        vector<Item*> items;
        int itemCnt = 0;
        loadItems(itemFiles[currentLevel], items, WIDTH, HEIGHT, itemCnt);

        for (int i = 0; i < enemyCnt; i++) {
            credits[enemies[i]->getPosY()][enemies[i]->getPosX()] = false;
        }
        for (auto& item : items) {
            credits[item->getPosY()][item->getPosX()] = false;
        }
        credits[1][1] = false;

        char move_key = 'd';

        // 每一關內隨著玩家操作檢查並更新畫面
        while (true) {
            move_key = _getch();    // for windows
            //move_key = getch();     // for mac

            // 退出遊戲
            if (move_key == 'q') {
                clearScreen();
                cout << endl << "Goodbye..." << endl;
                currentLevel = -1;
                break;
            }

            player.move(maze, move_key);

            // 先檢查是否遇到敵人而受傷(玩家移動後)
            if (player.isHurt(enemies, enemyCnt)) {
                player.playerHurt();
                if (player.isLose()) {
                    clearScreen();
                    displayMaze(mazeNames[currentLevel], maze, player, enemies, items, credits, isNight, radius, nickname, currentLevel);
                    cout << "\nYou LOSE!" << endl;
                    break;
                }
                continue;
            }

            // 敵人移動
            for (int i = 0; i < enemyCnt; i++) {
                enemies[i]->moveEnemy(maze, gen, dis);
            }

            // 再次檢查是否遇到敵人而受傷(敵人移動後)
            if (player.isHurt(enemies, enemyCnt)) {
                player.playerHurt();
                if (player.isLose()) {
                    clearScreen();
                    displayMaze(mazeNames[currentLevel], maze, player, enemies, items, credits, isNight, radius, nickname, currentLevel);
                    cout << "\nYou LOSE!" << endl;
                    player.resetCredit();
                    break;
                }
                continue;
            }

            // 檢查各個道具是否被撿起
            for (auto& item : items) {
                if (!item->getIsCollected()) {
                    item->checkAndCollect(player.getPosX(), player.getPosY(), maze);
                    if(dynamic_cast<IdCard*>(item) && item->getIsCollected()) {
                        player.setIdCard();
                    }
                    else if (dynamic_cast<Lover*>(item) && item->getIsCollected()) {
                        player.setIsSingle();
                    }
                    else if (dynamic_cast<Food*>(item) && item->getIsCollected()) {
                        player.increaseHP();
                    }
                    else if (dynamic_cast<Bicycle*>(item) && item->getIsCollected()) {
                        player.setIsRidingBike(true);
                        player.setSymbol('Q');
                    }
                }
            }

            // 檢查是否撿到學分
            if (credits[player.getPosY()][player.getPosX()]) {
                credits[player.getPosY()][player.getPosX()] = false;
                player.increaseCredit();
            }

            // 檢查是否通關
            if (player.isWin(maze, move_key) && player.getHasIdCard()) {
                clearScreen();
                currentLevel ++;
                break;
            }
            else if(player.isWin(maze, move_key) && !player.getHasIdCard()) {
                clearScreen();
                displayMaze(mazeNames[currentLevel], maze, player, enemies, items, credits, isNight, radius, nickname, currentLevel);
                cout << "\nYou has no Id Card to access!";
                continue;
            }

            clearScreen();
            displayMaze(mazeNames[currentLevel], maze, player, enemies, items, credits, isNight, radius, nickname, currentLevel);
        }

        cleanupEnemies(enemies);
        cleanupItems(items);
    }

    // 全破後的結束畫面
    if(currentLevel == 5) {
        ifstream inFile(cutsceneFiles[currentLevel]); 
        if (!inFile) {
            cerr << "error: " << cutsceneFiles[currentLevel] << endl;
            return;
        }

        clearScreen();
        string line;
        while (getline(inFile, line)) { 
            cout << line << endl;
        }
        inFile.close();

        cout << "\n------------------------------\n";
        cout << "Your final Credit: " << player.getCredit() << "\n";
        if(player.getCredit() < 134) {
            cout << "You have NOT graduated yet. ;(\n\n";
        }
        else {
            cout << "You GRADUATED! :D\n\n";
        }

        cout << "Your final GPA: " << player.getHP() << ".0\n";
        if(player.getHP() == 8) {
            cout << "You got A+ :O";
        }
        else if(player.getHP() == 7) {
            cout << "You got A :O";
        }
        else if(player.getHP() == 6) {
            cout << "You got A- :O";
        }
        else if(player.getHP() == 5) {
            cout << "You got B+ :I";
        }
        else if(player.getHP() == 4) {
            cout << "You got B :I";
        }
        else if(player.getHP() == 3) {
            cout << "You got B- :I";
        }
        else if(player.getHP() == 2) {
            cout << "You got C :(";
        }
        else if(player.getHP() == 1) {
            cout << "You FAILED >:(";
        }

        if(!player.getIsSingle()) {
            cout << "\n\nYou got the LOVE credit! (Someone loves you deeply. <3 <3 <3)";
        }
        else {
            cout << "\n\nYou are still SINGLE in the end... (You're back in your warm cave.)";
        }

        if(player.getIsRidingBike()) {
            cout << "\n\nYou recovered your old bike that was stolen in freshman year!";
        }
        
        cout << "\n------------------------------\n";
    }
    
    return;
}