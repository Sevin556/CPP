#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "autobusclass.h"
#include "ulicaclass.h"
#include "zastavkaclass.h"
#include "myscene.h"
#include "mydialog.h"

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QTimer>
#include <QGraphicsLineItem>
#include <QMap>
#include <QTime>
#include <QFont>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new MyScene(ui->graphicsView);
    ui->graphicsView->setRenderHints( QPainter::Antialiasing | QPainter::HighQualityAntialiasing );
    ui->graphicsView->setScene(scene);



    createMap();
    generateBusStops();
    linky = new linkaClass(&zoznamUlic, zoznamZastavok);
    timer = new QTimer(this);
    timer->setInterval(20);

    connect(timer, SIGNAL(timeout()), this, SLOT(timerBus()));
    connect(ui->zoomINBtn,&QPushButton::clicked,this,&MainWindow::zoomIN);
    connect(ui->zoomSlider,&QAbstractSlider::valueChanged,this,&MainWindow::zoomSLider);
    connect(ui->resetBtn,&QPushButton::clicked,this,&MainWindow::resetView);
    connect(ui->zoomOUTBtn,&QPushButton::clicked,this,&MainWindow::zoomOUT);
    connect(ui->startstopBtn,&QPushButton::clicked,this,&MainWindow::start_stop);
    connect(ui->speedSlider,&QAbstractSlider::valueChanged,this,&MainWindow::speed);
    connect(ui->lineEditTime, &QLineEdit::textEdited, this, &MainWindow::editTime);
    connect(ui->pridajBtn,&QPushButton::clicked,this,&MainWindow::vytvorAutobus);
    connect(ui->jizdniRadBtn, &QPushButton::clicked, this, &MainWindow::jizdniRadDialog);
    connect(scene,&MyScene::infoZmeneneUlica,this,&MainWindow::zmenPopisUlice);
    connect(scene,&MyScene::infoZmeneneZastavka,this,&MainWindow::zmenPopisZastavky);
    connect(scene,&MyScene::infoZmeneneAutobus,this,&MainWindow::zmenPopisAutbobusu);
    connect(scene,&MyScene::rightClick,this,&MainWindow::uzavriCestu);
    connect(scene,&MyScene::pridajUlicuDoLinky,this,&MainWindow::pridajUlicu);


     for (int i =0;i< zoznamAutobusov.size();i++){
         connect(zoznamAutobusov[i],&autobusClass::meskanieZmenene,this,&MainWindow::zmenPopisAutbobusu);

     }



    //posle informacii o aktualnych zoznamoch objektov na scene
    scene->addInfo(zoznamUlic,zoznamZastavok,&zoznamAutobusov);
}


MainWindow::~MainWindow()
{
    for(auto i = zoznamUlic.begin(); i != zoznamUlic.end(); ++i)
    {
        delete i.value();
    }

    for(auto i = zoznamZastavok.begin(); i != zoznamZastavok.end(); ++i)
    {
        delete i.value();
    }
    delete linky;
    delete ui;
}


void MainWindow::zoomIN()
{
    ui->graphicsView->scale(1.25,1.25);
    ui->zoomSlider->setValue(ui->zoomSlider->sliderPosition()*1.25);

}

void MainWindow::zoomOUT()
{
    ui->graphicsView->scale(0.8,0.8);
    ui->zoomSlider->setValue(ui->zoomSlider->sliderPosition()*0.8);
}

void MainWindow::zoomSLider(int value)
{
    auto original = ui->graphicsView->transform();
    qreal scale = value/10.0;
    QString labelText = "Zoom: ";
    labelText.append(QString::number(scale));
    ui->labelZoom->setText(labelText);
    ui->graphicsView->setTransform(QTransform(scale,original.m12(),original.m21(),scale,original.dx(),original.dy()));
}

void MainWindow::resetView()
{
    ui->graphicsView->resetMatrix();
    ui->labelZoom->setText("Zoom: 1.0");
}

