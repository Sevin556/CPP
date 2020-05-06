#ifndef MYDIALOG_H
#define MYDIALOG_H

#include <QDialog>
#include <QWidget>
#include "ulicaclass.h"

class myDialog:public QDialog
{
    Q_OBJECT
public:
    explicit myDialog(ulicaClass * ulica,QWidget *parent = nullptr);
    ulicaClass * ulica;

private slots:
    void nastavNormalnu();
    void nastavSpicku();
    void nastavZapchu();
};

#endif // MYDIALOG_H
