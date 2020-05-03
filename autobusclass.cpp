#include "autobusclass.h"
#include <QPainter>
#include <QDebug>
#include <QPointF>
#include <QGraphicsScene>
#include <QFile>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QObject>
#include <QMouseEvent>

autobusClass::autobusClass(QGraphicsScene *parentScene,QMap<int,ulicaClass*> *zoznamUlic,QObject * parent):
    QObject(parent)
{
   scene = parentScene;

    QFile file("linka.txt");
    zoznamUlicMesta = *zoznamUlic;
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "error opening file: " << file.error();
        return;
    }

    QTextStream instream(&file);
    QString line = instream.readLine(50);

    QStringList splitedLine= line.split(" ");

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(vykonajTrasu()));
    zaciatokTrasyX = splitedLine[0].toInt()-10;
    zaciatokTrasyY = splitedLine[1].toInt()-10;
    autobusItem = createBus(scene);
    line = instream.readLine(50);
    //nacitanie vsetkych ulic zo zoznamu a ulozenie do QList odkial sa budu brat pri cestovani
    int i =0;
    while (line != nullptr){

        zoznamUlicLinky.insert(i,line.toInt());
 /*       zoznamUlicLinky[i].x1=zoznamUlicMesta.value(line.toInt())->x1;
        zoznamUlicLinky[i].y1=zoznamUlicMesta.value(line.toInt())->y1;
        zoznamUlicLinky[i].x2=zoznamUlicMesta.value(line.toInt())->x2;
        zoznamUlicLinky[i].y2=zoznamUlicMesta.value(line.toInt())->y2;
        zoznamUlicLinky[i].nazovUlice=zoznamUlicMesta.value(line.toInt())->nazovUlice;
        zoznamUlicLinky[i].ID_ulice=zoznamUlicMesta.value(line.toInt())->ID_ulice;
        zoznamUlicLinky[i].ulicaItem=zoznamUlicMesta.value(line.toInt())->ulicaItem;
*/
        line = instream.readLine(50);
        i++;
    }
    file.close();
}

QGraphicsEllipseItem *autobusClass::createBus(QGraphicsScene *scene)
{
    auto * bus = scene->addEllipse(zaciatokTrasyX,zaciatokTrasyY,20,20,QPen(Qt::black,2),QBrush(Qt::green));
    scene->update();
    return bus;
}

void autobusClass::updateBus(qreal x,qreal y)
{
    autobusItem->moveBy(x,y);
}

void autobusClass::pocitajTrasu(int index)
{
    //static int index = 0;
    for (int i =index ;i< index+1;i++){
        int temp = zoznamUlicLinky[i];
    qDebug() <<"ID je :"<< zoznamUlicMesta.value(temp)->ID_ulice << "a I je :" << i;
    vzdialenostX = zoznamUlicMesta.value(temp)->x2 - zoznamUlicMesta.value(temp)->x1;
    vzdialenostY = zoznamUlicMesta.value(temp)->y2 - zoznamUlicMesta.value(temp)->y1;

    timer->start(20);
    //vykonajTrasu(vzdialenostX,vzdialenostY);

}
}

void autobusClass::vykonajTrasu()
{
    qDebug() <<"Robim :" <<vzdialenostX;
    qreal castX = vzdialenostX/99;
    qreal castY = vzdialenostY/99;
    updateBus(castX,castY);
        scene->update();


}