void MainWindow::start_stop()
{
    if(timer->isActive()){
        timer->stop();
        ui->startstopBtn->setText("Štart");
    }
    else{
        timer->start();
        ui->startstopBtn->setText("Stop");
    }

      // autobus->pocitajTrasu();
}

void MainWindow::speed(int value){
    timer->setInterval(50 - value);
}

void MainWindow::timerBus()
{

    linky->appendBus(&zoznamAutobusov, time, scene);

    //prejde zoznam autobusov a posunie ich vpre, potom updatne scenu
    for (int i =0;i< zoznamAutobusov.size();i++){
        if (zoznamAutobusov[i] != nullptr){
            if (zoznamAutobusov[i]->vykonajTrasu(time) == 1){
                zoznamAutobusov[i]=nullptr;
            }
        }
    }
    //qDebug() << "Posunul som bod";
   /* for (int i = 0;i<zoznamUlic.size();i++){
      zoznamUlic[i]->vypisInfo();
    }*/
    scene->update();
    ui->lineEditTime->setText(QTime::fromMSecsSinceStartOfDay(time * 1000).toString("hh:mm:ss"));
    time = (time + 1) % 86400; //86400 sekund == 1 den

}

/**
 * @brief MainWindow::editTime rucni urava casu, slot vymaze vsechny aktivni autobusy
 * a nastavi scenu novymi
 * @param text cas ve formatu hh:mm:ss
 */
void MainWindow::editTime(QString text)
{

    // prevedeni casu z formatu hh:mm:ss na sekundy
    QStringList list = text.split(":");
    if(list.size() != 3){
        return;
    }


    qDebug() << list[0] << list[1] << list[2];
    time = list[0].toInt() * 3600; //hodiny->sekundy
    time = time + list[1].toInt() * 60; //minuty->sekundy
    time = time + list[2].toInt(); //sekundy

    for (int i =0;i< zoznamAutobusov.size();i++){
        if (zoznamAutobusov[i] != nullptr){
            zoznamAutobusov[i]->autobusItem->hide();
            scene->removeItem(zoznamAutobusov[i]->autobusItem);
            zoznamAutobusov[i] = nullptr;
        }
    }
    zoznamAutobusov.clear();
    qDebug() << time;

    // nastaveni novych autobusu
    linky->setTime(&zoznamAutobusov, time, scene);
}


//prida autobus do zoznamu aby som otestoval ci to zvlada viac autobusov naraz
void MainWindow::vytvorAutobus()
{
  zoznamAutobusov.append(new autobusClass(nullptr, time,0, nullptr));
   scene->addItem(zoznamAutobusov.last()->autobusItem);
}

void MainWindow::jizdniRadDialog()
{

    myDialog * dialogWindow = new myDialog(linky);
    dialogWindow->show();
}

/**
* Slot, ktory zachytava signal o kliknuti na ulicu, zobrazi dialoogove okno a vypise info o ulici
* @param trieda kliknutej ulice
*/
void MainWindow::zmenPopisUlice(ulicaClass *ulica)
{
    //vytvori dialogove okno s vyberom rychlosti premavky na zadanej ulici
    auto * dialogWindow = new myDialog(ulica);
    dialogWindow->exec();
     QString textik ;
    QTextStream text(&textik);
   // qDebug() <<ulica->nazovUlice;
    text <<"ID ulice :" <<ulica->ID_ulice <<"<br> Nazov ulice :"<< ulica->nazovUlice <<"<br> Zaciatok :" <<ulica->x1 << ulica->y1 <<"<br> Koniec :" <<ulica->x2 << ulica->y2<<"<br> Rychlost premavky:"<<ulica->rychlostPremavky;
    QFont font;
    font.setItalic(true);
    font.setPointSize(20);
    ui->infoLabel->setFont(font);
    ui->infoLabel->setText(textik);

}


void MainWindow::uzavriCestu(ulicaClass* ulica)
{
    myDialog *dialog = new myDialog(true,ulica);
    dialog->show();
    connect(dialog,&myDialog::naklikajUlicu,this,&MainWindow::naklikajObchadzku);


}

