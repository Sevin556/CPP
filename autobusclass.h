#ifndef AUTOBUSCLASS_H
#define AUTOBUSCLASS_H

#include <QObject>
#include <QWidget>
#include "myscene.h"
#include <QGraphicsEllipseItem>
#include "ulicaclass.h"
#include "zastavkaclass.h"
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
    MyScene *scene;

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
    autobusClass(MyScene *parentScene,QMap<int,ulicaClass*> *zoznamUlic,QMap<int,zastavkaClass*>zoznamZastavok,QObject * parent);
    void posunAutobus();
    int pocitajTrasu();
    int vykonajTrasu();

private:
    QGraphicsEllipseItem* createBus(MyScene *scene);
private slots:

};

#endif // AUTOBUSCLASS_H
