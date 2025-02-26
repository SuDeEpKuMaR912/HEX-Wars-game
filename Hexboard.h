#ifndef HEXBOARD_H
#define HEXBOARD_H

#include <QList>
#include "Hex.h"

class HexBoard
{
public:
    // constructors
    HexBoard();

    // getters/setters
    QList<Hex*> gethexes();

    // public methods
    void placeh(int x, int y, int cols, int rows);

private:
    void createhcol(int x, int y, int rows);
    QList<Hex*> hexes;
};

#endif // HEXBOARD_H
