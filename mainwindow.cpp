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

/**
 * @brief MainWindow::MainWindow Konštruktor zobrazeného okna
 * @details vytvorí scénu,vykreslí ulice a zastávky,vytvorí timer a začne zachytávať všetky potrebné signály
 * @param parent prvok z ktorého dedí
 */
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
    connect(timer, SIGNAL(timeout()), this, SLOT(obnovInfo()));
    connect(ui->zoomINBtn,&QPushButton::clicked,this,&MainWindow::zoomIN);
    connect(ui->zoomSlider,&QAbstractSlider::valueChanged,this,&MainWindow::zoomSLider);
    connect(ui->resetBtn,&QPushButton::clicked,this,&MainWindow::resetView);
    connect(ui->zoomOUTBtn,&QPushButton::clicked,this,&MainWindow::zoomOUT);
    connect(ui->startstopBtn,&QPushButton::clicked,this,&MainWindow::start_stop);
    connect(ui->speedSlider,&QAbstractSlider::valueChanged,this,&MainWindow::speed);
    connect(ui->lineEditTime, &QLineEdit::textEdited, this, &MainWindow::editTime);
    connect(ui->jizdniRadBtn, &QPushButton::clicked, this, &MainWindow::jizdniRadDialog);
    connect(scene,&MyScene::infoZmeneneUlica,this,&MainWindow::zmenPopisUlice);
    connect(scene,&MyScene::infoZmeneneZastavka,this,&MainWindow::zmenPopisZastavky);
    connect(scene,&MyScene::infoZmeneneAutobus,this,&MainWindow::zmenPopisAutbobusu);
    connect(scene,&MyScene::rightClick,this,&MainWindow::uzavriCestu);
    connect(scene,&MyScene::pridajUlicuDoLinky,this,&MainWindow::pridajUlicu);
    connect(scene,&MyScene::odoberUlicu,this,&MainWindow::odoberUlicuZLinky);
    connect(scene,&MyScene::odoberZastavku,this,&MainWindow::odoberZastavkuZLinky);



     for (int i =0;i< zoznamAutobusov.size();i++){
         connect(zoznamAutobusov[i],&autobusClass::meskanieZmenene,this,&MainWindow::zmenPopisAutbobusu);

     }



    //posle informacii o aktualnych zoznamoch objektov na scene
    scene->addInfo(zoznamUlic,zoznamZastavok,&zoznamAutobusov);
}

/**
 * @brief MainWindow::~MainWindow Deštruktor
 */
MainWindow::~MainWindow()
{
    for(auto i = zoznamUlic.begin(); i != zoznamUlic.end(); ++i)
    {
        delete i.value();
    }

    for(auto i = zoznamZastavok.begin(); i != zoznamZastavok.end(); ++i)
    {
        delete i.value();
    }
    delete linky;
    delete ui;
}

/**
 * @brief MainWindow::zoomIN priblíži scénu
 */
void MainWindow::zoomIN()
{
    ui->graphicsView->scale(1.25,1.25);
    ui->zoomSlider->setValue(ui->zoomSlider->sliderPosition()*1.25);

}
/**
 * @brief MainWindow::zoomOUT oddiali scénu
 */
void MainWindow::zoomOUT()
{
    ui->graphicsView->scale(0.8,0.8);
    ui->zoomSlider->setValue(ui->zoomSlider->sliderPosition()*0.8);
}

/**
 * @brief MainWindow::zoomSLider priblíži/oddiali scénu
 * @param value hodnota slideru, podľa nej sa nastaví priblíženie scény
 */
void MainWindow::zoomSLider(int value)
{
    auto original = ui->graphicsView->transform();
    qreal scale = value/10.0;
    QString labelText = "Zoom: ";
    labelText.append(QString::number(scale));
    ui->labelZoom->setText(labelText);
    ui->graphicsView->setTransform(QTransform(scale,original.m12(),original.m21(),scale,original.dx(),original.dy()));
}

/**
 * @brief MainWindow::resetView nastaví pôvodný pohľad
 */
void MainWindow::resetView()
{
    ui->graphicsView->resetMatrix();
    ui->labelZoom->setText("Zoom: 1.0");
}

/**
 * @brief MainWindow::start_stop zastaví alebo spustí simuláciu
 */
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

}

/**
 * @brief MainWindow::speed zmení rýchlosť simulácie
 * @param value hodnota rýchlosti simulácie
 */
