
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

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
    /**
     * @brief zaciatokX začiatočná X-sová súradnica linky (autobus sa zobrazí na tejto súradnici)
     */
    int zaciatokX;
    /**
     * @brief zaciatokY začiatočná X-sová súradnica linky (autobus sa zobrazí na tejto súradnici)
     */
    int zaciatokY;
    /**
     * @brief bodyPohybu body po ktorých sa bude autobus pohybovať
     */
    QList<QPoint> *bodyPohybu = new QList<QPoint>();
    /**
     * @brief zoznamUlicLinky zoznam ukazateľov na všetky ulice na linke
     */
    QList<ulicaClass*> *zoznamUlicLinky = new QList<ulicaClass*>();

    /**
     * @brief zastavkyNaLince zastavky na dane lince + doba, za kterou by mel vyrazit z dane zastavky
     */
    QVector<QPair<zastavkaClass*, int>> *zastavkyNaLince = new QVector<QPair<zastavkaClass*,int>>();
};

#endif // BODYLINKY_H