void MainWindow::naklikajObchadzku(ulicaClass* uzavretaUlica)
{
    for (int i = 0 ;i<linky->seznamLinek.size();i++){
        for (int j = 0 ; j< linky->seznamLinek[i]->trasaLinky->zoznamUlicLinky->size();j++){
            if (uzavretaUlica == linky->seznamLinek[i]->trasaLinky->zoznamUlicLinky->value(j)){
                zmenLinkuItems itemy;
                itemy.uzatvaranaUlica = uzavretaUlica;
                itemy.linka = linky->seznamLinek[i];
                itemy.indexUliceNaLinke = j;
                linkyNaZmenu.append(itemy);


            }
        }
    }


    if (linkyNaZmenu.size() >0){
        for (int k = 0 ; k< linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->size();k++){
            linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(k)->ulicaItem->setPen(QPen(Qt::yellow,6));
        }
        uzavretaUlica->ulicaItem->setPen(QPen(Qt::red));
        scene->addLine((uzavretaUlica->x1+uzavretaUlica->x2)/2+10,(uzavretaUlica->y1+uzavretaUlica->y2)/2+10,(uzavretaUlica->x1+uzavretaUlica->x2)/2-10,(uzavretaUlica->y1+uzavretaUlica->y2)/2-10,QPen(Qt::red,3));
        scene->addLine((uzavretaUlica->x1+uzavretaUlica->x2)/2-10,(uzavretaUlica->y1+uzavretaUlica->y2)/2+10,(uzavretaUlica->x1+uzavretaUlica->x2)/2+10,(uzavretaUlica->y1+uzavretaUlica->y2)/2-10,QPen(Qt::red,3));

        int pocetZastavok = 0;
        for (int i = 0 ;i < linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok;i++){
            for (int j = 0;j < linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince.size();j++){
                if (linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(i).x() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince.value(j).first->X
                        && linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(i).y() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince.value(j).first->Y){
                    pocetZastavok++;
                    qDebug() << linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince.value(j).first->nazovZastavky;
                }

            }
        }
        linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->removeAt(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok);
    }
        myDialog *dialog = new myDialog(linkyNaZmenu[0].linka);
        dialog->show();
        connect(dialog,&myDialog::obchadzkaUkoncena,this,&MainWindow::ukonciPridavanieObchadzky);
        linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->removeAt(linkyNaZmenu[0].indexUliceNaLinke);
        scene->klikamObchadzku = true;
        scene->menenaLinka = linkyNaZmenu[0].linka;
        scene->indexPridavanejUlice = linkyNaZmenu[0].indexUliceNaLinke;
        nenaklikane = true;

}

void MainWindow::pridajUlicu(int index, vecItem *linka, ulicaClass *ulica)
{
    ulica->ulicaItem->setPen(QPen(Qt::yellow,6));
    linka->trasaLinky->zoznamUlicLinky->insert(index,ulica);
    int pocetZastavok = 0;
    for (int i = 0 ;i < index+pocetZastavok;i++){
        for (int j = 0;j<linka->trasaLinky->zastavkyNaLince.size();j++){
            if (linka->trasaLinky->bodyPohybu->value(i).x() == linka->trasaLinky->zastavkyNaLince.value(j).first->X
                    && linka->trasaLinky->bodyPohybu->value(i).y() == linka->trasaLinky->zastavkyNaLince.value(j).first->Y)
                pocetZastavok++;
        }
    }
    int miestoVlozenia = index+pocetZastavok;
        if (miestoVlozenia > 2){
            if ((linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).x() == ulica->x1 && linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).y() == ulica->y1) ||
                    (linka->trasaLinky->bodyPohybu->value(miestoVlozenia-2).x() == ulica->x1 && linka->trasaLinky->bodyPohybu->value(miestoVlozenia-2).y() == ulica->y1) ){
                linka->trasaLinky->bodyPohybu->insert(miestoVlozenia,QPoint(ulica->x2,ulica->y2));
            }else {
                linka->trasaLinky->bodyPohybu->insert(miestoVlozenia,QPoint(ulica->x1,ulica->y1));
            }
        }else {//druha ulica...nemoze byt i-2
            if (linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).x() == ulica->x1 && linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).y() == ulica->y1){
                linka->trasaLinky->bodyPohybu->insert(miestoVlozenia,QPoint(ulica->x2,ulica->y2));
            }else {
                linka->trasaLinky->bodyPohybu->insert(miestoVlozenia,QPoint(ulica->x1,ulica->y1));
            }
        }
}

