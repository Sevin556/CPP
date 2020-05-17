
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

#include "mydialog.h"
#include <QDialogButtonBox>
#include <QLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <ulicaclass.h>
#include <QTableWidget>
#include <QTime>
#include <QDebug>

/**
 * @brief myDialog::myDialog Konštruktor pre dialógové okno pri určovaní premavky na ulici
 * @param ulica kliknutá ulica, o ktorej má byť zobrazené info/zmenená rýchlosť premávky
 * @param parent objekt z ktorého dedí
 */
myDialog::myDialog(ulicaClass *ulica, QWidget *parent):
    QDialog(parent)
{

    this->ulica = ulica;
    setWindowTitle(ulica->nazovUlice);


    QVBoxLayout *layout = new QVBoxLayout;
    this->setLayout(layout);
    QLabel *label = new QLabel;
    label ->setText("Zvoľ rýchlost premávky na kliknutej ulici");
    layout->addWidget(label);
        //normalna rychlost button
    QPushButton *button1 = new QPushButton();
    button1->setText("Normalna");

        //spicka button
    QPushButton *button2 = new QPushButton();
    button2->setText("Špička");
        //zapcha button
    QPushButton *button3 = new QPushButton();
    button3->setText("Zápcha");

    layout->addWidget(button1);
    layout->addWidget(button2);
    layout->addWidget(button3);
        // create button box
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        //prepojenie signalov tlacitok na sloty
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(button1,&QPushButton::clicked,this,&myDialog::nastavNormalnu);
    connect(button2,&QPushButton::clicked,this,&myDialog::nastavSpicku);
    connect(button3,&QPushButton::clicked,this,&myDialog::nastavZapchu);
    layout->addWidget(buttonBox);
}

/**
 * @brief myDialog::myDialog  Konštruktor pre dialógové okno, ktoré zobrazí informácie o jízdnom řáde
 * @param linky zoznam všetkých autobusových liniek o ktorých chcem zobraziť informácie
 * @param parent objekt z ktorého dedí
 */
myDialog::myDialog(linkaClass* linky, QWidget *parent):
    QDialog(parent)
{
    // nastaveni dialogoveho okna
    setWindowTitle("Jízdní řád");
    QVBoxLayout* dialogLayout = new QVBoxLayout;
    this->setLayout(dialogLayout);
    this->resize(600, 500);
    QTabWidget* tabWidget = new QTabWidget;


    // pocet (stejnych) linek
    /*for(int i = 0; i < linky->seznamLinek.size(); i++){
        if(!list.contains(linky->seznamLinek[i].first)){
            list.append(linky->seznamLinek[i].first);
        }
    }*/

    for( int poradi = 0; poradi < linky->seznamLinek.size(); poradi++) {

        // inicializace zalozky s danou linkou
        QWidget* tab = new QWidget;
        QVBoxLayout* tabLayout = new QVBoxLayout(tab);
        tab->setLayout(tabLayout);
        QTableWidget* table = new QTableWidget(tab);

        // nastaveni sloupcu a radku
        int colCount = linky->seznamLinek[poradi]->zoznamOdchodov.size() + 1;
        int rowCount = linky->seznamLinek[poradi]->trasaLinky->zastavkyNaLince->size();
        table->setColumnCount(colCount);
        table->setRowCount(rowCount);



        // pres vsechny radky tabulky -> zastavky
        for(int indexZastavky = 0; indexZastavky < rowCount; indexZastavky++)
        {
            // nazev zastavky
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setText(linky->seznamLinek[poradi]->trasaLinky->zastavkyNaLince->value(indexZastavky).first->nazovZastavky);
            table->setItem(indexZastavky, 0, item);

            // jednotlive casy
            for(int indexOdchodu = 0; indexOdchodu < linky->seznamLinek[poradi]->zoznamOdchodov.size(); indexOdchodu++)
            {
                // vytvoreni polozky
                QTableWidgetItem* item = new QTableWidgetItem;
                int cas = linky->seznamLinek[poradi]->zoznamOdchodov[indexOdchodu];
                cas = cas + linky->seznamLinek[poradi]->trasaLinky->zastavkyNaLince->value(indexZastavky).second;
                // prevedeni sekund na format hh:mm:ss
                item->setText(QTime::fromMSecsSinceStartOfDay(cas * 1000).toString("hh:mm:ss"));
                // vlozeni polozky do tabulky (indexOdchodu + 1 kvuli nazvu zastavky)
                table->setItem(indexZastavky, indexOdchodu + 1, item);
            }
        }
        // zaneseni do listy zalozek
        tabLayout->addWidget(table);
        tabWidget->addTab(tab, linky->seznamLinek[poradi]->nazovLinky);
    }
    dialogLayout->addWidget(tabWidget);
    QPushButton* okBtn = new QPushButton;
    okBtn->setText("Ok");
    dialogLayout->addWidget(okBtn);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
}


