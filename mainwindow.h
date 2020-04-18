#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createMap();


private:
    Ui::MainWindow *ui;

private slots:
    void zoomIN();
    void zoomOUT();
    void zoomSLider(int value);
    void resetView();
};
#endif // MAINWINDOW_H