void MainWindow::ukonciPridavanieObchadzky()
{
    nenaklikane=false;
    scene->klikamObchadzku = false;
    linkyNaZmenu.removeFirst();
    for (QMap<int,ulicaClass*>::const_iterator i = zoznamUlic.constBegin();i !=zoznamUlic.constEnd();++i){
        i.value()->ulicaItem->setPen(QPen(Qt::black,4));
    }


    if (linkyNaZmenu.size() >0){
        for (int k = 0 ; k< linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->size();k++){
            linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(k)->ulicaItem->setPen(QPen(Qt::yellow,6));
        }
        linkyNaZmenu[0].uzatvaranaUlica->ulicaItem->setPen(QPen(Qt::red));
        scene->addLine((linkyNaZmenu[0].uzatvaranaUlica->x1+linkyNaZmenu[0].uzatvaranaUlica->x2)/2+10,(linkyNaZmenu[0].uzatvaranaUlica->y1+linkyNaZmenu[0].uzatvaranaUlica->y2)/2+10,(linkyNaZmenu[0].uzatvaranaUlica->x1+linkyNaZmenu[0].uzatvaranaUlica->x2)/2-10,(linkyNaZmenu[0].uzatvaranaUlica->y1+linkyNaZmenu[0].uzatvaranaUlica->y2)/2-10,QPen(Qt::red,3));
        scene->addLine((linkyNaZmenu[0].uzatvaranaUlica->x1+linkyNaZmenu[0].uzatvaranaUlica->x2)/2-10,(linkyNaZmenu[0].uzatvaranaUlica->y1+linkyNaZmenu[0].uzatvaranaUlica->y2)/2+10,(linkyNaZmenu[0].uzatvaranaUlica->x1+linkyNaZmenu[0].uzatvaranaUlica->x2)/2+10,(linkyNaZmenu[0].uzatvaranaUlica->y1+linkyNaZmenu[0].uzatvaranaUlica->y2)/2-10,QPen(Qt::red,3));

        myDialog *dialog = new myDialog(linkyNaZmenu[0].linka);
        dialog->show();
        connect(dialog,&myDialog::obchadzkaUkoncena,this,&MainWindow::ukonciPridavanieObchadzky);
        linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->removeAt(linkyNaZmenu[0].indexUliceNaLinke);
        scene->klikamObchadzku = true;
        scene->menenaLinka = linkyNaZmenu[0].linka;
        scene->indexPridavanejUlice = linkyNaZmenu[0].indexUliceNaLinke;
        nenaklikane = true;

        int pocetZastavok = 0;
        for (int i = 0 ;i < linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok;i++){
            for (int j = 0;j < linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince.size();j++){
                if (linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(i).x() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince.value(j).first->X
                        && linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(i).y() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince.value(j).first->Y)
                    pocetZastavok++;
            }
        }
        linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->removeAt(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok);
    }


}


/**
* Slot, ktory zachytava signal o kliknuti na ulicu, zobrazi info o ulici
* @param trieda kliknutej zastavky
*/
void MainWindow::zmenPopisZastavky(zastavkaClass *zastavka)
{
   QString textik ;
   QTextStream text(&textik);
   qDebug() <<zastavka->ID_zastavky;
   text <<"ID zastavky :" <<zastavka->ID_zastavky <<"<br> Nazov zastavky :"<< zastavka->nazovZastavky <<"<br>Pozicia :" <<zastavka->X <<zastavka->Y;
    ui->infoLabel->setText(textik);
}