void MainWindow::speed(int value){
    timer->setInterval(50 - value);
}

/**
 * @brief MainWindow::timerBus posunie autobusy o krok vpred a updatne scénu
 */
void MainWindow::timerBus()
{

    linky->appendBus(&zoznamAutobusov, time, scene);

    //prejde zoznam autobusov a posunie ich vpred, potom updatne scenu
    for (int i =0;i< zoznamAutobusov.size();i++){
        if (zoznamAutobusov[i] != nullptr){
            if (zoznamAutobusov[i]->vykonajTrasu(time) == 1){
                zoznamAutobusov[i]=nullptr;
            }
        }
    }
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
    if(list.size() != 3){
        return;
    }


    qDebug() << list[0] << list[1] << list[2];
    time = list[0].toInt() * 3600; //hodiny->sekundy
    time = time + list[1].toInt() * 60; //minuty->sekundy
    time = time + list[2].toInt(); //sekundy
    time = time%86400;

    for (int i =0;i< zoznamAutobusov.size();i++){
        if (zoznamAutobusov[i] != nullptr){
            zoznamAutobusov[i]->autobusItem->hide();
            scene->removeItem(zoznamAutobusov[i]->autobusItem);
            zoznamAutobusov[i] = nullptr;
        }
    }
    zoznamAutobusov.clear();
    qDebug() << time;

    // nastaveni novych autobusu
    linky->setTime(&zoznamAutobusov, time, scene);
    scene->update();
}

/**
 * @brief MainWindow::jizdniRadDialog zobrazí informácie o všetkých linkách
 */
void MainWindow::jizdniRadDialog()
{

    myDialog * dialogWindow = new myDialog(linky);
    dialogWindow->show();
}

/**
* @brief MainWindow::zmenPopisUlice Slot, ktory zachytava signal o kliknuti na ulicu, zobrazi dialogove okno a vypise info o ulici
* @param ulica trieda kliknutej ulice
*/
void MainWindow::zmenPopisUlice(ulicaClass *ulica)
{
    //vytvori dialogove okno s vyberom rychlosti premavky na zadanej ulici
    auto * dialogWindow = new myDialog(ulica);
    dialogWindow->exec();
     QString textik ;
    QTextStream text(&textik);
   // qDebug() <<ulica->nazovUlice;
    text <<"ID ulice :" <<ulica->ID_ulice <<"<br> Nazov ulice :"<< ulica->nazovUlice <<"<br> Zaciatok :" <<ulica->x1 << ulica->y1 <<"<br> Koniec :" <<ulica->x2 << ulica->y2<<"<br> Rychlost premavky:"<<ulica->rychlostPremavky;
    QFont font;
    font.setItalic(true);
    font.setPointSize(20);
    ui->infoLabel->setFont(font);
    ui->infoLabel->setText(textik);
    autobusInfo = nullptr;

}

/**
 * @brief MainWindow::uzavriCestu začne proces naklikania obchádzky
 * @param ulica uzatváraná ulica
 */
void MainWindow::uzavriCestu(ulicaClass* ulica)
{
    myDialog *dialog = new myDialog(true,ulica);
    dialog->show();
    connect(dialog,&myDialog::naklikajUlicu,this,&MainWindow::naklikajObchadzku);
}

/**
 * @brief MainWindow::naklikajObchadzku spracuje uzavretie ulice
 * @details zístí a uloží informácie o všetkých linkách v ktorých sa vyskytuje uzatváraná ulica, nakreslí krížik a odstráni ulicu z linky, zobrazí ukončovacie dialógové okno.
 * Ak je na rušenej ulici zastávky postará sa aj o jej odstránenie
 * @param uzavretaUlica ulica ktorá bola uzavretá
 */
