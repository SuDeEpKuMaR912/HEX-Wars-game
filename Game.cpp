#include "Game.h"
#include "HexBoard.h"
#include "Button.h"
#include <QGraphicsTextItem>
#include <stdlib.h> // srand() and rand()
#include <time.h> // time()

#include <QDebug>
Game::Game(QWidget *parent)
{
    // set up the screen
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1024,768);

    // set up the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1024,768);
    setScene(scene);

}

void Game::start()
{
    // initialize
    cardtopl = NULL;
    numofcards = 0;

    // clear the screen
    scene->clear();

    hexb = new HexBoard();
    hexb->placeh(200,30,7,7);
    drawgui();
    createcardsini();
}

void Game::restart()
{
    // clear some stuff then call start()
    player1cards.clear();
    player2cards.clear();
    hexb->gethexes().clear();
    scene->clear();
    start();
}

void Game::drawpane(int x, int y, int w, int h, QColor colour, double opac)
{
    // draws a panel at the specified location
    QGraphicsRectItem* panel = new QGraphicsRectItem(x,y,w,h);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(colour);
    panel->setBrush(brush);
    panel->setOpacity(opac);
    scene->addItem(panel);
}

void Game::drawgui()
{
    // draw the left panel
    drawpane(0,0,150,768,Qt::darkCyan,1);

    // draw the right panel
    drawpane(874,0,150,768,Qt::darkCyan,1);

    // place player 1 text
    QGraphicsTextItem* p1 = new QGraphicsTextItem("Player 1's Cards: ");
    p1->setPos(25,0);
    scene->addItem(p1);

    // place player 1 text
    QGraphicsTextItem* p2 = new QGraphicsTextItem("Player 2's Cards: ");
    p2->setPos(874+25,0);
    scene->addItem(p2);

    // place whosTurnText
    turntext = new QGraphicsTextItem();
    setturn(QString("PLAYER1"));
    turntext->setPos(490,0);
    scene->addItem(turntext);

}

void Game::createcard(QString player)
{
    // create the card
    Hex* card = new Hex();
    card->setowner(player);
    card->setplaced(false);

    // randommize side attacks of card
    for (size_t i = 0, n = 6; i < n; i++)
    {
        // generate a random number
        int randnum = rand() % 6 + 1; // 1 - 6
        card->setatk(i,randnum);
    }

    // make side attacks visible
    card->displaysatk();

    // add the card to the proper list
    if (player == QString("PLAYER1"))
    {
        player1cards.append(card);
    }
    else
    {
        player2cards.append(card);
    }

    // draw the cards
    drawcards();
}

void Game::createcardsini()
{
    // create player 1's cards
    for (size_t i = 0, n = 5; i < n; i++)
    {
        createcard(QString("PLAYER1"));
    }

    // create player 2's cards
    for (size_t i = 0, n = 5; i < n; i++)
    {
        createcard(QString("PLAYER2"));
    }

    drawcards();
}

void Game::drawcards()
{
    //go through list of cards and draw them

    //remove all of player1's cards from the scene
    for (size_t i = 0, n = player1cards.size(); i < n; i++)
    {
        scene->removeItem(player1cards[i]);
    }
    //remove all of player2's cards from the scene
    for (size_t i = 0, n = player2cards.size(); i < n; i++)
    {
        scene->removeItem(player2cards[i]);
    }

    // draw player1's cards
    for (size_t i = 0, n = player1cards.size(); i < n; i++)
    {
        Hex* card = player1cards[i];
        card->setPos(13,25+85*i);
        scene->addItem(card);
    }

    // draw player2's cards
    for (size_t i = 0, n = player2cards.size(); i < n; i++)
    {
        Hex* card = player2cards[i];
        card->setPos(874+13,25+85*i);
        scene->addItem(card);
    }
}

