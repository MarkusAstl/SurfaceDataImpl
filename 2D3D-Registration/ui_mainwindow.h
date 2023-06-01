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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLabel *label_Drr_1;
    QLabel *label_Drr_2;
    QLabel *label_Xray_1;
    QLabel *label_Xray_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QFrame *verticalFrame_3;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_Xray1ListLoad;
    QListWidget *listWidget_Xray1;
    QGridLayout *gridLayout_15;
    QRadioButton *radioButton_Mask11;
    QRadioButton *radioButton_Mask12;
    QGridLayout *gridLayout;
    QSlider *horizontalSlider_Mask1PosV;
    QLabel *label_12;
    QLabel *label_10;
    QSlider *horizontalSlider_Mask1PosH;
    QLabel *label_11;
    QSlider *horizontalSlider_Mask1Prop1;
    QLabel *label_3;
    QSlider *horizontalSlider_Mask1Prop2;
    QGridLayout *gridLayout_10;
    QPushButton *pushButton_LoadMask1;
    QPushButton *pushButton_SaveMask1;
    QLabel *label_Mask1;
    QFrame *verticalFrame_4;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *pushButton_Xray2ListLoad;
    QListWidget *listWidget_Xray2;
    QGridLayout *gridLayout_16;
    QRadioButton *radioButton_Mask21;
    QRadioButton *radioButton_Mask22;
    QGridLayout *gridLayout_2;
    QLabel *label_14;
    QSlider *horizontalSlider_Mask2PosV;
    QSlider *horizontalSlider_Mask2Prop1;
    QLabel *label_13;
    QSlider *horizontalSlider_Mask2PosH;
    QLabel *label_15;
    QSlider *horizontalSlider_Mask2Prop2;
    QLabel *label_27;
    QGridLayout *gridLayout_14;
    QPushButton *pushButton_LoadMask2;
    QPushButton *pushButton_SaveMask2;
    QLabel *label_Mask2;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout_43;
    QVBoxLayout *verticalLayout_2;
    QLabel *label_28;
    QComboBox *comboBox_RtSystem;
    QLabel *label_5;
    QComboBox *comboBox_RegistrationMode;
    QLabel *label_4;
    QComboBox *comboBox_MeritFunction;
    QLabel *label_6;
    QGridLayout *gridLayout_13;
    QCheckBox *checkBox_tx;
    QCheckBox *checkBox_ty;
    QCheckBox *checkBox_tz;
    QCheckBox *checkBox_ry;
    QCheckBox *checkBox_rx;
    QCheckBox *checkBox_rz;
    QLabel *label_20;
    QSlider *horizontalSlider_BackgroundSaturation;
    QGridLayout *gridLayout_12;
    QLabel *label_2;
    QGridLayout *gridLayout_11;
    QLabel *label_26;
    QToolButton *toolButton_DrrTransRotUp;
    QToolButton *toolButton_DrrTransformDown;
    QSlider *horizontalSlider_TransX;
    QSlider *horizontalSlider_RotZ;
    QSlider *horizontalSlider_RotY;
    QSlider *horizontalSlider_TransY;
    QSlider *horizontalSlider_RotX;
    QSlider *horizontalSlider_TransZ;
    QGridLayout *gridLayout_18;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_25;
    QSpacerItem *horizontalSpacer;
    QCheckBox *checkBox_XrayBS;
    QSlider *horizontalSlider_WeightingFactorXray;
    QLabel *label_weightsBS;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton_LoadVolume;
    QPushButton *pushButton_LoadConfig;
    QPushButton *pushButton_SaveConfig;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_Registration_2D3D;
    QPushButton *pushButton_BatchRegistration2D3D;
    QPushButton *pushButton_Test;
    QPushButton *pushButton_SaveDrrs;
    QPushButton *pushButton_CopyTransRot;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_7;
    QVBoxLayout *verticalLayout_9;
    QVBoxLayout *verticalLayout_8;
    QGridLayout *gridLayout_5;
    QDoubleSpinBox *doubleSpinBox_Cam1SourceDistance;
    QDoubleSpinBox *doubleSpinBox_Cam1RotX;
    QDoubleSpinBox *doubleSpinBox_Cam1RotY;
    QDoubleSpinBox *doubleSpinBox_Cam1RotZ;
    QLabel *label_18;
    QLabel *label_17;
    QDoubleSpinBox *doubleSpinBox_Cam1DetectorDistance;
    QGridLayout *gridLayout_8;
    QLabel *label;
    QToolButton *toolButton_Intensity1Up;
    QSlider *horizontalSlider_Intensity1;
    QLabel *label_7;
    QSlider *horizontalSlider_Windowing1Min;
    QSlider *horizontalSlider_Windowing1Max;
    QGridLayout *gridLayout_4;
    QToolButton *toolButton_VoxelFilter1ThresholdSwap;
    QToolButton *toolButton_VoxelFilter1ThresholdUp;
    QLabel *label_16;
    QSlider *horizontalSlider_VoxelFilter1ThresholdLow;
    QSlider *horizontalSlider_VoxelFilter1ThresholdHigh;
    QLabel *label_29;
    QSlider *horizontalSlider_WeightingFactorDrr1;
    QLabel *label_Cam1Settings;
    QVBoxLayout *verticalLayout_11;
    QGridLayout *gridLayout_7;
    QLabel *label_19;
    QLabel *label_22;
    QDoubleSpinBox *doubleSpinBox_Cam2SourceDistance;
    QDoubleSpinBox *doubleSpinBox_Cam2DetectorDistance;
    QDoubleSpinBox *doubleSpinBox_Cam2RotX;
    QDoubleSpinBox *doubleSpinBox_Cam2RotY;
    QDoubleSpinBox *doubleSpinBox_Cam2RotZ;
    QLabel *label_21;
    QGridLayout *gridLayout_9;
    QLabel *label_9;
    QToolButton *toolButton_Intensity2Down;
    QSlider *horizontalSlider_Intensity2;
    QLabel *label_31;
    QSlider *horizontalSlider_Windowing2Min;
    QSlider *horizontalSlider_Windowing2Max;
    QGridLayout *gridLayout_3;
    QToolButton *toolButton_VoxelFilter2ThresholdDown;
    QToolButton *toolButton_VoxelFilter2ThresholdSwap;
    QLabel *label_8;
    QSlider *horizontalSlider_VoxelFilter2ThresholdLow;
    QSlider *horizontalSlider_VoxelFilter2ThresholdHigh;
    QLabel *label_32;
    QSlider *horizontalSlider_WeightingFactorDrr2;
    QLabel *label_Cam2Settings;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_10;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_11;
    QHBoxLayout *horizontalLayout_13;
    QLabel *Load_label;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_45;
    QPushButton *SelectLoadingFileButton;
    QLabel *LoadingFileLabel;
    QHBoxLayout *horizontalLayout_46;
    QPushButton *LoadingButton;
    QPushButton *StopLoadingButton;
    QSpacerItem *horizontalSpacer_3;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_7;
    QLabel *ReadLabel;
    QRadioButton *DebugModeRadioButton;
    QHBoxLayout *horizontalLayout_9;
    QPushButton *SelectReadingFileButton;
    QLabel *ReadingFileLabel;
    QHBoxLayout *horizontalLayout_49;
    QPushButton *StartReadingButton;
    QPushButton *StopReadingButton;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *StartPhaseRecognitionButton;
    QLabel *phase_text;
    QLabel *phaseLabel;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_36;
    QLabel *fc_label;
    QSlider *fc_slider;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->setEnabled(true);
        MainWindow->resize(1780, 900);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1780, 900));
        MainWindow->setMaximumSize(QSize(1780, 900));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        label_Drr_1 = new QLabel(centralWidget);
        label_Drr_1->setObjectName(QStringLiteral("label_Drr_1"));
        label_Drr_1->setGeometry(QRect(630, 20, 410, 410));
        label_Drr_1->setMouseTracking(false);
        label_Drr_1->setFocusPolicy(Qt::ClickFocus);
        label_Drr_1->setAutoFillBackground(false);
        label_Drr_1->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        label_Drr_2 = new QLabel(centralWidget);
        label_Drr_2->setObjectName(QStringLiteral("label_Drr_2"));
        label_Drr_2->setGeometry(QRect(629, 440, 411, 411));
        label_Drr_2->setFocusPolicy(Qt::ClickFocus);
        label_Drr_2->setAutoFillBackground(false);
        label_Drr_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        label_Xray_1 = new QLabel(centralWidget);
        label_Xray_1->setObjectName(QStringLiteral("label_Xray_1"));
        label_Xray_1->setGeometry(QRect(1050, 20, 410, 410));
        label_Xray_1->setFocusPolicy(Qt::ClickFocus);
        label_Xray_1->setAutoFillBackground(false);
        label_Xray_1->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        label_Xray_2 = new QLabel(centralWidget);
        label_Xray_2->setObjectName(QStringLiteral("label_Xray_2"));
        label_Xray_2->setGeometry(QRect(1050, 440, 410, 410));
        label_Xray_2->setFocusPolicy(Qt::ClickFocus);
        label_Xray_2->setAutoFillBackground(false);
        label_Xray_2->setStyleSheet(QStringLiteral("background-color: rgb(0, 0, 0);"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(1470, 20, 291, 831));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalFrame_3 = new QFrame(verticalLayoutWidget);
        verticalFrame_3->setObjectName(QStringLiteral("verticalFrame_3"));
        verticalLayout_5 = new QVBoxLayout(verticalFrame_3);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_Xray1ListLoad = new QPushButton(verticalFrame_3);
        pushButton_Xray1ListLoad->setObjectName(QStringLiteral("pushButton_Xray1ListLoad"));

        horizontalLayout_2->addWidget(pushButton_Xray1ListLoad);


        verticalLayout_5->addLayout(horizontalLayout_2);

        listWidget_Xray1 = new QListWidget(verticalFrame_3);
        listWidget_Xray1->setObjectName(QStringLiteral("listWidget_Xray1"));
        listWidget_Xray1->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        listWidget_Xray1->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        listWidget_Xray1->setDragDropMode(QAbstractItemView::DragDrop);
        listWidget_Xray1->setSelectionMode(QAbstractItemView::ExtendedSelection);

        verticalLayout_5->addWidget(listWidget_Xray1);

        gridLayout_15 = new QGridLayout();
        gridLayout_15->setSpacing(6);
        gridLayout_15->setObjectName(QStringLiteral("gridLayout_15"));
        radioButton_Mask11 = new QRadioButton(verticalFrame_3);
        radioButton_Mask11->setObjectName(QStringLiteral("radioButton_Mask11"));
        radioButton_Mask11->setChecked(true);

        gridLayout_15->addWidget(radioButton_Mask11, 0, 0, 1, 1);

        radioButton_Mask12 = new QRadioButton(verticalFrame_3);
        radioButton_Mask12->setObjectName(QStringLiteral("radioButton_Mask12"));
        radioButton_Mask12->setEnabled(true);

        gridLayout_15->addWidget(radioButton_Mask12, 0, 1, 1, 1);


        verticalLayout_5->addLayout(gridLayout_15);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSlider_Mask1PosV = new QSlider(verticalFrame_3);
        horizontalSlider_Mask1PosV->setObjectName(QStringLiteral("horizontalSlider_Mask1PosV"));
        horizontalSlider_Mask1PosV->setMinimum(1);
        horizontalSlider_Mask1PosV->setMaximum(410);
        horizontalSlider_Mask1PosV->setSliderPosition(205);
        horizontalSlider_Mask1PosV->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_Mask1PosV, 1, 1, 1, 1);

        label_12 = new QLabel(verticalFrame_3);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout->addWidget(label_12, 2, 0, 1, 1);

        label_10 = new QLabel(verticalFrame_3);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout->addWidget(label_10, 0, 0, 1, 1);

        horizontalSlider_Mask1PosH = new QSlider(verticalFrame_3);
        horizontalSlider_Mask1PosH->setObjectName(QStringLiteral("horizontalSlider_Mask1PosH"));
        horizontalSlider_Mask1PosH->setMinimum(1);
        horizontalSlider_Mask1PosH->setMaximum(410);
        horizontalSlider_Mask1PosH->setValue(205);
        horizontalSlider_Mask1PosH->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_Mask1PosH, 0, 1, 1, 1);

        label_11 = new QLabel(verticalFrame_3);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout->addWidget(label_11, 1, 0, 1, 1);

        horizontalSlider_Mask1Prop1 = new QSlider(verticalFrame_3);
        horizontalSlider_Mask1Prop1->setObjectName(QStringLiteral("horizontalSlider_Mask1Prop1"));
        horizontalSlider_Mask1Prop1->setMaximum(410);
        horizontalSlider_Mask1Prop1->setValue(0);
        horizontalSlider_Mask1Prop1->setSliderPosition(0);
        horizontalSlider_Mask1Prop1->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_Mask1Prop1, 2, 1, 1, 1);

        label_3 = new QLabel(verticalFrame_3);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        horizontalSlider_Mask1Prop2 = new QSlider(verticalFrame_3);
        horizontalSlider_Mask1Prop2->setObjectName(QStringLiteral("horizontalSlider_Mask1Prop2"));
        horizontalSlider_Mask1Prop2->setMaximum(410);
        horizontalSlider_Mask1Prop2->setValue(0);
        horizontalSlider_Mask1Prop2->setSliderPosition(0);
        horizontalSlider_Mask1Prop2->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_Mask1Prop2, 3, 1, 1, 1);


        verticalLayout_5->addLayout(gridLayout);

        gridLayout_10 = new QGridLayout();
        gridLayout_10->setSpacing(6);
        gridLayout_10->setObjectName(QStringLiteral("gridLayout_10"));
        pushButton_LoadMask1 = new QPushButton(verticalFrame_3);
        pushButton_LoadMask1->setObjectName(QStringLiteral("pushButton_LoadMask1"));

        gridLayout_10->addWidget(pushButton_LoadMask1, 0, 0, 1, 1);

        pushButton_SaveMask1 = new QPushButton(verticalFrame_3);
        pushButton_SaveMask1->setObjectName(QStringLiteral("pushButton_SaveMask1"));

        gridLayout_10->addWidget(pushButton_SaveMask1, 0, 1, 1, 1);


        verticalLayout_5->addLayout(gridLayout_10);

        label_Mask1 = new QLabel(verticalFrame_3);
        label_Mask1->setObjectName(QStringLiteral("label_Mask1"));

        verticalLayout_5->addWidget(label_Mask1);


        verticalLayout->addWidget(verticalFrame_3);

        verticalFrame_4 = new QFrame(verticalLayoutWidget);
        verticalFrame_4->setObjectName(QStringLiteral("verticalFrame_4"));
        verticalLayout_6 = new QVBoxLayout(verticalFrame_4);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        pushButton_Xray2ListLoad = new QPushButton(verticalFrame_4);
        pushButton_Xray2ListLoad->setObjectName(QStringLiteral("pushButton_Xray2ListLoad"));

        horizontalLayout_3->addWidget(pushButton_Xray2ListLoad);


        verticalLayout_6->addLayout(horizontalLayout_3);

        listWidget_Xray2 = new QListWidget(verticalFrame_4);
        listWidget_Xray2->setObjectName(QStringLiteral("listWidget_Xray2"));
        listWidget_Xray2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        listWidget_Xray2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        listWidget_Xray2->setDragDropMode(QAbstractItemView::DragDrop);
        listWidget_Xray2->setSelectionMode(QAbstractItemView::ExtendedSelection);

        verticalLayout_6->addWidget(listWidget_Xray2);

        gridLayout_16 = new QGridLayout();
        gridLayout_16->setSpacing(6);
        gridLayout_16->setObjectName(QStringLiteral("gridLayout_16"));
        radioButton_Mask21 = new QRadioButton(verticalFrame_4);
        radioButton_Mask21->setObjectName(QStringLiteral("radioButton_Mask21"));
        radioButton_Mask21->setChecked(true);

        gridLayout_16->addWidget(radioButton_Mask21, 0, 0, 1, 1);

        radioButton_Mask22 = new QRadioButton(verticalFrame_4);
        radioButton_Mask22->setObjectName(QStringLiteral("radioButton_Mask22"));

        gridLayout_16->addWidget(radioButton_Mask22, 0, 1, 1, 1);


        verticalLayout_6->addLayout(gridLayout_16);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_14 = new QLabel(verticalFrame_4);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_2->addWidget(label_14, 2, 0, 1, 1);

        horizontalSlider_Mask2PosV = new QSlider(verticalFrame_4);
        horizontalSlider_Mask2PosV->setObjectName(QStringLiteral("horizontalSlider_Mask2PosV"));
        horizontalSlider_Mask2PosV->setMinimum(1);
        horizontalSlider_Mask2PosV->setMaximum(410);
        horizontalSlider_Mask2PosV->setValue(205);
        horizontalSlider_Mask2PosV->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalSlider_Mask2PosV, 2, 1, 1, 1);

        horizontalSlider_Mask2Prop1 = new QSlider(verticalFrame_4);
        horizontalSlider_Mask2Prop1->setObjectName(QStringLiteral("horizontalSlider_Mask2Prop1"));
        horizontalSlider_Mask2Prop1->setMaximum(410);
        horizontalSlider_Mask2Prop1->setValue(0);
        horizontalSlider_Mask2Prop1->setSliderPosition(0);
        horizontalSlider_Mask2Prop1->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalSlider_Mask2Prop1, 3, 1, 1, 1);

        label_13 = new QLabel(verticalFrame_4);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout_2->addWidget(label_13, 1, 0, 1, 1);

        horizontalSlider_Mask2PosH = new QSlider(verticalFrame_4);
        horizontalSlider_Mask2PosH->setObjectName(QStringLiteral("horizontalSlider_Mask2PosH"));
        horizontalSlider_Mask2PosH->setMinimum(1);
        horizontalSlider_Mask2PosH->setMaximum(410);
        horizontalSlider_Mask2PosH->setValue(205);
        horizontalSlider_Mask2PosH->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalSlider_Mask2PosH, 1, 1, 1, 1);

        label_15 = new QLabel(verticalFrame_4);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_2->addWidget(label_15, 3, 0, 1, 1);

        horizontalSlider_Mask2Prop2 = new QSlider(verticalFrame_4);
        horizontalSlider_Mask2Prop2->setObjectName(QStringLiteral("horizontalSlider_Mask2Prop2"));
        horizontalSlider_Mask2Prop2->setMaximum(410);
        horizontalSlider_Mask2Prop2->setValue(0);
        horizontalSlider_Mask2Prop2->setOrientation(Qt::Horizontal);

        gridLayout_2->addWidget(horizontalSlider_Mask2Prop2, 4, 1, 1, 1);

        label_27 = new QLabel(verticalFrame_4);
        label_27->setObjectName(QStringLiteral("label_27"));

        gridLayout_2->addWidget(label_27, 4, 0, 1, 1);


        verticalLayout_6->addLayout(gridLayout_2);

        gridLayout_14 = new QGridLayout();
        gridLayout_14->setSpacing(6);
        gridLayout_14->setObjectName(QStringLiteral("gridLayout_14"));
        pushButton_LoadMask2 = new QPushButton(verticalFrame_4);
        pushButton_LoadMask2->setObjectName(QStringLiteral("pushButton_LoadMask2"));

        gridLayout_14->addWidget(pushButton_LoadMask2, 0, 0, 1, 1);

        pushButton_SaveMask2 = new QPushButton(verticalFrame_4);
        pushButton_SaveMask2->setObjectName(QStringLiteral("pushButton_SaveMask2"));

        gridLayout_14->addWidget(pushButton_SaveMask2, 0, 1, 1, 1);


        verticalLayout_6->addLayout(gridLayout_14);

        label_Mask2 = new QLabel(verticalFrame_4);
        label_Mask2->setObjectName(QStringLiteral("label_Mask2"));

        verticalLayout_6->addWidget(label_Mask2);


        verticalLayout->addWidget(verticalFrame_4);

        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(19, 22, 591, 741));
        horizontalLayout_43 = new QHBoxLayout(layoutWidget);
        horizontalLayout_43->setSpacing(6);
        horizontalLayout_43->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_43->setObjectName(QStringLiteral("horizontalLayout_43"));
        horizontalLayout_43->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        label_28 = new QLabel(layoutWidget);
        label_28->setObjectName(QStringLiteral("label_28"));

        verticalLayout_2->addWidget(label_28);

        comboBox_RtSystem = new QComboBox(layoutWidget);
        comboBox_RtSystem->setObjectName(QStringLiteral("comboBox_RtSystem"));

        verticalLayout_2->addWidget(comboBox_RtSystem);

        label_5 = new QLabel(layoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_2->addWidget(label_5);

        comboBox_RegistrationMode = new QComboBox(layoutWidget);
        comboBox_RegistrationMode->setObjectName(QStringLiteral("comboBox_RegistrationMode"));

        verticalLayout_2->addWidget(comboBox_RegistrationMode);

        label_4 = new QLabel(layoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_2->addWidget(label_4);

        comboBox_MeritFunction = new QComboBox(layoutWidget);
        comboBox_MeritFunction->setObjectName(QStringLiteral("comboBox_MeritFunction"));

        verticalLayout_2->addWidget(comboBox_MeritFunction);

        label_6 = new QLabel(layoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_2->addWidget(label_6);

        gridLayout_13 = new QGridLayout();
        gridLayout_13->setSpacing(6);
        gridLayout_13->setObjectName(QStringLiteral("gridLayout_13"));
        checkBox_tx = new QCheckBox(layoutWidget);
        checkBox_tx->setObjectName(QStringLiteral("checkBox_tx"));
        checkBox_tx->setChecked(true);

        gridLayout_13->addWidget(checkBox_tx, 0, 0, 1, 1);

        checkBox_ty = new QCheckBox(layoutWidget);
        checkBox_ty->setObjectName(QStringLiteral("checkBox_ty"));
        checkBox_ty->setChecked(false);

        gridLayout_13->addWidget(checkBox_ty, 0, 1, 1, 1);

        checkBox_tz = new QCheckBox(layoutWidget);
        checkBox_tz->setObjectName(QStringLiteral("checkBox_tz"));
        checkBox_tz->setChecked(true);

        gridLayout_13->addWidget(checkBox_tz, 0, 2, 1, 1);

        checkBox_ry = new QCheckBox(layoutWidget);
        checkBox_ry->setObjectName(QStringLiteral("checkBox_ry"));

        gridLayout_13->addWidget(checkBox_ry, 0, 4, 1, 1);

        checkBox_rx = new QCheckBox(layoutWidget);
        checkBox_rx->setObjectName(QStringLiteral("checkBox_rx"));

        gridLayout_13->addWidget(checkBox_rx, 0, 3, 1, 1);

        checkBox_rz = new QCheckBox(layoutWidget);
        checkBox_rz->setObjectName(QStringLiteral("checkBox_rz"));

        gridLayout_13->addWidget(checkBox_rz, 0, 5, 1, 1);


        verticalLayout_2->addLayout(gridLayout_13);

        label_20 = new QLabel(layoutWidget);
        label_20->setObjectName(QStringLiteral("label_20"));

        verticalLayout_2->addWidget(label_20);

        horizontalSlider_BackgroundSaturation = new QSlider(layoutWidget);
        horizontalSlider_BackgroundSaturation->setObjectName(QStringLiteral("horizontalSlider_BackgroundSaturation"));
        horizontalSlider_BackgroundSaturation->setMaximum(255);
        horizontalSlider_BackgroundSaturation->setValue(128);
        horizontalSlider_BackgroundSaturation->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(horizontalSlider_BackgroundSaturation);

        gridLayout_12 = new QGridLayout();
        gridLayout_12->setSpacing(6);
        gridLayout_12->setObjectName(QStringLiteral("gridLayout_12"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout_12->addWidget(label_2, 8, 0, 1, 1);

        gridLayout_11 = new QGridLayout();
        gridLayout_11->setSpacing(6);
        gridLayout_11->setObjectName(QStringLiteral("gridLayout_11"));
        label_26 = new QLabel(layoutWidget);
        label_26->setObjectName(QStringLiteral("label_26"));

        gridLayout_11->addWidget(label_26, 0, 0, 1, 1);

        toolButton_DrrTransRotUp = new QToolButton(layoutWidget);
        toolButton_DrrTransRotUp->setObjectName(QStringLiteral("toolButton_DrrTransRotUp"));

        gridLayout_11->addWidget(toolButton_DrrTransRotUp, 0, 2, 1, 1);

        toolButton_DrrTransformDown = new QToolButton(layoutWidget);
        toolButton_DrrTransformDown->setObjectName(QStringLiteral("toolButton_DrrTransformDown"));

        gridLayout_11->addWidget(toolButton_DrrTransformDown, 0, 1, 1, 1);


        gridLayout_12->addLayout(gridLayout_11, 2, 0, 1, 1);

        horizontalSlider_TransX = new QSlider(layoutWidget);
        horizontalSlider_TransX->setObjectName(QStringLiteral("horizontalSlider_TransX"));
        horizontalSlider_TransX->setMinimum(-10000);
        horizontalSlider_TransX->setMaximum(10000);
        horizontalSlider_TransX->setOrientation(Qt::Horizontal);

        gridLayout_12->addWidget(horizontalSlider_TransX, 3, 0, 1, 1);

        horizontalSlider_RotZ = new QSlider(layoutWidget);
        horizontalSlider_RotZ->setObjectName(QStringLiteral("horizontalSlider_RotZ"));
        horizontalSlider_RotZ->setMinimum(-1800);
        horizontalSlider_RotZ->setMaximum(1800);
        horizontalSlider_RotZ->setOrientation(Qt::Horizontal);

        gridLayout_12->addWidget(horizontalSlider_RotZ, 11, 0, 1, 1);

        horizontalSlider_RotY = new QSlider(layoutWidget);
        horizontalSlider_RotY->setObjectName(QStringLiteral("horizontalSlider_RotY"));
        horizontalSlider_RotY->setMinimum(-1800);
        horizontalSlider_RotY->setMaximum(1800);
        horizontalSlider_RotY->setOrientation(Qt::Horizontal);

        gridLayout_12->addWidget(horizontalSlider_RotY, 10, 0, 1, 1);

        horizontalSlider_TransY = new QSlider(layoutWidget);
        horizontalSlider_TransY->setObjectName(QStringLiteral("horizontalSlider_TransY"));
        horizontalSlider_TransY->setMinimum(-10000);
        horizontalSlider_TransY->setMaximum(10000);
        horizontalSlider_TransY->setOrientation(Qt::Horizontal);

        gridLayout_12->addWidget(horizontalSlider_TransY, 4, 0, 1, 1);

        horizontalSlider_RotX = new QSlider(layoutWidget);
        horizontalSlider_RotX->setObjectName(QStringLiteral("horizontalSlider_RotX"));
        horizontalSlider_RotX->setMinimum(-1800);
        horizontalSlider_RotX->setMaximum(1800);
        horizontalSlider_RotX->setOrientation(Qt::Horizontal);

        gridLayout_12->addWidget(horizontalSlider_RotX, 9, 0, 1, 1);

        horizontalSlider_TransZ = new QSlider(layoutWidget);
        horizontalSlider_TransZ->setObjectName(QStringLiteral("horizontalSlider_TransZ"));
        horizontalSlider_TransZ->setMinimum(-10000);
        horizontalSlider_TransZ->setMaximum(10000);
        horizontalSlider_TransZ->setOrientation(Qt::Horizontal);

        gridLayout_12->addWidget(horizontalSlider_TransZ, 5, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_12);

        gridLayout_18 = new QGridLayout();
        gridLayout_18->setSpacing(6);
        gridLayout_18->setObjectName(QStringLiteral("gridLayout_18"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_25 = new QLabel(layoutWidget);
        label_25->setObjectName(QStringLiteral("label_25"));

        horizontalLayout_4->addWidget(label_25);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);

        checkBox_XrayBS = new QCheckBox(layoutWidget);
        checkBox_XrayBS->setObjectName(QStringLiteral("checkBox_XrayBS"));

        horizontalLayout_4->addWidget(checkBox_XrayBS);


        gridLayout_18->addLayout(horizontalLayout_4, 0, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout_18);

        horizontalSlider_WeightingFactorXray = new QSlider(layoutWidget);
        horizontalSlider_WeightingFactorXray->setObjectName(QStringLiteral("horizontalSlider_WeightingFactorXray"));
        horizontalSlider_WeightingFactorXray->setMaximum(100);
        horizontalSlider_WeightingFactorXray->setValue(100);
        horizontalSlider_WeightingFactorXray->setOrientation(Qt::Horizontal);

        verticalLayout_2->addWidget(horizontalSlider_WeightingFactorXray);

        label_weightsBS = new QLabel(layoutWidget);
        label_weightsBS->setObjectName(QStringLiteral("label_weightsBS"));

        verticalLayout_2->addWidget(label_weightsBS);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pushButton_LoadVolume = new QPushButton(layoutWidget);
        pushButton_LoadVolume->setObjectName(QStringLiteral("pushButton_LoadVolume"));

        horizontalLayout_5->addWidget(pushButton_LoadVolume);

        pushButton_LoadConfig = new QPushButton(layoutWidget);
        pushButton_LoadConfig->setObjectName(QStringLiteral("pushButton_LoadConfig"));

        horizontalLayout_5->addWidget(pushButton_LoadConfig);

        pushButton_SaveConfig = new QPushButton(layoutWidget);
        pushButton_SaveConfig->setObjectName(QStringLiteral("pushButton_SaveConfig"));

        horizontalLayout_5->addWidget(pushButton_SaveConfig);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));

        verticalLayout_2->addLayout(horizontalLayout);

        pushButton_Registration_2D3D = new QPushButton(layoutWidget);
        pushButton_Registration_2D3D->setObjectName(QStringLiteral("pushButton_Registration_2D3D"));

        verticalLayout_2->addWidget(pushButton_Registration_2D3D);

        pushButton_BatchRegistration2D3D = new QPushButton(layoutWidget);
        pushButton_BatchRegistration2D3D->setObjectName(QStringLiteral("pushButton_BatchRegistration2D3D"));

        verticalLayout_2->addWidget(pushButton_BatchRegistration2D3D);

        pushButton_Test = new QPushButton(layoutWidget);
        pushButton_Test->setObjectName(QStringLiteral("pushButton_Test"));

        verticalLayout_2->addWidget(pushButton_Test);

        pushButton_SaveDrrs = new QPushButton(layoutWidget);
        pushButton_SaveDrrs->setObjectName(QStringLiteral("pushButton_SaveDrrs"));

        verticalLayout_2->addWidget(pushButton_SaveDrrs);

        pushButton_CopyTransRot = new QPushButton(layoutWidget);
        pushButton_CopyTransRot->setObjectName(QStringLiteral("pushButton_CopyTransRot"));

        verticalLayout_2->addWidget(pushButton_CopyTransRot);


        horizontalLayout_43->addLayout(verticalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_43->addItem(horizontalSpacer_2);

        verticalLayout_7 = new QVBoxLayout();
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        verticalLayout_9 = new QVBoxLayout();
        verticalLayout_9->setSpacing(6);
        verticalLayout_9->setObjectName(QStringLiteral("verticalLayout_9"));
        verticalLayout_8 = new QVBoxLayout();
        verticalLayout_8->setSpacing(6);
        verticalLayout_8->setObjectName(QStringLiteral("verticalLayout_8"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        doubleSpinBox_Cam1SourceDistance = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_Cam1SourceDistance->setObjectName(QStringLiteral("doubleSpinBox_Cam1SourceDistance"));
        doubleSpinBox_Cam1SourceDistance->setEnabled(false);
        doubleSpinBox_Cam1SourceDistance->setDecimals(1);
        doubleSpinBox_Cam1SourceDistance->setMinimum(-2000);
        doubleSpinBox_Cam1SourceDistance->setMaximum(2000);

        gridLayout_5->addWidget(doubleSpinBox_Cam1SourceDistance, 1, 0, 1, 1);

        doubleSpinBox_Cam1RotX = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_Cam1RotX->setObjectName(QStringLiteral("doubleSpinBox_Cam1RotX"));
        doubleSpinBox_Cam1RotX->setEnabled(false);
        doubleSpinBox_Cam1RotX->setDecimals(1);
        doubleSpinBox_Cam1RotX->setMinimum(-180);
        doubleSpinBox_Cam1RotX->setMaximum(180);

        gridLayout_5->addWidget(doubleSpinBox_Cam1RotX, 4, 0, 1, 1);

        doubleSpinBox_Cam1RotY = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_Cam1RotY->setObjectName(QStringLiteral("doubleSpinBox_Cam1RotY"));
        doubleSpinBox_Cam1RotY->setEnabled(false);
        doubleSpinBox_Cam1RotY->setDecimals(1);
        doubleSpinBox_Cam1RotY->setMinimum(-180);
        doubleSpinBox_Cam1RotY->setMaximum(180);

        gridLayout_5->addWidget(doubleSpinBox_Cam1RotY, 4, 1, 1, 1);

        doubleSpinBox_Cam1RotZ = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_Cam1RotZ->setObjectName(QStringLiteral("doubleSpinBox_Cam1RotZ"));
        doubleSpinBox_Cam1RotZ->setEnabled(false);
        doubleSpinBox_Cam1RotZ->setDecimals(1);
        doubleSpinBox_Cam1RotZ->setMinimum(-180);
        doubleSpinBox_Cam1RotZ->setMaximum(180);

        gridLayout_5->addWidget(doubleSpinBox_Cam1RotZ, 4, 2, 1, 1);

        label_18 = new QLabel(layoutWidget);
        label_18->setObjectName(QStringLiteral("label_18"));

        gridLayout_5->addWidget(label_18, 3, 0, 1, 3);

        label_17 = new QLabel(layoutWidget);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout_5->addWidget(label_17, 0, 0, 1, 3);

        doubleSpinBox_Cam1DetectorDistance = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_Cam1DetectorDistance->setObjectName(QStringLiteral("doubleSpinBox_Cam1DetectorDistance"));
        doubleSpinBox_Cam1DetectorDistance->setEnabled(false);
        doubleSpinBox_Cam1DetectorDistance->setDecimals(1);
        doubleSpinBox_Cam1DetectorDistance->setMinimum(-2000);
        doubleSpinBox_Cam1DetectorDistance->setMaximum(2000);

        gridLayout_5->addWidget(doubleSpinBox_Cam1DetectorDistance, 1, 1, 1, 1);


        verticalLayout_8->addLayout(gridLayout_5);

        gridLayout_8 = new QGridLayout();
        gridLayout_8->setSpacing(6);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout_8->addWidget(label, 0, 0, 1, 1);

        toolButton_Intensity1Up = new QToolButton(layoutWidget);
        toolButton_Intensity1Up->setObjectName(QStringLiteral("toolButton_Intensity1Up"));

        gridLayout_8->addWidget(toolButton_Intensity1Up, 0, 1, 1, 1);


        verticalLayout_8->addLayout(gridLayout_8);

        horizontalSlider_Intensity1 = new QSlider(layoutWidget);
        horizontalSlider_Intensity1->setObjectName(QStringLiteral("horizontalSlider_Intensity1"));
        horizontalSlider_Intensity1->setMinimum(0);
        horizontalSlider_Intensity1->setMaximum(1000);
        horizontalSlider_Intensity1->setValue(500);
        horizontalSlider_Intensity1->setSliderPosition(500);
        horizontalSlider_Intensity1->setOrientation(Qt::Horizontal);

        verticalLayout_8->addWidget(horizontalSlider_Intensity1);

        label_7 = new QLabel(layoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout_8->addWidget(label_7);

        horizontalSlider_Windowing1Min = new QSlider(layoutWidget);
        horizontalSlider_Windowing1Min->setObjectName(QStringLiteral("horizontalSlider_Windowing1Min"));
        horizontalSlider_Windowing1Min->setMaximum(65535);
        horizontalSlider_Windowing1Min->setOrientation(Qt::Horizontal);

        verticalLayout_8->addWidget(horizontalSlider_Windowing1Min);

        horizontalSlider_Windowing1Max = new QSlider(layoutWidget);
        horizontalSlider_Windowing1Max->setObjectName(QStringLiteral("horizontalSlider_Windowing1Max"));
        horizontalSlider_Windowing1Max->setMaximum(65535);
        horizontalSlider_Windowing1Max->setValue(65535);
        horizontalSlider_Windowing1Max->setOrientation(Qt::Horizontal);

        verticalLayout_8->addWidget(horizontalSlider_Windowing1Max);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        toolButton_VoxelFilter1ThresholdSwap = new QToolButton(layoutWidget);
        toolButton_VoxelFilter1ThresholdSwap->setObjectName(QStringLiteral("toolButton_VoxelFilter1ThresholdSwap"));

        gridLayout_4->addWidget(toolButton_VoxelFilter1ThresholdSwap, 0, 1, 1, 1);

        toolButton_VoxelFilter1ThresholdUp = new QToolButton(layoutWidget);
        toolButton_VoxelFilter1ThresholdUp->setObjectName(QStringLiteral("toolButton_VoxelFilter1ThresholdUp"));

        gridLayout_4->addWidget(toolButton_VoxelFilter1ThresholdUp, 0, 2, 1, 1);

        label_16 = new QLabel(layoutWidget);
        label_16->setObjectName(QStringLiteral("label_16"));

        gridLayout_4->addWidget(label_16, 0, 0, 1, 1);


        verticalLayout_8->addLayout(gridLayout_4);

        horizontalSlider_VoxelFilter1ThresholdLow = new QSlider(layoutWidget);
        horizontalSlider_VoxelFilter1ThresholdLow->setObjectName(QStringLiteral("horizontalSlider_VoxelFilter1ThresholdLow"));
        horizontalSlider_VoxelFilter1ThresholdLow->setMaximum(65535);
        horizontalSlider_VoxelFilter1ThresholdLow->setOrientation(Qt::Horizontal);

        verticalLayout_8->addWidget(horizontalSlider_VoxelFilter1ThresholdLow);

        horizontalSlider_VoxelFilter1ThresholdHigh = new QSlider(layoutWidget);
        horizontalSlider_VoxelFilter1ThresholdHigh->setObjectName(QStringLiteral("horizontalSlider_VoxelFilter1ThresholdHigh"));
        horizontalSlider_VoxelFilter1ThresholdHigh->setMaximum(65535);
        horizontalSlider_VoxelFilter1ThresholdHigh->setValue(65535);
        horizontalSlider_VoxelFilter1ThresholdHigh->setOrientation(Qt::Horizontal);

        verticalLayout_8->addWidget(horizontalSlider_VoxelFilter1ThresholdHigh);

        label_29 = new QLabel(layoutWidget);
        label_29->setObjectName(QStringLiteral("label_29"));

        verticalLayout_8->addWidget(label_29);

        horizontalSlider_WeightingFactorDrr1 = new QSlider(layoutWidget);
        horizontalSlider_WeightingFactorDrr1->setObjectName(QStringLiteral("horizontalSlider_WeightingFactorDrr1"));
        horizontalSlider_WeightingFactorDrr1->setMaximum(100);
        horizontalSlider_WeightingFactorDrr1->setValue(100);
        horizontalSlider_WeightingFactorDrr1->setOrientation(Qt::Horizontal);

        verticalLayout_8->addWidget(horizontalSlider_WeightingFactorDrr1);

        label_Cam1Settings = new QLabel(layoutWidget);
        label_Cam1Settings->setObjectName(QStringLiteral("label_Cam1Settings"));

        verticalLayout_8->addWidget(label_Cam1Settings);


        verticalLayout_9->addLayout(verticalLayout_8);


        verticalLayout_7->addLayout(verticalLayout_9);

        verticalLayout_11 = new QVBoxLayout();
        verticalLayout_11->setSpacing(6);
        verticalLayout_11->setObjectName(QStringLiteral("verticalLayout_11"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setSpacing(6);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        label_19 = new QLabel(layoutWidget);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_7->addWidget(label_19, 2, 0, 1, 3);

        label_22 = new QLabel(layoutWidget);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout_7->addWidget(label_22, 0, 0, 1, 3);

        doubleSpinBox_Cam2SourceDistance = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_Cam2SourceDistance->setObjectName(QStringLiteral("doubleSpinBox_Cam2SourceDistance"));
        doubleSpinBox_Cam2SourceDistance->setEnabled(false);
        doubleSpinBox_Cam2SourceDistance->setDecimals(1);
        doubleSpinBox_Cam2SourceDistance->setMinimum(-2000);
        doubleSpinBox_Cam2SourceDistance->setMaximum(2000);

        gridLayout_7->addWidget(doubleSpinBox_Cam2SourceDistance, 1, 0, 1, 1);

        doubleSpinBox_Cam2DetectorDistance = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_Cam2DetectorDistance->setObjectName(QStringLiteral("doubleSpinBox_Cam2DetectorDistance"));
        doubleSpinBox_Cam2DetectorDistance->setEnabled(false);
        doubleSpinBox_Cam2DetectorDistance->setDecimals(1);
        doubleSpinBox_Cam2DetectorDistance->setMinimum(-2000);
        doubleSpinBox_Cam2DetectorDistance->setMaximum(2000);

        gridLayout_7->addWidget(doubleSpinBox_Cam2DetectorDistance, 1, 1, 1, 1);

        doubleSpinBox_Cam2RotX = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_Cam2RotX->setObjectName(QStringLiteral("doubleSpinBox_Cam2RotX"));
        doubleSpinBox_Cam2RotX->setEnabled(false);
        doubleSpinBox_Cam2RotX->setDecimals(1);
        doubleSpinBox_Cam2RotX->setMinimum(-180);
        doubleSpinBox_Cam2RotX->setMaximum(180);

        gridLayout_7->addWidget(doubleSpinBox_Cam2RotX, 3, 0, 1, 1);

        doubleSpinBox_Cam2RotY = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_Cam2RotY->setObjectName(QStringLiteral("doubleSpinBox_Cam2RotY"));
        doubleSpinBox_Cam2RotY->setEnabled(false);
        doubleSpinBox_Cam2RotY->setDecimals(1);
        doubleSpinBox_Cam2RotY->setMinimum(-180);
        doubleSpinBox_Cam2RotY->setMaximum(180);

        gridLayout_7->addWidget(doubleSpinBox_Cam2RotY, 3, 1, 1, 1);

        doubleSpinBox_Cam2RotZ = new QDoubleSpinBox(layoutWidget);
        doubleSpinBox_Cam2RotZ->setObjectName(QStringLiteral("doubleSpinBox_Cam2RotZ"));
        doubleSpinBox_Cam2RotZ->setEnabled(false);
        doubleSpinBox_Cam2RotZ->setDecimals(1);
        doubleSpinBox_Cam2RotZ->setMinimum(-180);
        doubleSpinBox_Cam2RotZ->setMaximum(180);

        gridLayout_7->addWidget(doubleSpinBox_Cam2RotZ, 3, 2, 1, 1);


        verticalLayout_11->addLayout(gridLayout_7);

        label_21 = new QLabel(layoutWidget);
        label_21->setObjectName(QStringLiteral("label_21"));

        verticalLayout_11->addWidget(label_21);

        gridLayout_9 = new QGridLayout();
        gridLayout_9->setSpacing(6);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        label_9 = new QLabel(layoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_9->addWidget(label_9, 0, 0, 1, 1);

        toolButton_Intensity2Down = new QToolButton(layoutWidget);
        toolButton_Intensity2Down->setObjectName(QStringLiteral("toolButton_Intensity2Down"));

        gridLayout_9->addWidget(toolButton_Intensity2Down, 0, 1, 1, 1);


        verticalLayout_11->addLayout(gridLayout_9);

        horizontalSlider_Intensity2 = new QSlider(layoutWidget);
        horizontalSlider_Intensity2->setObjectName(QStringLiteral("horizontalSlider_Intensity2"));
        horizontalSlider_Intensity2->setMaximum(1000);
        horizontalSlider_Intensity2->setValue(500);
        horizontalSlider_Intensity2->setOrientation(Qt::Horizontal);

        verticalLayout_11->addWidget(horizontalSlider_Intensity2);

        label_31 = new QLabel(layoutWidget);
        label_31->setObjectName(QStringLiteral("label_31"));

        verticalLayout_11->addWidget(label_31);

        horizontalSlider_Windowing2Min = new QSlider(layoutWidget);
        horizontalSlider_Windowing2Min->setObjectName(QStringLiteral("horizontalSlider_Windowing2Min"));
        horizontalSlider_Windowing2Min->setMaximum(65535);
        horizontalSlider_Windowing2Min->setOrientation(Qt::Horizontal);

        verticalLayout_11->addWidget(horizontalSlider_Windowing2Min);

        horizontalSlider_Windowing2Max = new QSlider(layoutWidget);
        horizontalSlider_Windowing2Max->setObjectName(QStringLiteral("horizontalSlider_Windowing2Max"));
        horizontalSlider_Windowing2Max->setMaximum(65535);
        horizontalSlider_Windowing2Max->setValue(65535);
        horizontalSlider_Windowing2Max->setOrientation(Qt::Horizontal);

        verticalLayout_11->addWidget(horizontalSlider_Windowing2Max);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setSpacing(6);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        toolButton_VoxelFilter2ThresholdDown = new QToolButton(layoutWidget);
        toolButton_VoxelFilter2ThresholdDown->setObjectName(QStringLiteral("toolButton_VoxelFilter2ThresholdDown"));

        gridLayout_3->addWidget(toolButton_VoxelFilter2ThresholdDown, 0, 2, 1, 1);

        toolButton_VoxelFilter2ThresholdSwap = new QToolButton(layoutWidget);
        toolButton_VoxelFilter2ThresholdSwap->setObjectName(QStringLiteral("toolButton_VoxelFilter2ThresholdSwap"));

        gridLayout_3->addWidget(toolButton_VoxelFilter2ThresholdSwap, 0, 1, 1, 1);

        label_8 = new QLabel(layoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout_3->addWidget(label_8, 0, 0, 1, 1);


        verticalLayout_11->addLayout(gridLayout_3);

        horizontalSlider_VoxelFilter2ThresholdLow = new QSlider(layoutWidget);
        horizontalSlider_VoxelFilter2ThresholdLow->setObjectName(QStringLiteral("horizontalSlider_VoxelFilter2ThresholdLow"));
        horizontalSlider_VoxelFilter2ThresholdLow->setMaximum(65535);
        horizontalSlider_VoxelFilter2ThresholdLow->setOrientation(Qt::Horizontal);

        verticalLayout_11->addWidget(horizontalSlider_VoxelFilter2ThresholdLow);

        horizontalSlider_VoxelFilter2ThresholdHigh = new QSlider(layoutWidget);
        horizontalSlider_VoxelFilter2ThresholdHigh->setObjectName(QStringLiteral("horizontalSlider_VoxelFilter2ThresholdHigh"));
        horizontalSlider_VoxelFilter2ThresholdHigh->setMaximum(65535);
        horizontalSlider_VoxelFilter2ThresholdHigh->setValue(65535);
        horizontalSlider_VoxelFilter2ThresholdHigh->setOrientation(Qt::Horizontal);

        verticalLayout_11->addWidget(horizontalSlider_VoxelFilter2ThresholdHigh);

        label_32 = new QLabel(layoutWidget);
        label_32->setObjectName(QStringLiteral("label_32"));

        verticalLayout_11->addWidget(label_32);

        horizontalSlider_WeightingFactorDrr2 = new QSlider(layoutWidget);
        horizontalSlider_WeightingFactorDrr2->setObjectName(QStringLiteral("horizontalSlider_WeightingFactorDrr2"));
        horizontalSlider_WeightingFactorDrr2->setMaximum(100);
        horizontalSlider_WeightingFactorDrr2->setValue(100);
        horizontalSlider_WeightingFactorDrr2->setOrientation(Qt::Horizontal);

        verticalLayout_11->addWidget(horizontalSlider_WeightingFactorDrr2);

        label_Cam2Settings = new QLabel(layoutWidget);
        label_Cam2Settings->setObjectName(QStringLiteral("label_Cam2Settings"));

        verticalLayout_11->addWidget(label_Cam2Settings);


        verticalLayout_7->addLayout(verticalLayout_11);


        horizontalLayout_43->addLayout(verticalLayout_7);

        widget = new QWidget(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(20, 760, 591, 86));
        horizontalLayout_10 = new QHBoxLayout(widget);
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        horizontalLayout_10->setContentsMargins(0, 0, 0, 0);
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        Load_label = new QLabel(widget);
        Load_label->setObjectName(QStringLiteral("Load_label"));

        horizontalLayout_13->addWidget(Load_label);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_8);


        horizontalLayout_11->addLayout(horizontalLayout_13);


        verticalLayout_3->addLayout(horizontalLayout_11);

        horizontalLayout_45 = new QHBoxLayout();
        horizontalLayout_45->setSpacing(6);
        horizontalLayout_45->setObjectName(QStringLiteral("horizontalLayout_45"));
        SelectLoadingFileButton = new QPushButton(widget);
        SelectLoadingFileButton->setObjectName(QStringLiteral("SelectLoadingFileButton"));

        horizontalLayout_45->addWidget(SelectLoadingFileButton);

        LoadingFileLabel = new QLabel(widget);
        LoadingFileLabel->setObjectName(QStringLiteral("LoadingFileLabel"));

        horizontalLayout_45->addWidget(LoadingFileLabel);


        verticalLayout_3->addLayout(horizontalLayout_45);

        horizontalLayout_46 = new QHBoxLayout();
        horizontalLayout_46->setSpacing(6);
        horizontalLayout_46->setObjectName(QStringLiteral("horizontalLayout_46"));
        LoadingButton = new QPushButton(widget);
        LoadingButton->setObjectName(QStringLiteral("LoadingButton"));

        horizontalLayout_46->addWidget(LoadingButton);

        StopLoadingButton = new QPushButton(widget);
        StopLoadingButton->setObjectName(QStringLiteral("StopLoadingButton"));

        horizontalLayout_46->addWidget(StopLoadingButton);


        verticalLayout_3->addLayout(horizontalLayout_46);


        horizontalLayout_10->addLayout(verticalLayout_3);

        horizontalSpacer_3 = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_7 = new QSpacerItem(88, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_7);

        ReadLabel = new QLabel(widget);
        ReadLabel->setObjectName(QStringLiteral("ReadLabel"));

        horizontalLayout_8->addWidget(ReadLabel);

        DebugModeRadioButton = new QRadioButton(widget);
        DebugModeRadioButton->setObjectName(QStringLiteral("DebugModeRadioButton"));

        horizontalLayout_8->addWidget(DebugModeRadioButton);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        SelectReadingFileButton = new QPushButton(widget);
        SelectReadingFileButton->setObjectName(QStringLiteral("SelectReadingFileButton"));

        horizontalLayout_9->addWidget(SelectReadingFileButton);

        ReadingFileLabel = new QLabel(widget);
        ReadingFileLabel->setObjectName(QStringLiteral("ReadingFileLabel"));

        horizontalLayout_9->addWidget(ReadingFileLabel);


        verticalLayout_4->addLayout(horizontalLayout_9);

        horizontalLayout_49 = new QHBoxLayout();
        horizontalLayout_49->setSpacing(6);
        horizontalLayout_49->setObjectName(QStringLiteral("horizontalLayout_49"));
        StartReadingButton = new QPushButton(widget);
        StartReadingButton->setObjectName(QStringLiteral("StartReadingButton"));

        horizontalLayout_49->addWidget(StartReadingButton);

        StopReadingButton = new QPushButton(widget);
        StopReadingButton->setObjectName(QStringLiteral("StopReadingButton"));

        horizontalLayout_49->addWidget(StopReadingButton);


        verticalLayout_4->addLayout(horizontalLayout_49);


        horizontalLayout_10->addLayout(verticalLayout_4);

        widget1 = new QWidget(centralWidget);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(20, 850, 591, 23));
        horizontalLayout_6 = new QHBoxLayout(widget1);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        StartPhaseRecognitionButton = new QPushButton(widget1);
        StartPhaseRecognitionButton->setObjectName(QStringLiteral("StartPhaseRecognitionButton"));

        horizontalLayout_6->addWidget(StartPhaseRecognitionButton);

        phase_text = new QLabel(widget1);
        phase_text->setObjectName(QStringLiteral("phase_text"));
        phase_text->setMaximumSize(QSize(1777197, 16777215));
        phase_text->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout_6->addWidget(phase_text);

        phaseLabel = new QLabel(widget1);
        phaseLabel->setObjectName(QStringLiteral("phaseLabel"));

        horizontalLayout_6->addWidget(phaseLabel);

        horizontalSpacer_4 = new QSpacerItem(13, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);

        label_36 = new QLabel(widget1);
        label_36->setObjectName(QStringLiteral("label_36"));

        horizontalLayout_6->addWidget(label_36);

        fc_label = new QLabel(widget1);
        fc_label->setObjectName(QStringLiteral("fc_label"));

        horizontalLayout_6->addWidget(fc_label);

        fc_slider = new QSlider(widget1);
        fc_slider->setObjectName(QStringLiteral("fc_slider"));
        fc_slider->setOrientation(Qt::Horizontal);

        horizontalLayout_6->addWidget(fc_slider);

        MainWindow->setCentralWidget(centralWidget);
        layoutWidget->raise();
        layoutWidget->raise();
        label_Drr_2->raise();
        label_Xray_1->raise();
        label_Xray_2->raise();
        verticalLayoutWidget->raise();
        layoutWidget->raise();
        label_Drr_1->raise();
        layoutWidget->raise();
        Load_label->raise();
        Load_label->raise();
        Load_label->raise();
        DebugModeRadioButton->raise();
        ReadLabel->raise();
        phase_text->raise();
        StartPhaseRecognitionButton->raise();
        phaseLabel->raise();
        phaseLabel->raise();
        phase_text->raise();
        StartPhaseRecognitionButton->raise();
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1780, 20));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(pushButton_Xray1ListLoad, SIGNAL(clicked()), MainWindow, SLOT(OnButtonXrayList1Load()));
        QObject::connect(pushButton_Xray2ListLoad, SIGNAL(clicked()), MainWindow, SLOT(OnButtonXrayList2Load()));
        QObject::connect(pushButton_LoadMask1, SIGNAL(clicked()), MainWindow, SLOT(OnButtonActiveMask1Load()));
        QObject::connect(pushButton_LoadMask2, SIGNAL(clicked()), MainWindow, SLOT(OnButtonActiveMask2Load()));
        QObject::connect(horizontalSlider_Mask1PosH, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnButtonActiveMask1Save()));
        QObject::connect(horizontalSlider_Mask2PosH, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnButtonActiveMask2Save()));
        QObject::connect(radioButton_Mask11, SIGNAL(clicked()), MainWindow, SLOT(OnRadioButtonSelectMask11AsActiveClicked()));
        QObject::connect(radioButton_Mask12, SIGNAL(clicked()), MainWindow, SLOT(OnRadioButtonSelectMask12AsActiveClicked()));
        QObject::connect(radioButton_Mask21, SIGNAL(clicked()), MainWindow, SLOT(OnRadioButtonSelectMask21AsActiveClicked()));
        QObject::connect(radioButton_Mask22, SIGNAL(clicked()), MainWindow, SLOT(OnRadioButtonSelectMask22AsActiveClicked()));
        QObject::connect(horizontalSlider_Mask1Prop1, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderActiveMask1ValueChanged()));
        QObject::connect(horizontalSlider_Mask1Prop2, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderActiveMask1ValueChanged()));
        QObject::connect(horizontalSlider_Mask2Prop1, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderActiveMask2ValueChanged()));
        QObject::connect(horizontalSlider_Mask2Prop2, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderActiveMask2ValueChanged()));
        QObject::connect(horizontalSlider_Mask1PosV, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderActiveMask1ValueChanged()));
        QObject::connect(horizontalSlider_Mask1PosH, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderActiveMask1ValueChanged()));
        QObject::connect(horizontalSlider_Mask2PosH, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderActiveMask2ValueChanged()));
        QObject::connect(horizontalSlider_Mask2PosV, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderActiveMask2ValueChanged()));
        QObject::connect(pushButton_SaveMask2, SIGNAL(clicked()), MainWindow, SLOT(OnButtonActiveMask2Save()));
        QObject::connect(listWidget_Xray1, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), MainWindow, SLOT(OnXrayList1SelectionChanged(QListWidgetItem*,QListWidgetItem*)));
        QObject::connect(listWidget_Xray2, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), MainWindow, SLOT(OnXrayList2SelectionChanged(QListWidgetItem*,QListWidgetItem*)));
        QObject::connect(horizontalSlider_RotZ, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderTransRotValueChanged()));
        QObject::connect(pushButton_LoadVolume, SIGNAL(clicked()), MainWindow, SLOT(OnButtonVolumeLoad()));
        QObject::connect(pushButton_BatchRegistration2D3D, SIGNAL(clicked()), MainWindow, SLOT(OnButtonBatchRegistration2D3D()));
        QObject::connect(horizontalSlider_VoxelFilter1ThresholdHigh, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderVoxelFilter1ThresholdValueChanged()));
        QObject::connect(horizontalSlider_Windowing1Min, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderWindowing1ValueChanged()));
        QObject::connect(horizontalSlider_RotX, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderTransRotValueChanged()));
        QObject::connect(horizontalSlider_TransX, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderTransRotValueChanged()));
        QObject::connect(horizontalSlider_TransZ, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderTransRotValueChanged()));
        QObject::connect(horizontalSlider_Windowing1Max, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderWindowing1ValueChanged()));
        QObject::connect(horizontalSlider_TransY, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderTransRotValueChanged()));
        QObject::connect(pushButton_Test, SIGNAL(clicked()), MainWindow, SLOT(OnButtonTest()));
        QObject::connect(pushButton_Registration_2D3D, SIGNAL(clicked()), MainWindow, SLOT(OnButtonRegistration2D3D()));
        QObject::connect(horizontalSlider_RotY, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderTransRotValueChanged()));
        QObject::connect(doubleSpinBox_Cam2RotY, SIGNAL(valueChanged(double)), MainWindow, SLOT(OnCam2SettingsChanged()));
        QObject::connect(doubleSpinBox_Cam1RotX, SIGNAL(valueChanged(double)), MainWindow, SLOT(OnCam1SettingsChanged()));
        QObject::connect(doubleSpinBox_Cam1RotY, SIGNAL(valueChanged(double)), MainWindow, SLOT(OnCam1SettingsChanged()));
        QObject::connect(doubleSpinBox_Cam2DetectorDistance, SIGNAL(valueChanged(double)), MainWindow, SLOT(OnCam2SettingsChanged()));
        QObject::connect(doubleSpinBox_Cam2SourceDistance, SIGNAL(valueChanged(double)), MainWindow, SLOT(OnCam2SettingsChanged()));
        QObject::connect(doubleSpinBox_Cam1RotZ, SIGNAL(valueChanged(double)), MainWindow, SLOT(OnCam1SettingsChanged()));
        QObject::connect(doubleSpinBox_Cam2RotZ, SIGNAL(valueChanged(double)), MainWindow, SLOT(OnCam2SettingsChanged()));
        QObject::connect(doubleSpinBox_Cam2RotX, SIGNAL(valueChanged(double)), MainWindow, SLOT(OnCam2SettingsChanged()));
        QObject::connect(doubleSpinBox_Cam1SourceDistance, SIGNAL(valueChanged(double)), MainWindow, SLOT(OnCam1SettingsChanged()));
        QObject::connect(doubleSpinBox_Cam1DetectorDistance, SIGNAL(valueChanged(double)), MainWindow, SLOT(OnCam1SettingsChanged()));
        QObject::connect(horizontalSlider_VoxelFilter1ThresholdLow, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderVoxelFilter1ThresholdValueChanged()));
        QObject::connect(toolButton_VoxelFilter2ThresholdSwap, SIGNAL(clicked()), MainWindow, SLOT(OnButtonVoxelFilter2ThresholdSwap()));
        QObject::connect(toolButton_DrrTransRotUp, SIGNAL(clicked()), MainWindow, SLOT(OnButtonTransRotUp()));
        QObject::connect(horizontalSlider_VoxelFilter2ThresholdHigh, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderVoxelFilter2ThresholdValueChanged()));
        QObject::connect(horizontalSlider_Intensity2, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderIntensity2ValueChanged()));
        QObject::connect(toolButton_VoxelFilter1ThresholdUp, SIGNAL(clicked()), MainWindow, SLOT(OnButtonVoxelFilter1ThresholdUp()));
        QObject::connect(toolButton_Intensity1Up, SIGNAL(clicked()), MainWindow, SLOT(OnButtonIntensity1Up()));
        QObject::connect(horizontalSlider_BackgroundSaturation, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderBackgroundSaturationValueChanged()));
        QObject::connect(comboBox_RegistrationMode, SIGNAL(currentIndexChanged(QString)), MainWindow, SLOT(OnComboBoxRegistrationModeChanged()));
        QObject::connect(toolButton_Intensity2Down, SIGNAL(clicked()), MainWindow, SLOT(OnButtonIntensity2Down()));
        QObject::connect(pushButton_SaveDrrs, SIGNAL(clicked()), MainWindow, SLOT(OnButtonSave()));
        QObject::connect(toolButton_VoxelFilter2ThresholdDown, SIGNAL(clicked()), MainWindow, SLOT(OnButtonVoxelFilter2ThresholdDown()));
        QObject::connect(horizontalSlider_Intensity1, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderIntensity1ValueChanged()));
        QObject::connect(horizontalSlider_VoxelFilter2ThresholdLow, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderVoxelFilter2ThresholdValueChanged()));
        QObject::connect(toolButton_VoxelFilter1ThresholdSwap, SIGNAL(clicked()), MainWindow, SLOT(OnButtonVoxelFilter1ThresholdSwap()));
        QObject::connect(comboBox_RtSystem, SIGNAL(currentIndexChanged(int)), MainWindow, SLOT(OnComboBoxRtSystemChanged(int)));
        QObject::connect(toolButton_DrrTransformDown, SIGNAL(clicked()), MainWindow, SLOT(OnButtonTransRotDown()));
        QObject::connect(checkBox_XrayBS, SIGNAL(clicked()), MainWindow, SLOT(OnCheckBoxXrayBSClicked()));
        QObject::connect(horizontalSlider_WeightingFactorDrr1, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderWeightingFactorDrrValueChanged()));
        QObject::connect(horizontalSlider_WeightingFactorXray, SIGNAL(valueChanged(int)), MainWindow, SLOT(OnSliderWeightingFactorXrayValueChanged()));
        QObject::connect(pushButton_LoadConfig, SIGNAL(clicked()), MainWindow, SLOT(OnButtonLoadConfig()));
        QObject::connect(pushButton_CopyTransRot, SIGNAL(clicked()), MainWindow, SLOT(OnButtonCopyTransRot()));
        QObject::connect(pushButton_SaveConfig, SIGNAL(clicked()), MainWindow, SLOT(OnButtonSaveConfig()));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "FIRE-Fast Intensity-Based Image Registration in Real-Time", Q_NULLPTR));
        label_Drr_1->setText(QString());
        label_Drr_2->setText(QString());
        label_Xray_1->setText(QString());
        label_Xray_2->setText(QString());
        pushButton_Xray1ListLoad->setText(QApplication::translate("MainWindow", "Load Xray Images", Q_NULLPTR));
        radioButton_Mask11->setText(QApplication::translate("MainWindow", "Mask 1", Q_NULLPTR));
        radioButton_Mask12->setText(QApplication::translate("MainWindow", "Mask 2", Q_NULLPTR));
        label_12->setText(QApplication::translate("MainWindow", "Width:", Q_NULLPTR));
        label_10->setText(QApplication::translate("MainWindow", "Pos H:", Q_NULLPTR));
        label_11->setText(QApplication::translate("MainWindow", "Pos V:", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Height:", Q_NULLPTR));
        pushButton_LoadMask1->setText(QApplication::translate("MainWindow", "Load Mask", Q_NULLPTR));
        pushButton_SaveMask1->setText(QApplication::translate("MainWindow", "Save Mask", Q_NULLPTR));
        label_Mask1->setText(QString());
        pushButton_Xray2ListLoad->setText(QApplication::translate("MainWindow", "Load Xray Images", Q_NULLPTR));
        radioButton_Mask21->setText(QApplication::translate("MainWindow", "Mask 1", Q_NULLPTR));
        radioButton_Mask22->setText(QApplication::translate("MainWindow", "Mask 2", Q_NULLPTR));
        label_14->setText(QApplication::translate("MainWindow", "Pos V:", Q_NULLPTR));
        label_13->setText(QApplication::translate("MainWindow", "Pos H:", Q_NULLPTR));
        label_15->setText(QApplication::translate("MainWindow", "Width:", Q_NULLPTR));
        label_27->setText(QApplication::translate("MainWindow", "Height:", Q_NULLPTR));
        pushButton_LoadMask2->setText(QApplication::translate("MainWindow", "Load Mask", Q_NULLPTR));
        pushButton_SaveMask2->setText(QApplication::translate("MainWindow", "Save Mask", Q_NULLPTR));
        label_Mask2->setText(QString());
        label_28->setText(QApplication::translate("MainWindow", "RT System", Q_NULLPTR));
        label_5->setText(QApplication::translate("MainWindow", "Registration Mode", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Merit Function", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Optimizer Settings", Q_NULLPTR));
        checkBox_tx->setText(QApplication::translate("MainWindow", "tX", Q_NULLPTR));
        checkBox_ty->setText(QApplication::translate("MainWindow", "tY", Q_NULLPTR));
        checkBox_tz->setText(QApplication::translate("MainWindow", "tZ", Q_NULLPTR));
        checkBox_ry->setText(QApplication::translate("MainWindow", "rY", Q_NULLPTR));
        checkBox_rx->setText(QApplication::translate("MainWindow", "rX", Q_NULLPTR));
        checkBox_rz->setText(QApplication::translate("MainWindow", "rZ", Q_NULLPTR));
        label_20->setText(QApplication::translate("MainWindow", "Background Saturation", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "DRR Rotation", Q_NULLPTR));
        label_26->setText(QApplication::translate("MainWindow", "DRR Translation", Q_NULLPTR));
        toolButton_DrrTransRotUp->setText(QApplication::translate("MainWindow", "\342\206\221", Q_NULLPTR));
        toolButton_DrrTransformDown->setText(QApplication::translate("MainWindow", "\342\206\223", Q_NULLPTR));
        label_25->setText(QApplication::translate("MainWindow", "X-Ray Bone Suppression", Q_NULLPTR));
        checkBox_XrayBS->setText(QString());
        label_weightsBS->setText(QString());
        pushButton_LoadVolume->setText(QApplication::translate("MainWindow", "Load Volume (BS)", Q_NULLPTR));
        pushButton_LoadConfig->setText(QApplication::translate("MainWindow", "Load Config", Q_NULLPTR));
        pushButton_SaveConfig->setText(QApplication::translate("MainWindow", "SaveConfig", Q_NULLPTR));
        pushButton_Registration_2D3D->setText(QApplication::translate("MainWindow", "Registration 2D/3D", Q_NULLPTR));
        pushButton_BatchRegistration2D3D->setText(QApplication::translate("MainWindow", "Batch Registration 2D/3D", Q_NULLPTR));
        pushButton_Test->setText(QApplication::translate("MainWindow", "Test", Q_NULLPTR));
        pushButton_SaveDrrs->setText(QApplication::translate("MainWindow", "Save", Q_NULLPTR));
        pushButton_CopyTransRot->setText(QApplication::translate("MainWindow", "Copy TransRot", Q_NULLPTR));
        label_18->setText(QApplication::translate("MainWindow", "Cam1 Reference Rotation Angle [deg]", Q_NULLPTR));
        label_17->setText(QApplication::translate("MainWindow", "Cam1 Source and Detector Position [mm]", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Cam1 Intensity", Q_NULLPTR));
        toolButton_Intensity1Up->setText(QApplication::translate("MainWindow", "\342\206\221", Q_NULLPTR));
        label_7->setText(QApplication::translate("MainWindow", "Cam 1 Windowing", Q_NULLPTR));
        toolButton_VoxelFilter1ThresholdSwap->setText(QApplication::translate("MainWindow", "\342\206\273", Q_NULLPTR));
        toolButton_VoxelFilter1ThresholdUp->setText(QApplication::translate("MainWindow", "\342\206\221", Q_NULLPTR));
        label_16->setText(QApplication::translate("MainWindow", "Cam1 Voxel Filter Thresholds", Q_NULLPTR));
        label_29->setText(QApplication::translate("MainWindow", "Cam1 Voxel Filter Suppression", Q_NULLPTR));
        label_Cam1Settings->setText(QString());
        label_19->setText(QApplication::translate("MainWindow", "Cam2 Reference Rotation Angle [deg]", Q_NULLPTR));
        label_22->setText(QApplication::translate("MainWindow", "Cam2 Source and Detector Position [mm]", Q_NULLPTR));
        label_21->setText(QString());
        label_9->setText(QApplication::translate("MainWindow", "Cam2 Intensity", Q_NULLPTR));
        toolButton_Intensity2Down->setText(QApplication::translate("MainWindow", "\342\206\223", Q_NULLPTR));
        label_31->setText(QApplication::translate("MainWindow", "Cam 2 Windowing", Q_NULLPTR));
        toolButton_VoxelFilter2ThresholdDown->setText(QApplication::translate("MainWindow", "\342\206\223", Q_NULLPTR));
        toolButton_VoxelFilter2ThresholdSwap->setText(QApplication::translate("MainWindow", "\342\206\273", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Cam2 Voxel Filter Thresholds", Q_NULLPTR));
        label_32->setText(QApplication::translate("MainWindow", "Cam 2 Voxel Filter Suppression", Q_NULLPTR));
        label_Cam2Settings->setText(QString());
        Load_label->setText(QApplication::translate("MainWindow", "Planning CT Data", Q_NULLPTR));
        SelectLoadingFileButton->setText(QApplication::translate("MainWindow", "Select File", Q_NULLPTR));
        LoadingFileLabel->setText(QString());
        LoadingButton->setText(QApplication::translate("MainWindow", "Load", Q_NULLPTR));
        StopLoadingButton->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        ReadLabel->setText(QApplication::translate("MainWindow", "Real Time Data", Q_NULLPTR));
        DebugModeRadioButton->setText(QApplication::translate("MainWindow", "Debug Mode", Q_NULLPTR));
        SelectReadingFileButton->setText(QApplication::translate("MainWindow", "Select File", Q_NULLPTR));
        ReadingFileLabel->setText(QString());
        StartReadingButton->setText(QApplication::translate("MainWindow", "Start Reading", Q_NULLPTR));
        StopReadingButton->setText(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        StartPhaseRecognitionButton->setText(QApplication::translate("MainWindow", "Start Phase Recognition", Q_NULLPTR));
        phase_text->setText(QApplication::translate("MainWindow", "Phase:", Q_NULLPTR));
        phaseLabel->setText(QApplication::translate("MainWindow", "0          ", Q_NULLPTR));
        label_36->setText(QApplication::translate("MainWindow", "fc:", Q_NULLPTR));
        fc_label->setText(QApplication::translate("MainWindow", "0.00 Hz", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
