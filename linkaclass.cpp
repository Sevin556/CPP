#include "linkaclass.h"

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>


linkaClass::linkaClass()
{
}
void linkaClass::vytvor()
{
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
    return;

}
/**
 * @brief linkaClass::appendBus pridani autobusu, pokud je cas jeho vyjezdu
 * @param seznamBusu seznam autobusu
 * @param seznamUlic parametr predavany tride autobusClass
 * @param seznamZastavek parametr predavany tride autobusClass
 * @param time aktualni cas
 * @param scene scena
 */
void linkaClass::appendBus(QList<autobusClass *> *seznamBusu, QMap<int, ulicaClass *> *seznamUlic, QMap<int, zastavkaClass *> seznamZastavek, int time, MyScene *scene)
{
    for (int i = 0; i < seznamLinek.size(); i++){
        if(seznamLinek[i].second == time){
            autobusClass *autobus = new autobusClass(seznamUlic,seznamZastavek, seznamLinek[i].first, time, nullptr) ;
            scene->addItem(autobus->autobusItem);
            seznamBusu->append(autobus);
        }
    }
}

