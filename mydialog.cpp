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
    QList<QString> list;


    // pocet (stejnych) linek
    /*for(int i = 0; i < linky->seznamLinek.size(); i++){
        if(!list.contains(linky->seznamLinek[i].first)){
            list.append(linky->seznamLinek[i].first);
        }
    }


    int poradi = 0;
    while(poradi < list.size()) {

        // inicializace zalozky s danou linkou
        QWidget* tab = new QWidget;
        QVBoxLayout* tabLayout = new QVBoxLayout(tab);
        tab->setLayout(tabLayout);
        QTableWidget* table = new QTableWidget(tab);

        // nastaveni sloupcu a radku
        int colCount = 0;
        int rowCount = 0;
        for(int i = 0; i < linky->seznamLinek.size(); i++)
        {
            if(linky->seznamLinek[i].first == list.at(poradi))
            {
                colCount++;
                if(rowCount == 0){
                    rowCount = linky->busList[i]->zastavkyNaLince.size();
                }
            }
        }
        table->setColumnCount(colCount);
        table->setRowCount(rowCount);



        // pres vsechny radky tabulky
        int counterCol = 0;
        for(int indexZastavky = 0; indexZastavky < rowCount; indexZastavky++)
        {
            //pres vsechny polozky seznamu linek
            counterCol = 0;
            for(int indexLinky = 0; indexLinky < linky->seznamLinek.size(); indexLinky++)
            {
                // nazev linky se shoduje s hledanou jinkou
                if(linky->seznamLinek[indexLinky].first == list.at(poradi))
                {
                    // vytvoreni polozky
                    QTableWidgetItem* item = new QTableWidgetItem;
                    if(counterCol == 0)
                    {
                        // nazev zastavky
                        item->setText(linky->busList[indexLinky]->zastavkyNaLince[indexZastavky].first->nazovZastavky);
                    }
                    else
                    {
                        // prevedeni sekund na format hh:mm:ss
                        item->setText(QTime::fromMSecsSinceStartOfDay(linky->busList[indexLinky]->zastavkyNaLince[indexZastavky].second * 1000).toString("hh:mm:ss"));
                    }
                    // vlozeni polozky do tabulky
                    table->setItem(indexZastavky, counterCol, item);
                    counterCol++;
                }
            }
        }


        // zaneseni do listy zalozek
        tabLayout->addWidget(table);
        tabWidget->addTab(tab, list.at(poradi));
        poradi++;
    }
    dialogLayout->addWidget(tabWidget);
    QPushButton* okBtn = new QPushButton;
    okBtn->setText("Ok");
    dialogLayout->addWidget(okBtn);
    connect(okBtn, SIGNAL(clicked()), this, SLOT(accept()));
    */
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
