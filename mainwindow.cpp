#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "autobusclass.h"
#include "ulicaclass.h"
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QGraphicsScene>
#include <QPushButton>
#include <QTimer>
#include <QGraphicsLineItem>
#include <QThread>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setRenderHints( QPainter::Antialiasing | QPainter::HighQualityAntialiasing );

    ui->graphicsView->setScene(scene);

    createMap(scene);
    generateBusStops(scene);
    qDebug() <<"deklarujem autobus";
    autobus = new autobusClass(scene,&zoznamUlic,nullptr) ;
  //  auto *line = scene->addLine(100,200,200,100);
    //QGraphicsLineItem linka = *line;
//
    connect(ui->zoomINBtn,&QPushButton::clicked,this,&MainWindow::zoomIN);
    connect(ui->zoomSlider,&QAbstractSlider::valueChanged,this,&MainWindow::zoomSLider);
    connect(ui->resetBtn,&QPushButton::clicked,this,&MainWindow::resetView);
    //connect(ui->farbaBtn,SIGNAL(clicked()),this,&MainWindow::farba(scene,&linka));
    connect(ui->zoomOUTBtn,&QPushButton::clicked,this,&MainWindow::zoomOUT);
    connect(ui->startBtn,&QPushButton::clicked,this,&MainWindow::start);
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


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerBus()));
    timer->start(2000);

      // autobus->pocitajTrasu();

}

void MainWindow::timerBus()
{
/*    static QGraphicsEllipseItem *autobusik = autobus->createBus(scene);
    autobus->updateBus(autobusik);
*/
    static int i = 0;
    qDebug() <<"Toto je index:"<<i;
    autobus->pocitajTrasu(i);
    i++;

}



void MainWindow::generateBusStops(QGraphicsScene * scene)
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
        int x = splitedLine[0].toInt();
        int y = splitedLine[1].toInt();
        qDebug() << x << y;
        auto *zastavka = new QGraphicsRectItem(x,y,20,15);
        zastavka ->setBrush(Qt::red);
        scene->addItem(zastavka);

        //scene->addRect(x,y,15, 10, QPen({Qt::black},2)),QBrush(Qt::yellow);

        line = instream.readLine(50);
    }
    file.close();
    return;

}


void MainWindow::createMap(QGraphicsScene * scene)
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
