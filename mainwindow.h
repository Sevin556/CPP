#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "autobusclass.h"
#include "ulicaclass.h"
#include "zastavkaclass.h"
#include "myscene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MyScene *scene ;
    autobusClass *autobus ;
    QMap <int,ulicaClass*>zoznamUlic;
    QList<autobusClass*> zoznamAutobusov;
    QMap <int,zastavkaClass*>zoznamZastavok;
    void createMap();
    void generateBusStops();
    void zobrazInfo(QGraphicsItem *kliknutyItem);

private:
    Ui::MainWindow *ui;

private slots:
    void zoomIN();
    void zoomOUT();
    void zoomSLider(int value);
    void resetView();
    void start();
    void timerBus();
    void vytvorAutobus();
    void zmenPopisUlice(ulicaClass* ulica);
    void zmenPopisZastavky(zastavkaClass *zastavka);
    void zmenPopisAutbobusu(autobusClass *autobus);

};
#endif // MAINWINDOW_H
