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
#include "bodylinky.h"

/**
 * @brief autobusClass::autobusClass Konštruktor triedy
 * @param linka linka na ktorej bude autobus premávať
 * @param time čas, v ktorý začína autobus jazdu
 * @param ID ID autobusu
 * @param nazovLinky názov linky na ktorej autobus premáva
 * @param parent dedí z neho
 */
autobusClass::autobusClass(bodyLinky* linka, int time,int ID,QString nazovLinky, QObject * parent):
    QObject(parent)
{

    this->nazovLinky = nazovLinky;
    MojeID = ID;
    //-10 kvoli tomu aby bol v strede cesty, kedze objekt ma 20x20 pixelov
    zaciatokTrasyX = linka->zaciatokX;
    zaciatokTrasyY = linka->zaciatokY;
    autobusItem = createBus();
    //tu treba priratat to co sa odcitalo aby sedeli stredy bodov a cesty
    aktualnaPozicia.setX(zaciatokTrasyX+10);
    aktualnaPozicia.setY(zaciatokTrasyY+10);


    //nacitanie vsetkych ulic zo zoznamu a ulozenie do QList odkial sa budu brat pri hladani dalsieho bodu
    bodyPohybu=linka->bodyPohybu;
            //bude brat rychlost premavky z tade
    zoznamUlicLinky= linka->zoznamUlicLinky;

    zastavkyNaLince = linka->zastavkyNaLince;
    pocatecniCas = time;

    //aby splnilo podmienku v dalsom kroku
    dalsiBod = aktualnaPozicia;
    dalsiaZastavka.setX(zastavkyNaLince->value(0).first->X);
    dalsiaZastavka.setY(zastavkyNaLince->value(0).first->Y);

}

/**
 * @brief autobusClass::createBus vytvorí grafický objekt autobusu, ktorý bude vykreslený na scéne
 * @return  grafický objekt autobusu
 */
QGraphicsEllipseItem *autobusClass::createBus()
{
    auto * bus = new QGraphicsEllipseItem(zaciatokTrasyX,zaciatokTrasyY,20,20);
    bus->setBrush(Qt::green);
    bus->setPen(QPen(2));
    return bus;
}

/**
 * @brief autobusClass::posunAutobus posúva grafický objekt autobusu po scéne o koeficient počítany v autobusClass::pocitajTrasu()
 */
void autobusClass::posunAutobus()
{
    autobusItem->moveBy(1*koeficientX,1*koeficientY);
    aktualnaPozicia.setX(autobusItem->scenePos().x()+zaciatokTrasyX+10);
    aktualnaPozicia.setY(autobusItem->scenePos().y()+zaciatokTrasyY+10);

}

/**
* @brief autobusCLass::pocitajTrasu Vypocita trasu do nasledujuceho bodu
* @details Spravi pytagorovu vetu z ktorej zisti dlzku prepony a teda trasy do bodu
* a tymto spolu s rychlostou premavky  vydeli vziadialenost X a Y aby urcil koeficient presunov
*  @return 1 ak nie je dalsi bod kam ist a teda jazda skoncila
*
*/
int autobusClass::pocitajTrasu()
{
    qDebug() << index << bodyPohybu->size();
    // ak uz nie je dalsi bod tak vrati 1
    if (index >= bodyPohybu->size()){
        qDebug() << "here";
        autobusItem->hide();
        index = 0;
        indexZastavky = 0;
        aktualnaPozicia.setX(zaciatokTrasyX+10);
        aktualnaPozicia.setY(zaciatokTrasyY+10);
        dalsiBod = aktualnaPozicia;
        dalsiaZastavka.setX(zastavkyNaLince->value(0).first->X);
        dalsiaZastavka.setY(zastavkyNaLince->value(0).first->Y);
        autobusItem->setPos(QPointF(0, 0));
        return 1;
    }

    //nacita dalsi bod na ktory treba ist
    dalsiBod.setX(bodyPohybu->value(index).x());
    dalsiBod.setY(bodyPohybu->value(index).y());


    bool idemDoZastavky = false;
    for(int i = 0; i < zastavkyNaLince->size(); i++){// projde vsechny zastavky
        // je na zastavce:
        if(dalsiBod.x() == zastavkyNaLince->value(i).first->X && dalsiBod.y() == zastavkyNaLince->value(i).first->Y){
            idemDoZastavky = true;
            qDebug() << "Idem do zastavky";
        }
    }

    //rychlost premavyk na zadanej ceste
    int premavka ;
    if (idemDoZastavky){
        if (index >1){
            // spomalnie berie z predchadzajucej cesty
             premavka = zoznamUlicLinky->value(index-(indexZastavky))->rychlostPremavky;
        }else
            premavka = 1;
    }else{
        if (indexZastavky == 0){
            premavka = zoznamUlicLinky->value(index)->rychlostPremavky;
        }else
        premavka = zoznamUlicLinky->value(index-(indexZastavky-1))->rychlostPremavky;
    }
    qDebug() << "heeeeeereeee";
    //pocitanie koeficientu na postup k dalsiemu bodu pod istym uhlom
    qreal trasa = qPow(dalsiBod.x()-aktualnaPozicia.x(),2) + qPow(dalsiBod.y()-aktualnaPozicia.y(),2);

    if (trasa >0){
        trasa = qSqrt(trasa) ;
        koeficientX = (dalsiBod.x()-aktualnaPozicia.x())/(trasa*premavka);
        koeficientY =  (dalsiBod.y()-aktualnaPozicia.y())/(trasa*premavka);
    }else {
        koeficientX = (dalsiBod.x()-aktualnaPozicia.x())/(premavka);
            koeficientY =  (dalsiBod.y()-aktualnaPozicia.y())/(premavka);

    }

    // existuje dalsi bod pohybu a teda vracia 0
    return 0;

}

