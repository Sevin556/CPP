#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QWidget>
#include "ulicaclass.h"
#include "linkaclass.h"

class myDialog:public QDialog
{
    Q_OBJECT
public:
    explicit myDialog(ulicaClass * ulica,QWidget *parent = nullptr);
    explicit myDialog(linkaClass * linky,QWidget *parent = nullptr);
    explicit myDialog(bool nastavujemObchadzku,ulicaClass * ulica,QWidget *parent = nullptr);
    explicit myDialog(vecItem *linka,QWidget* parent =nullptr);
    explicit myDialog(QWidget *parent=nullptr);
    ulicaClass * ulica;


signals:
    void naklikajUlicu(ulicaClass *ulica);
    void obchadzkaUkoncena();


private slots:
    void nastavNormalnu();
    void nastavSpicku();
    void nastavZapchu();
    void nastavObchadzku();
    void obchadzkaHotova();
};

#endif // MYDIALOG_H