/**
 * @brief myDialog::myDialog  Konštruktor pre dialógové okno pre uzavretie ulice a začatie obchádzky
 * @param nastavujemObchadzku slúži na rozlíšenie od konštruktora pre rýchlosť premávky
 * @param ulica kliknutá ulica, ktorá môže byť uzavretá
 * @param parent objekt z ktorého dedí
 */
myDialog::myDialog(bool nastavujemObchadzku, ulicaClass *ulica, QWidget *parent):
    QDialog(parent)
{
    this->ulica = ulica;
    setWindowTitle(ulica->nazovUlice);


    QVBoxLayout *layout = new QVBoxLayout;
    this->setLayout(layout);

    QLabel *label1 = new QLabel;
    label1->setText("Uzavrie cestu a zacne pridavat obchadzku (prave kliknutie na cestu");
    layout->addWidget(label1);
    QPushButton *button = new QPushButton();
    button->setText("Uzavri cestu a nastav obchádzku");
    layout->addWidget(button);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Cancel);
    layout->addWidget(buttonBox);

        //prepojenie signalov tlacitok na sloty
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    connect(button,&QPushButton::clicked,this,&myDialog::nastavObchadzku);
}

/**
 * @brief myDialog::myDialog  Konštruktor pre dialógové okno pre zobrazenie a ukončenie nastavovania obchádzky na určitej linke
 * @param linka linka na ktorej sa obchádzka nastavuje
 * @param parent objekt z ktorého dedí
 */
myDialog::myDialog(vecItem *linka, QWidget *parent):
    QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    this->setLayout(layout);

    QLabel * label = new QLabel;
    label->setText("Naklikaj obchádzku na linke "+linka->nazovLinky+" a po skonceni stlac Hotovo");
    layout->addWidget(label);

    QPushButton *button = new QPushButton();
    button->setText("Hotovo");
    layout->addWidget(button);
    connect(button,&QPushButton::clicked,this,&myDialog::obchadzkaHotova);
}

myDialog::myDialog(QString text,QWidget *parent):
    QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    this->setLayout(layout);

    QLabel * label = new QLabel;
    label->setText(text);
    layout->addWidget(label);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    layout->addWidget(buttonBox);
        //prepojenie signalov tlacitok na sloty
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

/**
 * @brief myDialog::nastavNormalnu nastaví normálnu rýchlosť premávky
 */
void myDialog::nastavNormalnu()
{
    ulica->rychlostPremavky = 1;
    accept();
}
/**
 * @brief myDialog::nastavSpicku nastaví spomalenú rýchlosť premávky
 */
void myDialog::nastavSpicku()
{
    ulica->rychlostPremavky = 3;
    accept();
}

/**
 * @brief myDialog::nastavZapchu nastaví najpomalšíu rýchlosť premávky
 */
void myDialog::nastavZapchu()
{
    ulica->rychlostPremavky = 5;
    accept();
}

/**
 * @brief myDialog::nastavObchadzku signál, slúži na oznámenie hlavnému oknu, že sa ide nastavovať obchádzka
 */
void myDialog::nastavObchadzku()
{
    naklikajUlicu(ulica);
    accept();
}

/**
 * @brief myDialog::obchadzkaHotova signál, slúži na oznámenie hlavnému oknu, že obchádzka je ukončená
 */
void myDialog::obchadzkaHotova()
{
    obchadzkaUkoncena();
    accept();
}

