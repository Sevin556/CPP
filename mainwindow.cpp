#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QGraphicsScene>
#include <QPushButton>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Idem na to";
    auto * scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);

    createMap(scene);
    generateBusStops(scene);

    connect(ui->zoomINBtn,&QPushButton::clicked,this,&MainWindow::zoomIN);
    connect(ui->zoomOUTBtn,&QPushButton::clicked,this,&MainWindow::zoomOUT);
    connect(ui->zoomSlider,&QAbstractSlider::valueChanged,this,&MainWindow::zoomSLider);
    connect(ui->resetBtn,&QPushButton::clicked,this,&MainWindow::resetView);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    while (line != nullptr){
        if (line[0] == '#'){
            line = instream.readLine(50);
        }
        QStringList splitedLine= line.split(" ");

        int x1 = splitedLine[0].toInt();
        int y1 = splitedLine[1].toInt();
        int x2 = splitedLine[2].toInt();
        int y2 = splitedLine[3].toInt();
        qDebug() << "first line: " << x1 << y1 << x2 << y2;
        scene->addLine(x1,y1,x2,y2,QPen({Qt::black},4));
        line = instream.readLine(50);
    }
    file.close();
    return;

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
        qDebug() << x, y;
        scene->addEllipse(x,y,4, 4, QPen({Qt::red}, 6));

        line = instream.readLine(50);
    }
    file.close();
    return;

}
