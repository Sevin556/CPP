#ifndef AUTOBUSCLASS_H
#define AUTOBUSCLASS_H

#include <QObject>
#include <QWidget>
#include <QGraphicsEllipseItem>
#include "ulicaclass.h"
#include "zastavkaclass.h"
#include "bodylinky.h"


class zastavkaClass;
class bodyLinky;
class autobusClass: public QObject
{
    Q_OBJECT
public:

    //inicializacia
    int zaciatokTrasyX;
    int zaciatokTrasyY;
    int poradi = 0;
    QList<ulicaClass*> *zoznamUlicLinky;
    int MojeID;
    QString nazovLinky;

    QMap<int,ulicaClass*>zoznamUlicMesta;
    QGraphicsEllipseItem *autobusItem;

    //pohyb
    int smerX;
    int smerY;
    int index =0;
    int stojim =0;

    QList<QPoint> *bodyPohybu;//mozno by bolo dobre ulozit body dopredu a len ich nacitavat doradu z tohoto zoznamu...zatial ho nevyuzivvam
    QPointF aktualnaPozicia;
    QPointF dalsiBod;

    qreal koeficientX;
    qreal koeficientY;
    int meskanie =0;
    int meskanieNaZastavke =0;
    int indexZastavky =0;
    QPoint dalsiaZastavka;

    //zastavky na dane lince + doba, za kterou by mel vyrazit z dane zastavky
    QVector<QPair<zastavkaClass*, int>>zastavkyNaLince;

    //funkcie
    autobusClass(bodyLinky *linka, int time,int ID,QString nazovLinky, QObject * parent);
    void posunAutobus();
    int pocitajTrasu();
    int vykonajTrasu(int time);
signals:
    void meskanieZmenene(autobusClass* autobus);

private:
    QGraphicsEllipseItem* createBus();


};

#endif // AUTOBUSCLASS_H
