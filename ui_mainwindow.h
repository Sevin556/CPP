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
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
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
    QVBoxLayout *verticalLayout_4;
    QPushButton *startstopBtn;
    QPushButton *jizdniRadBtn;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_2;
    QLineEdit *lineEditTime;
    QLabel *label;
    QSlider *speedSlider;
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *graphicsView;
    QVBoxLayout *verticalLayout_6;
    QLabel *labelNadpisInfa;
    QLabel *infoLabel;
    QSpacerItem *verticalSpacer;
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

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        startstopBtn = new QPushButton(centralwidget);
        startstopBtn->setObjectName(QStringLiteral("startstopBtn"));

        verticalLayout_4->addWidget(startstopBtn);

        jizdniRadBtn = new QPushButton(centralwidget);
        jizdniRadBtn->setObjectName(QStringLiteral("jizdniRadBtn"));

        verticalLayout_4->addWidget(jizdniRadBtn);


        horizontalLayout->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_2);

        lineEditTime = new QLineEdit(centralwidget);
        lineEditTime->setObjectName(QStringLiteral("lineEditTime"));

        verticalLayout_5->addWidget(lineEditTime);

        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label);

        speedSlider = new QSlider(centralwidget);
        speedSlider->setObjectName(QStringLiteral("speedSlider"));
        speedSlider->setMinimum(0);
        speedSlider->setMaximum(49);
        speedSlider->setValue(30);
        speedSlider->setOrientation(Qt::Horizontal);
        speedSlider->setInvertedAppearance(false);
        speedSlider->setInvertedControls(true);

        verticalLayout_5->addWidget(speedSlider);


        horizontalLayout->addLayout(verticalLayout_5);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, -1, -1, 0);
        graphicsView = new QGraphicsView(centralwidget);
        graphicsView->setObjectName(QStringLiteral("graphicsView"));
        graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

        horizontalLayout_2->addWidget(graphicsView);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(5, 10, -1, -1);
        labelNadpisInfa = new QLabel(centralwidget);
        labelNadpisInfa->setObjectName(QStringLiteral("labelNadpisInfa"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(labelNadpisInfa->sizePolicy().hasHeightForWidth());
        labelNadpisInfa->setSizePolicy(sizePolicy);
        labelNadpisInfa->setMinimumSize(QSize(258, 17));
        labelNadpisInfa->setTextFormat(Qt::AutoText);

        verticalLayout_6->addWidget(labelNadpisInfa, 0, Qt::AlignLeft|Qt::AlignTop);

        infoLabel = new QLabel(centralwidget);
        infoLabel->setObjectName(QStringLiteral("infoLabel"));

        verticalLayout_6->addWidget(infoLabel);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout_6);


        verticalLayout->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 1355, 22));
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
        startstopBtn->setText(QApplication::translate("MainWindow", "\305\240tart", 0));
        jizdniRadBtn->setText(QApplication::translate("MainWindow", "J\303\255zdn\303\255 \305\231\303\241d", 0));
        label_2->setText(QApplication::translate("MainWindow", "\304\214as:", 0));
        label->setText(QApplication::translate("MainWindow", "Rychlost:", 0));
        labelNadpisInfa->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:16pt; text-decoration: underline;\">Inform\303\241cie o kliknutom:</span></p></body></html>", 0));
        infoLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
