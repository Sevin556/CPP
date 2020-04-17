#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QGraphicsScene>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qDebug() << "Idem na to";
    createMap();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMap()
{
    auto * scene = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scene);
    QFile file("test.txt");

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "error opening file: " << file.error();
        return;
    }

    QTextStream instream(&file);
    QString line = instream.readLine(50);
    while (line != nullptr){
        if (line[0] == "#"){
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
