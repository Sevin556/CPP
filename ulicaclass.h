#ifndef ULICACLASS_H
#define ULICACLASS_H

#include <QWidget>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QObject>
#include <QWidget>
class ulicaClass :public QObject ,QGraphicsLineItem
{
    Q_OBJECT
public:

    ulicaClass(int startX, int startY, int endX, int endY, QString nazov, int ID);
    int x1;
    int y1;
    int x2;
    int y2;
    QString nazovUlice;
    int ID_ulice;
    QGraphicsLineItem * ulicaItem;
    int rychlostPremavky;
    ulicaClass vytvorUlicu(int startX,int startY,int endX, int endY,QString nazov, int ID);

protected:
};

#endif // ULICACLASS_H