void MainWindow::naklikajObchadzku(ulicaClass* uzavretaUlica)
{
    for (int i = 0 ;i<linky->seznamLinek.size();i++){
        for (int j = 0 ; j< linky->seznamLinek[i]->trasaLinky->zoznamUlicLinky->size();j++){
            if (uzavretaUlica == linky->seznamLinek[i]->trasaLinky->zoznamUlicLinky->value(j)){
                zmenLinkuItems itemy;
                itemy.uzatvaranaUlica = uzavretaUlica;
                itemy.linka = linky->seznamLinek[i];
                itemy.indexUliceNaLinke = j;
                linkyNaZmenu.append(itemy);
                qDebug() << "Pridavam do zoznamu "<< linky->seznamLinek.value(i)->nazovLinky << " index J "<<j << "ID " <<linky->seznamLinek[i]->trasaLinky->zoznamUlicLinky->value(j)->ID_ulice;

            }
        }
    }
    bool zmazalZastavku = false;


    if (linkyNaZmenu.size() >0){
        for (int k = 0 ; k< linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->size();k++){
            linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(k)->ulicaItem->setPen(QPen(Qt::yellow,6));
        }
        uzavretaUlica->ulicaItem->setPen(QPen(Qt::red));
        scene->addLine((uzavretaUlica->x1+uzavretaUlica->x2)/2+10,(uzavretaUlica->y1+uzavretaUlica->y2)/2+10,(uzavretaUlica->x1+uzavretaUlica->x2)/2-10,(uzavretaUlica->y1+uzavretaUlica->y2)/2-10,QPen(Qt::red,3));
        scene->addLine((uzavretaUlica->x1+uzavretaUlica->x2)/2-10,(uzavretaUlica->y1+uzavretaUlica->y2)/2+10,(uzavretaUlica->x1+uzavretaUlica->x2)/2+10,(uzavretaUlica->y1+uzavretaUlica->y2)/2-10,QPen(Qt::red,3));

        int pocetZastavok = 0;
        bool zniz = false;
        if(linkyNaZmenu[0].indexUliceNaLinke == 0){
            linkyNaZmenu[0].indexUliceNaLinke =1;
            zniz = true;
        }
        for (int i = 0 ;i < linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok;i++){
            for (int j = 0;j < linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->size();j++){
                if (linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(i).x() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->value(j).first->X
                        && linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(i).y() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->value(j).first->Y){
                    pocetZastavok++;
                    qDebug() << linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->value(j).first->nazovZastavky;
                }

            }
        }

        if (zniz){
            linkyNaZmenu[0].indexUliceNaLinke =0;
        }

        linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->removeAt(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok);


        for (int i = 0; i < linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->size();i++){
            if (linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok -1).x() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->value(i).first->X
                    && linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok -1).y() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->value(i).first->Y ){

                linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->removeAt(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok -1);
                linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->removeAt(i);
                qDebug() << "VYMAZAL SOM ZASTAVKU "<<linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok -1);
            }
        }

        }
        myDialog *dialog = new myDialog(linkyNaZmenu[0].linka);
        dialog->show();
        connect(dialog,&myDialog::obchadzkaUkoncena,this,&MainWindow::ukonciPridavanieObchadzky);
        linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->removeAt(linkyNaZmenu[0].indexUliceNaLinke);
        scene->klikamObchadzku = true;
        scene->menenaLinka = linkyNaZmenu[0].linka;
        if (zmazalZastavku){
            scene->indexPridavanejUlice = linkyNaZmenu[0].indexUliceNaLinke-1;

        }else {
            scene->indexPridavanejUlice = linkyNaZmenu[0].indexUliceNaLinke;
        }
        nenaklikane = true;

}

/**
 * @brief MainWindow::pridajUlicu pridáva ulicu do aktuálne menenej linky
 * @details vypočíta koľko zastávok bude na danej linke pred pridávanou ulicou a postará sa o vloženie správneho konca ulice do vectora bodov, po ktorých sa autobus pohybuje.
 *  Prekreslí pridávanú ulicu na žlto.
 * @param index index na ktorý vloží kliknutú ulicu
 * @param linka linka do ktorej sa pridáva ulica
 * @param ulica pridávaná ulica
 */
