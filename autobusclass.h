#ifndef AUTOBUSCLASS_H
#define AUTOBUSCLASS_H

#include <QObject>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include "ulicaclass.h"
class autobusClass: public QObject
{
    Q_OBJECT
public:
    qreal vzdialenostY;
    qreal vzdialenostX;
    int zaciatokTrasyX;
    int zaciatokTrasyY;
    QList<int> zoznamUlicLinky;
    QMap<int,ulicaClass*>zoznamUlicMesta;
    QGraphicsEllipseItem *autobusItem;
    QGraphicsScene *scene;
    QTimer *timer ;
    QList<QPoint> bodyPohybu;
    autobusClass(QGraphicsScene *parentScene,QMap<int,ulicaClass*> *zoznamUlic,QObject * parent);
    void updateBus(qreal x,qreal y);
    void pocitajTrasu(int index);

private:
    QGraphicsEllipseItem* createBus(QGraphicsScene *scene);
private slots:
    void vykonajTrasu();

};

#endif // AUTOBUSCLASS_H
