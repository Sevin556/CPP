#ifndef AUTOBUSCLASS_H
#define AUTOBUSCLASS_H

#include <QObject>
#include <QWidget>
//#include "myscene.h"
#include <QGraphicsEllipseItem>
#include "ulicaclass.h"
#include "zastavkaclass.h"

class zastavkaClass;
class autobusClass: public QObject
{
    Q_OBJECT
public:

    //inicializacia
    int zaciatokTrasyX;
    int zaciatokTrasyY;
    QList<ulicaClass*> zoznamUlicLinky;

    QMap<int,ulicaClass*>zoznamUlicMesta;
    QGraphicsEllipseItem *autobusItem;
   // MyScene *scene;

    //pohyb
    int smerX;
    int smerY;
    int index =0;

    QList<QPointF> bodyPohybu;//mozno by bolo dobre ulozit body dopredu a len ich nacitavat doradu z tohoto zoznamu...zatial ho nevyuzivvam
    QPointF aktualnaPozicia;
    QPointF dalsiBod;

    qreal koeficientX;
    qreal koeficientY;

    //zastavky na dane lince + doba, za kterou by mel vyrazit z dane zastavky
    QVector<QPair<zastavkaClass*, int>>zastavkyNaLince;

    //funkcie
    autobusClass(QMap<int,ulicaClass*> *zoznamUlic,QMap<int,zastavkaClass*>zoznamZastavok, QString linka, int time, QObject * parent);
    void posunAutobus();
    int pocitajTrasu();
    int vykonajTrasu(int time);

private:
    QGraphicsEllipseItem* createBus();
private slots:

};

#endif // AUTOBUSCLASS_H
