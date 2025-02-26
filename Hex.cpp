#include "Hex.h"
#include <QPointF>
#include <QPolygonF>
#include <QVector>
#include <QBrush>
#include "Game.h"

extern Game* game;

#include <QDebug>
Hex::Hex(QGraphicsItem *parent)
{
    // draw the hexagon
    QVector<QPointF> hexpts;
    hexpts << QPointF(1,0) << QPointF(2,0) << QPointF(3,1) << QPointF(2,2)
              << QPointF(1,2) << QPointF(0,1);

    // scale the points
    int scale = 40;
    for (size_t i = 0, n = hexpts.size(); i < n; ++i)
    {
        hexpts[i] = hexpts[i] * scale;
    }

    // create a polygon with the scaled points
    QPolygonF hexagon(hexpts);

    // draw the poly
    setPolygon(hexagon);

    // initialize
    isplaced = false;

    // create lines
    createln();

    // initialize side attacks to 0
    sa0= 0;
    sa1= 0;
    sa2= 0;
    sa3= 0;
    sa4= 0;
    sa5= 0;

    // create QGraphicsTextItems to represent visually each side's attack
    QGraphicsTextItem* text0 = new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem* text1 = new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem* text2 = new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem* text3 = new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem* text4 = new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem* text5 = new QGraphicsTextItem(QString::number(0),this);

    attacktxt.append(text0);
    attacktxt.append(text1);
    attacktxt.append(text2);
    attacktxt.append(text3);
    attacktxt.append(text4);
    attacktxt.append(text5);

    // set the correct position of the attack texts
    text0->setPos(50,0);
    text1->setPos(20,15);
    text2->setPos(20,40);
    text3->setPos(50,55);
    text4->setPos(80,40);
    text5->setPos(80,15);

    // make all attack texts invisible
    for (size_t i = 0, n = attacktxt.size(); i < n; i++)
    {
        attacktxt[i]->setVisible(false);
    }
}

int Hex::getatk(int side)
{
    if (side == 0)
    {
        return sa0;
    }
    else if (side == 1)
    {
        return sa1;
    }
    else if (side == 2)
    {
        return sa2;
    }
    else if (side == 3)
    {
        return sa3;
    }
    else if (side == 4)
    {
        return sa4;
    }
    else if (side == 5)
    {
        return sa5;
    }
}

bool Hex::getplaced()
{
    return isplaced;
}

QString Hex::getowner()
{
    return owner;
}

void Hex::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    // if this hex is NOT placed (so it is a card) then pick it up
    if (getplaced() == false)
    {
        game->pickup(this);
    }

    // if this hex is placed, then replace it
    else
    {
        game->placecard(this);
    }
}

void Hex::setatk(int side, int attack)
{
    // set the attack of the specified side
    switch (side){
    case 0:
        sa0= attack;
        attacktxt[0]->setPlainText(QString::number(attack));
    case 1:
        sa1= attack;
        attacktxt[1]->setPlainText(QString::number(attack));
    case 2:
        sa3= attack;
        attacktxt[2]->setPlainText(QString::number(attack));
    case 3:
        sa3= attack;
        attacktxt[3]->setPlainText(QString::number(attack));
    case 4:
        sa4= attack;
        attacktxt[4]->setPlainText(QString::number(attack));
    case 5:
        sa5= attack;
        attacktxt[5]->setPlainText(QString::number(attack));
    }
}

void Hex::setowner(QString player)
{
    // set the owner
    owner = player;

    // change the color
    if (player == QString("NOONE"))
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::lightGray);
        setBrush(brush);
    }

    if (player == QString("PLAYER1"))
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::blue);
        setBrush(brush);
    }

    if (player == QString("PLAYER2"))
    {
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::red);
        setBrush(brush);
    }
}

void Hex::setplaced(bool b)
{
    isplaced = b;
}

void Hex::displaysatk()
{
    // traverse through all the side_attack texts and make them visible
    for (size_t i = 0, n = attacktxt.size(); i < n; i++)
    {
        attacktxt[i]->setVisible(true);
    }
}

void Hex::createln()
{
    QPointF hexcen(x()+60,y()+40);
    QPointF finalpt(hexcen.x(),hexcen.y()-65);
    QLineF ln(hexcen,finalpt);

    for (size_t i = 0, n = 6; i < n; ++i)
    {
        QLineF lncopy(ln);
        lncopy.setAngle(90+60*i);
        QGraphicsLineItem* line = new QGraphicsLineItem(lncopy,this);
        lines.append(line);
        line->setVisible(false);
    }
}

void Hex::findn()
{
    for (size_t i = 0, n = lines.size(); i < n; i++)
    {
        // if the line collides with an item of type Hex, add it to neighbors
        QList<QGraphicsItem*> citems = lines[i]->collidingItems();
        for (size_t j = 0; j < citems.size(); j++)
        {
            Hex* item = dynamic_cast<Hex*>(citems[j]);
            if (citems[j] != this && item)
            {
                neighbors.append(item);
            }
        }
    }
}

void Hex::switchown()
{
    // if the owner is player1, make it player two and vice versa
    if (getowner() == QString("PLAYER1"))
    {
        setowner(QString("PLAYER2"));
    }
    else if (getowner() == QString("PLAYER2"))
    {
        setowner(QString("PLAYER1"));
    }
}

void Hex::capture()
{
    // traverses through neighbors, captures weaker neighbors
    for (size_t i = 0, n = neighbors.size(); i < n; i++)
    {
        bool enemy = false;
        bool notneut = false;

        if (this->getowner() != neighbors[i]->getowner())
        {
            enemy = true;
        }

        if (neighbors[i]->owner != QString("NOONE"))
        {
            notneut = true;
        }

        // it is an enemy and not neutral
        if (enemy && notneut)
        {
            // find attack of touching sides
            int thisatk = 0;
            int neighbatk = 0;
            if (i == 0)
            {
                thisatk = getatk(0);
                neighbatk = neighbors[0]->getatk(3);
            }
            else if (i == 1)
            {
                thisatk = getatk(1);
                neighbatk = neighbors[1]->getatk(4);
            }
            else if (i == 2)
            {
                thisatk = getatk(2);
                neighbatk = neighbors[2]->getatk(5);
            }
            else if (i == 3)
            {
                thisatk = getatk(3);
                neighbatk = neighbors[3]->getatk(0);
            }
            else if (i == 4)
            {
                thisatk = getatk(4);
                neighbatk = neighbors[4]->getatk(1);
            }
            else if (i == 5)
            {
                thisatk = getatk(5);
                neighbatk = neighbors[5]->getatk(2);
            }

            // if this has greater attack, switch neighbors owner
            if (thisatk > neighbatk)
            {
                neighbors[i]->switchown();
            }

        }
    }
}
