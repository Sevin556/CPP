
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

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
* @brief MyScene::addInfo Prida do triedy informacie o jej objektoch, ktore neboli dostupne pri jej inicializacii
* @param zoznamUlic zoznam tried ulic vykreslenych na scene
* @param zoznamZastavok tried zastavok vykreslenych na scene
* @param zoznamAutobusov autobusom, ktore su momentalne vykreslene na scene
*/
void MyScene::addInfo(QMap<int, ulicaClass*> zoznamUlic,QMap<int, zastavkaClass*>zoznamZastavok,QList<autobusClass*> *zoznamAutobusov)
{
    this->zoznamUlic = zoznamUlic;
    this->zoznamZastavok = zoznamZastavok;
    this->zoznamAutobusov = zoznamAutobusov;
}

/**
*@brief MyScene::mousePressEvent spracuje mouse event a rozhodne kam ho ďalej preposlať
*@details zistí či sa kliklo na nejaký objekt scény, či je mód pridávania ulíc do obchádzky zapnutý, preiteruje zoznamy objektov na scéne a
*  ak sa rovná pointer kliknutého a v pointer zozname tak ho pošle signálom dalšej funckii na spracovanie. Nakoniec prepošle event do QGraphicsScene::mousePressEvent
*@param event ukazateľ na mouse event, ktorý vyvolal signál
*/
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
                }

                QGraphicsScene::mousePressEvent(event);
                return;
            }
        }
        for (int i = 0 ;i < zoznamZastavok.size();i++){
            if (temp[0] == zoznamZastavok.value(i)->zastavkaItem && event->button() == Qt::RightButton){

            }
        }
    }else if (!klikamObchadzku) {
        for (QMap<int,ulicaClass*>::const_iterator i = zoznamUlic.constBegin();i !=zoznamUlic.constEnd();++i){
            i.value()->ulicaItem->setPen(QPen(Qt::black,4));
        }
    }


    //ak je temp nenulove znamena ze nasiel nejake itemy, ak ich je viac tak pouziva ten najvrchensi (posledny vykresleny)
    if (temp.size()>0){
        for (QMap<int,ulicaClass*>::const_iterator i = zoznamUlic.constBegin();i !=zoznamUlic.constEnd();++i){
            if (temp[0]== i.value()->ulicaItem){
                qDebug() <<"ID ULICE JE :" <<i.value()->ID_ulice<<i.value()->nazovUlice;
                if (event->button()==Qt::RightButton){
                    rightClick(i.value());
                }else{
                    i.value()->ulicaItem->setPen(QPen(Qt::green,5));
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
