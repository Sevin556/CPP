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
//    QMap<int, zastavkaClass *> zoznamZastavok;
    QMap<int, ulicaClass*> zoznamUlic;
    QMap<int, zastavkaClass*> zoznamZastavok;
    QList<autobusClass*> *zoznamAutobusov;

    bool klikamObchadzku = false;
    bool uzatvaramCesty = false;
    int indexPridavanejUlice =0;
    vecItem* menenaLinka;
  //  QMap<int, autobusClass> zoznamAutobusov;

    //QMainWindow * main;
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
