/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QPushButton *zoomINBtn;
    QPushButton *zoomOUTBtn;
    QPushButton *resetBtn;
    QVBoxLayout *verticalLayout_3;
    QLabel *labelZoom;
    QSlider *zoomSlider;
    QPushButton *startBtn;
    QPushButton *pridajBtn;
    QSpacerItem *horizontalSpacer_2;
    QLabel *labelTime;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *graphicsView;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1355, 1019);
        MainWindow->setMinimumSize(QSize(1355, 1019));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 50, -1);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(-1, -1, 0, -1);
        zoomINBtn = new QPushButton(centralwidget);
        zoomINBtn->setObjectName(QStringLiteral("zoomINBtn"));

        verticalLayout_2->addWidget(zoomINBtn);

        zoomOUTBtn = new QPushButton(centralwidget);
        zoomOUTBtn->setObjectName(QStringLiteral("zoomOUTBtn"));

        verticalLayout_2->addWidget(zoomOUTBtn);

        resetBtn = new QPushButton(centralwidget);
        resetBtn->setObjectName(QStringLiteral("resetBtn"));

        verticalLayout_2->addWidget(resetBtn);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(20, -1, 20, 20);
        labelZoom = new QLabel(centralwidget);
        labelZoom->setObjectName(QStringLiteral("labelZoom"));

        verticalLayout_3->addWidget(labelZoom, 0, Qt::AlignHCenter);

        zoomSlider = new QSlider(centralwidget);
        zoomSlider->setObjectName(QStringLiteral("zoomSlider"));
        zoomSlider->setMinimum(1);
        zoomSlider->setMaximum(100);
        zoomSlider->setSliderPosition(10);
        zoomSlider->setOrientation(Qt::Horizontal);

        verticalLayout_3->addWidget(zoomSlider);


        horizontalLayout->addLayout(verticalLayout_3);

        startBtn = new QPushButton(centralwidget);
        startBtn->setObjectName(QStringLiteral("startBtn"));

        horizontalLayout->addWidget(startBtn);

        pridajBtn = new QPushButton(centralwidget);
        pridajBtn->setObjectName(QStringLiteral("pridajBtn"));

        horizontalLayout->addWidget(pridajBtn);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        labelTime = new QLabel(centralwidget);
        labelTime->setObjectName(QStringLiteral("labelTime"));

        horizontalLayout->addWidget(labelTime);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, -1, -1, 0);
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

        horizontalLayout_2->addWidget(graphicsView);

        plainTextEdit = new QPlainTextEdit(centralwidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setEnabled(true);
        plainTextEdit->setMaximumSize(QSize(310, 16777215));
        plainTextEdit->setContextMenuPolicy(Qt::DefaultContextMenu);

        horizontalLayout_2->addWidget(plainTextEdit);


        verticalLayout->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1355, 20));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        zoomINBtn->setText(QApplication::translate("MainWindow", "ZoomIN", 0));
        zoomOUTBtn->setText(QApplication::translate("MainWindow", "ZoomOUT", 0));
        resetBtn->setText(QApplication::translate("MainWindow", "Reset", 0));
        labelZoom->setText(QApplication::translate("MainWindow", "Zoom: 1.0", 0));
        startBtn->setText(QApplication::translate("MainWindow", "\305\240tart", 0));
        pridajBtn->setText(QApplication::translate("MainWindow", "Pridaj Autobus", 0));
        labelTime->setText(QApplication::translate("MainWindow", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