/**
 * @brief autobusClass::vykonajTrasu Volana timerom, zistuje co ma autobus vykonat v nasledujucom intervale
 * @details Zistuje ci je autobus na zastavke a ma na nej čakať alebo či treba vypočítať novú trasu, poprípadne počíta meškanie
 * @param time aktuálny čas v programe, počíta pomocou neho meškanie
 * @return returnuje 1 ak je autobus v cielovom bode
 */
int autobusClass::vykonajTrasu(int time)
{

    // cekani na zastavce

    QPoint pozice = aktualnaPozicia.toPoint();
    if(pozice.x() == zastavkyNaLince->value(poradi).first->X && pozice.y() == zastavkyNaLince->value(poradi).first->Y){
        int casMeskania = time - zastavkyNaLince->value(poradi).second + pocatecniCas;
        if (casMeskania >= 0 ){
             meskanieNaZastavke = casMeskania/60;
             meskanie = meskanieNaZastavke;
        }
        if(time < zastavkyNaLince->value(poradi).second + pocatecniCas){
            return 0;
        }else if (stojim < 30){
            stojim++;
            return 0;
        }
        else{
            poradi = (poradi+1)%zastavkyNaLince->size();
        }
    }




    //mozno staci len bod
    if (!(aktualnaPozicia.x() <= dalsiBod.x()+0.5 && aktualnaPozicia.x() >= dalsiBod.x()-0.5  && aktualnaPozicia.y() <= dalsiBod.y()+0.5 && aktualnaPozicia.y() >= dalsiBod.y()-0.5)){
        posunAutobus();
        stojim = 0;
        return 0;
    }else{

        if (pocitajTrasu() == 1){
            return 1;
        }
        index++;
   }

    //pocitanie meskania
    if (dalsiaZastavka.x() == pozice.x() && dalsiaZastavka.y() == pozice.y()){
        indexZastavky++;
        if (indexZastavky >= zastavkyNaLince->size()){
            indexZastavky = 0;
        }else {
            qDebug() <<"nastavujem novu zastavku";
            dalsiaZastavka.setX(zastavkyNaLince->value(indexZastavky).first->X);
            dalsiaZastavka.setY(zastavkyNaLince->value(indexZastavky).first->Y);
        }
    }else {
        int casMeskania = time - zastavkyNaLince->value(indexZastavky).second + pocatecniCas;
        if (casMeskania > 0 && meskanieNaZastavke == 0){
            //meskanie sa vytvorilo az po poslednom zastaveni na zastavke
            meskanie = casMeskania/60;
        }else if (casMeskania > 0 && meskanieNaZastavke >0){
            // meskal uz na zastavke
            meskanie =meskanieNaZastavke + casMeskania/60;
            meskanieZmenene(this);
        }
    }

    return 0;
}