void MainWindow::pridajUlicu(int index, vecItem *linka, ulicaClass *ulica)
{

    int pocetZastavok = 0;
    for (int i = 0 ;i < index+pocetZastavok;i++){
        for (int j = 1;j < linka->trasaLinky->zastavkyNaLince->size()-1;j++){
            if (linka->trasaLinky->bodyPohybu->value(i).x() == linka->trasaLinky->zastavkyNaLince->value(j).first->X
                    && linka->trasaLinky->bodyPohybu->value(i).y() == linka->trasaLinky->zastavkyNaLince->value(j).first->Y){
                pocetZastavok++;
                qDebug() << "Mam zastavku "<<linka->trasaLinky->zastavkyNaLince->value(j).first->nazovZastavky;
            }

        }
    }
    int miestoVlozenia = index+pocetZastavok;
    qDebug() <<"Idem vkladat na " <<miestoVlozenia << " z indexov " <<index << " "<<pocetZastavok;

        if (miestoVlozenia == 0){
            if (((linka->trasaLinky->zaciatokX+10) == ulica->x1 && (linka->trasaLinky->zaciatokY+10) == ulica->y1)){
                linka->trasaLinky->bodyPohybu->insert(miestoVlozenia,QPoint(ulica->x2,ulica->y2));
                qDebug() << "mam " << linka->trasaLinky->zaciatokX <<" " <<linka->trasaLinky->zaciatokY <<" vkladam "<<ulica->x2 <<ulica->y2;
            }else if ((linka->trasaLinky->zaciatokX+10) == ulica->x2 && (linka->trasaLinky->zaciatokY+10) == ulica->y2) {
                linka->trasaLinky->bodyPohybu->insert(miestoVlozenia,QPoint(ulica->x1,ulica->y1));
                qDebug() << "mam " << linka->trasaLinky->zaciatokX <<" " <<linka->trasaLinky->zaciatokY <<" vkladam x1 "<<ulica->x1<<ulica->y1;

            }else{
                myDialog *dialog = new myDialog("Ulica nenadväzuje na predchádzajúcu, skús znovu ");
                dialog->exec();
                delete dialog;
                return;

            }
        } else if (miestoVlozenia >= 2){
            if ((linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).x() == ulica->x1 && linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).y() == ulica->y1) ||
                    (linka->trasaLinky->bodyPohybu->value(miestoVlozenia-2).x() == ulica->x1 && linka->trasaLinky->bodyPohybu->value(miestoVlozenia-2).y() == ulica->y1) ){
                linka->trasaLinky->bodyPohybu->insert(miestoVlozenia,QPoint(ulica->x2,ulica->y2));
                qDebug() <<" vkladam "<<ulica->x2 <<ulica->y2;

            }else if((linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).x() == ulica->x2 && linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).y() == ulica->y2) ||
                     (linka->trasaLinky->bodyPohybu->value(miestoVlozenia-2).x() == ulica->x2 && linka->trasaLinky->bodyPohybu->value(miestoVlozenia-2).y() == ulica->y2)) {
                linka->trasaLinky->bodyPohybu->insert(miestoVlozenia,QPoint(ulica->x1,ulica->y1));
                qDebug() <<" vkladam "<<ulica->x1 <<ulica->y1;

            } else {
                myDialog *dialog = new myDialog("Ulica nenadväzuje na predchádzajúcu, skús znovu ");
                dialog->exec();
                delete dialog;
                return;

            }
        }else {//druha ulica...nemoze byt i-2
            if (linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).x() == ulica->x1 && linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).y() == ulica->y1){
                linka->trasaLinky->bodyPohybu->insert(miestoVlozenia,QPoint(ulica->x2,ulica->y2));
                qDebug() <<" vkladam "<<ulica->x2 <<ulica->y2;

            }else if (linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).x() == ulica->x2 && linka->trasaLinky->bodyPohybu->value(miestoVlozenia-1).y() == ulica->y2){
                linka->trasaLinky->bodyPohybu->insert(miestoVlozenia,QPoint(ulica->x1,ulica->y1));
                qDebug() <<" vkladam "<<ulica->x1 <<ulica->y1;

            }else {
                myDialog *dialog = new myDialog("Ulica nenadväzuje na predchádzajúcu, skús znovu ");
                dialog->exec();
                delete dialog;
                return;
            }

        }
        scene->indexPridavanejUlice++;
        ulica->ulicaItem->setPen(QPen(Qt::yellow,6));
        linka->trasaLinky->zoznamUlicLinky->insert(index,ulica);
        linkyNaZmenu[0].miestoVLozenia = miestoVlozenia;
        for (int i = 0 ; i<zoznamAutobusov.size();i++){
            if (zoznamAutobusov.value(i)->nazovLinky == linka->nazovLinky){
                if (zoznamAutobusov.value(i)->index > miestoVlozenia){
                    zoznamAutobusov.value(i)->index++;
                }
            }
        }
}
/**
 * @brief MainWindow::ukonciPridavanieObchadzky Ukončí pridávanie ulíc do linky
 *  @details Ak nie je linka prepojená ( obchádzka nespojila ulice ktoré boli rozdelené) tak užívateľovi nedovolí ukončiť pridávanie obchadzky. Inak skontroluje či ide o poslednú linku
 *  na ktorej vznikla prekážka a ak nie tak začne nový cyklus pridávania uĺic do obchádzky.
 */
