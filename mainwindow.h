#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "autobusclass.h"
#include "ulicaclass.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsScene *scene ;
    autobusClass *autobus ;
    QMap <int,ulicaClass*>zoznamUlic;
    void createMap(QGraphicsScene *scene);
    void generateBusStops(QGraphicsScene * scene);


private:
    Ui::MainWindow *ui;

private slots:
    void zoomIN();
    void zoomOUT();
    void zoomSLider(int value);
    void resetView();
    void start();
    void timerBus();
};
#endif // MAINWINDOW_H
