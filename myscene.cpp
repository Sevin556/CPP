#include "myscene.h"
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include "ulicaclass.h"
#include <QDialogButtonBox>
#include <QDialog>
#include "linkaclass.h"

MyScene::MyScene(QObject *parent):
    QGraphicsScene(parent)
{
}

/**
* Prida do triedy informacie o jej objektoch, ktore neboli dostupne pri jej inicializacii
* @param zoznam tried ulic vykreslenych na scene
* @param zoznam tried zastavok vykreslenych na scene
* @param zoznam autobusom, ktore su momentalne vykreslene na scene
*/
void MyScene::addInfo(QMap<int, ulicaClass*> zoznamUlic,QMap<int, zastavkaClass*>zoznamZastavok,QList<autobusClass*> *zoznamAutobusov)
{
    this->zoznamUlic = zoznamUlic;
    this->zoznamZastavok = zoznamZastavok;
    this->zoznamAutobusov = zoznamAutobusov;
}


//preiteruje zoznamy objektov na scene a ak sa rovna pointer kliknuteho a v zozname tak ho posle signalom dalsim funckiam
void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() <<items(event->scenePos());
    qDebug() <<event->scenePos();
    //skusi nacitat itemy na kliknutej pozicii na scene
    QList<QGraphicsItem*>  temp = items(event->scenePos());



    if (klikamObchadzku && temp.size() >0){
        for (QMap<int,ulicaClass*>::const_iterator i = zoznamUlic.constBegin();i !=zoznamUlic.constEnd();++i){
            if (temp[0]== i.value()->ulicaItem){
                if (event->button() == Qt::RightButton){
                   odoberUlicu(menenaLinka,i.value());
                }else{
                    pridajUlicuDoLinky(indexPridavanejUlice,menenaLinka,i.value());
                    indexPridavanejUlice++;
                }

                QGraphicsScene::mousePressEvent(event);
                return;
            }
        }
    }else if (!klikamObchadzku) {
        for (QMap<int,ulicaClass*>::const_iterator i = zoznamUlic.constBegin();i !=zoznamUlic.constEnd();++i){
            i.value()->ulicaItem->setPen(QPen(Qt::black,4));
        }
    }


    //ak je temp nenulove znamena ze nasiel nejake itemy, ak ich je viac tak pouziva ten najvrchensi (posledny vykresleny)
    if (temp.size()>0){
        qDebug()<<"idem porovnavat";
        for (QMap<int,ulicaClass*>::const_iterator i = zoznamUlic.constBegin();i !=zoznamUlic.constEnd();++i){
            if (temp[0]== i.value()->ulicaItem){
                qDebug() <<"ID ULICE JE :" <<i.value()->ID_ulice<<i.value()->nazovUlice;
                if (event->button()==Qt::RightButton){
                    rightClick(i.value());
                }else{
                    infoZmeneneUlica(i.value());
                }
                QGraphicsScene::mousePressEvent(event);
                return;
            }
        }

        for (QMap<int,zastavkaClass*>::const_iterator i = zoznamZastavok.constBegin();i !=zoznamZastavok.constEnd();++i){
            if (temp[0]== i.value()->zastavkaItem){
                qDebug() <<"ID zastavkyJE :" <<i.value()->ID_zastavky<<i.value()->nazovZastavky;
                infoZmeneneZastavka(i.value());

                QGraphicsScene::mousePressEvent(event);
                return;
            }
        }
        for (int i = 0;i<zoznamAutobusov->size();i++){
            if(zoznamAutobusov->value(i) != nullptr){
                if (temp[0]== zoznamAutobusov->value(i)->autobusItem){
                    qDebug() <<"Autobus kliknuty";
                    infoZmeneneAutobus(zoznamAutobusov->value(i));
                }
            }
        }
        qDebug() <<"Skoncil som";
    }

    QGraphicsScene::mousePressEvent(event);
}
