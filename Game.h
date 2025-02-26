#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "HexBoard.h"
#include <QMouseEvent>

class Game: public QGraphicsView
{
    Q_OBJECT
public:
    // constructors
    Game(QWidget* parent=NULL);

    // public methods
    void dispmenu();
    QString getturn();
    void setturn(QString player);
    void pickup(Hex* card);
    void placecard(Hex* hextorepl);
    void nextturn();
    void removefrdeck(Hex* card,QString player);
    void gameover();
    void dispgameover(QString textToDisplay);

    // events
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent* event);

    // public attributes
    QGraphicsScene* scene;
    HexBoard* hexb;
    Hex* cardtopl;
    QPointF originalpos;
    int numofcards;

public slots:
    void start();
    void restart();

private:
    // private methods
    void drawpane(int x, int y, int w, int h, QColor colour, double opac);
    void drawgui();
    void createcard(QString player);
    void createcardsini();
    void drawcards();

    // private attributes
    QString turn;
    QGraphicsTextItem* turntext;
    QList<Hex* > player1cards;
    QList<Hex* > player2cards;
};

#endif // GAME_H