/**
* Slot, ktory zachytava signal o kliknuti na autobus, zobrazi info o autobuse
* @param trieda kliknuteho autobusu
*/
void MainWindow::zmenPopisAutbobusu(autobusClass *autobus)
{
    QString textik ;
    QTextStream text(&textik);

    text << "ID dalsieho bodu:" <<autobus->dalsiBod.x() << autobus->dalsiBod.y()<< "<br> Moje meskanie: " << autobus->meskanie <<" minut<br>"<<"Idem po ulici" << autobus->zoznamUlicLinky->value(autobus->index-autobus->indexZastavky)->ID_ulice;
    text << "<br>Linka: " << linky->linkaBusu(autobus->MojeID) << "<br>Zastavky:<br>";

    for(int i = 0; i < autobus->zastavkyNaLince.size(); i++){
        if(autobus->zastavkyNaLince[i].second < time){
            text << "<font color = 'gray'>";
            text << QTime::fromMSecsSinceStartOfDay(autobus->zastavkyNaLince[i].second * 1000).toString("hh:mm:ss");
            text << autobus->zastavkyNaLince[i].first->nazovZastavky << "</font><br>";
        }
        else{
            text << QTime::fromMSecsSinceStartOfDay(autobus->zastavkyNaLince[i].second * 1000).toString("hh:mm:ss");
            text << autobus->zastavkyNaLince[i].first->nazovZastavky << "<br>";
        }
    }
    ui->infoLabel->setTextFormat(Qt::RichText);
    ui->infoLabel->setText(textik);
    //ui->infoLabel->setStyleSheet("QLabel{color: red;}");


    for (int i = 0; i < autobus->zoznamUlicLinky->size();i++){
        autobus->zoznamUlicLinky->value(i)->ulicaItem->setPen(QPen(Qt::red,6));
    }
}


/**
* Nacita a prida do sceny a zoznamu zastavok autobusove zastavky
*/
void MainWindow::generateBusStops()
{
    QFile file("zastavky.txt");
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
        }
        QStringList splitedLine= line.split(" ");
        QString nazov = splitedLine[0];
        int x = splitedLine[1].toInt();
        int y = splitedLine[2].toInt();
        int ID = splitedLine[3].toInt();

        auto *zastavka = new zastavkaClass(x,y,nazov,ID);
        scene->addItem(zastavka->zastavkaItem);
        zoznamZastavok.insert(ID,zastavka);

        line = instream.readLine(50);
    }
    file.close();
    return;

}


/**
* Vytvorenie zoznamu ulíc a ich zakreslenie na mapu
* ulice načítava zo súboru
*/
void MainWindow::createMap()
{
    QFile file("test.txt");

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "error opening file: " << file.error();
        return;
    }

    QTextStream instream(&file);
    QString line = instream.readLine(50);

    //sluzi na rozlisenie ulic

    //nacitanie vsetkych ulic zo zoznamu
    while (line != nullptr){
        if (line[0] == '#'){
            qDebug() <<line;
            line = instream.readLine(50);

        }
        QStringList splitedLine= line.split(" ");

        int x1 = splitedLine[0].toInt();
        int y1 = splitedLine[1].toInt();
        int x2 = splitedLine[2].toInt();
        int y2 = splitedLine[3].toInt();
        int ID_Ulice = splitedLine[4].toInt();

        //vytvori objekt triedy ulicaClass aby ho mohol pridat na scenu a ulozit do zoznamu
        ulicaClass * ulica = new ulicaClass(x1,y1,x2,y2,"Ferkova",ID_Ulice);
        scene->addItem(ulica->ulicaItem);
        //uklada do zoznamu ulic - asi vyuzijeme pri obchadzach a spomaleniach
        zoznamUlic.insert(ID_Ulice,ulica);
        line = instream.readLine(50);
    }
    file.close();

}
