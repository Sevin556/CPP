
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */


#ifndef AUTOBUSCLASS_H
#define AUTOBUSCLASS_H

#include <QObject>
#include <QWidget>
#include <QGraphicsEllipseItem>
#include "ulicaclass.h"
#include "zastavkaclass.h"
#include "bodylinky.h"


class zastavkaClass;
class bodyLinky;
class autobusClass: public QObject
{
    Q_OBJECT
public:

    //inicializacia
    /**
     * @brief zaciatokTrasyX počiatočná X-sová súradnica kde sa autobus objaví
     */
    int zaciatokTrasyX;
    /**
     * @brief zaciatokTrasyY počiatočná Y-sová súradnica kde sa autobus objaví
     */
    int zaciatokTrasyY;
    /**
     * @brief poradi index ďaľšej zastávky na linke
     */
    int poradi = 0;

    /**
     * @brief pocatecniCas čas výjazdu autobusu
     */
    int pocatecniCas = 0;
    /**
     * @brief zoznamUlicLinky zoznam ukazateľov na ulice, po ktorých linka jazdí
     */
    QList<ulicaClass*> *zoznamUlicLinky;
    /**
     * @brief MojeID ID autobusu
     */
    int MojeID;
    /**
     * @brief nazovLinky názov linky na ktorej autobus jazdí
     */
    QString nazovLinky;
    /**
     * @brief autobusItem grafická reprezentácia autobusu
     */
    QGraphicsEllipseItem *autobusItem;

    //pohyb
    /**
     * @brief index ďaľšieho bodu v @see bodyPohybu
     */
    int index =0;
    /**
     * @brief stojim na simulovanie státia na zastávke
     */
    int stojim =0;

    /**
     * @brief bodyPohybu zoznam všetkých bodov po ktorých autobus prejde na koniec linky
     */
    QList<QPoint> *bodyPohybu;
    /**
     * @brief aktualnaPozicia aktuálna pozícia autobusu na scéne
     */
    QPointF aktualnaPozicia;
    /**
     * @brief dalsiBod bod do ktorého autobus momentálne smeruje
     */
    QPointF dalsiBod;

    /**
     * @brief koeficientX vypočítaný krok, ktorý sa vykoná pri každom zavolaní funkcie @see autobusClass::posunAutobus()
     */
    qreal koeficientX;
    /**
     * @brief koeficientX vypočítaný krok, ktorý sa vykoná pri každom zavolaní funkcie @see autobusClass::posunAutobus()
     */
    qreal koeficientY;
    /**
     * @brief meskanie aktuálne meškanie autobusu
     */
    int meskanie =0;
    /**
     * @brief meskanieNaZastavke meškanie autobusu na poslednej zastávke
     */
    int meskanieNaZastavke =0;
    /**
     * @brief indexZastavky počet prejdených zastávok
     */
    int indexZastavky =0;
    /**
     * @brief dalsiaZastavka ďaľšia zastávka na trase linky
     */
    QPoint dalsiaZastavka;

    /**
     * @brief zastavkyNaLince zastavky na dane lince + doba, za kterou by mel vyrazit z dane zastavky
     */
    QVector<QPair<zastavkaClass*, int>>*zastavkyNaLince;

    //funkcie
    autobusClass(bodyLinky *linka, int time,int ID,QString nazovLinky, QObject * parent);
    void posunAutobus();
    int pocitajTrasu();
    int vykonajTrasu(int time);
signals:
    void meskanieZmenene(autobusClass* autobus);

private:
    QGraphicsEllipseItem* createBus();


};

#endif // AUTOBUSCLASS_H
