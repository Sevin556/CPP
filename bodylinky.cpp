#include "bodylinky.h"
#include <QDebug>
#include <QPointF>
#include <QFile>
#include "ulicaclass.h"
#include "zastavkaclass.h"
#include <QTime>

bodyLinky::bodyLinky(QMap<int,ulicaClass*> *zoznamUlic,QMap<int,zastavkaClass*>zoznamZastavok,QString subor)
{

    QFile file(subor);
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
    //zaciatokX = splitedLine[0].toInt()-10;
    //zaciatokY = splitedLine[1].toInt()-10;
    auto * zastavka = zoznamZastavok.value(splitedLine[1].toInt());
    zaciatokX = zastavka->X -10;
    zaciatokY = zastavka->Y -10;
    //pridani zastavky do seznamu zastavek, kterymi projede bus
    zastavkyNaLince->append(qMakePair(zastavka, (splitedLine[2].toInt()) % 86400));
    qDebug() << "prvni zastavka - cas: " << zastavkyNaLince->value(0).second;
    //tu treba priratat to co sa odcitalo aby sedeli stredy bodov a cesty
    ulicaClass * prvaUlica;

   // qDebug() <<zaciatokX <<zaciatokY;
   // qDebug() <<zaciatokX <<zaciatokY;
    line = instream.readLine(50);
    //nacitanie vsetkych ulic zo zoznamu a ulozenie do QList odkial sa budu brat pri hladani dalsieho bodu
    int i =0;
    while (line != nullptr){
        if (line[0] == '#'){
            line = instream.readLine(50);
            continue;
        }
        QStringList splitedLine = line.split(" ");
        qDebug() << splitedLine.size();

       // qDebug() <<splitedLine.size()<<"index :"<<i;
        if (splitedLine.size() ==1){ // bod nacitavam z ulice
            auto * ulica = zoznamUlic->value(splitedLine[0].toInt());
            if (i == 0){
                prvaUlica = ulica;
                qDebug() <<"prva ulica ";
               /* if (zaciatokX+10 == ulica->x1 && (zaciatokY+10 == ulica->y1)){
                    bodyPohybu->insert(i,QPoint(ulica->x2,ulica->y2));

                }else {
                    bodyPohybu->insert(i,QPoint(ulica->x1,ulica->y1));
                }*/
            }else {
                if (i >= 2){
                    qDebug() << "dalsi ulice";
                    if ((bodyPohybu->value(i-1).x() == ulica->x1 && bodyPohybu->value(i-1).y() == ulica->y1) ||
                            (bodyPohybu->value(i-2).x() == ulica->x1 && bodyPohybu->value(i-2).y() == ulica->y1) ){
                        bodyPohybu->insert(i,QPoint(ulica->x2,ulica->y2));
                    }else {
                        bodyPohybu->insert(i,QPoint(ulica->x1,ulica->y1));
                    }
                }else {//druha ulica...nemoze byt i-2
                    qDebug() << "Idem na druu ulicu";
                    if (prvaUlica->x1 == ulica->x1 && prvaUlica->y1 == ulica->y1){
                        bodyPohybu->insert(i-1,QPoint(prvaUlica->x1,prvaUlica->y1));
                        bodyPohybu->insert(i,QPoint(ulica->x2,ulica->y2));
                    } else if (prvaUlica->x1 == ulica->x2 && prvaUlica->y1 == ulica->y2){
                        bodyPohybu->insert(i-1,QPoint(prvaUlica->x1,prvaUlica->y1));
                        bodyPohybu->insert(i,QPoint(ulica->x1,ulica->y1));

                    } else if (prvaUlica->x2 == ulica->x1 && prvaUlica->y2 == ulica->y1){
                        bodyPohybu->insert(i-1,QPoint(prvaUlica->x2,prvaUlica->y2));
                        bodyPohybu->insert(i,QPoint(ulica->x2,ulica->y2));
                    } else {
                        bodyPohybu->insert(i-1,QPoint(prvaUlica->x2,prvaUlica->y2));
                        bodyPohybu->insert(i,QPoint(ulica->x1,ulica->y1));
                    }

                    /*if (bodyPohybu->value(i-1).x() == ulica->x1 && bodyPohybu->value(i-1).y() == ulica->y1){
                        bodyPohybu->insert(i,QPoint(ulica->x2,ulica->y2));
                    }else {
                        bodyPohybu->insert(i,QPoint(ulica->x1,ulica->y1));
                    }*/
                }

            }

            //bude brat rychlost premavky z tade
            zoznamUlicLinky->append(ulica);

        }else { // bod nacitavam zo zastavky

            if(zoznamZastavok.contains(splitedLine[1].toInt())){
                auto * zastavka = zoznamZastavok[splitedLine[1].toInt()];
                bodyPohybu->insert(i,QPoint(zastavka->X,zastavka->Y));
                //pridani zastavky do seznamu zastavek, kterymi projede bus
                zastavkyNaLince->append(qMakePair(zastavka, (splitedLine[2].toInt()) % 86400));
            }
            else{
                qDebug() << "zastavka s id "<< splitedLine[1].toInt() << "nenalezena";
            }
        }
        line = instream.readLine(50);
        i++;
    }
    file.close();
}
