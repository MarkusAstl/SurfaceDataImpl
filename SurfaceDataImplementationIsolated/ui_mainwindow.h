/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
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
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QLabel *Load_label;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *SelectLoadingFileButton;
    QLabel *LoadingFileLabel;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *LoadingButton;
    QPushButton *StopLoadingButton;
    QSpacerItem *verticalSpacer;
    QLabel *ReadLabel;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *SelectReadingFileButton;
    QLabel *ReadingFileLabel;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *StartReadingButton;
    QPushButton *StopReadingButton;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *StartPhaseRecognitionButton;
    QLabel *phase_text;
    QLabel *phaseLabel;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLabel *fc_label;
    QSlider *fc_slider;
    QMenuBar *menubar;
    QMenu *menuControl_Window;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(531, 470);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        widget = new QWidget(centralwidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(30, 10, 281, 244));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Load_label = new QLabel(widget);
        Load_label->setObjectName(QStringLiteral("Load_label"));

        verticalLayout->addWidget(Load_label);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        SelectLoadingFileButton = new QPushButton(widget);
        SelectLoadingFileButton->setObjectName(QStringLiteral("SelectLoadingFileButton"));

        horizontalLayout_6->addWidget(SelectLoadingFileButton);

        LoadingFileLabel = new QLabel(widget);
        LoadingFileLabel->setObjectName(QStringLiteral("LoadingFileLabel"));

        horizontalLayout_6->addWidget(LoadingFileLabel);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        LoadingButton = new QPushButton(widget);
        LoadingButton->setObjectName(QStringLiteral("LoadingButton"));

        horizontalLayout_5->addWidget(LoadingButton);

        StopLoadingButton = new QPushButton(widget);
        StopLoadingButton->setObjectName(QStringLiteral("StopLoadingButton"));
        StopLoadingButton->setEnabled(false);

        horizontalLayout_5->addWidget(StopLoadingButton);


        verticalLayout->addLayout(horizontalLayout_5);

        verticalSpacer = new QSpacerItem(20, 17, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        ReadLabel = new QLabel(widget);
        ReadLabel->setObjectName(QStringLiteral("ReadLabel"));

        verticalLayout->addWidget(ReadLabel);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        SelectReadingFileButton = new QPushButton(widget);
        SelectReadingFileButton->setObjectName(QStringLiteral("SelectReadingFileButton"));

        horizontalLayout_4->addWidget(SelectReadingFileButton);

        ReadingFileLabel = new QLabel(widget);
        ReadingFileLabel->setObjectName(QStringLiteral("ReadingFileLabel"));

        horizontalLayout_4->addWidget(ReadingFileLabel);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        StartReadingButton = new QPushButton(widget);
        StartReadingButton->setObjectName(QStringLiteral("StartReadingButton"));
        StartReadingButton->setEnabled(false);

        horizontalLayout_3->addWidget(StartReadingButton);

        StopReadingButton = new QPushButton(widget);
        StopReadingButton->setObjectName(QStringLiteral("StopReadingButton"));
        StopReadingButton->setEnabled(false);

        horizontalLayout_3->addWidget(StopReadingButton);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        StartPhaseRecognitionButton = new QPushButton(widget);
        StartPhaseRecognitionButton->setObjectName(QStringLiteral("StartPhaseRecognitionButton"));
        StartPhaseRecognitionButton->setEnabled(false);

        horizontalLayout_2->addWidget(StartPhaseRecognitionButton);

        phase_text = new QLabel(widget);
        phase_text->setObjectName(QStringLiteral("phase_text"));
        phase_text->setMaximumSize(QSize(1777197, 16777215));
        phase_text->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout_2->addWidget(phase_text);

        phaseLabel = new QLabel(widget);
        phaseLabel->setObjectName(QStringLiteral("phaseLabel"));

        horizontalLayout_2->addWidget(phaseLabel);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(widget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        fc_label = new QLabel(widget);
        fc_label->setObjectName(QStringLiteral("fc_label"));

        horizontalLayout->addWidget(fc_label);


        verticalLayout->addLayout(horizontalLayout);

        fc_slider = new QSlider(widget);
        fc_slider->setObjectName(QStringLiteral("fc_slider"));
        fc_slider->setEnabled(false);
        fc_slider->setMinimum(0);
        fc_slider->setMaximum(100);
        fc_slider->setSliderPosition(0);
        fc_slider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(fc_slider);

        MainWindow->setCentralWidget(centralwidget);
        SelectReadingFileButton->raise();
        ReadingFileLabel->raise();
        StartReadingButton->raise();
        StopReadingButton->raise();
        StartPhaseRecognitionButton->raise();
        phase_text->raise();
        phaseLabel->raise();
        fc_slider->raise();
        label->raise();
        fc_label->raise();
        ReadLabel->raise();
        Load_label->raise();
        LoadingButton->raise();
        StopLoadingButton->raise();
        fc_slider->raise();
        fc_label->raise();
        SelectLoadingFileButton->raise();
        LoadingFileLabel->raise();
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 531, 20));
        menuControl_Window = new QMenu(menubar);
        menuControl_Window->setObjectName(QStringLiteral("menuControl_Window"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuControl_Window->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        Load_label->setText(QApplication::translate("MainWindow", " Load avg. Surface Data (Planning CT)", Q_NULLPTR));
        SelectLoadingFileButton->setText(QApplication::translate("MainWindow", "Select File", Q_NULLPTR));
        LoadingFileLabel->setText(QString());
        LoadingButton->setText(QApplication::translate("MainWindow", "Load", Q_NULLPTR));
        StopLoadingButton->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        ReadLabel->setText(QApplication::translate("MainWindow", "Surface Data (Real Time)", Q_NULLPTR));
        SelectReadingFileButton->setText(QApplication::translate("MainWindow", "Select File", Q_NULLPTR));
        ReadingFileLabel->setText(QString());
        StartReadingButton->setText(QApplication::translate("MainWindow", " Start Reading", Q_NULLPTR));
        StopReadingButton->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        StartPhaseRecognitionButton->setText(QApplication::translate("MainWindow", "Start Phase Recognition", Q_NULLPTR));
        phase_text->setText(QApplication::translate("MainWindow", "Phase:", Q_NULLPTR));
        phaseLabel->setText(QApplication::translate("MainWindow", "0", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Set Cutoff Frequency:", Q_NULLPTR));
        fc_label->setText(QApplication::translate("MainWindow", "0.00 Hz", Q_NULLPTR));
        menuControl_Window->setTitle(QApplication::translate("MainWindow", "Control Window", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
