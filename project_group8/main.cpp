#include <iostream>
#include <vector>
#include <string>

#include "Player.h"
#include "Enemy.h"
#include "Global.h"
#include "Item.h"
#include "Game.h"
//#include "conio.h"


int main() {
    srand(time(NULL));
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1,4);

    clearScreen();

    Game game;
    game.intro();
    game.start(gen, dis);

    return 0;
}
