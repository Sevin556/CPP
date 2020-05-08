#include "linkaclass.h"

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>


linkaClass::linkaClass(QMap<int, ulicaClass *> *seznamUlic, QMap<int, zastavkaClass *> seznamZastavek)
{

    // nacteni linek ze souboru
    QFile file("linky-seznam.txt");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "error opening file: " << file.error();
        return;
    }
    QPair<QString, int> pair;
    QTextStream instream(&file);
    QString line = instream.readLine(50);
    while (line != nullptr){
        if (line[0] == '#'){
            line = instream.readLine(50);
            continue;
        }
        QStringList splitedLine= line.split(" ");
        // nazev souboru s trasou linky
        pair.first = splitedLine[0];
        // cas zahajeni cesty
        QStringList time = splitedLine[1].split(":");
        pair.second = time[0].toInt() * 3600; //hodiny
        pair.second += time[1].toInt() * 60; //minuty
        pair.second += time[2].toInt(); //sekundy

        seznamLinek.append(pair);

        line = instream.readLine(50);
    }
    file.close();
    for (int i = 0; i < seznamLinek.size(); i++){
        autobusClass *autobus =  new autobusClass(seznamUlic,seznamZastavek, seznamLinek[i].first, seznamLinek[i].second, nullptr);
        busList.append(autobus);
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
    for (int i = 0; i < seznamLinek.size(); i++){
        if(seznamLinek[i].second == time){
            autobusClass *autobus = busList[i];
            scene->addItem(autobus->autobusItem);
            seznamBusu->append(autobus);
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

    for (int i = 0; i < seznamLinek.size(); i++){
        casLinky = seznamLinek[i].second;
        autobus = busList[i];
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
