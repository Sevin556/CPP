#ifndef LINKYCLASS_H
#define LINKYCLASS_H
#include <QObject>
#include "autobusclass.h"
#include "ulicaclass.h"
#include "zastavkaclass.h"
#include "myscene.h"
class linkaClass
{
public:
    linkaClass(QMap<int, ulicaClass *> *seznamUlic, QMap<int, zastavkaClass *> seznamZastavek);

    // list vsech autobusu (aktivnich i neaktivnich)
    QList<autobusClass*> busList;

    void appendBus(QList<autobusClass *> *seznamBusu, int time, MyScene *scene);
    void setTime(QList<autobusClass *> *seznamBusu, int time, MyScene *scene);
    QVector<QPair<QString, int>> seznamLinek;
    void vytvorJizdniRad();
};

#endif // LINKYCLASS_H
