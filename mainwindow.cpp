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

    //posle informacii o aktualnych zoznamoch objektov na scene
    scene->addInfo(zoznamUlic,zoznamZastavok,&zoznamAutobusov);
}


MainWindow::~MainWindow()
{
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

    // nastaveni novych autobusu
    linky->setTime(&zoznamAutobusov, time, scene);
}


//prida autobus do zoznamu aby som otestoval ci to zvlada viac autobusov naraz
void MainWindow::vytvorAutobus()
{
  zoznamAutobusov.append(new autobusClass(&zoznamUlic,zoznamZastavok, nullptr, time, nullptr));
   scene->addItem(zoznamAutobusov.last()->autobusItem);
}

void MainWindow::jizdniRadDialog()
{
    QDialog * dialogWindow = new QDialog();
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
    text <<"ID ulice :" <<ulica->ID_ulice <<"\n Nazov ulice :"<< ulica->nazovUlice <<"\n Zaciatok :" <<ulica->x1 << ulica->y2 <<"\n Koniec :" <<ulica->x2 << ulica->y2<<"\n Rychlost premavky:"<<ulica->rychlostPremavky;
    QFont font;
    font.setItalic(true);
    font.setPointSize(20);
    ui->infoLabel->setFont(font);
    ui->infoLabel->setText(textik);

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
   text <<"ID zastavky :" <<zastavka->ID_zastavky <<"\n Nazov zastavky :"<< zastavka->nazovZastavky <<"\n Pozicia :" <<zastavka->X <<zastavka->Y;
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
   qDebug() <<autobus->index;
   text <<"ID dalsieho bodu:" <<autobus->dalsiBod.x() << autobus->dalsiBod.y();
  ui->infoLabel->setText(textik);
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

        auto *zastavka =new zastavkaClass(x,y,nazov,ID, scene);
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
