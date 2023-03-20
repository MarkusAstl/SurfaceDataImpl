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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_15;
    QVBoxLayout *verticalLayout_12;
    QLabel *ReadLabel_4;
    QHBoxLayout *horizontalLayout_16;
    QPushButton *StartReadingButton_4;
    QPushButton *StopReadingButton_4;
    QGridLayout *gridLayout_28;
    QLabel *label_71;
    QLabel *timeLabel_4;
    QLabel *label_72;
    QLabel *label_73;
    QLabel *amp1Label_4;
    QLabel *label_74;
    QLabel *label_75;
    QLabel *amp2Label_4;
    QLabel *label_76;
    QGridLayout *gridLayout_29;
    QLabel *label_77;
    QHBoxLayout *horizontalLayout_17;
    QPushButton *LoadingButton_4;
    QPushButton *StopLoadingButton_4;
    QGridLayout *gridLayout_30;
    QLabel *label_78;
    QLabel *timeLabelAvg_4;
    QLabel *label_79;
    QLabel *label_80;
    QLabel *amp1LabelAvg_4;
    QLabel *label_81;
    QLabel *label_82;
    QLabel *amp2LabelAvg_4;
    QLabel *label_83;
    QHBoxLayout *horizontalLayout;
    QPushButton *StartPhaseRecognitionButton;
    QLabel *phaseLabel;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLabel *fc_label;
    QSlider *fc_slider;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(473, 317);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(12, 12, 411, 236));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        verticalLayout_12 = new QVBoxLayout();
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        ReadLabel_4 = new QLabel(layoutWidget);
        ReadLabel_4->setObjectName(QStringLiteral("ReadLabel_4"));

        verticalLayout_12->addWidget(ReadLabel_4);

        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        StartReadingButton_4 = new QPushButton(layoutWidget);
        StartReadingButton_4->setObjectName(QStringLiteral("StartReadingButton_4"));

        horizontalLayout_16->addWidget(StartReadingButton_4);

        StopReadingButton_4 = new QPushButton(layoutWidget);
        StopReadingButton_4->setObjectName(QStringLiteral("StopReadingButton_4"));
        StopReadingButton_4->setEnabled(true);

        horizontalLayout_16->addWidget(StopReadingButton_4);


        verticalLayout_12->addLayout(horizontalLayout_16);

        gridLayout_28 = new QGridLayout();
        gridLayout_28->setObjectName(QStringLiteral("gridLayout_28"));
        label_71 = new QLabel(layoutWidget);
        label_71->setObjectName(QStringLiteral("label_71"));

        gridLayout_28->addWidget(label_71, 0, 0, 1, 1);

        timeLabel_4 = new QLabel(layoutWidget);
        timeLabel_4->setObjectName(QStringLiteral("timeLabel_4"));

        gridLayout_28->addWidget(timeLabel_4, 0, 1, 1, 1);

        label_72 = new QLabel(layoutWidget);
        label_72->setObjectName(QStringLiteral("label_72"));

        gridLayout_28->addWidget(label_72, 0, 2, 1, 1);

        label_73 = new QLabel(layoutWidget);
        label_73->setObjectName(QStringLiteral("label_73"));

        gridLayout_28->addWidget(label_73, 1, 0, 1, 1);

        amp1Label_4 = new QLabel(layoutWidget);
        amp1Label_4->setObjectName(QStringLiteral("amp1Label_4"));

        gridLayout_28->addWidget(amp1Label_4, 1, 1, 1, 1);

        label_74 = new QLabel(layoutWidget);
        label_74->setObjectName(QStringLiteral("label_74"));

        gridLayout_28->addWidget(label_74, 1, 2, 1, 1);

        label_75 = new QLabel(layoutWidget);
        label_75->setObjectName(QStringLiteral("label_75"));

        gridLayout_28->addWidget(label_75, 2, 0, 1, 1);

        amp2Label_4 = new QLabel(layoutWidget);
        amp2Label_4->setObjectName(QStringLiteral("amp2Label_4"));

        gridLayout_28->addWidget(amp2Label_4, 2, 1, 1, 1);

        label_76 = new QLabel(layoutWidget);
        label_76->setObjectName(QStringLiteral("label_76"));

        gridLayout_28->addWidget(label_76, 2, 2, 1, 1);


        verticalLayout_12->addLayout(gridLayout_28);


        horizontalLayout_15->addLayout(verticalLayout_12);

        gridLayout_29 = new QGridLayout();
        gridLayout_29->setObjectName(QStringLiteral("gridLayout_29"));
        label_77 = new QLabel(layoutWidget);
        label_77->setObjectName(QStringLiteral("label_77"));

        gridLayout_29->addWidget(label_77, 0, 0, 1, 1);

        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        LoadingButton_4 = new QPushButton(layoutWidget);
        LoadingButton_4->setObjectName(QStringLiteral("LoadingButton_4"));

        horizontalLayout_17->addWidget(LoadingButton_4);

        StopLoadingButton_4 = new QPushButton(layoutWidget);
        StopLoadingButton_4->setObjectName(QStringLiteral("StopLoadingButton_4"));

        horizontalLayout_17->addWidget(StopLoadingButton_4);


        gridLayout_29->addLayout(horizontalLayout_17, 1, 0, 1, 1);

        gridLayout_30 = new QGridLayout();
        gridLayout_30->setObjectName(QStringLiteral("gridLayout_30"));
        label_78 = new QLabel(layoutWidget);
        label_78->setObjectName(QStringLiteral("label_78"));

        gridLayout_30->addWidget(label_78, 0, 0, 1, 1);

        timeLabelAvg_4 = new QLabel(layoutWidget);
        timeLabelAvg_4->setObjectName(QStringLiteral("timeLabelAvg_4"));

        gridLayout_30->addWidget(timeLabelAvg_4, 0, 1, 1, 1);

        label_79 = new QLabel(layoutWidget);
        label_79->setObjectName(QStringLiteral("label_79"));

        gridLayout_30->addWidget(label_79, 0, 2, 1, 1);

        label_80 = new QLabel(layoutWidget);
        label_80->setObjectName(QStringLiteral("label_80"));

        gridLayout_30->addWidget(label_80, 1, 0, 1, 1);

        amp1LabelAvg_4 = new QLabel(layoutWidget);
        amp1LabelAvg_4->setObjectName(QStringLiteral("amp1LabelAvg_4"));

        gridLayout_30->addWidget(amp1LabelAvg_4, 1, 1, 1, 1);

        label_81 = new QLabel(layoutWidget);
        label_81->setObjectName(QStringLiteral("label_81"));

        gridLayout_30->addWidget(label_81, 1, 2, 1, 1);

        label_82 = new QLabel(layoutWidget);
        label_82->setObjectName(QStringLiteral("label_82"));

        gridLayout_30->addWidget(label_82, 2, 0, 1, 1);

        amp2LabelAvg_4 = new QLabel(layoutWidget);
        amp2LabelAvg_4->setObjectName(QStringLiteral("amp2LabelAvg_4"));

        gridLayout_30->addWidget(amp2LabelAvg_4, 2, 1, 1, 1);

        label_83 = new QLabel(layoutWidget);
        label_83->setObjectName(QStringLiteral("label_83"));

        gridLayout_30->addWidget(label_83, 2, 2, 1, 1);


        gridLayout_29->addLayout(gridLayout_30, 2, 0, 1, 1);


        horizontalLayout_15->addLayout(gridLayout_29);


        verticalLayout->addLayout(horizontalLayout_15);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        StartPhaseRecognitionButton = new QPushButton(layoutWidget);
        StartPhaseRecognitionButton->setObjectName(QStringLiteral("StartPhaseRecognitionButton"));

        horizontalLayout->addWidget(StartPhaseRecognitionButton);

        phaseLabel = new QLabel(layoutWidget);
        phaseLabel->setObjectName(QStringLiteral("phaseLabel"));

        horizontalLayout->addWidget(phaseLabel);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_2->addLayout(verticalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        fc_label = new QLabel(layoutWidget);
        fc_label->setObjectName(QStringLiteral("fc_label"));

        horizontalLayout_2->addWidget(fc_label);

        fc_slider = new QSlider(layoutWidget);
        fc_slider->setObjectName(QStringLiteral("fc_slider"));
        fc_slider->setMinimum(0);
        fc_slider->setMaximum(100);
        fc_slider->setSliderPosition(0);
        fc_slider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(fc_slider);


        verticalLayout_2->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 473, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        ReadLabel_4->setText(QApplication::translate("MainWindow", "        Read Surface Data\n"
"             (Real Time)", Q_NULLPTR));
        StartReadingButton_4->setText(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
        StopReadingButton_4->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        label_71->setText(QApplication::translate("MainWindow", "Time:", Q_NULLPTR));
        timeLabel_4->setText(QString());
        label_72->setText(QApplication::translate("MainWindow", "ms", Q_NULLPTR));
        label_73->setText(QApplication::translate("MainWindow", "Amp1:", Q_NULLPTR));
        amp1Label_4->setText(QString());
        label_74->setText(QApplication::translate("MainWindow", "mm", Q_NULLPTR));
        label_75->setText(QApplication::translate("MainWindow", "Amp2:", Q_NULLPTR));
        amp2Label_4->setText(QString());
        label_76->setText(QApplication::translate("MainWindow", "mm", Q_NULLPTR));
        label_77->setText(QApplication::translate("MainWindow", "     Load avg. Surface Data\n"
"            (Planning CT)", Q_NULLPTR));
        LoadingButton_4->setText(QApplication::translate("MainWindow", "Load", Q_NULLPTR));
        StopLoadingButton_4->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        label_78->setText(QApplication::translate("MainWindow", "Time:", Q_NULLPTR));
        timeLabelAvg_4->setText(QString());
        label_79->setText(QApplication::translate("MainWindow", "ms", Q_NULLPTR));
        label_80->setText(QApplication::translate("MainWindow", "Amp1:", Q_NULLPTR));
        amp1LabelAvg_4->setText(QString());
        label_81->setText(QApplication::translate("MainWindow", "mm", Q_NULLPTR));
        label_82->setText(QApplication::translate("MainWindow", "Amp2:", Q_NULLPTR));
        amp2LabelAvg_4->setText(QString());
        label_83->setText(QApplication::translate("MainWindow", "mm", Q_NULLPTR));
        StartPhaseRecognitionButton->setText(QApplication::translate("MainWindow", "Start Phase Recognition", Q_NULLPTR));
        phaseLabel->setText(QApplication::translate("MainWindow", "Phase: ", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Set Cutoff Frequency", Q_NULLPTR));
        fc_label->setText(QApplication::translate("MainWindow", "0.00 Hz", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
