#include "mydialog.h"
#include <QDialogButtonBox>
#include <QLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <ulicaclass.h>

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
