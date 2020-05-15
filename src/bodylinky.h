#ifndef BODYLINKY_H
#define BODYLINKY_H

#include <QMap>
#include "ulicaclass.h"
#include "zastavkaclass.h"

class ulicaClass;
class zastavkaClass;

class bodyLinky
{
public:
    bodyLinky(QMap<int,ulicaClass*> *zoznamUlic,QMap<int,zastavkaClass*>zoznamZastavok,QString subor);
    int zaciatokX;
    int zaciatokY;
    QList<QPoint> *bodyPohybu = new QList<QPoint>();
    QList<ulicaClass*> *zoznamUlicLinky = new QList<ulicaClass*>();

    //zastavky na dane lince + doba, za kterou by mel vyrazit z dane zastavky
    QVector<QPair<zastavkaClass*, int>> *zastavkyNaLince = new QVector<QPair<zastavkaClass*,int>>();
};

#endif // BODYLINKY_H
