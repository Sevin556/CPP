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
#include <QtMath>
/**
* Konstruktor triedy
* @param scena na ktorej ma byt zobrazený grafický objekt-autobus
* @param zoznam ulic mesta, aby bolo možné načítavať body z nich
* @param objekt z ktorého sa dedia signaly
*/
autobusClass::autobusClass(QGraphicsScene *parentScene,QMap<int,ulicaClass*> *zoznamUlic,QObject * parent):
    QObject(parent)
{
   //scene = parentScene;

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
    //-10 kvoli tomu aby bol v strede cesty, kedze objekt ma 20x20 pixelov
    zaciatokTrasyX = splitedLine[0].toInt()-10;
    zaciatokTrasyY = splitedLine[1].toInt()-10;
    autobusItem = createBus(parentScene);
    //tu treba priratat to co sa odcitalo aby sedeli stredy bodov a cesty
    aktualnaPozicia.setX(zaciatokTrasyX+10);
    aktualnaPozicia.setY(zaciatokTrasyY+10);
    qDebug() <<zaciatokTrasyX <<zaciatokTrasyY;
    qDebug() <<zaciatokTrasyX <<zaciatokTrasyY;
    line = instream.readLine(50);
    //nacitanie vsetkych ulic zo zoznamu a ulozenie do QList odkial sa budu brat pri hladani dalsieho bodu
    int i =0;
    while (line != nullptr){

        zoznamUlicLinky.insert(i,line.toInt());
        line = instream.readLine(50);
        i++;
    }

    pocitajTrasu();
    file.close();
}

/**
* Vytvorenie obejektu autobusu na scene
* @param scena na ktorej má byť objekt vytvorený
* @return grafický objekt autobusu na scéne
*/
QGraphicsEllipseItem *autobusClass::createBus(QGraphicsScene *scene)
{
    auto * bus = scene->addEllipse(zaciatokTrasyX,zaciatokTrasyY,20,20,QPen(Qt::black,2),QBrush(Qt::green));
    scene->update();
    return bus;
}

void autobusClass::posunAutobus()
{
    autobusItem->moveBy(1*koeficientX,1*koeficientY);
    aktualnaPozicia.setX(autobusItem->scenePos().x()+zaciatokTrasyX+10);
    aktualnaPozicia.setY(autobusItem->scenePos().y()+zaciatokTrasyY+10);
    qDebug() << autobusItem->scenePos() <<aktualnaPozicia << dalsiBod;

}

/**
* Vypocita trasu do nasledujuceho bodu
* Spravi pytagorovu vetu z ktorej zisti dlzku prepony a teda trasy do bodu
* a tymto vydeli vziadialenost X a Y aby urcil koeficient presunov
*  returnuje 1 ak nie je dalsi bod kam ist a teda jazda skoncila
*
* treba dorobit zastavky a zastavenie v nich
*/
int autobusClass::pocitajTrasu()
{
    //static int index = 0;
    qDebug() << index << zoznamUlicLinky.size();
    if (index == zoznamUlicLinky.size()){
        autobusItem->hide();
        return 1;
    }
    int temp = zoznamUlicLinky[index];
   // qDebug() <<"ID je :"<< zoznamUlicMesta.value(temp)->ID_ulice << "a I je :" << i;
    dalsiBod.setX(zoznamUlicMesta.value(temp)->x2);
    dalsiBod.setY(zoznamUlicMesta.value(temp)->y2) ;

    //pocitanie koeficientu na postup k dalsiemu bodu pod istym uhlom
    qreal trasa = (zoznamUlicMesta.value(temp)->x2-aktualnaPozicia.x())*(zoznamUlicMesta.value(temp)->x2-aktualnaPozicia.x()) + (zoznamUlicMesta.value(temp)->y2-aktualnaPozicia.y())*(zoznamUlicMesta.value(temp)->y2-aktualnaPozicia.y());
    trasa = qSqrt(trasa) ;
    koeficientX = (zoznamUlicMesta.value(temp)->x2-aktualnaPozicia.x())/trasa;
    koeficientY =  (zoznamUlicMesta.value(temp)->y2-aktualnaPozicia.y())/trasa;

    qDebug() <<zoznamUlicMesta.value(temp)->x2-aktualnaPozicia.x();
    qDebug() <<zoznamUlicMesta.value(temp)->y2-aktualnaPozicia.y();
    qDebug() <<trasa << koeficientX << koeficientY;

    //vykonajTrasu(vzdialenostX,vzdialenostY);
    return 0;

}
/**
* Volana timerom, vola vykonanie pohybu alebo vypocet noveho cieloveho bodu
* returnuje 1 ak je autobus v cielovom bode
*/
int autobusClass::vykonajTrasu()
{
    //mozno staci len bod, preistotu je to takto zatial
    if (!(aktualnaPozicia.x() <= dalsiBod.x()+0.5 && aktualnaPozicia.x() >= dalsiBod.x()-0.5  && aktualnaPozicia.y() <= dalsiBod.y()+0.5 && aktualnaPozicia.y() >= dalsiBod.y()-0.5)){
        posunAutobus();
    }else{

        if (pocitajTrasu() == 1){
            return 1;
        }

        index++;
   }

    return 0;
}