void MainWindow::ukonciPridavanieObchadzky()
{
    bool pokracuj = false;
    if (linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice)->x1 == linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice-1)->x1
            &&  linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice)->y1 == linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice-1)->y1 ){
        pokracuj = true;
    } else if (linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice)->x1 == linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice-1)->x2
               &&  linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice)->y1 == linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice-1)->y2 ){
        pokracuj = true;
    } else if (linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice)->x2 == linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice-1)->x2
               &&  linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice)->y2 == linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice-1)->y2 ){
        pokracuj = true;
    } else if (linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice)->x2 == linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice-1)->x1
               &&  linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice)->y2 == linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(scene->indexPridavanejUlice-1)->y1 ){
        pokracuj  = true;
    }

    if (!pokracuj){
        myDialog *dialog1 = new myDialog("ULICE MUSIA BYT SPOJENE");
        dialog1->exec();
        myDialog *dialog = new myDialog(linkyNaZmenu[0].linka);
        dialog->show();
        connect(dialog,&myDialog::obchadzkaUkoncena,this,&MainWindow::ukonciPridavanieObchadzky);
        return;
    }
    nenaklikane=false;
    scene->klikamObchadzku = false;
    for (int i = 0 ; i<zoznamAutobusov.size();i++){
        if (zoznamAutobusov.value(i)->nazovLinky == linkyNaZmenu[0].linka->nazovLinky){
            if (zoznamAutobusov.value(i)->index >linkyNaZmenu[0].miestoVLozenia){
                zoznamAutobusov.value(i)->index--;
            }
        }
    }
    linkyNaZmenu.removeFirst();
    qDebug() << "Mazem prve zo zonamu" << linkyNaZmenu.size();
    for (QMap<int,ulicaClass*>::const_iterator i = zoznamUlic.constBegin();i !=zoznamUlic.constEnd();++i){
        i.value()->ulicaItem->setPen(QPen(Qt::black,4));
    }


    if (linkyNaZmenu.size() >0){
        for (int k = 0 ; k< linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->size();k++){
            linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->value(k)->ulicaItem->setPen(QPen(Qt::yellow,6));
        }
        linkyNaZmenu[0].uzatvaranaUlica->ulicaItem->setPen(QPen(Qt::red));
        scene->addLine((linkyNaZmenu[0].uzatvaranaUlica->x1+linkyNaZmenu[0].uzatvaranaUlica->x2)/2+10,(linkyNaZmenu[0].uzatvaranaUlica->y1+linkyNaZmenu[0].uzatvaranaUlica->y2)/2+10,(linkyNaZmenu[0].uzatvaranaUlica->x1+linkyNaZmenu[0].uzatvaranaUlica->x2)/2-10,(linkyNaZmenu[0].uzatvaranaUlica->y1+linkyNaZmenu[0].uzatvaranaUlica->y2)/2-10,QPen(Qt::red,3));
        scene->addLine((linkyNaZmenu[0].uzatvaranaUlica->x1+linkyNaZmenu[0].uzatvaranaUlica->x2)/2-10,(linkyNaZmenu[0].uzatvaranaUlica->y1+linkyNaZmenu[0].uzatvaranaUlica->y2)/2+10,(linkyNaZmenu[0].uzatvaranaUlica->x1+linkyNaZmenu[0].uzatvaranaUlica->x2)/2+10,(linkyNaZmenu[0].uzatvaranaUlica->y1+linkyNaZmenu[0].uzatvaranaUlica->y2)/2-10,QPen(Qt::red,3));

        myDialog *dialog = new myDialog(linkyNaZmenu[0].linka);
        dialog->show();
        connect(dialog,&myDialog::obchadzkaUkoncena,this,&MainWindow::ukonciPridavanieObchadzky);
        linkyNaZmenu[0].linka->trasaLinky->zoznamUlicLinky->removeAt(linkyNaZmenu[0].indexUliceNaLinke);
        scene->klikamObchadzku = true;
        scene->menenaLinka = linkyNaZmenu[0].linka;
        scene->indexPridavanejUlice = linkyNaZmenu[0].indexUliceNaLinke;
        nenaklikane = true;

        int pocetZastavok = 0;
        bool zniz = false;
        if(linkyNaZmenu[0].indexUliceNaLinke == 0){
            linkyNaZmenu[0].indexUliceNaLinke =1;
            zniz = true;
        }
        for (int i = 0 ;i < linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok;i++){
            for (int j = 0;j < linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->size();j++){
                if (linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(i).x() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->value(j).first->X
                        && linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(i).y() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->value(j).first->Y)
                    pocetZastavok++;
            }
        }
        if (zniz){
            linkyNaZmenu[0].indexUliceNaLinke =0;
        }

        qDebug() << "Mazem bod " <<linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok) <<" s indexom "<<linkyNaZmenu[0].indexUliceNaLinke  <<" "<<pocetZastavok;

        linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->removeAt(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok);

        for (int i = 0; i < linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->size();i++){
            if (linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok -1).x() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->value(i).first->X
                    && linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok -1).y() == linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->value(i).first->Y ){

                linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->removeAt(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok -1);
                linkyNaZmenu[0].linka->trasaLinky->zastavkyNaLince->removeAt(i);
                scene->indexPridavanejUlice = (linkyNaZmenu[0].indexUliceNaLinke)-1;
                qDebug() << "VYMAZAL SOM ZASTAVKU "<<linkyNaZmenu[0].linka->trasaLinky->bodyPohybu->value(linkyNaZmenu[0].indexUliceNaLinke+pocetZastavok -1);


            }
        }



    }


}