void Game::dispmenu()
{
    // create the title text
    QGraphicsTextItem* title = new QGraphicsTextItem(QString("HexWars"));
    QFont titlef("comic sans",50);
    title->setFont(titlef);
    int txpos = this->width()/2 - title->boundingRect().width()/2;
    int typos = 150;
    title->setPos(txpos,typos);
    scene->addItem(title);

    // create the play button
    Button* play = new Button(QString("Play"));
    int pxpos = this->width()/2 - play->boundingRect().width()/2;
    int pypos = 275;
    play->setPos(pxpos,pypos);
    connect(play,SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(play);

    // create the quit button
    Button* quit = new Button(QString("Quit"));
    int qxpos = this->width()/2 - quit->boundingRect().width()/2;
    int qypos = 350;
    quit->setPos(qxpos,qypos);
    connect(quit,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quit);
}

QString Game::getturn()
{
    return turn;
}

void Game::setturn(QString player)
{
    // change the QString
    turn = player;

    // change the QGraphicsTextItem
    turntext->setPlainText(QString("Whos turn: ") + player);
}

void Game::pickup(Hex *card)
{
    // picks up the specified card
    if (card->getowner() == getturn() && cardtopl == NULL){
        cardtopl = card;
        originalpos = card->pos();
        return;
    }
}

void Game::placecard(Hex *hextorepl){
    // replaces the specified hex with the card to place
    cardtopl->setPos(hextorepl->pos());
    hexb->gethexes().removeAll(hextorepl);
    hexb->gethexes().append(cardtopl);
    scene->removeItem(hextorepl);
    cardtopl->setplaced(true);
    removefrdeck(cardtopl,getturn());

    //find neighbors
    cardtopl->findn();
    cardtopl->capture();

    // make card to place null
    cardtopl = NULL;

    // replace the placed card w a new one
    createcard(getturn());

    // make it the next players turn
    nextturn();

    numofcards++;

    if (numofcards >= hexb->gethexes().size()){
        gameover();
    }
}

void Game::nextturn()
{
    if (getturn() == QString("PLAYER1"))
    {
        setturn(QString("PLAYER2"));
    }
    else
    {
        setturn(QString("PLAYER1"));
    }
}

void Game::removefrdeck(Hex *card, QString player)
{
    if (player == QString("PLAYER1"))
    {
        // remove from player 1
        player1cards.removeAll(card);
    }

    if (player == QString("PLAYER2"))
    {
        // remove from player 1
        player2cards.removeAll(card);
    }

}

void Game::gameover()
{
    // count hexes, determine who has more, set messege
    int p1hexes = 0;
    int p2hexes = 0;
    for (size_t i = 0, n = hexb->gethexes().size(); i < n; ++i)
    {
        if (hexb->gethexes()[i]->getowner() == QString("PLAYER1")){
            p1hexes++;
        }
        else if (hexb->gethexes()[i]->getowner() == QString("PLAYER2")){
            p2hexes++;
        }
    }

    QString messege;
    if (p1hexes > p2hexes)
    {
        messege = "Player 1 has won!";
    }
    else if (p2hexes > p1hexes)
    {
        messege = "Player 2 has won!";
    }
    else
    {
        messege = "Tie game!";
    }

    dispgameover(messege);

}

void Game::dispgameover(QString texttodis)
{
    // disable all scene items
    for (size_t i = 0, n = scene->items().size(); i < n; i++){
        scene->items()[i]->setEnabled(false);
    }

    // pop up semi transparent panel
    drawpane(0,0,1024,768,Qt::black,0.65);

    // draw panel
    drawpane(312,184,400,400,Qt::lightGray,0.75);

    // create playAgain button
    Button* replay = new Button(QString("Play Again"));
    replay->setPos(410,300);
    scene->addItem(replay);
    connect(replay,SIGNAL(clicked()),this,SLOT(restart()));

    // create quit button
    Button* quit = new Button(QString("Quit"));
    quit->setPos(410,375);
    scene->addItem(quit);
    connect(quit,SIGNAL(clicked()),this,SLOT(close()));

    // create text annoucning winner
    QGraphicsTextItem* overtxt = new QGraphicsTextItem(texttodis);
    overtxt->setPos(460,225);
    scene->addItem(overtxt);

}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    // if there is a card to place, then make it follow the mouse
    if (cardtopl)
    {
        cardtopl->setPos(event->pos());
    }

    QGraphicsView::mouseMoveEvent(event);
}

void Game::mousePressEvent(QMouseEvent *event)
{
    // make right click return card to originalPos
    if (event->button() == Qt::RightButton){
        if (cardtopl){
            cardtopl->setPos(originalpos);
            cardtopl = NULL;
            return;
        }
    }

    QGraphicsView::mousePressEvent(event);
}
