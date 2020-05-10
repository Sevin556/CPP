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
        int rowCount = linky->seznamLinek[poradi]->trasaLinky->zastavkyNaLince.size();
        table->setColumnCount(colCount);
        table->setRowCount(rowCount);



        // pres vsechny radky tabulky -> zastavky
        for(int indexZastavky = 0; indexZastavky < rowCount; indexZastavky++)
        {
            // nazev zastavky
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setText(linky->seznamLinek[poradi]->trasaLinky->zastavkyNaLince[indexZastavky].first->nazovZastavky);
            table->setItem(indexZastavky, 0, item);

            // jednotlive casy
            for(int indexOdchodu = 0; indexOdchodu < linky->seznamLinek[poradi]->zoznamOdchodov.size(); indexOdchodu++)
            {
                // vytvoreni polozky
                QTableWidgetItem* item = new QTableWidgetItem;
                int cas = linky->seznamLinek[poradi]->zoznamOdchodov[indexOdchodu];
                cas = cas + linky->seznamLinek[poradi]->trasaLinky->zastavkyNaLince[indexZastavky].second;
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


void myDialog::nastavNormalnu()
{
    ulica->rychlostPremavky = 1;
    accept();
}

void myDialog::nastavSpicku()
{
    ulica->rychlostPremavky = 3;
    accept();
}

void myDialog::nastavZapchu()
{
    ulica->rychlostPremavky = 5;
    accept();
}

void myDialog::nastavObchadzku()
{
    naklikajUlicu(ulica);
    accept();
}

void myDialog::obchadzkaHotova()
{
    obchadzkaUkoncena();
    accept();
}

