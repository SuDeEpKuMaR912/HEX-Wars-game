#include "HexBoard.h"
#include "Game.h"

extern Game* game;

HexBoard::HexBoard(){
}

QList<Hex *> HexBoard::gethexes()
{
    return hexes;
}

void HexBoard::placeh(int x, int y, int cols, int rows)
{
    int xshift = 82;
    int yshift = 41;

    for (size_t i = 0, n = cols; i < n; i++)
    {
        if (i % 2 == 0)
        { // even column
            yshift = 0;
        }
        else
        { // odd column
            yshift = 41;
        }
        createhcol(x+xshift*i,y+yshift,rows);
    }
}

void HexBoard::createhcol(int x, int y, int rows)
{
    // creates a column of hexagons
    for (size_t i = 0, n =rows; i < n; i++)
    {
        Hex* hex = new Hex();
        hex->setPos(x,y+82*i);
        hexes.append(hex);
        game->scene->addItem(hex);

        // initialize
        hex->setowner(QString("NOONE"));
        hex->setplaced(true);
    }
}
