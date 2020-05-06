#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>
#include <ulicaclass.h>
#include "zastavkaclass.h"
#include "autobusclass.h"

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

  //  QMap<int, autobusClass> zoznamAutobusov;

    //QMainWindow * main;
    void addInfo(QMap<int,ulicaClass*> zoznamUlic,QMap<int, zastavkaClass*>zoznamZastavok,QList<autobusClass*> *zoznamAutobusov);
    void zobrazInfo(QGraphicsItem *item);

signals:
    void infoZmeneneUlica(ulicaClass* ulica);
    void infoZmeneneZastavka(zastavkaClass *zastavka);
    void infoZmeneneAutobus(autobusClass* autobus);
    void zobrazModal(autobusClass*);

private slots:


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // MYSCENE_H