/**
 * @brief MainWindow::odoberUlicuZLinky odoberie ulicu z linky
 * @param linka linka z ktorej sa odoberá daná ulica
 * @param ulica odoberaná ulica
 */
void MainWindow::odoberUlicuZLinky(vecItem *linka, ulicaClass *ulica)
{
    int indexUliceNaLinke;
    for (int i =0;i<linka->trasaLinky->zoznamUlicLinky->size();i++){
        if (ulica == linka->trasaLinky->zoznamUlicLinky->value(i)){
            indexUliceNaLinke = i;
        }
    }
    ulica->ulicaItem->setPen(QPen(Qt::red));
    scene->addLine((ulica->x1+ulica->x2)/2+10,(ulica->y1+ulica->y2)/2+10,(ulica->x1+ulica->x2)/2-10,(ulica->y1+ulica->y2)/2-10,QPen(Qt::red,3));
    scene->addLine((ulica->x1+ulica->x2)/2-10,(ulica->y1+ulica->y2)/2+10,(ulica->x1+ulica->x2)/2+10,(ulica->y1+ulica->y2)/2-10,QPen(Qt::red,3));

    linka->trasaLinky->zoznamUlicLinky->removeAt(indexUliceNaLinke);
    int pocetZastavok = 0;
    for (int i = 0 ;i < indexUliceNaLinke+pocetZastavok;i++){
        for (int j = 0;j < linka->trasaLinky->zastavkyNaLince->size();j++){
            if (linka->trasaLinky->bodyPohybu->value(i).x() == linka->trasaLinky->zastavkyNaLince->value(j).first->X
                    && linka->trasaLinky->bodyPohybu->value(i).y() == linka->trasaLinky->zastavkyNaLince->value(j).first->Y)
                pocetZastavok++;
        }
    }
    qDebug() << "Mazem bod " <<linka->trasaLinky->bodyPohybu->value(indexUliceNaLinke+pocetZastavok) <<" s indexom "<<indexUliceNaLinke  <<" "<<pocetZastavok;
    linka->trasaLinky->bodyPohybu->removeAt(indexUliceNaLinke+pocetZastavok);
    if (scene->indexPridavanejUlice > indexUliceNaLinke+pocetZastavok){
        scene->indexPridavanejUlice = indexUliceNaLinke+pocetZastavok;
    }

    for (int i = 0; i < linka->trasaLinky->zastavkyNaLince->size();i++){
        if (linka->trasaLinky->bodyPohybu->value(indexUliceNaLinke+pocetZastavok -1).x() == linka->trasaLinky->zastavkyNaLince->value(i).first->X
                && linka->trasaLinky->bodyPohybu->value(indexUliceNaLinke+pocetZastavok -1).y() == linka->trasaLinky->zastavkyNaLince->value(i).first->Y ){

           linka->trasaLinky->bodyPohybu->removeAt(indexUliceNaLinke+pocetZastavok -1);
           linka->trasaLinky->zastavkyNaLince->removeAt(i);
        }
    }
}

