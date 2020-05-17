
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

#ifndef VECITEM_H
#define VECITEM_H

#include <QString>
#include <QVector>
#include "bodylinky.h"

class vecItem
{
public:
    vecItem();

    /**
     * @brief nazovLinky názov linky
     */
    QString nazovLinky;
    /**
     * @brief suborTrasy názov súboru odkiaľ sa náčíta trasa linky
     */
    QString suborTrasy;
    /**
     * @brief zoznamOdchodov zoznam začiatkov jazdy na linke
     */
    QVector<int> zoznamOdchodov;
    /**
     * @brief trasaLinky trasa linky
     */
    bodyLinky* trasaLinky;

};

#endif // VECITEM_H
