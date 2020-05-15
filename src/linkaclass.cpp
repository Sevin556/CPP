#include "linkaclass.h"

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "bodylinky.h"
#include <QVector>
#include <QMap>

/**
 * @brief linkaClass::linkaClass Konštruktor triedy
 * @details zo súboru načíta a vytvorí jednotlivé linky a vytvorí autobus pre každú jazdu linky
 * @param seznamUlic zoznam ulíc na scéne
 * @param seznamZastavek zoznam zastávok na scéne
 */
linkaClass::linkaClass(QMap<int, ulicaClass *> *seznamUlic, QMap<int, zastavkaClass *> seznamZastavek)
{

    // nacteni linek ze souboru
    QString basedir = "../examples/";
    QString seznam = basedir + "linky-seznam.txt";
    qDebug() << "seznam: " << seznam;
    QFile file(seznam);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "error opening file: " << seznam;
        return;
    }

    QTextStream instream(&file);
    QString line = instream.readLine(50);
    while (line != nullptr){
        if (line[0] == '#'){
            line = instream.readLine(50);
            continue;
        }

        QStringList splitedLine= line.split(" ");
        bool najdene = false;

        for (int i =0; i<seznamLinek.size();i++){
            if (seznamLinek[i]->nazovLinky==splitedLine[0]){
                // uz existuje linka s dany nazvom
                najdene = true;

                //preistotu kontrola ci nie je linka definovana inym suborom ako naposledy
                if (seznamLinek[i]->suborTrasy != basedir + splitedLine[1]){
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
        if (!najdene){

            //linka s danym nazvom este neexistuje a treba ju vytvorit
            vecItem *item =new vecItem();
            item->nazovLinky =  splitedLine[0];
            item->suborTrasy = basedir + splitedLine[1];
            // cas zahajeni cesty

            item->trasaLinky = new bodyLinky(seznamUlic,seznamZastavek,item->suborTrasy);
            QStringList time = splitedLine[2].split(":");
            int cas = time[0].toInt() * 3600; //hodiny
            cas += time[1].toInt() * 60; //minuty
            cas += time[2].toInt(); //sekundy

            item->zoznamOdchodov.append(cas);
            seznamLinek.append(item);
        }


        line = instream.readLine(50);
    }


    file.close();
    //sluzi na lahsie identifikovanie autobusov
    int indexAutobusu = 0;
    // vytvori autobus pre kazdu jazdu linky
    for (int i = 0; i < seznamLinek.size(); i++){
        for(int j = 0; j < seznamLinek[i]->zoznamOdchodov.size();j++){
            indexAutobusu++;
            autobusClass *autobus =  new autobusClass( seznamLinek[i]->trasaLinky, seznamLinek[i]->zoznamOdchodov[j],indexAutobusu,seznamLinek[i]->nazovLinky, nullptr);
            busList.insert(indexAutobusu,autobus);

        }

    }

    return;

}
/**
 * @brief linkaClass::~linkaClass deštruktor
 */
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
                qDebug() << "ID append: " << indexAutobusu;
                autobusClass *autobus = busList[indexAutobusu];
                scene->addItem(autobus->autobusItem);
                autobus->autobusItem->show();
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

/**
 * @brief linkaClass::linkaBusu nájde linku konkretneho autobusu
 * @param id id hľadaného autobusu
 * @return názov linky, ktorej hľadaný autobus patrí
 */
QString linkaClass::linkaBusu(int id){
    int indexAutobusu=0;
    for (int i = 0; i < seznamLinek.size(); i++){
        for(int j = 0; j < seznamLinek[i]->zoznamOdchodov.size();j++){
            indexAutobusu++;
            if (indexAutobusu == id){
                return seznamLinek[i]->nazovLinky;
            }
        }
    }
    return "";
}
