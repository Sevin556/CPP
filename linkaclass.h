#ifndef LINKYCLASS_H
#define LINKYCLASS_H
#include <QObject>
#include "autobusclass.h"
#include "ulicaclass.h"
#include "zastavkaclass.h"
#include "myscene.h"
//#include "vecitem.h"

class vecItem
{
public:
    QString nazovLinky;
    QString suborTrasy;
    QVector<int> zoznamOdchodov;
    bodyLinky* trasaLinky;
};

//class zoznamLiniekClass;

class linkaClass
{
public:
    linkaClass(QMap<int, ulicaClass *> *seznamUlic, QMap<int, zastavkaClass *> seznamZastavek);
    ~linkaClass();

    // list vsech autobusu (aktivnich i neaktivnich)
    QMap<int,autobusClass*> busList;

    void appendBus(QList<autobusClass *> *seznamBusu, int time, MyScene *scene);
    void setTime(QList<autobusClass *> *seznamBusu, int time, MyScene *scene);


    QVector<vecItem*> seznamLinek;

};



#endif // LINKYCLASS_H
