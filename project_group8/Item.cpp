#include "Item.h"

Item::Item(int x, int y) : locationX_(x), locationY_(y), isCollected_(false) {}
Item::~Item() {}
const int Item::getPosX() const { return this->locationX_; }
const int Item::getPosY() const { return this->locationY_; }
const char Item::getSymbol() { return ' '; };
void Item::checkAndCollect(const int playerLocX, const int playerLocY, const vector<string>& maze) {
    if (playerLocX == locationX_ && playerLocY == locationY_ && !isCollected_) {
        isCollected_ = true;
    }
}
const bool Item::getIsCollected() const {
    return isCollected_;
}

IdCard::IdCard(int x, int y) : Item(x, y) {}
IdCard::~IdCard() {}
const char IdCard::getSymbol() { return '%'; }

Lover::Lover(int x, int y) : Item(x, y) {}
Lover::~Lover() {}
const char Lover::getSymbol() { return '!'; }

Bicycle::Bicycle(int x, int y) : Item(x, y) {}
Bicycle::~Bicycle() {}
const char Bicycle::getSymbol() { return '~'; }

Food::Food(int x, int y) : Item(x, y) {}
Food::~Food() {}
const char Food::getSymbol() { return '*'; }
