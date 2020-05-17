
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <ulicaclass.h>
#include "zastavkaclass.h"
#include "autobusclass.h"
#include "vecitem.h"

class MyScene;

class ulicaClass;
class zastavkaClass;
class autobusClass;

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QObject *parent =nullptr);

    /**
     * @brief zoznamUlic zoznam ukazateľov všetkých ulíc na scéne
     */
    QMap<int, ulicaClass*> zoznamUlic;
    /**
     * @brief zoznamZastavok zoznam ukazateľov všetkých ulíc na scéne
     */
    QMap<int, zastavkaClass*> zoznamZastavok;
    /**
     * @brief zoznamAutobusov zoznam ukazateľov všetkých autobusov
     */
    QList<autobusClass*> *zoznamAutobusov;

    /**
     * @brief klikamObchadzku ak je @value true dovoľuje klikať obchádzku
     */
    bool klikamObchadzku = false;

    /**
     * @brief indexPridavanejUlice index na ktorým pridám novokliknutú ulicu do linky @see myscene::menenaLinka
     */
    int indexPridavanejUlice =0;
    /**
     * @brief menenaLinka informácie o aktuálne menenej linke
     */
    vecItem* menenaLinka;


    //funkcie
    void addInfo(QMap<int,ulicaClass*> zoznamUlic,QMap<int, zastavkaClass*>zoznamZastavok,QList<autobusClass*> *zoznamAutobusov);
    void zobrazInfo(QGraphicsItem *item);

signals:
    void infoZmeneneUlica(ulicaClass* ulica);
    void infoZmeneneZastavka(zastavkaClass *zastavka);
    void infoZmeneneAutobus(autobusClass* autobus);
    void zobrazModal(autobusClass*);
    void rightClick(ulicaClass*);
    void pridajUlicuDoLinky(int index,vecItem* linka,ulicaClass*ulica);
    void odoberUlicu(vecItem* linka,ulicaClass * ulica);
    void odoberZastavku(vecItem* linka,zastavkaClass *zastavka);

private slots:


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MYSCENE_H
