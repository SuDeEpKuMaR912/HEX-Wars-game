#ifndef HEX_H
#define HEX_H

#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>

class Hex: public QGraphicsPolygonItem
{
public:
    // constructors
    Hex(QGraphicsItem* parent=NULL);
    // getters
    int getatk(int side);
    bool getplaced();
    QString getowner();

    // event
    void mousePressEvent(QGraphicsSceneMouseEvent* event);

    // setters
    void setatk(int side, int attack);
    void setowner(QString player);
    void setplaced(bool b);

    void displaysatk();
    void createln();
    void findn();
    void switchown();
    void capture();

private:
    QList<QGraphicsLineItem*> lines;
    QList<Hex*> neighbors;
    bool isplaced;
    QString owner;
    int sa0,sa1,sa2,sa3,sa4,sa5;
    QList<QGraphicsTextItem*> attacktxt;
};

#endif // HEX_H
