#ifndef CHARACTER_H
#define CHARACTER_H

class Character {
protected:
    int posX;
    int posY;
    char symbol;
public:
    Character();
    const int getPosX() const;
    const int getPosY() const;
    const char getSymbol() const;
};

#endif
