
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

#ifndef ULICACLASS_H
#define ULICACLASS_H

#include <QWidget>
#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QObject>
#include <QWidget>
class ulicaClass :public QObject ,QGraphicsLineItem
{
    Q_OBJECT
public:

    ulicaClass(int startX, int startY, int endX, int endY, QString nazov, int ID);
    /**
     * @brief x1 súradnica x1
     */
    int x1;
    /**
     * @brief y1 súradnica y1
     */
    int y1;
    /**
     * @brief x2 súradnica x2
     */
    int x2;
    /**
     * @brief y2 súradnica y2
     */
    int y2;
    /**
     * @brief nazovUlice názov ulice
     */
    QString nazovUlice;
    /**
     * @brief ID_ulice ID ulice
     */
    int ID_ulice;
    /**
     * @brief ulicaItem grafická reprezentácia ulice
     */
    QGraphicsLineItem * ulicaItem;
    /**
     * @brief rychlostPremavky rýchlosť premávky na ulici
     */
    int rychlostPremavky;


private:

protected:
};

#endif // ULICACLASS_H
