#include "ulicaclass.h"
#include <QGraphicsLineItem>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include<QDebug>

ulicaClass::ulicaClass(int startX, int startY, int endX, int endY, QString nazov, int ID)
{
    x1 = startX;
    x2 = endX;
    y1 = startY;
    y2 = endY;
    nazovUlice = nazov;
    ID_ulice = ID;
    QPen *pen = new QPen({Qt::black},4);
    ulicaItem = new QGraphicsLineItem(x1,y1,x2,y2,Q_NULLPTR);
    ulicaItem->setPen(*pen);
    ulicaItem->setFlag(QGraphicsItem::ItemIsSelectable,true);
    rychlostPremavky = 1;
}


