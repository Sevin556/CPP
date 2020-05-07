#include "autobusclass.h"
#include "myscene.h"
#include <QPainter>
#include <QDebug>
#include <QPointF>
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
autobusClass::autobusClass(QMap<int,ulicaClass*> *zoznamUlic,QMap<int,zastavkaClass*>zoznamZastavok, QString linka, int time, QObject * parent):
    QObject(parent)
{
   //scene = parentScene;

	if (linka == nullptr){
		linka = "linka.txt";
	}
    QFile file(linka);
    //zoznamUlicMesta = *zoznamUlic;
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "error opening file: " << file.error();
        return;
    }

    QTextStream instream(&file);
    QString line = instream.readLine(50);

    QStringList splitedLine= line.split(" ");


    //-10 kvoli tomu aby bol v strede cesty, kedze objekt ma 20x20 pixelov
    zaciatokTrasyX = splitedLine[0].toInt()-10;
    zaciatokTrasyY = splitedLine[1].toInt()-10;
    autobusItem = createBus();
    //tu treba priratat to co sa odcitalo aby sedeli stredy bodov a cesty
    aktualnaPozicia.setX(zaciatokTrasyX+10);
    aktualnaPozicia.setY(zaciatokTrasyY+10);

   // qDebug() <<zaciatokTrasyX <<zaciatokTrasyY;
   // qDebug() <<zaciatokTrasyX <<zaciatokTrasyY;
    line = instream.readLine(50);
    //nacitanie vsetkych ulic zo zoznamu a ulozenie do QList odkial sa budu brat pri hladani dalsieho bodu
    int i =0;
    while (line != nullptr){
        if (line[0] == '#'){
            line = instream.readLine(50);
            continue;
        }
        QStringList splitedLine = line.split(" ");

       // qDebug() <<splitedLine.size()<<"index :"<<i;
        if (splitedLine.size() ==1){ // bod nacitavam z ulice
            auto * ulica = zoznamUlic->value(splitedLine[0].toInt());
            if (i == 0){
                if (zaciatokTrasyX+10 == ulica->x1 && (zaciatokTrasyY+10 == ulica->y1)){
                    bodyPohybu.insert(i,QPoint(ulica->x2,ulica->y2));
                }else {
                    bodyPohybu.insert(i,QPoint(ulica->x1,ulica->y1));
                }
            }else {
                if (i > 2){

                    if ((bodyPohybu[i-1].x() == ulica->x1 && bodyPohybu[i-1].y() == ulica->y1) ||
                            (bodyPohybu[i-2].x() == ulica->x1 && bodyPohybu[i-2].y() == ulica->y1) ){
                        bodyPohybu.insert(i,QPoint(ulica->x2,ulica->y2));
                    }else {
                        bodyPohybu.insert(i,QPoint(ulica->x1,ulica->y1));
                    }
                }else {//druha ulica...nemoze byt i-2
                    if (bodyPohybu[i-1].x() == ulica->x1 && bodyPohybu[i-1].y() == ulica->y1){
                        bodyPohybu.insert(i,QPoint(ulica->x2,ulica->y2));
                    }else {
                        bodyPohybu.insert(i,QPoint(ulica->x1,ulica->y1));
                    }
                }

            }
            //bude brat rychlost premavky z tade
            zoznamUlicLinky.append(ulica);

        }else { // bod nacitavam zo zastavky
            auto * zastavka = zoznamZastavok.value(splitedLine[1].toInt());
            bodyPohybu.insert(i,QPoint(zastavka->X,zastavka->Y));
            //pridani zastavky do seznamu zastavek, kterymi projede bus
            zastavkyNaLince.append(qMakePair(zastavka, (time + splitedLine[2].toInt()) % 86400));
        }
        line = instream.readLine(50);
        i++;
    }
    //aby splnilo podmienku v dalsom kroku
    dalsiBod = aktualnaPozicia;
    file.close();
}

/**
* Vytvorenie obejektu autobusu na scene
* @param scena na ktorej má byť objekt vytvorený
* @return grafický objekt autobusu na scéne
*/
QGraphicsEllipseItem *autobusClass::createBus()
{
    auto * bus = new QGraphicsEllipseItem(zaciatokTrasyX,zaciatokTrasyY,20,20);
    bus->setBrush(Qt::green);
    bus->setPen(QPen(2));
    return bus;
}

void autobusClass::posunAutobus()
{
    autobusItem->moveBy(1*koeficientX,1*koeficientY);
    aktualnaPozicia.setX(autobusItem->scenePos().x()+zaciatokTrasyX+10);
    aktualnaPozicia.setY(autobusItem->scenePos().y()+zaciatokTrasyY+10);
    //qDebug() << autobusItem->scenePos() <<aktualnaPozicia << dalsiBod;

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
    qDebug() << "index: " << index << zoznamUlicLinky.size();
    if (index >= bodyPohybu.size() || index >= zoznamUlicLinky.size()){
        autobusItem->hide();
        return 1;
    }
   // int temp = zoznamUlicLinky[index];
   // qDebug() <<"ID je :"<< zoznamUlicMesta.value(temp)->ID_ulice << "a I je :" << i;
    dalsiBod.setX(bodyPohybu.value(index).x());
    dalsiBod.setY(bodyPohybu.value(index).y());

    //rychlost premavyk na zadanej ceste
    //--------------------Dorobit aby to ignorovalo zastavky a teda nacitalo dalsiu cestu dopredu
    int premavka = zoznamUlicLinky.value(index)->rychlostPremavky;
    //pocitanie koeficientu na postup k dalsiemu bodu pod istym uhlom
    qreal trasa = qPow(dalsiBod.x()-aktualnaPozicia.x(),2) + qPow(dalsiBod.y()-aktualnaPozicia.y(),2);
    trasa = qSqrt(trasa) ;
    koeficientX = (dalsiBod.x()-aktualnaPozicia.x())/(trasa*premavka);
    koeficientY =  (dalsiBod.y()-aktualnaPozicia.y())/(trasa*premavka);
    qDebug() <<dalsiBod.x() << aktualnaPozicia.x();
    //qDebug() <<zoznamUlicMesta.value(temp)->x2-aktualnaPozicia.x();
    //qDebug() <<zoznamUlicMesta.value(temp)->y2-aktualnaPozicia.y();
    qDebug() <<"\n PREMAVKA JE :" <<premavka << index <<zoznamUlicLinky.value(index)->ID_ulice;

    //vykonajTrasu(vzdialenostX,vzdialenostY);
    return 0;

}
/**
* Volana timerom, vola vykonanie pohybu alebo vypocet noveho cieloveho bodu
* returnuje 1 ak je autobus v cielovom bode
*/
int autobusClass::vykonajTrasu(int time)
{
    // cekani na zastavce
    QPoint pozice = aktualnaPozicia.toPoint();
    for(int i = 0; i < zastavkyNaLince.size(); i++){// projde vsechny zastavky
        // je na zastavce:
        if(pozice.x() == zastavkyNaLince[i].first->X && pozice.y() == zastavkyNaLince[i].first->Y){
            //ceka (pokud nema zpozdeni)
            if(time < zastavkyNaLince[i].second){
                return 0;
            }
        }
    }
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
