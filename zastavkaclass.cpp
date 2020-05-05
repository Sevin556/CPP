#include "zastavkaclass.h"
#include "myscene.h"

zastavkaClass::zastavkaClass(int startX, int startY, QString nazov, int ID,MyScene *scene)
{

    X = startX;
    Y = startY;
    nazovZastavky = nazov;
    ID_zastavky = ID;
    zastavkaItem = scene->addRect(X-10,Y-7,20,15,QPen(4),QBrush(Qt::red));
}
