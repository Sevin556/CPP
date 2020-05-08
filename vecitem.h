#ifndef VECITEM_H
#define VECITEM_H

#include "bodylinky.h"
#include <QString>


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
