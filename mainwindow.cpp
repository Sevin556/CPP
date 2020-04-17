#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QObject>
#include <QDebug>
#include <QFile>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    createMap();
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
    QStringList splitedLine= line.split(" ");
    int x1 = splitedLine[0].toInt();
    int y1 = splitedLine[1].toInt();
    int x2 = splitedLine[2].toInt();
    int y2 = splitedLine[3].toInt();
    qDebug() << "first line: " << x1 << y1 << x2 << y2;
    file.close();
    return;

}
