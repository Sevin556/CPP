
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

/** \mainpage Hlavná stránka
 *
 * \section autori Autori
 *  Všetky súčasti tohoto projektu boli vytvorené autormi Ivan Halomi(xhalom00) a Michal Sova(xsovam00) podľa zadania projektu do ICP z roku 2019/2020
 * \section intro_sec Obsluha
 *
 *  Pre zmenu rýchlosti premávky na ulici je potrebné kliknúť na ňu ľavým tlačidlom myši a následne vybrať jednu z možností
 *
 *  Pre uzavretie ulice a následné vytvorenie obchádzky treba kliknút na ulicu, ktorú chceme uzavrieť pravým tlačidlom myši. Následne program vyhodnotí,
 * ktoré linky sú touto uzávierkou ovplyvnené a ukáže dialógové okno pre ktorú linku treba naklikať obchádzku. Do obchádzky sa pridávajú ulice ľavým kliknutím.
 * Počas klikania obchádzky je možné uzavrieť aj ďalšie ulice pre zjednodušenie cesty autobusu. Proces sa ukončí po kliknutí na Hotovo v dialógovom okne
 *  (ak treba upraviť ďaľšiu ulicu, tak sa zobrazí nové dialógové okno).
 */

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
    /**
     * @brief scene scéna na ktorej sa všetko vykresľuje
     */
    MyScene *scene ;
    /**
     * @brief autobusInfo uloženie ukazovateľa na autobus o ktorom zobrazujeme informácie
     */
    autobusClass *autobusInfo = nullptr;
    /**
     * @brief aktualniZastavka aktuálna zastávka ku ktorej smeruje kliknutý autobus
     */
    QPair<zastavkaClass*, int> *aktualniZastavka = nullptr;
    /**
     * @brief time čas simulácie
     */
    int time = 0;
    /**
     * @brief linky všetky autobusové linky
     */
    linkaClass *linky;
    /**
     * @brief timer slúži na simuláciu času
     */
    QTimer *timer;
    /**
     * @brief zoznamUlic zoznam všetkých ukazateľov ulíc na scéne
     */
    QMap <int,ulicaClass*>zoznamUlic;
    /**
     * @brief zoznamAutobusov zoznam všetkých ukazateľov na autobusy či už aktívne alebo nie
     */
    QList<autobusClass*> zoznamAutobusov;
    /**
     * @brief zoznamZastavok zoznam všetkých ukazateľov zastávok na scéne
     */
    QMap <int,zastavkaClass*>zoznamZastavok;

    /**
     * @brief The zmenLinkuItems struct uloží linku a informácie o uzavretej ulici pri vytváraní obchádzky
     */
    struct zmenLinkuItems{
        ulicaClass* uzatvaranaUlica;
        vecItem* linka;
        int indexUliceNaLinke;
        int miestoVLozenia;
    };
    /**
     * @brief linkyNaZmenu zoznam všetkých menených liniek pri obchádzke
     */
    QVector<zmenLinkuItems> linkyNaZmenu;
    bool nenaklikane;


    //funkcie
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
