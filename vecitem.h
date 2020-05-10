#ifndef VECITEM_H
#define VECITEM_H

#include <QString>
#include <QVector>
#include "bodylinky.h"

class vecItem
{
public:
    vecItem();

        QString nazovLinky;
        QString suborTrasy;
        QVector<int> zoznamOdchodov;
        bodyLinky* trasaLinky;

};

#endif // VECITEM_H
