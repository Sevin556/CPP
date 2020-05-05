#include "myscene.h"
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

MyScene::MyScene(QObject *parent):
    QGraphicsScene(parent)
{

}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() <<items(event->scenePos());
    QList<QGraphicsItem*>  temp = items(event->scenePos());
    if (temp.size()>0){
        temp[0]->hide();
    }
    QGraphicsScene::mousePressEvent(event);
}
