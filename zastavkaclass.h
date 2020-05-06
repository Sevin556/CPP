#ifndef ZASTAVKACLASS_H
#define ZASTAVKACLASS_H

#include <QObject>
#include <QWidget>
#include <QGraphicsRectItem>
#include "myscene.h"

class MyScene;

class zastavkaClass
{
public:
    int X;
    int Y;
    QString nazovZastavky;
    int ID_zastavky;
    QGraphicsRectItem *zastavkaItem;
    zastavkaClass(int startX, int startY,QString nazov, int ID,MyScene *scene);
};

#endif // ZASTAVKACLASS_H
