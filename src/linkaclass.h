
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

#ifndef LINKYCLASS_H
#define LINKYCLASS_H
#include <QObject>
#include "autobusclass.h"
#include "ulicaclass.h"
#include "zastavkaclass.h"
#include "myscene.h"
#include "vecitem.h"



//class zoznamLiniekClass;

class linkaClass
{
public:
    linkaClass(QMap<int, ulicaClass *> *seznamUlic, QMap<int, zastavkaClass *> seznamZastavek);
    ~linkaClass();

    /**
     * @brief busList list vsech autobusu (aktivnich i neaktivnich)
     */
    QMap<int,autobusClass*> busList;



    /**
     * @brief seznamLinek zoznam všetkých vytvorených liniek
     */
    QVector<vecItem*> seznamLinek;

    //funkcie
    QString linkaBusu(int id);
    void appendBus(QList<autobusClass *> *seznamBusu, int time, MyScene *scene);
    void setTime(QList<autobusClass *> *seznamBusu, int time, MyScene *scene);
};



#endif // LINKYCLASS_H
