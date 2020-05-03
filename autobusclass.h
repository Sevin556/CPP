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

    //inicializacia
    int zaciatokTrasyX;
    int zaciatokTrasyY;
    QList<int> zoznamUlicLinky;

    QMap<int,ulicaClass*>zoznamUlicMesta;
    QGraphicsEllipseItem *autobusItem;
    QGraphicsScene *scene;

    //pohyb
    int smerX;
    int smerY;
    int index =0;

    QList<QPointF> bodyPohybu;//mozno by bolo dobre ulozit body dopredu a len ich nacitavat doradu z tohoto zoznamu...zatial ho nevyuzivvam
    QPointF aktualnaPozicia;
    QPointF dalsiBod;

    qreal koeficientX;
    qreal koeficientY;

    //funkcie
    autobusClass(QGraphicsScene *parentScene,QMap<int,ulicaClass*> *zoznamUlic,QObject * parent);
    void posunAutobus();
    int pocitajTrasu();
    int vykonajTrasu();

private:
    QGraphicsEllipseItem* createBus(QGraphicsScene *scene);
private slots:

};

#endif // AUTOBUSCLASS_H
