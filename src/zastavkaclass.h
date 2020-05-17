
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

#ifndef ZASTAVKACLASS_H
#define ZASTAVKACLASS_H

#include <QObject>
#include <QWidget>
#include <QGraphicsRectItem>

class MyScene;

class zastavkaClass
{
public:
    /**
     * @brief X X-ová súradnica objektu zastávky
     */
    int X;
    /**
     * @brief Y Y-ová súradnica objektu zastávky
     */
    int Y;
    /**
     * @brief nazovZastavky názov zastávky
     */
    QString nazovZastavky;
    /**
     * @brief ID_zastavky ID zastávky
     */
    int ID_zastavky;
    /**
     * @brief zastavkaItem grafická reprezentácia zastávky
     */
    QGraphicsRectItem *zastavkaItem;
    zastavkaClass(int startX, int startY,QString nazov, int ID);
};

#endif // ZASTAVKACLASS_H
