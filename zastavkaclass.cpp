#include "zastavkaclass.h"
#include "myscene.h"

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
