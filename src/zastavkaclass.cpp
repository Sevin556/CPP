
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

#include "zastavkaclass.h"
#include "myscene.h"

/**
 * @brief zastavkaClass::zastavkaClass vytvorí triedu zastávky
 * @details vytvorí grafický objekt, ktorý reálne začína inde ako zadané súradnice, no je vykreslený aby stred ležal na ulici
 * @param startX X-sová súradnica ľavého horného rohu objektu na ulici
 * @param startY Y-ová súradnica ľavého horného rohu objektu na ulici
 * @param nazov názov zastávky
 * @param ID ID zastávky
 */
zastavkaClass::zastavkaClass(int startX, int startY, QString nazov, int ID)
{

    X = startX;
    Y = startY;
    nazovZastavky = nazov;
    ID_zastavky = ID;
    zastavkaItem = new QGraphicsRectItem(X-10,Y-7,20,15);
    zastavkaItem->setPen(QPen(Qt::black,2));
    zastavkaItem->setBrush(QBrush(Qt::red));
}
