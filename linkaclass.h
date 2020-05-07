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
    linkaClass();
    void appendBus(QList<autobusClass *> *seznamBusu, QMap<int, ulicaClass *> *seznamUlic, QMap<int, zastavkaClass *> seznamZastavek, int time, MyScene *scene);
    void setTime(QList<autobusClass *> *seznamBusu, QMap<int, ulicaClass *> *seznamUlic, QMap<int, zastavkaClass *> seznamZastavek, int time, MyScene *scene);
private:
    QVector<QPair<QString, int>> seznamLinek;
};

#endif // LINKYCLASS_H
