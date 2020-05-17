
/*
 *Autori súboru:
 * Ivan Halomi xhalom00
 * Michal Sova xsovam00
 * Projekt do ICP 2019/2020
 */

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
    explicit myDialog(QString text,QWidget *parent=nullptr);
    /**
     * @brief ulica kliknutá ulica, či už pri obchádzke alebo zmene premávky
     */
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
