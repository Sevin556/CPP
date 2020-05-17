#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "autobusclass.h"
#include "ulicaclass.h"
#include "zastavkaclass.h"
#include "myscene.h"
#include "linkaclass.h"

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
    autobusClass *autobusInfo = nullptr;
    QPair<zastavkaClass*, int> *aktualniZastavka = nullptr;
    int time = 0;
    linkaClass *linky;
    QTimer *timer;
    QMap <int,ulicaClass*>zoznamUlic;
    QList<autobusClass*> zoznamAutobusov;
    QMap <int,zastavkaClass*>zoznamZastavok;

    struct zmenLinkuItems{
        ulicaClass* uzatvaranaUlica;
        vecItem* linka;
        int indexUliceNaLinke;
        int miestoVLozenia;
    };
    QVector<zmenLinkuItems> linkyNaZmenu;

    bool nenaklikane;
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
    void start_stop();
    void speed(int value);
    void timerBus();
    void zmenPopisUlice(ulicaClass* ulica);
    void zmenPopisZastavky(zastavkaClass *zastavka);
    void zmenPopisAutbobusu(autobusClass *autobus);
    void editTime(QString string);
    void jizdniRadDialog();
    void uzavriCestu(ulicaClass* ulica);
    void naklikajObchadzku(ulicaClass* uzavretaUlica);
    void pridajUlicu(int index,vecItem* linka,ulicaClass * ulica);
    void ukonciPridavanieObchadzky();
    void odoberUlicuZLinky(vecItem* linka, ulicaClass *ulica);
    void odoberZastavkuZLinky(vecItem* linka,zastavkaClass * zastavka);
    void obnovInfo();
};
#endif // MAINWINDOW_H