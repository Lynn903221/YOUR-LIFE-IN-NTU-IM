#include "Character.h"

Character::Character() : posX(0), posY(0), symbol(' ') {}

const int Character::getPosX() const { return posX; }
const int Character::getPosY() const { return posY; }
const char Character::getSymbol() const { return symbol; }