/**
 * @brief MainWindow::odoberZastavkuZLinky odoberie zastávku z linky pri vytváraní obchadzky
 * @param linka linka z ktorej sa odoberie zastávka
 * @param zastavka odoberaná zastávka
 */
void MainWindow::odoberZastavkuZLinky(vecItem *linka, zastavkaClass * zastavka)
{
    for (int i = 0;i < linka->trasaLinky->zastavkyNaLince->size();i++){
        if (zastavka ==linka->trasaLinky->zastavkyNaLince->value(i).first){
            linka->trasaLinky->zastavkyNaLince->removeAt(i);
        }
    }
    for (int i = 0; i < linka->trasaLinky->bodyPohybu->size();i++){
        if (zastavka->X == linka->trasaLinky->bodyPohybu->value(i).x() && zastavka->Y == linka->trasaLinky->bodyPohybu->value(i).y()){
            linka->trasaLinky->bodyPohybu->removeAt(i);
        }
    }

}


/**
* @brief MainWindow::zmenPopisZastavky Slot, ktory zachytava signal o kliknuti na ulicu, zobrazi info o ulici
* @param trieda kliknutej zastavky
*/
void MainWindow::zmenPopisZastavky(zastavkaClass *zastavka)
{
   QString textik ;
   QTextStream text(&textik);
   qDebug() <<zastavka->ID_zastavky;
   text <<"ID zastavky :" <<zastavka->ID_zastavky <<"<br> Nazov zastavky :"<< zastavka->nazovZastavky <<"<br>Pozicia :" <<zastavka->X <<zastavka->Y;
    ui->infoLabel->setText(textik);
    autobusInfo = nullptr;
}

void MainWindow::obnovInfo()
{
    if(autobusInfo != nullptr && aktualniZastavka != nullptr)
    {
        if(aktualniZastavka->second < time){
            zmenPopisAutbobusu(autobusInfo);
        }

    }
}

/**
* @brief MainWindow::zmenPopisAutbobusu  zobrazi info o kliknutom autobuse
* @param autobus kliknuty autobus
*/
void MainWindow::zmenPopisAutbobusu(autobusClass *autobus)
{
    autobusInfo = autobus;
    QString textik ;
    QTextStream text(&textik);

    text << "ID dalsieho bodu:" <<autobus->dalsiBod.x() << autobus->dalsiBod.y()<< "<br> Moje meskanie: " << autobus->meskanie <<" minut<br>"<<"Idem po ulici" << autobus->zoznamUlicLinky->value(autobus->index-autobus->indexZastavky)->ID_ulice;
    text << "<br>Linka: " << linky->linkaBusu(autobus->MojeID) << "<br>Zastavky:<br>";

    for(int i = 0; i < autobus->zastavkyNaLince.size(); i++){
        if(autobus->zastavkyNaLince.value(i).second < time){
            text << "<font color = 'gray'>";
            text << QTime::fromMSecsSinceStartOfDay(autobus->zastavkyNaLince.value(i).second * 1000).toString("hh:mm:ss");
            text << autobus->zastavkyNaLince.value(i).first->nazovZastavky << "</font><br>";
            if(i + 1 != autobus->zastavkyNaLince.size()){
                aktualniZastavka = &(autobus->zastavkyNaLince[i+1]);
            }
            else{
                aktualniZastavka = nullptr;
            }
        }
        else{
            text << QTime::fromMSecsSinceStartOfDay(autobus->zastavkyNaLince.value(i).second * 1000).toString("hh:mm:ss");
            text << autobus->zastavkyNaLince.value(i).first->nazovZastavky << "<br>";
        }
    }
    ui->infoLabel->setTextFormat(Qt::RichText);
    ui->infoLabel->setText(textik);
    //ui->infoLabel->setStyleSheet("QLabel{color: red;}");


    for (int i = 0; i < autobus->zoznamUlicLinky->size();i++){
        autobus->zoznamUlicLinky->value(i)->ulicaItem->setPen(QPen(Qt::red,6));
    }
}

/**
 * @brief MainWindow::generateBusStops pridá  do scény a zoznamu zastávok autobusové zastávky
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

        auto *zastavka = new zastavkaClass(x,y,nazov,ID);
        scene->addItem(zastavka->zastavkaItem);
        zoznamZastavok.insert(ID,zastavka);

        line = instream.readLine(50);
    }
    file.close();
    return;

}


/**
 * @brief MainWindow::createMap pridá na scénu ulice zo súboru
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
