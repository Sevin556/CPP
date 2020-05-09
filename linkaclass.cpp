#include "linkaclass.h"

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "bodylinky.h"
#include <QVector>
#include <QMap>


linkaClass::linkaClass(QMap<int, ulicaClass *> *seznamUlic, QMap<int, zastavkaClass *> seznamZastavek)
{

    // nacteni linek ze souboru
    QFile file("linky-seznam.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "error opening file: " << file.error();
        return;
    }

    QTextStream instream(&file);
    QString line = instream.readLine(50);
    while (line != nullptr){
        if (line[0] == '#'){
            line = instream.readLine(50);
            continue;
        }
        qDebug() <<line;

        //    QVector<QPair<QString,QPair<bodyLinky*,QList<int>>>> seznamLinek;
        QStringList splitedLine= line.split(" ");
        // nazev souboru s trasou linky
        bool najdene = false;

        for (int i =0; i<seznamLinek.size();i++){
            if (seznamLinek[i]->nazovLinky==splitedLine[0]){
                najdene = true;
                if (seznamLinek[i]->suborTrasy != splitedLine[1]){
                    qDebug() << "chybne zadany subor--linka uz je definovana z ineho suboru";
                    exit(1);
                }
                QStringList time = splitedLine[2].split(":");
                int cas = time[0].toInt() * 3600; //hodiny
                cas += time[1].toInt() * 60; //minuty
                cas += time[2].toInt(); //sekundy
                seznamLinek[i]->zoznamOdchodov.append(cas);
            }
        }
        if (najdene){
            line = instream.readLine(50);
            continue;
        }

        vecItem *item =new vecItem();
        item->nazovLinky =  splitedLine[0];
        item->suborTrasy = splitedLine[1];
        // cas zahajeni cesty

        item->trasaLinky = new bodyLinky(seznamUlic,seznamZastavek,item->suborTrasy);
        QStringList time = splitedLine[2].split(":");
        int cas = time[0].toInt() * 3600; //hodiny
        cas += time[1].toInt() * 60; //minuty
        cas += time[2].toInt(); //sekundy
        item->zoznamOdchodov.append(cas);
        seznamLinek.append(item);

        line = instream.readLine(50);
    }
    qDebug() <<"skoncil som";
    file.close();
    //sluzi na lahsie identifikovanie autobusov
    int indexAutobusu = 0;
    for (int i = 0; i < seznamLinek.size(); i++){
        for(int j = 0; j < seznamLinek[i]->zoznamOdchodov.size();j++){
            autobusClass *autobus =  new autobusClass( seznamLinek[i]->trasaLinky, seznamLinek[i]->zoznamOdchodov[j],indexAutobusu++, nullptr);
            busList.insert(indexAutobusu,autobus);

        }

    }

    return;

}

linkaClass::~linkaClass()
{
    for(int i = 0; i < busList.size(); i++)
    {
        delete busList[i];
    }
}
/**
 * @brief linkaClass::appendBus pridani autobusu, pokud je cas jeho vyjezdu
 * @param seznamBusu seznam autobusu
 * @param time aktualni cas
 * @param scene scena
 */
void linkaClass::appendBus(QList<autobusClass *> *seznamBusu, int time, MyScene *scene)
{
    int indexAutobusu = 0;
    for (int i = 0; i < seznamLinek.size(); i++){
        for(int j = 0; j < seznamLinek[i]->zoznamOdchodov.size();j++){
            indexAutobusu++;

            if(seznamLinek[i]->zoznamOdchodov[j] == time && busList.find(indexAutobusu) != busList.end()){
                autobusClass *autobus = busList[indexAutobusu];
                scene->addItem(autobus->autobusItem);
                seznamBusu->append(autobus);

            }
        }

    }
}
/**
 * @brief linkaClass::setTime spusteni simulace do nastaveneho casu
 * @param seznamBusu seznam aktivnich autobusu
 * @param time aktualni cas (vnitrni hodiny)
 * @param scene scena, na kterou se autobus zobrazi
 */
void linkaClass::setTime(QList<autobusClass *> *seznamBusu, int time, MyScene *scene){

    int casLinky;
    autobusClass * autobus;

    int indexAutobusu=0;
    for (int i = 0; i < seznamLinek.size(); i++){
        for(int j = 0; j < seznamLinek[i]->zoznamOdchodov.size();j++){
            indexAutobusu++;

            casLinky = seznamLinek[i]->zoznamOdchodov[j];
            autobus = busList[indexAutobusu];
            while(autobus != nullptr && casLinky != time){

                if(autobus->vykonajTrasu(casLinky) == 1){
                    autobus->index = 0;
                    autobus = nullptr;
                }
                casLinky = (casLinky+1)%86400;

            }
            if(autobus != nullptr){
                autobus->autobusItem->show();
                scene->addItem(autobus->autobusItem);
                seznamBusu->append(autobus);
            }
        }

    }
}
