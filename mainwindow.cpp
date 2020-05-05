#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "autobusclass.h"
#include "ulicaclass.h"
#include "zastavkaclass.h"
#include "myscene.h"

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QPushButton>
#include <QTimer>
#include <QGraphicsLineItem>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new MyScene(ui->graphicsView);
    ui->graphicsView->setRenderHints( QPainter::Antialiasing | QPainter::HighQualityAntialiasing );

    ui->graphicsView->setScene(scene);

    createMap(scene);
    generateBusStops(scene);
    //vytvori prvy autobus
    autobus = new autobusClass(scene,&zoznamUlic,zoznamZastavok,nullptr) ;
    zoznamAutobusov.append(autobus);
  //  auto *line = scene->addLine(100,200,200,100);
    //QGraphicsLineItem linka = *line;
//
    connect(ui->zoomINBtn,&QPushButton::clicked,this,&MainWindow::zoomIN);
    connect(ui->zoomSlider,&QAbstractSlider::valueChanged,this,&MainWindow::zoomSLider);
    connect(ui->resetBtn,&QPushButton::clicked,this,&MainWindow::resetView);
    //connect(ui->farbaBtn,SIGNAL(clicked()),this,&MainWindow::farba(scene,&linka));
    connect(ui->zoomOUTBtn,&QPushButton::clicked,this,&MainWindow::zoomOUT);
    connect(ui->startBtn,&QPushButton::clicked,this,&MainWindow::start);

    connect(ui->pridajBtn,&QPushButton::clicked,this,&MainWindow::vytvorAutobus);

}


MainWindow::~MainWindow()
{
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

void MainWindow::start()
{

    //vytvori timer podla ktoreho idu...treba nejako prerobit na čas aby sa mohlo riadit rozvrhom linky
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerBus()));
    timer->start(10);

      // autobus->pocitajTrasu();

}

void MainWindow::timerBus()
{
    //prejde zoznam autobusov a posunie ich vpre, potom updatne scenu
    for (int i =0;i< zoznamAutobusov.size();i++){
        if (zoznamAutobusov[i] != nullptr){
            if (zoznamAutobusov[i]->vykonajTrasu()==1){
                zoznamAutobusov[i]=nullptr;
            }

        }
    }
    //qDebug() << "Posunul som bod";
   /* for (int i = 0;i<zoznamUlic.size();i++){
      zoznamUlic[i]->vypisInfo();
    }*/
    scene->update();

}


//prida autobus do zoznamu aby som otestoval ci to zvlada viac autobusov naraz
void MainWindow::vytvorAutobus()
{
  zoznamAutobusov.append(new autobusClass(scene,&zoznamUlic,zoznamZastavok,nullptr))  ;

}



void MainWindow::generateBusStops(MyScene * scene)
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

        qDebug() << x << y;
        auto *zastavka =new zastavkaClass(x,y,nazov,ID,scene);
        zoznamZastavok.insert(ID,zastavka);

        line = instream.readLine(50);
    }
    file.close();
    return;

}

/**
* Vytvorenie zoznamu ulíc a ich zakreslenie na mapu
* @param scéna na, ktorú sa vykreslia ulice
*/
void MainWindow::createMap(MyScene * scene)
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
