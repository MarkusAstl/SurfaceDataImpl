// Author: Ing. Ingo Gulyas, MMSc
// Date: 21.11.2019
// Medical University of Vienna / AKH Wien
// Department of Radiotherapy

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ImageDataFile.h"
#include "CudaDrrRenderer.h"
#include "MeritFunctions.h"

#include "Matrix.h"
#include "Registration.h"
#include "MeritFunctions.h"
#include "Mask.h"
#include "Timer.h"

#include <math.h>

#ifdef _WIN32
    #include "unistd_.h"
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#include <QFileDialog>
#include <QDateTime>
#include <QShortcut>
#include <QThread>
#include <QProgressDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QKeyEvent>
#include <QtMath>
#include <QTextStream>
#include <QString>
#include <QClipboard>
#include <armadillo>

// Phase Recognition
#include <surfacedata.h>
#include <loadingthread.h>
#include <qcustomplot.h>
#include <QThread>
#include <QLocale>
#include <QFile>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include <QLayout>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <new>
#include <chrono>

using namespace arma;

#pragma message("INFO: performance improvement -> use rectangular mask for cuda convolution and use cudaMallocHost for ImageDataFile to aquire pinned memory!")
#pragma message("INFO: performance improvement -> allocate kernel data and image memory and run blurring function calls without allocation/free procedures within cuda code!")


#define BONE_SUPPRESSION 1

#define CONVERT_SLIDER_VALUE_TO_INTENSITY(x) (pow((double)(x), 4.)/100000000000.)
#define CONVERT_INTENSITY_TO_SLIDER_VALUE(x) (qPow(x*100000000000., 1./4.))


static void CallbackDrrRenderer(void* instance);
static void CallbackBatchDrrRenderer(void* instance);
static void CallbackBatchDrrRendererRibsBS(void* instance);
static void CallbackBatchDrrRendererTumorBS(void* instance);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    RtSystemsConfigInit();

    qApp->installEventFilter(this);

    volume_dir = QDir::home().currentPath();

    // init drr and mask memory
    memset(drr_1, 0x00, sizeof(drr_1));
    memset(drr_2, 0x00, sizeof(drr_2));
    Mask_Init(&mask_11, drr_width, drr_height);
    Mask_Init(&mask_12, drr_width, drr_height);
    Mask_Init(&mask_21, drr_width, drr_height);
    Mask_Init(&mask_22, drr_width, drr_height);

    // enable strechted images
    ui->label_Drr_1->setScaledContents(true);
    ui->label_Drr_2->setScaledContents(true);
    ui->label_Xray_1->setScaledContents(true);
    ui->label_Xray_2->setScaledContents(true);

    // add registration methods to comboBox
    this->ui->comboBox_RegistrationMode->clear();
    for(int i=0; i<ARRAY_ELEMENTS(RegistrationMode_N); i++)
    {
        this->ui->comboBox_RegistrationMode->addItem(RegistrationMode_N[i]);
    }

    // add merit functions to comboBox
    this->ui->comboBox_MeritFunction->clear();
    for(int i=0; i<ARRAY_ELEMENTS(MeritFunctionName_T); i++)
    {
        this->ui->comboBox_MeritFunction->addItem(MeritFunctionName_T[i]);
    }

    // add configurations to combobox
    this->ui->comboBox_RtSystem->clear();
    for(int i=0; i<ARRAY_ELEMENTS(RtSysConfig); i++)
    {
        this->ui->comboBox_RtSystem->addItem(RtSysConfig[i].name);
    }

    // init windowing values, intensity, ..
    OnSliderWindowing1ValueChanged();
    OnSliderTransRotValueChanged();
    OnSliderIntensity1ValueChanged();
    OnSliderIntensity2ValueChanged();

    // init cuda + masks
    Cuda_Init();
    Cuda_Drr_LoadMask(0, mask_11.width, mask_11.height, mask_11.img);
    Cuda_Drr_LoadMask(1, mask_12.width, mask_12.height, mask_12.img);
    Cuda_Drr_LoadMask(2, mask_21.width, mask_21.height, mask_21.img);
    Cuda_Drr_LoadMask(3, mask_22.width, mask_22.height, mask_22.img);

    // set mask size to max
    this->ui->horizontalSlider_Mask1Prop1->setValue(this->ui->horizontalSlider_Mask1Prop1->maximum());
    this->ui->horizontalSlider_Mask1Prop2->setValue(this->ui->horizontalSlider_Mask1Prop2->maximum());
    this->ui->horizontalSlider_Mask2Prop1->setValue(this->ui->horizontalSlider_Mask2Prop1->maximum());
    this->ui->horizontalSlider_Mask2Prop2->setValue(this->ui->horizontalSlider_Mask2Prop2->maximum());


    //--------------------------- Phase Recognition --------------------------------------------------------------------

    // thread for reading surface data dynamically during treatment
    rThread = new SurfaceData(this, ui);    // SurfaceData is a QThread object

    // thread for loading surface data from planning CT
    lThread = new LoadingThread(this);

    // Show initial cutoff frequency
    QString fc_text = QString::number(rThread->fc) + " Hz";
    this->ui->fc_label->setText(fc_text);
    int start_fc = rThread->fc * 100;
    ui->fc_slider->setSliderPosition(start_fc);
}

MainWindow::~MainWindow()
{
    Cuda_DeInit();
    delete ui;
}


// SECTION: RADIATHERAPY SYSTEM CONFIGURATION ///////////////////////////////////////////////////////////////
void MainWindow::RtSystemsConfigInit()
{
    RtSysConfig[0].name = "Elekta Versa Linac";
    RtSysConfig[0].imgSys1.name = "Xray";
    //RtSysConfig[0].imgSys1.source_distance = 536.f;
    //RtSysConfig[0].imgSys1.detector_distance = -1000.f;
    //Vector4_T VersaLinac_xray_rot_vect = {-90.f, 0.f, 0.f, 1.0f};
    RtSysConfig[0].imgSys1.source_distance = -536.f;
    RtSysConfig[0].imgSys1.detector_distance = 1000.f;
    Vector4_T VersaLinac_xray_rot_vect = {+90.f, 180.f, 180.f, 1.0f};
    RtSysConfig[0].imgSys1.pixel_spacing_ratio = 512./410.*0.796f;
    memcpy(&RtSysConfig[0].imgSys1.rotation_ref, &VersaLinac_xray_rot_vect, sizeof(RtSysConfig[0].imgSys1.rotation_ref));
    RtSysConfig[0].imgSys2.name = "";
    RtSysConfig[0].imgSys2.source_distance = 0.f;
    RtSysConfig[0].imgSys2.detector_distance = 0.f;
    RtSysConfig[0].imgSys2.pixel_spacing_ratio = 0.f;
    Vector4_T VersaLinac_beam_rot_vect = {0.f, 0.f, 0.f, 1.0f};
    memcpy(&RtSysConfig[0].imgSys2.rotation_ref, &VersaLinac_beam_rot_vect, sizeof(RtSysConfig[0].imgSys2.rotation_ref));

    RtSysConfig[1].name = "MedAustron Imaging Ring";
    RtSysConfig[1].imgSys1.name = "Xray";
    RtSysConfig[1].imgSys1.source_distance = 470.1f;
    RtSysConfig[1].imgSys1.detector_distance = -630.6f;
    RtSysConfig[1].imgSys1.pixel_spacing_ratio = 1.f;
    // Detector has 1024x1024 pixels with spacing of 0.4 mm
    // FIRE is rendering 410x410 pixels -> spacing = 1024/410*0.4 = 0.999 mm
    Vector4_T ImagingRing_xray_rot_vect = {-90.f, 180.f, 0.f, 1024.f/410.f*0.4f};
    memcpy(&RtSysConfig[1].imgSys1.rotation_ref, &ImagingRing_xray_rot_vect, sizeof(RtSysConfig[1].imgSys1.rotation_ref));
    RtSysConfig[1].imgSys2.name = "";
    RtSysConfig[1].imgSys2.source_distance = 0.f;
    RtSysConfig[1].imgSys2.detector_distance = 0.f;
    RtSysConfig[1].imgSys2.pixel_spacing_ratio = 0.f;
    Vector4_T ImagingRing_beam_rot_vect = {0.f, 0.f, 0.f, 1.0f};
    memcpy(&RtSysConfig[1].imgSys2.rotation_ref, &ImagingRing_beam_rot_vect, sizeof(RtSysConfig[1].imgSys2.rotation_ref));

    RtSysConfig[2].name = "Elekta Versa Linac Hugo";
    RtSysConfig[2].imgSys1.name = "Xray";
    RtSysConfig[2].imgSys1.source_distance = -1000.f-362.f;
    RtSysConfig[2].imgSys1.detector_distance = 1000.f-362.f;
    RtSysConfig[2].imgSys1.pixel_spacing_ratio = 2.f;
    Vector4_T VersaLinacHugo_xray_rot_vect = {-90.f, 0.f, 90.f, 1.0f};
    memcpy(&RtSysConfig[2].imgSys1.rotation_ref, &VersaLinacHugo_xray_rot_vect, sizeof(RtSysConfig[2].imgSys1.rotation_ref));
    RtSysConfig[2].imgSys2.name = "";
    RtSysConfig[2].imgSys2.source_distance = 0.f;
    RtSysConfig[2].imgSys2.detector_distance = 0.f;
    RtSysConfig[2].imgSys2.pixel_spacing_ratio = 0.f;
    Vector4_T VersaLinacHugo_beam_rot_vect = {0.f, 0.f, 0.f, 1.0f};
    memcpy(&RtSysConfig[2].imgSys2.rotation_ref, &VersaLinacHugo_beam_rot_vect, sizeof(RtSysConfig[2].imgSys2.rotation_ref));
}

// SECTION: IMPORT VOLUME(S) AND SAVED SETTINGS (IF AVAILABLE) //////////////////////////////////////////////
void MainWindow::OnButtonVolumeLoad()
{
    // Setup filter string for file open dialog box
    QString filterStr = "(*.avw *.nii)";

    // show file opening dialog
    QString filePath;
    if(!OpenSingleFileDialog("Select volume", volume_dir, filterStr, &filePath, true)) return;

    if(ImportVolumeData(filePath))
    {
        //Cuda_Drr_LoadMask(0, mask_11.width, mask_11.height, mask_11.img);
        UpdateVoxelFilterSliders();


#if !(BONE_SUPPRESSION)
        RegistrationMode_T reg_mode = (RegistrationMode_T)this->ui->comboBox_RegistrationMode->currentIndex();
        if(reg_mode == REGISTRATION_MODE_SINGLE)
        {
            RenderDrr(DrrSelector_T(DRR_1), &idf_vol);
            UpdateView(UpdateSelector_T(UPDATE_DRR_1));
        }
        else
        {
            RenderDrr(DISP_DRR_ALL, &idf_vol);
            UpdateView(DISP_DRR_ALL);
        }
#else
        // BONE-SUBTRACTION
        RenderDrr(DISP_DRR_ALL, &idf_vol);
        UpdateView(DISP_ALL);
#endif
    }
}
void MainWindow::OnButtonSaveConfig()
{
    if(!idf_vol.IsInitialized())
    {
        DBG_WARNING("please load volume data first!\n");
        return;
    }

    QString path, name, ext;
    FileInfo(idf_vol.filePath, path, name, ext);
    QString opt_settings = QString::number(ui->checkBox_tx->isChecked()) + QString::number(ui->checkBox_ty->isChecked()) + QString::number(ui->checkBox_tz->isChecked()) +
                           QString::number(ui->checkBox_rx->isChecked()) + QString::number(ui->checkBox_ry->isChecked()) + QString::number(ui->checkBox_rz->isChecked());

    name = name + "_MERIT" + QString(MeritFunctionNameShort_T[ui->comboBox_MeritFunction->currentIndex()]) + "_OPT" + opt_settings + "_BS" + QString::number(ui->checkBox_XrayBS->isChecked()) + ".fire";

    QString fileComplex = QString(path + QDir::separator() + "%1").arg(name);
    QString filepath = QFileDialog::getSaveFileName(this, "Save Settings File", fileComplex, "FIRE-settings(*.fire)");

    // create/rewrite config file
    QFile file(filepath);
    if(file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text))
    {
        // first line contains filepath to volume data

        // absolute path
        QString data_strg = idf_vol.filePath + "\n";

        // relative path (config file -> volume / xrays)
        //QDir cfgDir = QDir(filepath);
        //QString relpath = cfgDir.relativeFilePath(idf_vol.filePath);
        //QString data_strg = relpath + "\n";

        // second line contains configuration settings
        // INFO: the configuration structure is designed for forward compatiblity, therefore newly introduced parameters are appended at the end of configuration section (violation of hierarchical order)
        data_strg += QString::number(ui->comboBox_RtSystem->currentIndex()) + "|" +
                     QString::number(ui->comboBox_RegistrationMode->currentIndex()) + "|" +
                     QString::number(ui->comboBox_MeritFunction->currentIndex()) + "|" +
                     QString::number(ui->horizontalSlider_BackgroundSaturation->value()) + "|" +
                     QString::number(ui->checkBox_tx->isChecked()) + "|" + QString::number(ui->checkBox_ty->isChecked()) + "|" + QString::number(ui->checkBox_tz->isChecked()) + "|" +
                     QString::number(ui->checkBox_rx->isChecked()) + "|" + QString::number(ui->checkBox_ry->isChecked()) + "|" + QString::number(ui->checkBox_rz->isChecked()) + "|" +
                     QString::number(drr_transrot.trans_X) + "|" + QString::number(drr_transrot.trans_Y) + "|" + QString::number(drr_transrot.trans_Z) + "|" +
                     QString::number(drr_transrot.rot_X) + "|" + QString::number(drr_transrot.rot_Y) + "|" + QString::number(drr_transrot.rot_Z) + "|" +
                     QString::number(CONVERT_SLIDER_VALUE_TO_INTENSITY(ui->horizontalSlider_Intensity1->value())) + "|" +
                     QString::number(ui->horizontalSlider_VoxelFilter1ThresholdLow->value()) + "|" +
                     QString::number(ui->horizontalSlider_VoxelFilter1ThresholdHigh->value()) + "|" +
                     QString::number(ui->horizontalSlider_WeightingFactorDrr1->value()) + "|" +
                     QString::number(ui->horizontalSlider_WeightingFactorXray->value()) + "|" +
                     QString::number(ui->checkBox_XrayBS->isChecked()) + "|" +
                     QString::number(mask_11.pos_x) + "|" + QString::number(mask_11.pos_y) + "|" + QString::number(mask_11.rect_width) + "|" + QString::number(mask_11.rect_height) + "|" +
                     QString::number(mask_12.pos_x) + "|" + QString::number(mask_12.pos_y) + "|" + QString::number(mask_12.rect_width) + "|" + QString::number(mask_12.rect_height) + "|" +
                     QString::number((ui->radioButton_Mask11->isChecked() ? 1:2)) + "|" +
                     // config v1 ends here (31 parameters)
                     QString::number(ui->horizontalSlider_Windowing1Min->value()) + "|" +
                     QString::number(ui->horizontalSlider_Windowing1Max->value()) + "|" +
                     QString::number(CONVERT_SLIDER_VALUE_TO_INTENSITY(ui->horizontalSlider_Intensity2->value())) + "|" +
                     QString::number(ui->horizontalSlider_Windowing2Min->value()) + "|" +
                     QString::number(ui->horizontalSlider_Windowing2Max->value()) + "|" +
                     QString::number(ui->horizontalSlider_VoxelFilter2ThresholdLow->value()) + "|" +
                     QString::number(ui->horizontalSlider_VoxelFilter2ThresholdHigh->value()) + "|" +
                     QString::number(ui->horizontalSlider_WeightingFactorDrr2->value()) + "|" +
                     QString::number(mask_21.pos_x) + "|" + QString::number(mask_21.pos_y) + "|" + QString::number(mask_21.rect_width) + "|" + QString::number(mask_21.rect_height) + "|" +
                     QString::number(mask_22.pos_x) + "|" + QString::number(mask_22.pos_y) + "|" + QString::number(mask_22.rect_width) + "|" + QString::number(mask_22.rect_height) + "|" +
                     QString::number((ui->radioButton_Mask21->isChecked() ? 1:2)) + "\n";
                     // config v2 ends here (48 parameters)

        // third line contains filepaths of x-ray series (cam 1)
        for(int i=0; i<ui->listWidget_Xray1->count(); i++)
        {
            data_strg += ui->listWidget_Xray1->item(i)->text();
            if(i < ui->listWidget_Xray1->count()-1)
            {
                data_strg += "|";
            }
        }
        data_strg += "\n";

        // fourth line contains filepaths of x-ray series (cam 2)
        for(int i=0; i<ui->listWidget_Xray2->count(); i++)
        {
            data_strg += ui->listWidget_Xray2->item(i)->text();
            if(i < ui->listWidget_Xray2->count()-1)
            {
                data_strg += "|";
            }
        }
        data_strg += "\n";

        QByteArray qbyteArr = data_strg.toUtf8();
        if(file.write(qbyteArr) == qbyteArr.length())
        {
            QMessageBox msgBox;
            msgBox.setText("Settings saved");
            msgBox.exec();
        }

        file.close();
    }
}
void MainWindow::OnButtonLoadConfig()
{
    QString path, filename, ext;
    FileInfo(idf_vol.filePath, path, filename, ext);
    OpenSingleFileDialog("Select Settings File", volume_dir, "*.fire", &path, true);

    // import settings
    int active_mask_1_id = 1;
    int active_mask_2_id = 1;

    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString line = in.readLine();
        ImportVolumeData(line);
        line = in.readLine();
        QStringList lst = line.split("|");

        // restore settings
        if((lst.count() == 31) || (lst.count() == 48))
        {
            ui->comboBox_RtSystem->setCurrentIndex(lst.at(0).toInt());
            ui->comboBox_RegistrationMode->setCurrentIndex(lst.at(1).toInt());
            ui->comboBox_MeritFunction->setCurrentIndex(lst.at(2).toInt());
            ui->horizontalSlider_BackgroundSaturation->setValue(lst.at(3).toInt());
            ui->checkBox_tx->setChecked(lst.at(4).toInt());
            ui->checkBox_ty->setChecked(lst.at(5).toInt());
            ui->checkBox_tz->setChecked(lst.at(6).toInt());
            ui->checkBox_rx->setChecked(lst.at(7).toInt());
            ui->checkBox_ry->setChecked(lst.at(8).toInt());
            ui->checkBox_rz->setChecked(lst.at(9).toInt());
            drr_transrot.trans_X = lst.at(10).toFloat();
            drr_transrot.trans_Y = lst.at(11).toFloat();
            drr_transrot.trans_Z = lst.at(12).toFloat();
            drr_transrot.rot_X = lst.at(13).toFloat();
            drr_transrot.rot_Y = lst.at(14).toFloat();
            drr_transrot.rot_Z = lst.at(15).toFloat();
            ui->horizontalSlider_Intensity1->setValue(CONVERT_INTENSITY_TO_SLIDER_VALUE(lst.at(16).toFloat()));
            ui->horizontalSlider_VoxelFilter1ThresholdLow->setValue(lst.at(17).toInt());
            ui->horizontalSlider_VoxelFilter1ThresholdHigh->setValue(lst.at(18).toInt());

            ui->horizontalSlider_WeightingFactorDrr1->setValue(lst.at(19).toInt());
            ui->horizontalSlider_WeightingFactorXray->setValue(lst.at(20).toInt());
            ui->checkBox_XrayBS->setChecked(lst.at(21).toInt());

            Mask_DrawRect(&mask_11, lst.at(22).toInt(), lst.at(23).toInt(), lst.at(24).toInt(), lst.at(25).toInt(), ui->horizontalSlider_BackgroundSaturation->value());
            Mask_DrawRect(&mask_12, lst.at(26).toInt(), lst.at(27).toInt(), lst.at(28).toInt(), lst.at(29).toInt(), ui->horizontalSlider_BackgroundSaturation->value());
            active_mask_1_id = lst.at(30).toInt();
            Cuda_Drr_LoadMask(0, mask_11.width, mask_11.height, mask_11.img);
            Cuda_Drr_LoadMask(1, mask_12.width, mask_12.height, mask_12.img);

            if(lst.count() == 48)
            {
                ui->horizontalSlider_Windowing1Min->setValue(lst.at(31).toInt());
                ui->horizontalSlider_Windowing1Max->setValue(lst.at(32).toInt());
                ui->horizontalSlider_Intensity2->setValue(CONVERT_INTENSITY_TO_SLIDER_VALUE(lst.at(33).toFloat()));
                ui->horizontalSlider_Windowing2Min->setValue(lst.at(34).toInt());
                ui->horizontalSlider_Windowing2Max->setValue(lst.at(35).toInt());
                ui->horizontalSlider_VoxelFilter2ThresholdLow->setValue(lst.at(36).toInt());
                ui->horizontalSlider_VoxelFilter2ThresholdHigh->setValue(lst.at(37).toInt());
                ui->horizontalSlider_WeightingFactorDrr2->setValue(lst.at(38).toInt());

                Mask_DrawRect(&mask_21, lst.at(39).toInt(), lst.at(40).toInt(), lst.at(41).toInt(), lst.at(42).toInt(), ui->horizontalSlider_BackgroundSaturation->value());
                Mask_DrawRect(&mask_22, lst.at(43).toInt(), lst.at(44).toInt(), lst.at(45).toInt(), lst.at(46).toInt(), ui->horizontalSlider_BackgroundSaturation->value());
                active_mask_2_id = lst.at(47).toInt();
                Cuda_Drr_LoadMask(2, mask_21.width, mask_21.height, mask_21.img);
                Cuda_Drr_LoadMask(3, mask_22.width, mask_22.height, mask_22.img);

                DBG_PRINT("settings restored\n");
            }
            else
            {
                // INFO: standard settings were used for cam2 since config file does not provide parameters (config v1 loaded)
                ui->horizontalSlider_Windowing1Min->setValue(ui->horizontalSlider_Windowing1Min->minimum());
                ui->horizontalSlider_Windowing1Max->setValue(ui->horizontalSlider_Windowing1Max->maximum());
                ui->horizontalSlider_Intensity2->setValue(ui->horizontalSlider_Intensity2->maximum());
                ui->horizontalSlider_Windowing2Min->setValue(ui->horizontalSlider_Windowing2Min->minimum());
                ui->horizontalSlider_Windowing2Max->setValue(ui->horizontalSlider_Windowing2Max->maximum());
                ui->horizontalSlider_VoxelFilter2ThresholdLow->setValue(ui->horizontalSlider_VoxelFilter2ThresholdLow->minimum());
                ui->horizontalSlider_VoxelFilter2ThresholdHigh->setValue(ui->horizontalSlider_VoxelFilter2ThresholdHigh->maximum());
                ui->horizontalSlider_WeightingFactorDrr2->setValue(ui->horizontalSlider_WeightingFactorDrr2->minimum());

                Mask_DrawRect(&mask_21, 0, 0, drr_width, drr_height, ui->horizontalSlider_BackgroundSaturation->value());
                Mask_DrawRect(&mask_22, 0, 0, drr_width, drr_height, ui->horizontalSlider_BackgroundSaturation->value());
                active_mask_2_id = 0;
                Cuda_Drr_LoadMask(2, mask_21.width, mask_21.height, mask_21.img);
                Cuda_Drr_LoadMask(3, mask_22.width, mask_22.height, mask_22.img);

                // show msg box to inform user about using standard settings for paramter 31-47
                QMessageBox msgBox;
                msgBox.setWindowTitle("WARNING");
                msgBox.setText("Config file v1 detected -> the following parameters are missing:\n\nCam1:\n\tWindowing(min, max)\nCam2:\n\tIntensity\n\tWindowing(min, max)\n\tVoxelFilterThreshold(min, max)\n\tWeightingFactorDRR2\n\tMask21(posx, posy, width, height, bg_sat)\n\tMask22(posx, posy, width, height, bg_sat)\n\nSTANDARD VALUES WERE USED!\nPLEASE CHECK CORRESPONDING SETTINGS...");
                msgBox.exec();
            }
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("ERROR");
            msgBox.setText("Config file invalid!");
            msgBox.exec();

            DBG_WARNING("invalid number of configuration parameters found, %d instead of %d (v1) or %d (v2) -> import canceled!\n", lst.count(), 31, 47);
        }


        // restore x-ray list cam1
        ui->listWidget_Xray1->clear();
        line = in.readLine();
        lst = line.split("|");
        if(!line.isEmpty() && (lst.count() > 0))
        {
            for(int i=0; i<lst.count(); i++)
            {
                if(QFile::exists(lst.at(i)))
                {
                    ui->listWidget_Xray1->addItem(lst.at(i));
                }
                else
                {
                    DBG_WARNING("x-ray file >%s< for cam1 not found -> data import canceled!", lst.at(i).toLatin1().constData());
                    break;
                }
            }

            // select first x-ray image from list if records are present
            if(ui->listWidget_Xray1->count() > 0)
            {
                ui->listWidget_Xray1->setCurrentRow(0);
            }
        }

        // restore x-ray list cam2
        ui->listWidget_Xray2->clear();
        line = in.readLine();
        lst = line.split("|");
        if(!line.isEmpty() && (lst.count() > 0))
        {
            for(int i=0; i<lst.count(); i++)
            {
                if(QFile::exists(lst.at(i)))
                {
                    ui->listWidget_Xray2->addItem(lst.at(i));
                }
                else
                {
                    DBG_WARNING("x-ray file >%s< for cam2 not found -> data import canceled!", lst.at(i).toLatin1().constData());
                    break;
                }
            }

            // select first x-ray image from list if records are present
            if(ui->listWidget_Xray2->count() > 0)
            {
                ui->listWidget_Xray2->setCurrentRow(0);
            }
        }


        file.close();

        // trigger mask1 update
        ui->radioButton_Mask11->setChecked(false);
        ui->radioButton_Mask12->setChecked(false);
        if(active_mask_1_id == 1)
        {
            ui->radioButton_Mask11->setChecked(true);
            OnRadioButtonSelectMask11AsActiveClicked();
        }
        else
        {
            ui->radioButton_Mask12->setChecked(true);
            OnRadioButtonSelectMask12AsActiveClicked();
        }

        // trigger mask2 update
        ui->radioButton_Mask21->setChecked(false);
        ui->radioButton_Mask22->setChecked(false);
        if(active_mask_2_id == 1)
        {
            ui->radioButton_Mask21->setChecked(true);
            OnRadioButtonSelectMask21AsActiveClicked();
        }
        else
        {
            ui->radioButton_Mask22->setChecked(true);
            OnRadioButtonSelectMask22AsActiveClicked();
        }


        UpdateStatusBar();
        OnButtonTransRotUp();
        RenderDrr(DISP_DRR_ALL, &idf_vol);
        UpdateView(DISP_ALL);

        OnSliderWeightingFactorXrayValueChanged();
    }
    else
    {
        DBG_WARNING("could not access settings file!\n");
    }
}
bool MainWindow::ImportVolumeData(QString filePath)
{
    bool res = idf_vol.Load(filePath);

    if(res)
    {
        DBG_PRINT("volume data loaded w/h/d: %d/%d/%d\n", idf_vol.width, idf_vol.height, idf_vol.depth);
        Cuda_Drr_LoadVolume(0, idf_vol.imgData, idf_vol.width, idf_vol.height, idf_vol.depth);

        // try to load volume used for bone suppression (no tumor) if existing
        QFileInfo f_info(filePath);
        QString path, name, ext;
        FileInfo(filePath, path, name, ext);
        QString filePathBS = QDir(path).absoluteFilePath(name + "-BS." + ext);

        DBG_PRINT("trying to load volume data used for bone suppression\n\t>%s<\n", filePathBS.toLatin1().constData());

        if(idf_vol_bs.Load(filePathBS))
        {
            DBG_PRINT("\t-> volume loaded\n");
            Cuda_Drr_LoadVolume(1, idf_vol_bs.imgData, idf_vol.width, idf_vol.height, idf_vol.depth);
        }
        else
        {
            DBG_PRINT("\t-> loading failed\n");
            if(idf_vol_bs.IsInitialized())
            {
                Cuda_Drr_UnloadVolume(1);
                idf_vol_bs.Free();
            }
        }

        UpdateVoxelFilterSliders();
    }

    return res;
}
void MainWindow::UpdateVoxelFilterSliders()
{
    //update voxel filter threshold slider ranges
    unsigned short min, max, min_bs, max_bs;

    GetMinMax(idf_vol.imgData, idf_vol.width*idf_vol.height*idf_vol.depth, &min, &max);

    if(idf_vol_bs.IsInitialized())
    {
        GetMinMax(idf_vol_bs.imgData, idf_vol_bs.width*idf_vol_bs.height*idf_vol_bs.depth, &min_bs, &max_bs);

        if(min_bs < min)
        {
            min = min_bs;
        }

        if(max_bs > max)
        {
            max = max_bs;
        }
    }

    ui->horizontalSlider_VoxelFilter1ThresholdLow->setMinimum(min);
    ui->horizontalSlider_VoxelFilter1ThresholdLow->setMaximum(max);
    ui->horizontalSlider_VoxelFilter1ThresholdHigh->setMinimum(min);
    ui->horizontalSlider_VoxelFilter1ThresholdHigh->setMaximum(max);
    ui->horizontalSlider_VoxelFilter2ThresholdLow->setMinimum(min);
    ui->horizontalSlider_VoxelFilter2ThresholdLow->setMaximum(max);
    ui->horizontalSlider_VoxelFilter2ThresholdHigh->setMinimum(min);
    ui->horizontalSlider_VoxelFilter2ThresholdHigh->setMaximum(max);

    ui->horizontalSlider_VoxelFilter1ThresholdLow->setValue(min);
    ui->horizontalSlider_VoxelFilter1ThresholdHigh->setValue(max);
    ui->horizontalSlider_VoxelFilter2ThresholdLow->setValue(min);
    ui->horizontalSlider_VoxelFilter2ThresholdHigh->setValue(max);
}

// SECTION: IMPORT AND SELECT X-RAYS ////////////////////////////////////////////////////////////////////////
void MainWindow::OnButtonXrayList1Load()
{
    QStringList file_lst;

    OpenMultipleFilesDialog("Select X-Ray(s)", volume_dir, "(*.pgm *.his)", &file_lst, false);

    if(file_lst.count() == 0) return;

    ui->listWidget_Xray1->clear();

    // add filepaths to list widget
    for(int i=0; i<file_lst.count(); i++)
    {
       ui->listWidget_Xray1->addItem(file_lst.at(i));
    }

    ui->listWidget_Xray1->setCurrentRow(0);
}
void MainWindow::OnButtonXrayList2Load()
{
    QStringList file_lst;

    OpenMultipleFilesDialog("Select X-Ray(s)", volume_dir, "(*.pgm *.his)", &file_lst, false);

    if(file_lst.count() == 0) return;

    ui->listWidget_Xray2->clear();

    // add filepaths to list widget
    for(int i=0; i<file_lst.count(); i++)
    {
       ui->listWidget_Xray2->addItem(file_lst.at(i));
    }

    ui->listWidget_Xray2->setCurrentRow(0);
}
void MainWindow::OnXrayList1SelectionChanged(QListWidgetItem* li, QListWidgetItem* li_prev)
{
    if(li != NULL)
    {
        idf_xray_1.Load(li->text().toLatin1().constData());

        if((idf_xray_1.width != drr_width) || (idf_xray_1.height != drr_height))
        {
            QMessageBox msgBox;
            msgBox.setText("Invalid image dimensions -> expected (" + \
                           QString::number(drr_width) + "/" + QString::number(drr_height) + ") " + \
                           "instead of (" + \
                           QString::number(idf_xray_1.width) + "/" + QString::number(idf_xray_1.height) + ")!");
            msgBox.exec();

            idf_xray_1.Free();
        }

        if(ui->checkBox_XrayBS->isChecked())
        {
            SuppressBonesInXray(ui->horizontalSlider_VoxelFilter1ThresholdLow->value(), ui->horizontalSlider_VoxelFilter1ThresholdHigh->value(), ui->horizontalSlider_WeightingFactorXray->value()/100.);
        }
    }

    UpdateView(DISP_XRAY_1);
}
void MainWindow::OnXrayList2SelectionChanged(QListWidgetItem* li, QListWidgetItem* li_prev)
{
    if(li != NULL)
    {
        idf_xray_2.Load(li->text().toLatin1().constData());

        if((idf_xray_2.width != drr_width) || (idf_xray_2.height != drr_height))
        {
            QMessageBox msgBox;
            msgBox.setText("Invalid image dimensions -> expected (" + \
                           QString::number(drr_width) + "/" + QString::number(drr_height) + ") " + \
                           "instead of (" + \
                           QString::number(idf_xray_2.width) + "/" + QString::number(idf_xray_2.height) + ")!");
            msgBox.exec();

            idf_xray_2.Free();
        }
    }

    UpdateView(DISP_XRAY_2);
}

// SECTION: MASK MANAGEMENT /////////////////////////////////////////////////////////////////////////////////
void MainWindow::OnButtonActiveMask1Load()
{
    // Setup filter string for file open dialog box
    QString filterStr = "(*.pgm)";

    // Show file open dialog
    QString filePath;
    if(!OpenSingleFileDialog("Select active Mask 1", volume_dir, filterStr, &filePath, false))
    {
        Mask_Init(active_mask_1, drr_width, drr_height);
        ui->label_Mask1->setText("");
        Cuda_Drr_LoadMask(0, active_mask_1->width, active_mask_1->height, active_mask_1->img);
        RenderDrr(DISP_DRR_1, &idf_vol);
        UpdateView(DisplaySelector_T(DISP_DRR_1 | DISP_XRAY_1));
        return;
    }

    // import mask image
    Mask_LoadPgm(active_mask_1, filePath);

    // show images including mask
    Cuda_Drr_LoadMask(active_mask_1_cuda_id, active_mask_1->width, active_mask_1->height, active_mask_1->img);
    RenderDrr(DISP_DRR_1, &idf_vol);
    UpdateView(DisplaySelector_T(DISP_DRR_1 | DISP_XRAY_1));

    // display mask file name
    ui->label_Mask1->setText("mask file: " + active_mask_1->file_path);
}
void MainWindow::OnButtonActiveMask2Load()
{
    // Setup filter string for file open dialog box
    QString filterStr = "(*.pgm)";

    // Show file open dialog
    QString filePath;
    if(!OpenSingleFileDialog("Selecte active Mask 2", volume_dir, filterStr, &filePath, false))
    {
        Mask_Init(active_mask_2, drr_width, drr_height);
        ui->label_Mask2->setText("");
        Cuda_Drr_LoadMask(1, active_mask_2->width, active_mask_2->height, active_mask_2->img);
        RenderDrr(DISP_DRR_2, &idf_vol);
        UpdateView(DisplaySelector_T(DISP_DRR_2 | DISP_XRAY_2));
        return;
    }

    // import mask image
    Mask_LoadPgm(active_mask_2, filePath);

    // show images including mask
    Cuda_Drr_LoadMask(active_mask_2_cuda_id, active_mask_2->width, active_mask_2->height, active_mask_2->img);
    RenderDrr(DISP_DRR_2, &idf_vol);
    UpdateView(DisplaySelector_T(DISP_DRR_2 | DISP_XRAY_2));

    // display mask file name
    ui->label_Mask2->setText("mask file: " + active_mask_2->file_path);
}
void MainWindow::OnButtonActiveMask1Save()
{
    Mask_SavePgm(active_mask_1, QDir::homePath() + "/Mask1.pgm", true);
}
void MainWindow::OnButtonActiveMask2Save()
{
    Mask_SavePgm(active_mask_2, QDir::homePath() + "/Mask2.pgm", true);
}

void MainWindow::OnRadioButtonSelectMask11AsActiveClicked()
{
    active_mask_1 = &mask_11;
    active_mask_1_cuda_id = 0;

    // block slider signals and update slider positions
    ui->horizontalSlider_Mask1PosH->blockSignals(true);
    ui->horizontalSlider_Mask1PosV->blockSignals(true);
    ui->horizontalSlider_Mask1Prop1->blockSignals(true);
    ui->horizontalSlider_Mask1Prop2->blockSignals(true);
    ui->horizontalSlider_Mask1PosH->setValue(active_mask_1->pos_x);
    ui->horizontalSlider_Mask1PosV->setValue(active_mask_1->pos_y);
    ui->horizontalSlider_Mask1Prop1->setValue(active_mask_1->rect_width);
    ui->horizontalSlider_Mask1Prop2->setValue(active_mask_1->rect_height);
    ui->horizontalSlider_Mask1PosH->blockSignals(false);
    ui->horizontalSlider_Mask1PosV->blockSignals(false);
    ui->horizontalSlider_Mask1Prop1->blockSignals(false);
    ui->horizontalSlider_Mask1Prop2->blockSignals(false);

    Mask_ChangeBackground(active_mask_1, ui->horizontalSlider_BackgroundSaturation->value());
    Cuda_Drr_LoadMask(active_mask_1_cuda_id, active_mask_1->width, active_mask_1->height, active_mask_1->img);
    RenderDrr(DISP_DRR_1, &idf_vol);
    UpdateView(DisplaySelector_T(DISP_DRR_1 | DISP_XRAY_1));
    ui->label_Mask1->setText("x/y/w/h: " + QString::number(active_mask_1->pos_x) + "/" + QString::number(active_mask_1->pos_y) + "/" + QString::number(active_mask_1->rect_width) + "/" + QString::number(active_mask_1->rect_height));
}
void MainWindow::OnRadioButtonSelectMask12AsActiveClicked()
{
    active_mask_1 = &mask_12;
    active_mask_1_cuda_id = 1;

    // block slider signals and update slider positions
    ui->horizontalSlider_Mask1PosH->blockSignals(true);
    ui->horizontalSlider_Mask1PosV->blockSignals(true);
    ui->horizontalSlider_Mask1Prop1->blockSignals(true);
    ui->horizontalSlider_Mask1Prop2->blockSignals(true);
    ui->horizontalSlider_Mask1PosH->setValue(active_mask_1->pos_x);
    ui->horizontalSlider_Mask1PosV->setValue(active_mask_1->pos_y);
    ui->horizontalSlider_Mask1Prop1->setValue(active_mask_1->rect_width);
    ui->horizontalSlider_Mask1Prop2->setValue(active_mask_1->rect_height);
    ui->horizontalSlider_Mask1PosH->blockSignals(false);
    ui->horizontalSlider_Mask1PosV->blockSignals(false);
    ui->horizontalSlider_Mask1Prop1->blockSignals(false);
    ui->horizontalSlider_Mask1Prop2->blockSignals(false);

    Mask_ChangeBackground(active_mask_1, ui->horizontalSlider_BackgroundSaturation->value());
    Cuda_Drr_LoadMask(active_mask_1_cuda_id, active_mask_1->width, active_mask_1->height, active_mask_1->img);
    RenderDrr(DISP_DRR_1, &idf_vol_bs);
    UpdateView(DisplaySelector_T(DISP_DRR_1 | DISP_XRAY_1));
    ui->label_Mask1->setText("x/y/w/h: " + QString::number(active_mask_1->pos_x) + "/" + QString::number(active_mask_1->pos_y) + "/" + QString::number(active_mask_1->rect_width) + "/" + QString::number(active_mask_1->rect_height));
}
void MainWindow::OnRadioButtonSelectMask21AsActiveClicked()
{
    /*active_mask_2 = &mask_21;
    active_mask_2_cuda_id = 2;

    // block slider signals and update slider positions
    ui->horizontalSlider_Mask2PosH->blockSignals(true);
    ui->horizontalSlider_Mask2PosV->blockSignals(true);
    ui->horizontalSlider_Mask2Prop1->blockSignals(true);
    ui->horizontalSlider_Mask2Prop2->blockSignals(true);
    ui->horizontalSlider_Mask2PosH->setValue(active_mask_2->pos_x);
    ui->horizontalSlider_Mask2PosV->setValue(active_mask_2->pos_y);
    ui->horizontalSlider_Mask2Prop1->setValue(active_mask_2->rect_width);
    ui->horizontalSlider_Mask2Prop2->setValue(active_mask_2->rect_height);
    ui->horizontalSlider_Mask2PosH->blockSignals(false);
    ui->horizontalSlider_Mask2PosV->blockSignals(false);
    ui->horizontalSlider_Mask2Prop1->blockSignals(false);
    ui->horizontalSlider_Mask2Prop2->blockSignals(false);

    Mask_ChangeBackground(active_mask_2, ui->horizontalSlider_BackgroundSaturation->value());
    Cuda_Drr_LoadMask(active_mask_2_cuda_id, active_mask_2->width, active_mask_2->height, active_mask_2->img);
    RenderDrr(DISP_DRR_2, &idf_vol);
    UpdateView(DisplaySelector_T(DISP_DRR_2 | DISP_XRAY_2));
    ui->label_Mask2->setText("x/y/w/h: " + QString::number(active_mask_2->pos_x) + "/" + QString::number(active_mask_2->pos_y) + "/" + QString::number(active_mask_2->rect_width) + "/" + QString::number(active_mask_2->rect_height));*/
}
void MainWindow::OnRadioButtonSelectMask22AsActiveClicked()
{
    /*active_mask_2 = &mask_22;
    active_mask_2_cuda_id = 3;

    // block slider signals and update slider positions
    ui->horizontalSlider_Mask2PosH->blockSignals(true);
    ui->horizontalSlider_Mask2PosV->blockSignals(true);
    ui->horizontalSlider_Mask2Prop1->blockSignals(true);
    ui->horizontalSlider_Mask2Prop2->blockSignals(true);
    ui->horizontalSlider_Mask2PosH->setValue(active_mask_2->pos_x);
    ui->horizontalSlider_Mask2PosV->setValue(active_mask_2->pos_y);
    ui->horizontalSlider_Mask2Prop1->setValue(active_mask_2->rect_width);
    ui->horizontalSlider_Mask2Prop2->setValue(active_mask_2->rect_height);
    ui->horizontalSlider_Mask2PosH->blockSignals(false);
    ui->horizontalSlider_Mask2PosV->blockSignals(false);
    ui->horizontalSlider_Mask2Prop1->blockSignals(false);
    ui->horizontalSlider_Mask2Prop2->blockSignals(false);

    Mask_ChangeBackground(active_mask_2, ui->horizontalSlider_BackgroundSaturation->value());
    Cuda_Drr_LoadMask(active_mask_2_cuda_id, active_mask_2->width, active_mask_2->height, active_mask_2->img);
    RenderDrr(DISP_DRR_2, &idf_vol);
    UpdateView(DisplaySelector_T(DISP_DRR_2 | DISP_XRAY_2));
    ui->label_Mask2->setText("x/y/w/h: " + QString::number(active_mask_2->pos_x) + "/" + QString::number(active_mask_2->pos_y) + "/" + QString::number(active_mask_2->rect_width) + "/" + QString::number(active_mask_2->rect_height));*/
}

void MainWindow::OnSliderActiveMask1ValueChanged()
{
    int pos_h = ui->horizontalSlider_Mask1PosH->value();
    int pos_v = ui->horizontalSlider_Mask1PosV->value();
    int width = ui->horizontalSlider_Mask1Prop1->value();
    int height = ui->horizontalSlider_Mask1Prop2->value();

    Mask_DrawRect(active_mask_1, pos_h, pos_v, width, height, background_saturation);
    //Mask_DrawDisk(active_mask_1, pos_h, pos_v, width, background_saturation);
    ui->label_Mask1->setText("x/y/w/h: " + QString::number(pos_h) + "/" + QString::number(pos_v) + "/" + QString::number(width) + "/" + QString::number(height));

#pragma message ("INFO: use mask typedef within cuda code!")
    Cuda_Drr_LoadMask(active_mask_1_cuda_id, active_mask_1->width, active_mask_1->height, active_mask_1->img);
    RenderDrr(DISP_DRR_1, &idf_vol);

    if(ui->checkBox_XrayBS->isChecked() && (active_mask_1 == &mask_12))
    {
        idf_xray_1.Load(idf_xray_1.filePath);
        SuppressBonesInXray(ui->horizontalSlider_VoxelFilter1ThresholdLow->value(), ui->horizontalSlider_VoxelFilter1ThresholdHigh->value(), ui->horizontalSlider_WeightingFactorXray->value()/100.);
    }

    UpdateView(DisplaySelector_T(DISP_DRR_1 | DISP_XRAY_1));
}
void MainWindow::OnSliderActiveMask2ValueChanged()
{
    int pos_h = ui->horizontalSlider_Mask2PosH->value();
    int pos_v = ui->horizontalSlider_Mask2PosV->value();
    int width = ui->horizontalSlider_Mask2Prop1->value();
    int height = ui->horizontalSlider_Mask2Prop2->value();

    Mask_DrawRect(active_mask_2, pos_h, pos_v, width, height, background_saturation);
    //Mask_DrawDisk(active_mask_2, pos_h, pos_v, radius, background_saturation);
    ui->label_Mask2->setText("x/y/w/h: " + QString::number(pos_h) + "/" + QString::number(pos_v) + "/" + QString::number(width) + "/" + QString::number(height));

#pragma message ("INFO: use mask typedef within cuda code!")
    Cuda_Drr_LoadMask(active_mask_2_cuda_id, active_mask_2->width, active_mask_2->height, active_mask_2->img);
    RenderDrr(DISP_DRR_2, &idf_vol);
    UpdateView(DisplaySelector_T(DISP_DRR_2 | DISP_XRAY_2));
}

// SECTION: RAY CASTING /////////////////////////////////////////////////////////////////////////////////////
void MainWindow::OnComboBoxRtSystemChanged(int sel)
{
    if((sel >= 0) && (sel < ARRAY_ELEMENTS(RtSysConfig)))
    {
        RtSysActive = &RtSysConfig[sel];
        RenderDrr(DISP_DRR_ALL, &idf_vol);
        UpdateView(DISP_DRR_ALL);
    }
}
void MainWindow::OnComboBoxRegistrationModeChanged()
{
    RegistrationMode_T reg_mode = (RegistrationMode_T)this->ui->comboBox_RegistrationMode->currentIndex();

    if(reg_mode == REGISTRATION_MODE_SINGLE)
    {
        RenderDrr(DISP_DRR_1, &idf_vol);
        UpdateView(DISP_DRR_1);
    }
    else
    {
        RenderDrr(DISP_DRR_ALL, &idf_vol);
        UpdateView(DISP_DRR_ALL);
    }
}
void MainWindow::OnCam1SettingsChanged()
{
#pragma message ("INFO: deprecated code -> camera settings are now predefined within RtSysConf!")
    /*RtSysConf_active->xray_source_distance = ui->doubleSpinBox_Cam1SourceDistance->value();
    RtSysConf_active->xray_detector_distance = ui->doubleSpinBox_Cam1DetectorDistance->value();
    RtSysConf_active->xray_rotation_refz[0] = ui->doubleSpinBox_Cam1RotX->value();
    RtSysConf_active->xray_rotation_refz[1] = ui->doubleSpinBox_Cam1RotY->value();
    RtSysConf_active->xray_rotation_refz[2] = ui->doubleSpinBox_Cam1RotZ->value();
    RenderDrr(DrrSelector_T(DRR_1), &idf_vol);
    UpdateView(UPDATE_DRR_1);*/
}
void MainWindow::OnCam2SettingsChanged()
{
#pragma message ("INFO: deprecated code -> camera settings are now predefined within RtSysConf!")
    /*RtSysConf_active->beam_source_distance = ui->doubleSpinBox_Cam2SourceDistance->value();
    RtSysConf_active->beam_detector_distance = ui->doubleSpinBox_Cam2DetectorDistance->value();
    RtSysConf_active->beam_rotation_refz[0] = ui->doubleSpinBox_Cam2RotX->value();
    RtSysConf_active->beam_rotation_refz[1] = ui->doubleSpinBox_Cam2RotY->value();
    RtSysConf_active->beam_rotation_refz[2] = ui->doubleSpinBox_Cam2RotZ->value();
    RenderDrr(DrrSelector_T(DRR_2), &idf_vol);
    UpdateView(UPDATE_DRR_2);*/
}
void MainWindow::OnSliderIntensity1ValueChanged()
{
    drr_intensity1 = CONVERT_SLIDER_VALUE_TO_INTENSITY(ui->horizontalSlider_Intensity1->value());
    RenderDrr(DISP_DRR_1, &idf_vol);
    UpdateView(DISP_DRR_1);
    UpdateCam1Status();
}
void MainWindow::OnSliderIntensity2ValueChanged()
{
    drr_intensity2 = CONVERT_SLIDER_VALUE_TO_INTENSITY(ui->horizontalSlider_Intensity2->value());
    RenderDrr(DISP_DRR_2, &idf_vol);
    UpdateView(DISP_DRR_2);
    UpdateCam2Status();
}
void MainWindow::OnSliderVoxelFilter1ThresholdValueChanged()
{
    if(idf_vol.IsInitialized())
    {
        RenderDrr(DISP_DRR_1, &idf_vol);
        UpdateView(DISP_DRR_1);
    }

    if(ui->checkBox_XrayBS->isChecked() && idf_xray_1.IsInitialized())
    {
        idf_xray_1.Load(idf_xray_1.filePath);
        SuppressBonesInXray(ui->horizontalSlider_VoxelFilter1ThresholdLow->value(), ui->horizontalSlider_VoxelFilter1ThresholdHigh->value(), ui->horizontalSlider_WeightingFactorXray->value()/100.);
        UpdateView(DISP_XRAY_1);
    }

    UpdateCam1Status();
}
void MainWindow::OnSliderVoxelFilter2ThresholdValueChanged()
{
    RegistrationMode_T reg_mode = (RegistrationMode_T)this->ui->comboBox_RegistrationMode->currentIndex();

    if(idf_vol.IsInitialized() && reg_mode == REGISTRATION_MODE_DOUBLE)
    {
        RenderDrr(DISP_DRR_ALL, &idf_vol);
        UpdateView(DISP_DRR_ALL);
    }
    UpdateCam2Status();
}
void MainWindow::OnSliderBackgroundSaturationValueChanged()
{
    background_saturation = ui->horizontalSlider_BackgroundSaturation->value();

    Mask_ChangeBackground(&mask_11, background_saturation);
    Mask_ChangeBackground(&mask_12, background_saturation);
    Mask_ChangeBackground(&mask_21, background_saturation);
    Mask_ChangeBackground(&mask_22, background_saturation);

    Cuda_Drr_LoadMask(0, mask_11.width, mask_11.height, mask_11.img);
    Cuda_Drr_LoadMask(1, mask_12.width, mask_12.height, mask_12.img);
    Cuda_Drr_LoadMask(2, mask_21.width, mask_21.height, mask_21.img);
    Cuda_Drr_LoadMask(3, mask_22.width, mask_22.height, mask_22.img);

    RenderDrr(DISP_DRR_ALL, &idf_vol);
    UpdateView(DISP_ALL);
}
void MainWindow::OnSliderTransRotValueChanged()
{
    drr_transrot.trans_X = ui->horizontalSlider_TransX->value()/10.;
    drr_transrot.trans_Y = ui->horizontalSlider_TransY->value()/10.;
    drr_transrot.trans_Z = ui->horizontalSlider_TransZ->value()/10.;

    drr_transrot.rot_X = ui->horizontalSlider_RotX->value()/10.;
    drr_transrot.rot_Y = ui->horizontalSlider_RotY->value()/10.;
    drr_transrot.rot_Z = ui->horizontalSlider_RotZ->value()/10.;

#if !(BONE_SUPPRESSION)
    RegistrationMode_T reg_mode = (RegistrationMode_T)this->ui->comboBox_RegistrationMode->currentIndex();
    if(reg_mode == REGISTRATION_MODE_SINGLE)
    {
        RenderDrr(DrrSelector_T(DRR_1), &idf_vol);
        UpdateView(UpdateSelector_T(UPDATE_DRR_1));
    }
    else
    {
        RenderDrr(DISP_DRR_ALL, &idf_vol);
        UpdateView(DISP_DRR_ALL);
    }
#else
    // BONE-SUBTRATCION
    RenderDrr(DISP_DRR_1, &idf_vol);
    UpdateView(DISP_DRR_1);

    if(ui->checkBox_XrayBS->isChecked() && idf_xray_1.IsInitialized())
    {
        idf_xray_1.Load(idf_xray_1.filePath);
        SuppressBonesInXray(ui->horizontalSlider_VoxelFilter1ThresholdLow->value(), ui->horizontalSlider_VoxelFilter1ThresholdHigh->value(), ui->horizontalSlider_WeightingFactorXray->value()/100.);
        UpdateView(DISP_XRAY_1);
    }
#endif
}
void MainWindow::OnButtonTransRotUp()
{
    // disable slider signals
    ui->horizontalSlider_RotX->blockSignals(true);
    ui->horizontalSlider_RotY->blockSignals(true);
    ui->horizontalSlider_RotZ->blockSignals(true);
    ui->horizontalSlider_TransX->blockSignals(true);
    ui->horizontalSlider_TransY->blockSignals(true);

    // update slider values
    ui->horizontalSlider_RotX->setValue(round(drr_transrot.rot_X*10.));
    ui->horizontalSlider_RotY->setValue(round(drr_transrot.rot_Y*10.));
    ui->horizontalSlider_RotZ->setValue(round(drr_transrot.rot_Z*10.));
    ui->horizontalSlider_TransX->setValue(round(drr_transrot.trans_X*10.));
    ui->horizontalSlider_TransY->setValue(round(drr_transrot.trans_Y*10.));
    ui->horizontalSlider_TransZ->setValue(round(drr_transrot.trans_Z*10.));

    // enable slider signals
    ui->horizontalSlider_RotX->blockSignals(false);
    ui->horizontalSlider_RotY->blockSignals(false);
    ui->horizontalSlider_RotZ->blockSignals(false);
    ui->horizontalSlider_TransX->blockSignals(false);
    ui->horizontalSlider_TransY->blockSignals(false);
}
void MainWindow::OnButtonTransRotDown()
{
    OnSliderTransRotValueChanged();
}
void MainWindow::OnSliderWeightingFactorDrrValueChanged()
{
    RenderDrr(DISP_DRR_ALL, &idf_vol);
    UpdateView(DISP_DRR_ALL);
    UpdateCam1Status();
}
void MainWindow::OnSliderWeightingFactorXrayValueChanged()
{
    if((ui->listWidget_Xray1->currentRow() >= 0) && ui->checkBox_XrayBS->isChecked())
    {
        idf_xray_1.Load(ui->listWidget_Xray1->currentItem()->text());
        SuppressBonesInXray(ui->horizontalSlider_VoxelFilter1ThresholdLow->value(),
                            ui->horizontalSlider_VoxelFilter1ThresholdHigh->value(),
                            ui->horizontalSlider_WeightingFactorXray->value()/100.);
        UpdateView(DISP_XRAY_1);
    }
    UpdateBSStatus();
}
void MainWindow::OnCheckBoxXrayBSClicked()
{
    if(idf_xray_1.IsInitialized())
    {
        idf_xray_1.Load(idf_xray_1.filePath);
        if(ui->checkBox_XrayBS->isChecked())
        {
            SuppressBonesInXray(ui->horizontalSlider_VoxelFilter1ThresholdLow->value(), ui->horizontalSlider_VoxelFilter1ThresholdHigh->value(), ui->horizontalSlider_WeightingFactorXray->value()/100.);
        }
        UpdateView(DISP_XRAY_1);
    }
}
void MainWindow::RenderDrr(DisplaySelector_T sel, ImageDataFile* idf)
{
    if((idf!=NULL) && (idf_vol.IsInitialized()))
    {
        if(sel & DISP_DRR_1)
        {
            unsigned short* drr_bs = NULL;
            unsigned short* drr_orig = NULL;

            Rendering(RDROPT_DIFF, &idf_vol, active_mask_1_cuda_id, &drr_transrot, &RtSysActive->imgSys1, ui->horizontalSlider_VoxelFilter1ThresholdLow->value(), ui->horizontalSlider_VoxelFilter1ThresholdHigh->value(), ui->horizontalSlider_WeightingFactorDrr1->value()/100., ui->horizontalSlider_Windowing1Min->value(), ui->horizontalSlider_Windowing1Max->value(), CONVERT_SLIDER_VALUE_TO_INTENSITY(ui->horizontalSlider_Intensity1->value()), &drr_bs, &drr_orig);
            //Cuda_Conv_Run(drr_orig, drr_width, drr_height, kernel_weights_gauss9, 3);

            memcpy(drr_1, drr_bs, sizeof(drr_1));
            memcpy(drr_2, drr_orig, sizeof(drr_2));

            UpdateView(DisplaySelector_T(DISP_DRR_2));
        }

        if(sel & DISP_DRR_2)
        {

        }
    }
}
void MainWindow::Rendering(RenderOption_T rdr_opt, ImageDataFile* idf, unsigned short cuda_mask_id, TransRot_T* transrot, ImagingSystemConfig* imgConf, unsigned short threshold_min, unsigned short threshold_max, float weighting, unsigned short windowing_min, unsigned short windowing_max, float intensity, unsigned short** buf1_out, unsigned short** buf2_out)
{
    Matrix4x4_T mat_view;
    Matrix4x4_InitIdentity(mat_view);
    Matrix4x4_Translate(mat_view, transrot->trans_X+idf->width/2, transrot->trans_Y+idf->height/2, transrot->trans_Z+idf->depth/2);
    Matrix4x4_Rotate(mat_view, transrot->rot_X, transrot->rot_Y, transrot->rot_Z);
    Cuda_Drr_SetProperties(drr_width, drr_height, imgConf->pixel_spacing_ratio);

    Vector4_T source_pos_rot;
    Vector4_T detector_pos_rot;
    Matrix4x4_Rotate(mat_view, imgConf->rotation_ref[0], imgConf->rotation_ref[1], imgConf->rotation_ref[2]);

    Vector4_T source_pos_rel = {0., 0., imgConf->detector_distance, 1.};
    Vector4_T detector_pos_rel = {0., 0., imgConf->source_distance, 1.};
    Matrix4x4_MultiplyVector(mat_view, source_pos_rel, source_pos_rot);
    Matrix4x4_MultiplyVector(mat_view, detector_pos_rel, detector_pos_rot);

    unsigned short cuda_volume_id = 0;
    if(idf == &idf_vol_bs)
    {
        cuda_volume_id = 1;
    }

    Cuda_Drr_Threshold_Render(rdr_opt, cuda_volume_id, cuda_mask_id, buf1_out, buf2_out, threshold_min, threshold_max, 1.-weighting, windowing_min, windowing_max, source_pos_rot, detector_pos_rot, intensity, mat_view);
}

// SECTION: IMAGE DISPLAY ///////////////////////////////////////////////////////////////////////////////////
void MainWindow::UpdateView(DisplaySelector_T sel)
{
    if(idf_vol.IsInitialized())
    {
        if(sel & DISP_DRR_1)
        {
            // display drr
            unsigned short buffer[drr_size];
            memcpy(buffer, drr_1, drr_size*sizeof(unsigned short));
            Windowing8Bit(buffer, drr_size, 0, UINT16_MAX);
            DisplayImage(buffer, drr_width, drr_height, this->ui->label_Drr_1, NULL);
        }

        if(sel & DISP_DRR_2)
        {
            // display drr
            unsigned short buffer[drr_size];
            memcpy(buffer, drr_2, drr_size*sizeof(unsigned short));
            Windowing8Bit(buffer, drr_size, 0, UINT16_MAX);
            DisplayImage(buffer, drr_width, drr_height, this->ui->label_Drr_2, NULL);
        }
    }

    if(sel & DISP_XRAY_1)
    {
        if(idf_xray_1.IsInitialized())
        {
            int sz = idf_xray_1.width*idf_xray_1.height;

            // allocate memory for image data
            unsigned short* buffer = new (std::nothrow) unsigned short[sz];
            if(buffer == NULL)
            {
                DBG_ERROR("memory allocation for image data failed");
                return;
            }

            memcpy(buffer, idf_xray_1.imgData, sz*sizeof(unsigned short));
            Windowing8Bit(buffer, sz, 0, UINT16_MAX);
            DisplayImage(buffer, idf_xray_1.width, idf_xray_1.height, this->ui->label_Xray_1, active_mask_1);

            delete buffer;
        }
        else
        {
            ui->label_Xray_1->clear();
        }
    }

    if(sel & DISP_XRAY_2)
    {
        if(idf_xray_2.IsInitialized())
        {
            int sz = idf_xray_2.width*idf_xray_2.height;

            // allocate memory for image data
            unsigned short* buffer = new (std::nothrow) unsigned short[sz];
            if(buffer == NULL)
            {
                DBG_ERROR("memory allocation for image data failed");
                return;
            }

            memcpy(buffer, idf_xray_2.imgData, sz*sizeof(unsigned short));
            Windowing8Bit(buffer, sz, 0, UINT16_MAX);
            DisplayImage(buffer, idf_xray_2.width, idf_xray_2.height, this->ui->label_Xray_2, active_mask_2);

            delete buffer;

            DisplayImage(idf_xray_2.imgData, idf_xray_2.width, idf_xray_2.height, this->ui->label_Xray_2, active_mask_2);
        }
        else
        {
            ui->label_Xray_2->clear();
        }
    }

    UpdateStatusBar();
}
void MainWindow::DisplayImage(unsigned short data[], unsigned int width, unsigned int height, QLabel* label, Mask_T* mask)
{
    // allocate memory for image data
    unsigned short* buffer = new (std::nothrow) unsigned short[width*height];
    if(buffer == NULL)
    {
        DBG_ERROR("memory allocation for image data failed");
        return;
    }

    if(mask)
    {
        for(unsigned int i=0; i<width*height; i++)
        {
            buffer[i] = data[i]*mask->img[i]/255.;
        }
    }
    else
    {
        memcpy(buffer, data, width*height*sizeof(unsigned short));
    }

    // init image
    QImage qimg(width, height, QImage::Format_RGB32);
    Init8BitImage(&qimg, buffer);

    // show pixmap
    QPixmap pixmap = QPixmap::fromImage(qimg);
    label->setPixmap(pixmap);

    delete buffer;
}
void MainWindow::Init8BitImage(QImage* image, unsigned short data[])
{
    int val;
    int height = image->height();
    int width = image->width();

    for(int h=0; h<height; h++)
    {
        for(int w=0; w<width; w++)
        {
            val = data[h*width+w];
            image->setPixel(w, h, qRgb(val, val, val));
        }
    }
}
void MainWindow::OnSliderWindowing1ValueChanged()
{
    UpdateCam1Status();
    RenderDrr(DISP_DRR_1, &idf_vol);
    UpdateView(DISP_DRR_1);
}

// SECTION: 2D/3D REGISTRATION //////////////////////////////////////////////////////////////////////////////
int MainWindow::UpdateOptimizerTransRotAccordingSelection()
{
    // INFO: generate translation & rotation vector for optimizer according to state of checkboxes

    int dim = 0;

    if(ui->checkBox_tx->isChecked())
    {
        opt_transrot[dim++] = drr_transrot.trans_X;
    }
    if(ui->checkBox_ty->isChecked())
    {
        opt_transrot[dim++] = drr_transrot.trans_Y;
    }
    if(ui->checkBox_tz->isChecked())
    {
        opt_transrot[dim++] = drr_transrot.trans_Z;
    }
    if(ui->checkBox_rx->isChecked())
    {
        opt_transrot[dim++] = drr_transrot.rot_X;
    }
    if(ui->checkBox_rx->isChecked())
    {
        opt_transrot[dim++] = drr_transrot.rot_Y;
    }
    if(ui->checkBox_rx->isChecked())
    {
        opt_transrot[dim++] = drr_transrot.rot_Z;
    }

    return dim;
}
int MainWindow::UpdateDrrTransRotAccordingSelection()
{
    // INFO: update translation & rotation vector of DRRs according to state of checkboxes

    int dim = 0;

    if(ui->checkBox_tx->isChecked())
    {
        drr_transrot.trans_X = opt_transrot[dim++];
    }
    if(ui->checkBox_ty->isChecked())
    {
        drr_transrot.trans_Y = opt_transrot[dim++];
    }
    if(ui->checkBox_tz->isChecked())
    {
        drr_transrot.trans_Z = opt_transrot[dim++];
    }
    if(ui->checkBox_rx->isChecked())
    {
        drr_transrot.rot_X = opt_transrot[dim++];
    }
    if(ui->checkBox_ry->isChecked())
    {
        drr_transrot.rot_Y = opt_transrot[dim++];
    }
    if(ui->checkBox_rz->isChecked())
    {
        drr_transrot.rot_Z = opt_transrot[dim++];
    }

    return dim;
}
void MainWindow::OnButtonRegistration2D3D()
{
    MeritFunction_T merit_func = (MeritFunction_T)this->ui->comboBox_MeritFunction->currentIndex();
    RegistrationMode_T reg_mode = (RegistrationMode_T)this->ui->comboBox_RegistrationMode->currentIndex();


    if(idf_xray_1.width != drr_width || idf_xray_1.height != drr_height)
    {
        DBG_ERROR("size incosistency between xray-1(%d/%d) and drr image(%d/%d)\n", idf_xray_1.width, idf_xray_1.height, drr_width, drr_height);
        return;
    }

    if(active_mask_1->width != drr_width || active_mask_1->height != drr_height)
    {
        DBG_ERROR("size incosistency between mask-1(%d/%d) and drr images(%d/%d)\n", active_mask_1->width, active_mask_1->height, drr_width, drr_height);
        return;
    }

    if(reg_mode == REGISTRATION_MODE_DOUBLE)
    {
        if(idf_xray_2.width != drr_width || idf_xray_2.height != drr_height)
        {
            DBG_ERROR("size incosistency between xray-2(%d/%d) and drr image(%d/%d)\n", idf_xray_2.width, idf_xray_2.height, drr_width, drr_height);
        }

        if(active_mask_2->width != drr_width || active_mask_2->height != drr_height)
        {
            DBG_ERROR("size incosistency between mask-2(%d/%d) and/or drr images(%d/%d)\n", active_mask_2->width, active_mask_2->height, drr_width, drr_height);
            return;
        }
    }

    this->setEnabled(false); // disable GUI


    // select translation & rotation from vector elements accroding to checkboxes
    int dim = UpdateOptimizerTransRotAccordingSelection();

    // run registration
    int iterations;
    //Registration(&drr_transrot, reg_mode, opt_mode, merit_func, CallbackDrrRenderer, this, drr_1, idf_xray_1.imgData, active_mask_1, drr_1, idf_xray_2.imgData, active_mask_2, drr_width, drr_height, &iterations);
    Registration(opt_transrot, dim, reg_mode, merit_func, CallbackDrrRenderer, this, drr_1, idf_xray_1.imgData, active_mask_1, drr_1, idf_xray_2.imgData, active_mask_2, drr_width, drr_height, &iterations);
    this->setEnabled(true); // enable GUI

    DBG_PRINT("registration finished after %d iterations\n", iterations);
}
static void CallbackDrrRenderer(void* instance)
{
    MainWindow* mainwdw = (MainWindow*) instance;

    // update translation & rotation vector according to selction (checkboxes)
    mainwdw->UpdateDrrTransRotAccordingSelection();

    // run ray-casting of drrs
    mainwdw->RenderDrr(DISP_DRR_ALL, &mainwdw->idf_vol);
    //mainwdw->RenderDrr(DrrSelector_T(DRR_1), &idf_vol);
    mainwdw->UpdateView(DISP_DRR_ALL);
    QCoreApplication::processEvents(); // workaround to unblock GUI without using threading

#ifdef WIN32
    QThread::usleep(50000);
#else
    usleep(50000);
#endif
}
void MainWindow::OnButtonBatchRegistration2D3D()
{
    if(!ui->checkBox_XrayBS->isChecked())
    {
        BatchRegistration2D3D();
    }
    else
    {
        BatchRegistration2D3DBS();
    }
}
void MainWindow::BatchRegistration2D3D()
{
    bool abort = false;

    //ui->radioButton_Mask12->setChecked(true);
    //OnRadioButtonSelectMask12AsActiveClicked();

    MeritFunction_T merit_func = (MeritFunction_T)this->ui->comboBox_MeritFunction->currentIndex();
    RegistrationMode_T reg_mode = (RegistrationMode_T)this->ui->comboBox_RegistrationMode->currentIndex();

    // check if volume is loaded
    if(!idf_vol.IsInitialized())
    {
        DBG_ERROR("load volume data before starting batch registration!");
        return;
    }

    // check if images within lists have same count
    if((reg_mode == REGISTRATION_MODE_DOUBLE) && (ui->listWidget_Xray1->count() != ui->listWidget_Xray2->count()))
    {
        DBG_ERROR("same amount of x-ray images required for batch registration!");
        return;
    }

    // if no xray images are present return
    int xray_image_count = ui->listWidget_Xray1->count();
    if(xray_image_count <= 0) return;

    this->setEnabled(false); // disable GUI

    // start logging
    JsonLogger jlogger(this);
    jlogger.AddSettings();

    QProgressDialog progressDlg("Registration in progress...", "Cancel", 0, xray_image_count, this);
    progressDlg.setWindowModality(Qt::WindowModal);
    progressDlg.setValue(0);
    progressDlg.show();

    QString filepath_xray1, filepath_xray2;

    // for each image in list
    for (int i = 0; i < xray_image_count; i++)
    {
        // load xray image 1
        filepath_xray1 = ui->listWidget_Xray1->item(i)->text();
        if(!idf_xray_1.Load(filepath_xray1))
        {
            DBG_ERROR("failed to load xray image %s", filepath_xray1.toLatin1().constData());
            progressDlg.cancel();
            break;
        }
        UpdateView(DISP_XRAY_1);

        /*if(reg_mode == REGISTRATION_MODE_DOUBLE)
        {
            // load xray image 2
            filepath_xray2 = ui->listWidget_Xray2->item(i)->text();
            if(!idf_xray_2.Load(filepath_xray2))
            {
                DBG_ERROR("failed to load xray image %s", filepath_xray2.toLatin1().constData());
                progressDlg.cancel();
                break;
            }
            UpdateView(DISP_XRAY_2);
        }*/

        // select translatio & rotation from vector elements accroding to checkboxes
        int dim = UpdateOptimizerTransRotAccordingSelection();

        // run registration including time measurement
        QTime timer;
        int iterations;
        timer.start();
        Registration(opt_transrot, dim, reg_mode, merit_func, CallbackBatchDrrRenderer, this, drr_1, idf_xray_1.imgData, active_mask_1, drr_2, idf_xray_2.imgData, active_mask_2, drr_width, drr_height, &iterations);
        int timer_ms = timer.elapsed();
        DBG_PRINT("%dms required for %d iterations\n", timer_ms, iterations);

        UpdateView(DISP_DRR_1);
        QCoreApplication::processEvents();
        QThread::usleep(500000);

        /*if(reg_mode == REGISTRATION_MODE_DOUBLE)
        {
            UpdateView(DISP_DRR_2);
        }*/

        jlogger.AddRecord(&drr_transrot, iterations, timer_ms, filepath_xray1, filepath_xray2);

        progressDlg.setValue(i+1);

        // registration canceled by user
        QCoreApplication::processEvents(); // workaround to unblock GUI without threading
        if(progressDlg.wasCanceled())
        {
            abort = true;
            break;
        }
    }

    progressDlg.close();

    if(!abort)
    {
        jlogger.Save();
    }

    this->setEnabled(true); // enable GUI
}
static void CallbackBatchDrrRenderer(void* instance)
{
    MainWindow* mainwdw = (MainWindow*) instance;

    // update translation & rotation vector according to selction (checkboxes)
    mainwdw->UpdateDrrTransRotAccordingSelection();

    // drr rendering
    unsigned short* drr_orig = NULL;
    mainwdw->Rendering(RDROPT_BONESUP, &mainwdw->idf_vol, mainwdw->active_mask_1_cuda_id, &mainwdw->drr_transrot, &mainwdw->RtSysActive->imgSys1, mainwdw->ui->horizontalSlider_VoxelFilter1ThresholdLow->minimum(), mainwdw->ui->horizontalSlider_VoxelFilter1ThresholdHigh->maximum(), 1.0, mainwdw->ui->horizontalSlider_Windowing1Min->value(), mainwdw->ui->horizontalSlider_Windowing1Max->value(), CONVERT_SLIDER_VALUE_TO_INTENSITY(mainwdw->ui->horizontalSlider_Intensity1->value()), NULL, &drr_orig);

    // memory transfer and image display
    memcpy(&mainwdw->drr_1, drr_orig, sizeof(mainwdw->drr_1));
    //mainwdw->UpdateView(DISP_DRR_1);
}
void MainWindow::BatchRegistration2D3DBS()
{
    QTime timer;
    int timer_ms_ribs, timer_ms_bs, timer_ms_tumor;
    int iterations_ribs, iterations_tumor;
    int dim;
    int i;
    bool abort = false;

    //ui->radioButton_Mask12->setChecked(true);
    //OnRadioButtonSelectMask12AsActiveClicked();

    // check if volumes are loaded
    if(!idf_vol.IsInitialized() || !idf_vol_bs.IsInitialized())
    {
        DBG_ERROR("load volumes before starting batch registration with bone suppression!");
        return;
    }

    // if no xray images are present return
    int xray_image_count = ui->listWidget_Xray1->count();
    if(xray_image_count <= 0) return;

    // save initial transrot vector
    TransRot_T transrot_ribs;
    TransRot_T transrot_tumor;
    memcpy(&transrot_ribs, &drr_transrot, sizeof(transrot_ribs));
    memcpy(&transrot_tumor, &drr_transrot, sizeof(transrot_tumor));

    MeritFunction_T merit_func = (MeritFunction_T)ui->comboBox_MeritFunction->currentIndex();
    RegistrationMode_T reg_mode = (RegistrationMode_T)ui->comboBox_RegistrationMode->currentIndex();

    this->setEnabled(false); // disable GUI

    QString filepath_xray;
    JsonLogger jlogger(this);
    jlogger.AddSettings();

    QProgressDialog progressDlg("Registration in progress...", "Cancel", 0, xray_image_count, this);
    progressDlg.setWindowModality(Qt::WindowModal);
    progressDlg.setValue(0);
    progressDlg.show();

    // for each image in list
    for (i=0; i<xray_image_count; i++)
    {
        // load xray image
        filepath_xray = ui->listWidget_Xray1->item(i)->text();
        if(!idf_xray_1.Load(filepath_xray))
        {
            DBG_ERROR("failed to load xray image %s", filepath_xray.toLatin1().constData());
            break;
        }
        UpdateView(DISP_XRAY_1);

        // restore previous rib registration result
        memcpy(&drr_transrot, &transrot_ribs, sizeof(drr_transrot));
        dim = UpdateOptimizerTransRotAccordingSelection();

        // registration of ribs
        timer.start();
        Registration(opt_transrot, dim, reg_mode, merit_func, CallbackBatchDrrRendererRibsBS, this, drr_1, idf_xray_1.imgData, &mask_11, drr_2, NULL, NULL, drr_width, drr_height, &iterations_ribs);
        timer_ms_ribs = timer.elapsed();
        DBG_PRINT("%d ms required for rib registration within %d iterations\n", timer_ms_ribs, iterations_ribs);
        memcpy(&transrot_ribs, &drr_transrot, sizeof(transrot_ribs));
        UpdateView(DISP_DRR_1);
        QCoreApplication::processEvents(); // workaround to unblock GUI without threading


        // bone suppression
        timer.start();
        SuppressBonesInXray(ui->horizontalSlider_VoxelFilter1ThresholdLow->value(), ui->horizontalSlider_VoxelFilter1ThresholdHigh->value(), ui->horizontalSlider_WeightingFactorXray->value()/100.);
        timer_ms_bs = timer.elapsed();
        DBG_PRINT("%d ms required for bone suppression\n", timer_ms_bs);
        UpdateView(DISP_XRAY_1);
        QCoreApplication::processEvents(); // workaround to unblock GUI without threading
        QThread::usleep(500000);


        // restore previous tumor registration result
        memcpy(&drr_transrot, &transrot_tumor, sizeof(drr_transrot));
        dim = UpdateOptimizerTransRotAccordingSelection();

        // registration of tumor
        timer.start();
        Registration(opt_transrot, dim, reg_mode, merit_func, CallbackBatchDrrRendererTumorBS, this, drr_1, idf_xray_1.imgData, &mask_12, drr_2, NULL, NULL, drr_width, drr_height, &iterations_tumor);
        timer_ms_tumor = timer.elapsed();
        DBG_PRINT("%d ms required for tumor registration within %d iterations\n", timer_ms_tumor, iterations_tumor);
        memcpy(&transrot_tumor, &drr_transrot, sizeof(transrot_tumor));
        UpdateView(DISP_DRR_1);
        QCoreApplication::processEvents();
        QThread::usleep(500000);

        jlogger.AddRecordBS(&transrot_ribs, &transrot_tumor, iterations_ribs, iterations_tumor, timer_ms_ribs, timer_ms_tumor, timer_ms_bs, filepath_xray, "");

        progressDlg.setValue(i+1);

        // registration canceled by user
        QCoreApplication::processEvents(); // workaround to unblock GUI without threading
        if (progressDlg.wasCanceled())
        {
            abort = true;
            break;
        }
    }

    progressDlg.close();

    if(!abort)
    {
        jlogger.Save();
    }

    //ui->listWidget_Xray1->setCurrentRow((i>0)? i-1 : i);

    this->setEnabled(true); // enable GUI
}
static void CallbackBatchDrrRendererRibsBS(void* instance)
{
    MainWindow* mainwdw = (MainWindow*)instance;

    // update translation & rotation vector according to selction (checkboxes)
    mainwdw->UpdateDrrTransRotAccordingSelection();

    // drr rendering
    unsigned short* drr_orig = NULL;
    mainwdw->Rendering(RDROPT_BONESUP, &mainwdw->idf_vol, 0, &mainwdw->drr_transrot, &mainwdw->RtSysActive->imgSys1, mainwdw->ui->horizontalSlider_VoxelFilter1ThresholdLow->minimum(), mainwdw->ui->horizontalSlider_VoxelFilter1ThresholdHigh->maximum(), 1.0,  mainwdw->ui->horizontalSlider_Windowing1Min->value(), mainwdw->ui->horizontalSlider_Windowing1Max->value(), CONVERT_SLIDER_VALUE_TO_INTENSITY(mainwdw->ui->horizontalSlider_Intensity1->value()), NULL, &drr_orig);

    // memory transfer and image display
    memcpy(&mainwdw->drr_1, drr_orig, sizeof(mainwdw->drr_1));
    //mainwdw->UpdateView(DISP_DRR_1);
}
static void CallbackBatchDrrRendererTumorBS(void* instance)
{
    MainWindow* mainwdw = (MainWindow*)instance;

    // update translation & rotation vector according to selction (checkboxes)
    mainwdw->UpdateDrrTransRotAccordingSelection();

    // drr rendering
    unsigned short* drr_bs = NULL;
    mainwdw->Rendering(RDROPT_BONESUP, &mainwdw->idf_vol, 1, &mainwdw->drr_transrot, &mainwdw->RtSysActive->imgSys1, mainwdw->ui->horizontalSlider_VoxelFilter1ThresholdLow->value(), mainwdw->ui->horizontalSlider_VoxelFilter1ThresholdHigh->value(), mainwdw->ui->horizontalSlider_WeightingFactorDrr1->value()/100.,  mainwdw->ui->horizontalSlider_Windowing1Min->value(), mainwdw->ui->horizontalSlider_Windowing1Max->value(), CONVERT_SLIDER_VALUE_TO_INTENSITY(mainwdw->ui->horizontalSlider_Intensity1->value()), &drr_bs, NULL);

    // memory transfer and image display
    memcpy(&mainwdw->drr_1, drr_bs, sizeof(mainwdw->drr_1));
    //mainwdw->UpdateView(DISP_DRR_1);
}
void MainWindow::SuppressBonesInXray(unsigned short threshold_min, unsigned short threshold_max, float weighting)
{
    if(!idf_vol.IsInitialized())
    {
        DBG_PRINT("volume not loaded!\n");
        return;
    }

    if(!idf_vol_bs.IsInitialized())
    {
        DBG_PRINT("volume for bone suppression (without tumor) not loaded!");
        return;
    }

    if(!idf_xray_1.IsInitialized())
    {
        DBG_PRINT("xray image not loaded!");
        return;
    }

    // render original DRR (with tumor and bones)
    unsigned short* drr_orig = NULL;
    unsigned short* drr_bs = NULL;
    Rendering(RDROPT_BONESUP, &idf_vol, 1, &drr_transrot, &RtSysActive->imgSys1, threshold_min, threshold_max, 0.,  ui->horizontalSlider_Windowing1Min->value(), ui->horizontalSlider_Windowing1Max->value(), CONVERT_SLIDER_VALUE_TO_INTENSITY(ui->horizontalSlider_Intensity1->value()), &drr_bs, &drr_orig);

    // sample intensity transfer function of roi2
    int pix_cnt = mask_12.pixel_count;
    if(pix_cnt <= 0) return;
    double* roi_drr = new (std::nothrow) double[pix_cnt];
    double* roi_xray = new (std::nothrow) double[pix_cnt];
    double* roi_drr_bs = new (std::nothrow) double[pix_cnt];
    if(!roi_drr || !roi_xray || !roi_drr_bs)
    {
        delete roi_drr;
        delete roi_xray;
        delete roi_drr_bs;
        DBG_ERROR("memory allocation for rois failed!");
        return;
    }

    // select and convert pixel intensities to double
    for(int i=0; i<pix_cnt; i++)
    {
        roi_drr[i] = drr_orig[mask_12.pixel_offset[i]];
        roi_xray[i] = idf_xray_1.imgData[mask_12.pixel_offset[i]];
    }

    // fit intensity transfer function
    vec vec_roi_drr(roi_drr, pix_cnt, false);
    vec vec_roi_xray(roi_xray, pix_cnt, false);
    vec vec_itf_coefs = polyfit(vec_roi_drr, vec_roi_xray, 2);
    DBG_PRINT("itf coefficients: %.3f   %.3f   %.3f\n", vec_itf_coefs(0), vec_itf_coefs(1), vec_itf_coefs(2));

    // render bone suppressed (drr_1) and original (drr_2) image without tumor
    unsigned short* drr_ntumor_bs = NULL;
    unsigned short* drr_ntumor = NULL;
    Rendering(RDROPT_BONESUP, &idf_vol_bs, 1, &drr_transrot, &RtSysActive->imgSys1, threshold_min, threshold_max, weighting, ui->horizontalSlider_Windowing1Min->value(), ui->horizontalSlider_Windowing1Max->value(), CONVERT_SLIDER_VALUE_TO_INTENSITY(ui->horizontalSlider_Intensity1->value()), &drr_ntumor_bs, &drr_ntumor);

    // select and convert pixel intensities to double
    for(int i=0; i<pix_cnt; i++)
    {
        roi_drr[i] = drr_ntumor[mask_12.pixel_offset[i]];
        roi_drr_bs[i] = drr_ntumor_bs[mask_12.pixel_offset[i]];
    }

    vec vec_roi_drr_ntumor(roi_drr, pix_cnt, false);
    vec vec_roi_drr_ntumor_bs(roi_drr_bs, pix_cnt, false);

    // apply intensity transform
    vec_roi_drr_ntumor = polyval(vec_itf_coefs, vec_roi_drr_ntumor);
    vec_roi_drr_ntumor_bs = polyval(vec_itf_coefs, vec_roi_drr_ntumor_bs);

    // bone subtraction
    vec vec_xray_bs = vec_roi_xray-(vec_roi_drr_ntumor-vec_roi_drr_ntumor_bs);

    // update roi within xray
    for(int i=0; i<pix_cnt; i++)
    {
        if(vec_xray_bs(i) >= 0)
        {
            idf_xray_1.imgData[mask_12.pixel_offset[i]] = vec_xray_bs(i);
        }
        else
        {
            idf_xray_1.imgData[mask_12.pixel_offset[i]] = 0;
        }
    }

    delete roi_drr;
    delete roi_drr_bs;
    delete roi_xray;
}

// SECTION: DATA LOGGING ////////////////////////////////////////////////////////////////////////////////////
JsonLogger::JsonLogger(MainWindow* handle)
{
    mwdw = handle;
}
void JsonLogger::AddSettings()
{
    QJsonObject j_registration_config;

    j_registration_config["RtSystem"] = mwdw->ui->comboBox_RtSystem->currentText();
    j_registration_config["RegistrationMode"] = mwdw->ui->comboBox_RegistrationMode->currentText();
    j_registration_config["MeritFunction"] = mwdw->ui->comboBox_MeritFunction->currentText();
    j_registration_config["VolumeFile"] = mwdw->idf_vol.filePath;
    j_registration_config["VolumeFileBs"] = mwdw->idf_vol_bs.filePath;
    j_registration_config["DrrWidth"] = mwdw->drr_width;
    j_registration_config["DrrHeight"] = mwdw->drr_height;
    j_registration_config["DrrDetectorPixelSpacing"] = Cuda_Get_PixelSpacing();

    // optmizer dof selection
    QJsonObject j_optimizer_dof_selection;
    j_optimizer_dof_selection["TransX"] = mwdw->ui->checkBox_tx->isChecked();
    j_optimizer_dof_selection["TransY"] = mwdw->ui->checkBox_ty->isChecked();
    j_optimizer_dof_selection["TransZ"] = mwdw->ui->checkBox_tz->isChecked();
    j_optimizer_dof_selection["RotX"] = mwdw->ui->checkBox_rx->isChecked();
    j_optimizer_dof_selection["RotY"] = mwdw->ui->checkBox_ry->isChecked();
    j_optimizer_dof_selection["RotZ"] = mwdw->ui->checkBox_rz->isChecked();
    j_registration_config["OptimizerSettings"] = j_optimizer_dof_selection;

    QJsonObject j_initial_transrot;
    j_initial_transrot["TransX"] = QString::number(mwdw->drr_transrot.trans_X, 'f', 9);
    j_initial_transrot["TransY"] = QString::number(mwdw->drr_transrot.trans_Y, 'f', 9);
    j_initial_transrot["TransZ"] = QString::number(mwdw->drr_transrot.trans_Z, 'f', 9);
    j_initial_transrot["RotX"] = QString::number(mwdw->drr_transrot.rot_X, 'f', 9);
    j_initial_transrot["RotY"] = QString::number(mwdw->drr_transrot.rot_Y, 'f', 9);
    j_initial_transrot["RotZ"] = QString::number(mwdw->drr_transrot.rot_Z, 'f', 9);
    j_registration_config["InitialTransrot"] = j_initial_transrot;

    QJsonObject j_cam1_settings;
    j_cam1_settings["DistSrc"] = mwdw->RtSysActive->imgSys1.source_distance;
    j_cam1_settings["DistDet"] = mwdw->RtSysActive->imgSys1.detector_distance;
    j_cam1_settings["DetPsr"] = mwdw->RtSysActive->imgSys1.pixel_spacing_ratio;
    j_cam1_settings["RotX"] = mwdw->RtSysActive->imgSys1.rotation_ref[0];
    j_cam1_settings["RotY"] = mwdw->RtSysActive->imgSys1.rotation_ref[1];
    j_cam1_settings["RotZ"] = mwdw->RtSysActive->imgSys1.rotation_ref[2];
    j_cam1_settings["Intensity"] = CONVERT_SLIDER_VALUE_TO_INTENSITY(mwdw->ui->horizontalSlider_Intensity1->value());
    j_cam1_settings["WindowingLow"] = QString::number(mwdw->ui->horizontalSlider_Windowing1Min->value());
    j_cam1_settings["WindowingHigh"] = QString::number(mwdw->ui->horizontalSlider_Windowing1Min->value());
    j_cam1_settings["VoxelFilterLow"] = mwdw->ui->horizontalSlider_VoxelFilter1ThresholdLow->value();
    j_cam1_settings["VoxelFilterHigh"] = mwdw->ui->horizontalSlider_VoxelFilter1ThresholdHigh->value();
    j_cam1_settings["WeightingFactorDrr"] = mwdw->ui->horizontalSlider_WeightingFactorDrr1->value();
    j_cam1_settings["XrayBoneSuppressionActive"] = mwdw->ui->checkBox_XrayBS->checkState();
    j_cam1_settings["WeightingFactorXray"] = mwdw->ui->horizontalSlider_WeightingFactorXray->value();
    j_registration_config["Cam1Settings"] = j_cam1_settings;

    QJsonObject j_cam2_settings;
    j_cam2_settings["DistSrc"] = mwdw->RtSysActive->imgSys2.source_distance;
    j_cam2_settings["DistDet"] = mwdw->RtSysActive->imgSys2.detector_distance;
    j_cam2_settings["DetPsr"] = mwdw->RtSysActive->imgSys2.pixel_spacing_ratio;
    j_cam2_settings["RotX"] = mwdw->RtSysActive->imgSys2.rotation_ref[0];
    j_cam2_settings["RotY"] = mwdw->RtSysActive->imgSys2.rotation_ref[1];
    j_cam2_settings["RotZ"] = mwdw->RtSysActive->imgSys2.rotation_ref[2];
    j_cam2_settings["Intensity"] = CONVERT_SLIDER_VALUE_TO_INTENSITY(mwdw->ui->horizontalSlider_Intensity2->value());
    j_cam2_settings["WindowingLow"] = QString::number(mwdw->ui->horizontalSlider_Windowing2Min->value());
    j_cam2_settings["WindowingHigh"] = QString::number(mwdw->ui->horizontalSlider_Windowing2Min->value());
    j_cam2_settings["VoxelFilterLow"] = mwdw->ui->horizontalSlider_VoxelFilter2ThresholdLow->value();
    j_cam2_settings["VoxelFilterHigh"] = mwdw->ui->horizontalSlider_VoxelFilter2ThresholdHigh->value();
    j_cam2_settings["WeightingFactorDrr"] = mwdw->ui->horizontalSlider_WeightingFactorDrr2->value();
    j_registration_config["Cam2Settings"] = j_cam2_settings;

    QJsonObject j_mask11_settings;
    j_mask11_settings["IsActive"] = mwdw->ui->radioButton_Mask11->isChecked();
    j_mask11_settings["File"] = mwdw->mask_11.file_path;
    j_mask11_settings["PosX"] = mwdw->mask_11.pos_x;
    j_mask11_settings["PosY"] = mwdw->mask_11.pos_y;
    j_mask11_settings["RectWidth"] = mwdw->mask_11.rect_width;
    j_mask11_settings["RectHeight"] = mwdw->mask_11.rect_height;
    j_registration_config["Mask11Settings"] = j_mask11_settings;

    QJsonObject j_mask12_settings;
    j_mask12_settings["IsActive"] = mwdw->ui->radioButton_Mask12->isChecked();
    j_mask12_settings["File"] = mwdw->mask_12.file_path;
    j_mask12_settings["PosX"] = mwdw->mask_12.pos_x;
    j_mask12_settings["PosY"] = mwdw->mask_12.pos_y;
    j_mask12_settings["RectWidth"] = mwdw->mask_12.rect_width;
    j_mask12_settings["RectHeight"] = mwdw->mask_12.rect_height;
    j_registration_config["Mask12Settings"] = j_mask12_settings;

    QJsonObject j_mask21_settings;
    j_mask21_settings["IsActive"] = mwdw->ui->radioButton_Mask21->isChecked();
    j_mask21_settings["File"] = mwdw->mask_21.file_path;
    j_mask21_settings["PosX"] = mwdw->mask_21.pos_x;
    j_mask21_settings["PosY"] = mwdw->mask_21.pos_y;
    j_mask21_settings["RectWidth"] = mwdw->mask_21.rect_width;
    j_mask21_settings["RectHeight"] = mwdw->mask_21.rect_height;
    j_registration_config["Mask21Settings"] = j_mask21_settings;

    QJsonObject j_mask22_settings;
    j_mask22_settings["IsActive"] = mwdw->ui->radioButton_Mask22->isChecked();
    j_mask22_settings["File"] = mwdw->mask_22.file_path;
    j_mask22_settings["PosX"] = mwdw->mask_22.pos_x;
    j_mask22_settings["PosY"] = mwdw->mask_22.pos_y;
    j_mask22_settings["RectWidth"] = mwdw->mask_22.rect_width;
    j_mask22_settings["RectHeight"] = mwdw->mask_22.rect_height;
    j_registration_config["Mask22Settings"] = j_mask22_settings;

    j_log_obj.insert("Settings", j_registration_config);
}
void JsonLogger::AddRecord(TransRot_T* transrot, int iterations, int time_ms, QString filepath_xray1, QString filepath_xray2)
{
    QJsonObject j_reg_rec;

    // add result to json record array
    j_reg_rec["Xray1File"] = filepath_xray1;
    j_reg_rec["Xray2File"] = filepath_xray2;
    j_reg_rec["TransXTumor"] = transrot->trans_X;
    j_reg_rec["TransYTumor"] = transrot->trans_Y;
    j_reg_rec["TransZTumor"] = transrot->trans_Z;
    j_reg_rec["RotXTumor"] = transrot->rot_X;
    j_reg_rec["RotYTumor"] = transrot->rot_Y;
    j_reg_rec["RotZTumor"] = transrot->rot_Z;
    j_reg_rec["RegIterationsTumor"] = iterations;
    j_reg_rec["RegTimeMsTumor"] = time_ms;

    // append registration record to json array
    j_reg_recs_arr.append(j_reg_rec);
}
void JsonLogger::AddRecordBS(TransRot_T* transrot_ribs, TransRot_T* transrot_tumor, int iterations_ribs, int iterations_tumor, int time_ms_ribs, int time_ms_tumor, int time_ms_bs, QString filepath_xray1, QString filepath_xray2)
{
    QJsonObject j_reg_rec;

    // add result to json record array
    j_reg_rec["Xray1File"] = filepath_xray1;
    j_reg_rec["Xray2File"] = filepath_xray2;

    j_reg_rec["TransXRibs"] = transrot_ribs->trans_X;
    j_reg_rec["TransYRibs"] = transrot_ribs->trans_Y;
    j_reg_rec["TransZRibs"] = transrot_ribs->trans_Z;
    j_reg_rec["RotXRibs"] = transrot_ribs->rot_X;
    j_reg_rec["RotYRibs"] = transrot_ribs->rot_Y;
    j_reg_rec["RotZRibs"] = transrot_ribs->rot_Z;
    j_reg_rec["RegIterationsRibs"] = iterations_ribs;
    j_reg_rec["RegTimeMsRibs"] = time_ms_ribs;

    j_reg_rec["TransXTumor"] = transrot_tumor->trans_X;
    j_reg_rec["TransYTumor"] = transrot_tumor->trans_Y;
    j_reg_rec["TransZTumor"] = transrot_tumor->trans_Z;
    j_reg_rec["RotXTumor"] = transrot_tumor->rot_X;
    j_reg_rec["RotYTumor"] = transrot_tumor->rot_Y;
    j_reg_rec["RotZTumor"] = transrot_tumor->rot_Z;
    j_reg_rec["RegIterationsTumor"] = iterations_tumor;
    j_reg_rec["RegTimeMsTumor"] = time_ms_tumor;

    j_reg_rec["BsTimeMs"] = time_ms_bs;

    // append registration record to json array
    j_reg_recs_arr.append(j_reg_rec);
}
void JsonLogger::Save()
{
    QString path, name, ext;
    mwdw->FileInfo(mwdw->idf_vol.filePath, path, name, ext);
    QString opt_settings = QString::number(mwdw->ui->checkBox_tx->isChecked()) + QString::number(mwdw->ui->checkBox_ty->isChecked()) + QString::number(mwdw->ui->checkBox_tz->isChecked()) +
                           QString::number(mwdw->ui->checkBox_rx->isChecked()) + QString::number(mwdw->ui->checkBox_ry->isChecked()) + QString::number(mwdw->ui->checkBox_rz->isChecked());

    name = mwdw->GetDateTimeString() + "_" + name + "_MERIT" + QString(MeritFunctionNameShort_T[mwdw->ui->comboBox_MeritFunction->currentIndex()]) + "_OPT" + opt_settings + "_BS" + QString::number(mwdw->ui->checkBox_XrayBS->isChecked()) + ".reglog";

    QString fileComplex = QString(path + QDir::separator() + "%1").arg(name);
    QString logfilePath = QFileDialog::getSaveFileName(mwdw, "Save Registration Logfile", fileComplex, "FIRE-RegistrationLog(*.reglog)");
    if(logfilePath.isEmpty()) return;

    // create logfile
    QFile logfile(logfilePath);

    if(!logfile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        DBG_ERROR("creating logfile %s failed!", logfilePath.toLatin1().constData());
        return;
    }

    // add records to json object and write json logfile
    j_log_obj["Data"] = j_reg_recs_arr;
    QString log_str = QJsonDocument(j_log_obj).toJson(QJsonDocument::Compact);
    logfile.write(log_str.toUtf8());

    // close logfile
    logfile.flush();
    logfile.close();
}

// SECTION: STATUS LABEL UPDATE WITHIN GUI //////////////////////////////////////////////////////////////////
void MainWindow::UpdateCam1Status()
{
    int windowing_min = ui->horizontalSlider_Windowing1Min->value();
    int windowing_max = ui->horizontalSlider_Windowing1Max->value();
    int filter_min = ui->horizontalSlider_VoxelFilter1ThresholdLow->value();
    int filter_max = ui->horizontalSlider_VoxelFilter1ThresholdHigh->value();
    float weight_drr = ui->horizontalSlider_WeightingFactorDrr1->value()/100.;

    ui->label_Cam1Settings->setText(QString::number(drr_intensity1) + "/" +  QString::number(windowing_min) + "/" + QString::number(windowing_max) + "/" + QString::number(filter_min) + "/" + QString::number(filter_max) + "/" + QString::number(weight_drr));
}
void MainWindow::UpdateCam2Status()
{
    int windowing_min = ui->horizontalSlider_Windowing2Min->value();
    int windowing_max = ui->horizontalSlider_Windowing2Max->value();
    int filter_min = ui->horizontalSlider_VoxelFilter2ThresholdLow->value();
    int filter_max = ui->horizontalSlider_VoxelFilter2ThresholdHigh->value();

    ui->label_Cam2Settings->setText(QString::number(drr_intensity2) + "/" +  QString::number(windowing_min) + "/" + QString::number(windowing_max) + "/" + QString::number(filter_min) + "/" + QString::number(filter_max));
}
void MainWindow::UpdateBSStatus()
{
    ui->label_weightsBS->setText("weight: " + QString::number(ui->horizontalSlider_WeightingFactorXray->value()/100.));
}
void MainWindow::UpdateStatusBar()
{
    this->ui->statusBar->showMessage("X/Y/Z   rX/rY/rZ: " +
                                     QString::number(drr_transrot.trans_X, 'f', 3) + "/" +
                                     QString::number(drr_transrot.trans_Y, 'f', 3) + "/" +
                                     QString::number(drr_transrot.trans_Z, 'f', 3) + "   " +
                                     QString::number(drr_transrot.rot_X, 'f', 3) + "/" +
                                     QString::number(drr_transrot.rot_Y, 'f', 3) + "/" +
                                     QString::number(drr_transrot.rot_Z, 'f', 3));
}

// SECTION: SWAP CAM1/CAM2 SETTINGS /////////////////////////////////////////////////////////////////////////
void MainWindow::OnButtonIntensity1Up()
{
    ui->horizontalSlider_Intensity1->setValue(ui->horizontalSlider_Intensity2->value());
}
void MainWindow::OnButtonIntensity2Down()
{
    ui->horizontalSlider_Intensity2->setValue(ui->horizontalSlider_Intensity1->value());
}
void MainWindow::OnButtonVoxelFilter1ThresholdUp()
{
    ui->horizontalSlider_VoxelFilter1ThresholdHigh->setValue(ui->horizontalSlider_VoxelFilter2ThresholdHigh->value());
    ui->horizontalSlider_VoxelFilter1ThresholdLow->setValue(ui->horizontalSlider_VoxelFilter2ThresholdLow->value());
}
void MainWindow::OnButtonVoxelFilter1ThresholdSwap()
{
    int max = ui->horizontalSlider_VoxelFilter1ThresholdHigh->value();
    ui->horizontalSlider_VoxelFilter1ThresholdHigh->setValue(ui->horizontalSlider_VoxelFilter1ThresholdLow->value());
    ui->horizontalSlider_VoxelFilter1ThresholdLow->setValue(max);
}
void MainWindow::OnButtonVoxelFilter2ThresholdDown()
{
    ui->horizontalSlider_VoxelFilter2ThresholdHigh->setValue(ui->horizontalSlider_VoxelFilter1ThresholdHigh->value());
    ui->horizontalSlider_VoxelFilter2ThresholdLow->setValue(ui->horizontalSlider_VoxelFilter1ThresholdLow->value());
}
void MainWindow::OnButtonVoxelFilter2ThresholdSwap()
{
    int max = ui->horizontalSlider_VoxelFilter2ThresholdHigh->value();
    ui->horizontalSlider_VoxelFilter2ThresholdHigh->setValue(ui->horizontalSlider_VoxelFilter2ThresholdLow->value());
    ui->horizontalSlider_VoxelFilter2ThresholdLow->setValue(max);
}

// SECTION: KEYBOARD INPUT //////////////////////////////////////////////////////////////////////////////////
// event handler is triggered on key press
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        QKeyEvent* key_event = static_cast<QKeyEvent*>(event);
        QWidget* fw = qApp->focusWidget();

        if(key_event->key() == Qt::Key_Delete)
        {
            // remove x-rays from list
            if(fw->objectName() == "listWidget_Xray1")
            {
                QList<QListWidgetItem*> items_sel = ui->listWidget_Xray1->selectedItems();
                foreach(QListWidgetItem * item, items_sel)
                {
                    delete ui->listWidget_Xray1->takeItem(ui->listWidget_Xray1->row(item));
                }
                idf_xray_1.Free();
                UpdateView(DISP_XRAY_1);
            }
            else if(fw->objectName() == "listWidget_Xray2")
            {
                QList<QListWidgetItem*> items_sel = ui->listWidget_Xray2->selectedItems();
                foreach(QListWidgetItem * item, items_sel)
                {
                    delete ui->listWidget_Xray2->takeItem(ui->listWidget_Xray2->row(item));
                }
                idf_xray_2.Free();
                UpdateView(DISP_XRAY_2);
            }
        }
        else
        {
            // resizing or repositioning mask
            QSlider* slider_w;
            QSlider* slider_h;
            QSlider* slider_x;
            QSlider* slider_y;


            // check which element has focus
            if(fw->objectName() == "label_Drr_1" || fw->objectName() == "label_Xray_1")
            {
                slider_w = ui->horizontalSlider_Mask1Prop1;
                slider_h = ui->horizontalSlider_Mask1Prop2;
                slider_x = ui->horizontalSlider_Mask1PosH;
                slider_y = ui->horizontalSlider_Mask1PosV;
            }
            else if(fw->objectName() == "label_Drr_2" || fw->objectName() == "label_Xray_2")
            {
                slider_w = ui->horizontalSlider_Mask2Prop1;
                slider_h = ui->horizontalSlider_Mask2Prop2;
                slider_x = ui->horizontalSlider_Mask2PosH;
                slider_y = ui->horizontalSlider_Mask2PosV;
            }
            else
            {
                return QObject::eventFilter(obj, event);
            }


            if(key_event->modifiers() & Qt::ShiftModifier)
            {
                // change mask size (shift + arrow key)
                if(key_event->key() == Qt::Key_Up)
                {
                    slider_h->setValue(slider_h->value()+1);
                }
                else if(key_event->key() == Qt::Key_Down)
                {
                    slider_h->setValue(slider_h->value()-1);
                }
                else if(key_event->key() == Qt::Key_Right)
                {
                    slider_w->setValue(slider_w->value()+1);
                }
                else if(key_event->key() == Qt::Key_Left)
                {
                    slider_w->setValue(slider_w->value()-1);
                }

                return true;
            }
            else
            {
                // change mask position (arrow key)
                if(key_event->key() == Qt::Key_Up)
                {
                    slider_y->setValue(slider_y->value()-1);
                }
                else if(key_event->key() == Qt::Key_Down)
                {
                    slider_y->setValue(slider_y->value()+1);
                }
                else if(key_event->key() == Qt::Key_Right)
                {
                    slider_x->setValue(slider_x->value()+1);
                }
                else if(key_event->key() == Qt::Key_Left)
                {
                    slider_x->setValue(slider_x->value()-1);
                }

                return true;
            }
        }
    }

    return QObject::eventFilter(obj, event);
}

// SETCION: HELPER FUNCTIONS ////////////////////////////////////////////////////////////////////////////////
bool MainWindow::OpenSingleFileDialog(QString title, QString path, QString filterStr, QString* filePath_out, bool updateWorkingDir)
{
    // INFO: filterStr e.g. "(*.pgm)" or "Images (*.pgm)"
    // Show file open dialog
    QString pathComplex = QString(path + QDir::separator() + "%1").arg("");
    *filePath_out = QFileDialog::getOpenFileName(this, title, pathComplex, filterStr);

    if(filePath_out->isNull())
    {
        return false;
    }

    // update working directory
    QFileInfo fi(*filePath_out);

    if(updateWorkingDir)
    {
        volume_dir = fi.dir().absolutePath();
    }

    return true;
}
unsigned int MainWindow::OpenMultipleFilesDialog(QString title, QString path, QString filterStr, QStringList* filePaths_out, bool updateWorkingDir)
{
    // INFO: filterStr e.g. "(*.pgm *.hes)" or "Images (*.pgm *.hes)"
    // Show file open dialog (for multiple files)
    QString pathComplex = QString(volume_dir + QDir::separator() + "%1").arg("");
    *filePaths_out = QFileDialog::getOpenFileNames(this, title, pathComplex, filterStr);

    if(filePaths_out->isEmpty())
    {
        return 0;
    }

    // update last working directory
    QFileInfo fi(filePaths_out->at(0));

    if(updateWorkingDir)
    {
        volume_dir = fi.dir().absolutePath();
    }

    return filePaths_out->count();
}
void MainWindow::FileInfo(QString filePath, QString &path, QString &name, QString &ext)
{
    QFileInfo f_info(filePath);
    path = f_info.absolutePath().toLatin1().constData();
    name = f_info.completeBaseName().toLatin1().constData();
    ext = f_info.suffix().toLatin1().constData();
}
QString MainWindow::GetDateTimeString()
{
    QString format = "yyyyMMdd-HHmmss";
    QDateTime dt = QDateTime::currentDateTime();

    return dt.toString(format);
}
void MainWindow::GetMinMax(unsigned short data[], unsigned int length, unsigned short* minimum, unsigned short* maximum)
{
    unsigned short min;
    unsigned short max;
    unsigned short val;

    min = data[0];
    max = data[0];

    for(unsigned int i=1; i<length; i++)
    {
        val = data[i];
        if(val < min) min = val;
        if(val > max) max = val;
    }

    *minimum = min;
    *maximum = max;
}
void MainWindow::Windowing8Bit(unsigned short data[], unsigned int length, unsigned short minimum, unsigned short maximum)
{
    int range = maximum - minimum;
    float scale = range/255.f;

    if(range == 0)
    {
        DBG_WARNING("Windowing skipped due to same value for min and max!");
        return;
    }

    for(unsigned int i=0; i<length; i++)
    {
        if(data[i] < minimum)
        {
            data[i] = 0;
        }
        else if(data[i] >= maximum)
        {
            data[i] = 255;
        }
        else
        {
            data[i] = (unsigned short)round(((float)(data[i]-minimum)/scale));
        }
    }
}
void MainWindow::OnButtonSave()
{
    ImageDataFile idf;

    idf.Load(drr_1, drr_width, drr_height, 1);
    idf.SavePgm(QDir::homePath() + "/Drr1.pgm", true);

    idf.Load(drr_2, drr_width, drr_height, 1);
    idf.SavePgm(QDir::homePath() + "/Drr2.pgm", true);

    if(idf_xray_1.IsInitialized())
    {
        idf_xray_1.SavePgm(QDir::homePath() + "/Xray1.pgm", true);
    }

    if(idf_xray_2.IsInitialized())
    {
        idf_xray_2.SavePgm(QDir::homePath() + "/Xray2.pgm", true);
    }
}

// SECTION: CODE UNDER CONSTRUCTION /////////////////////////////////////////////////////////////////////////

void MainWindow::OnButtonTest()
{
    /*QString path, name, ext;
    FileInfo(idf_vol.filePath, path, name, ext);
    QString opt_settings = QString::number(ui->checkBox_tx->isChecked()) + QString::number(ui->checkBox_ty->isChecked()) + QString::number(ui->checkBox_tz->isChecked()) +
                           QString::number(ui->checkBox_rx->isChecked()) + QString::number(ui->checkBox_ry->isChecked()) + QString::number(ui->checkBox_rz->isChecked());

    name = GetDateTimeString() + "_" + name + "_MERIT" + QString(MeritFunctionNameShort_T[ui->comboBox_MeritFunction->currentIndex()]) + "_OPT" + opt_settings + "_BS" + QString::number(ui->checkBox_XrayBS->isChecked()) + ".firelog";

    QString fileComplex = QString(path + QDir::separator() + "%1").arg(name);
    QString fileName = QFileDialog::getSaveFileName(this, "Save Settings File", fileComplex, "FIRE-settings(*firelog)");*/



    /*QString path, filename_vol, file_ext;
    FileInfo(idf_vol.filePath, path, filename_vol, file_ext);
    QString opt_settings = QString::number(ui->checkBox_tx->isChecked()) + QString::number(ui->checkBox_ty->isChecked()) + QString::number(ui->checkBox_tz->isChecked()) +
                           QString::number(ui->checkBox_rx->isChecked()) + QString::number(ui->checkBox_ry->isChecked()) + QString::number(ui->checkBox_rz->isChecked());

    QString logfilePath = QDir::homePath() + QDir::separator() + GetDateTimeString() + "_" + filename_vol + "_MERIT-" + QString(MeritFunctionNameShort_T[ui->comboBox_MeritFunction->currentIndex()]) + "_OPT" + opt_settings + "_BS" + QString::number(ui->checkBox_XrayBS->isChecked()) + ".firelog";

    DBG_PRINT("%s\n", logfilePath.toLatin1().constData());*/



    /*int threshold_ribs_min = ui->horizontalSlider_VoxelFilter1ThresholdMin->value();
    int threshold_ribs_max = ui->horizontalSlider_VoxelFilter1ThresholdMax->value();
    SuppressBonesInXray(threshold_ribs_min, threshold_ribs_max, 0.3);
    UpdateView(DISP_XRAY_1);*/

    return;
}
void MainWindow::OnButtonCopyTransRot()
{
    QClipboard* clipboard = QApplication::clipboard();

    QString text = "";
    text.sprintf("%.3f/%.3f/%.3f %.3f/%.3f/%.3f", drr_transrot.trans_X, drr_transrot.trans_Y, drr_transrot.trans_Z, drr_transrot.rot_X, drr_transrot.rot_Y, drr_transrot.rot_Z);

    clipboard->setText(text, QClipboard::Clipboard);

    if (clipboard->supportsSelection())
    {
        clipboard->setText(text, QClipboard::Selection);
    }
#ifndef WIN32
    QThread::msleep(1); //workaround for linux os
#endif
}

/*void MainWindow::GenerateMeritMap(unsigned int h, unsigned int v, float res)
{
    // check if volume is loaded
    if(!idf_vol.IsInitialized())
    {
        DBG_ERROR("load volume data before generation of merit map!");
        return;
    }

    MeritFunction_T merit_fctn = (MeritFunction_T)this->ui->comboBox_MeritFunction->currentIndex();
    float fit = 0.;

    TransRot_T tr = drr_transrot;
    unsigned int steps_x, steps_y, x, y;

    steps_x = (2*h)/res;
    steps_y = (2*v)/res;

    DBG_PRINT("\n");
    //x = vertical, y = depth, z = horizontal
    for(y=0, drr_transrot.trans_Y=(tr.trans_Y-v); y<=steps_y; y++, drr_transrot.trans_Y=(tr.trans_Y-v)+res*y)
    {
        for(x=0, drr_transrot.trans_X=(tr.trans_X-h); x<=steps_x; x++, drr_transrot.trans_X=(tr.trans_X-h)+res*x)
        {
            RenderDrr(DISP_DRR_1, &idf_vol);
            UpdateView(DISP_DRR_1);

            switch(merit_fctn)
            {
                case MERIT_FUNCTION_SRC: fit = 1.-StochastikRankCorrelation(drr_1, idf_xray_1.imgData, drr_width, drr_height, active_mask_1); break;
                case MERIT_FUNCTION_NCC: fit = -1.*NormalizedCrossCorrelation(drr_1, idf_xray_1.imgData, drr_width, drr_height, active_mask_1); break;
                case MERIT_FUNCTION_PI: fit = 1.-PatternIntensity(drr_1, idf_xray_1.imgData, drr_width, drr_height, active_mask_1); break;
                case MERIT_FUNCTION_MI: fit = -1.*MutualInformation(drr_1, idf_xray_1.imgData, drr_width, drr_height, active_mask_1); break;
                case MERIT_FUNCTION_CR: fit = -1.*CorrelationRatio(drr_1, idf_xray_1.imgData, drr_width, drr_height, active_mask_1); break;
                default: fit = 0.; break;
            }

            DBG_PRINT("%.6f", fit);
            if(x<steps_x) DBG_PRINT(",");
            //DBG_PRINT("%.3f/%.3f ", drr_transrot.trans_X, drr_transrot.trans_Z);
        }
        DBG_PRINT("\n");
    }
    DBG_PRINT("\n");

    drr_transrot = tr;
    RenderDrr(DISP_DRR_1, &idf_vol);
    UpdateView(DISP_DRR_1);
}*/

/*void CreateEllipsoid()
{
    const unsigned int width = 512;
    const unsigned int height = 512;
    const unsigned int depth = 512;

    const unsigned int pos_x = width/2;
    const unsigned int pos_y = height/2;
    const unsigned int pos_z = depth/2;

    const double a = 50;
    const double b = 40;
    const double c = 30;

    unsigned short* vol = new unsigned short[width*height*depth];

    double r;

    for(unsigned int w=0; w<width; w++)
    {
        for(unsigned int h=0; h<height; h++)
        {
            for(unsigned int d=0; d<depth; d++)
            {
                r = SQUARE(w-pos_x)/SQUARE(a) + SQUARE(h-pos_y)/SQUARE(b) + SQUARE(d-pos_z)/SQUARE(c);

                if(r <= 1.)
                {
                    vol[w+h*width+d*width*height] = 30000;
                }
                else
                {
                    vol[w+h*width+d*width*height] = 0;
                }
            }
        }
    }

    //idf_vol.Load(vol, width, height, depth);
    delete vol;

    //Cuda_LoadVolume(idf_vol.imgData, idf_vol.width, idf_vol.height, idf_vol.depth);
    //UpdateView(UpdateSelector_T());
}*/

/*void MainWindow::PrepareVolume(unsigned short* data, unsigned short threshold_min, unsigned short threshold_max)
{
    // INFO: Function is obsolete since cuda-renderer is capable to filter out voxels during rendering
    unsigned int size = idf_vol.width*idf_vol.height*idf_vol.depth;
    unsigned short val;
    static unsigned short* buffer = NULL;

    for(unsigned int i=0; i<size; i++)
    {
        val = idf_vol.imgData[i];
        if(val < threshold_min)
        {
            data[i] = 0;
        }
        else if(val >= threshold_max)
        {
            data[i] = threshold_max;
        }
        else
        {
            data[i] = val;
        }
    }
}*/


//--------------------------- Phase Recognition --------------------------------------------------------------------

void MainWindow::on_StartReadingButton_clicked()
{
    // Manage UI widgets
    ui->StartReadingButton->setText("Reading..");
    ui->StartReadingButton->setEnabled(false);
    ui->StopReadingButton->setEnabled(true);
    ui->StartPhaseRecognitionButton->setEnabled(true);

    // Define default path of real time data file
    rThread->path = MainWindow::rPath;

    // Create log file
    QDateTime dateTime = QDateTime::currentDateTime();
    QString formattedDateTime = dateTime.toString("yyyyMMdd_hhXmmXss");
    QString logFileName = "SurfaceData/LogFiles/" + rThread->path.left(rThread->path.length() - 4) + "_" + formattedDateTime + ".txt";
    rThread->logFile.setFileName(logFileName);
    if (!rThread->logFile.open(QIODevice::WriteOnly|QIODevice::Text)) {
        qDebug() << "Error in MainWindow::on_StartReadingButton_clicked(). Creating log file was not successful";
    }
    else {
        rThread->logStream.setDevice(&rThread->logFile);
    }
    rThread->logStream << rThread->path << ";" << ui->DebugModeRadioButton->isChecked() << ";";

    // Adjust time delay between datapoints according to chosen mode
    if (ui->DebugModeRadioButton->isChecked()){
        // Current phase will be shown in UI and data is read faster
        connect(rThread,SIGNAL(showCurrentPhase(int*)), dThread, SLOT(phaseChanged(int*)));
        rThread->sleepingTime = 25;
    }
    else{
        // Data is read in expected time steps
        rThread->sleepingTime = 75;
    }
    rThread->logStream << rThread->sleepingTime << "\n";

    // Open data file
    rThread->f.setFileName("SurfaceData/" + rThread->path);
    if (!rThread->f.open(QIODevice::ReadOnly)) {
        rThread->logStream << "Error in SurfaceData::run(). Opening SurfaceData file was not successful";
    }

    // Read header lines of csv file and find first datapoint
    if(!rThread->readHeaderLines(&rThread->f)){
            rThread->logStream << "Error in SurfaceData::run()."
                        " Reading header lines in function readHeaderLines(QFile*) was not successfull";
    }

    // Start reading of data
    rThread->start();

    // Thread for plotting data
    dThread = new PlottingThread(rThread, lThread, ui, this);

    // Creates charts for real time data
    connect(rThread,SIGNAL(CreateChart(double, double, double)), dThread, SLOT(on_CreateChart(double, double, double)));

    //Start timer for dynamical reading
    rThread->IterTimer = new QTimer(rThread);
    connect(rThread->IterTimer, &QTimer::timeout, rThread, &SurfaceData::ReadIter);
    rThread->IterTimer->start(rThread->sleepingTime);
}

void MainWindow::on_StopReadingButton_clicked()
{
    // Stop reading of real time data
    rThread->fout.close();
    rThread->terminate();
    ui->StartReadingButton->setText("Reading stopped");
    rThread->logStream << "Reading stopped";
    rThread->logFile.close();
}

void MainWindow::on_LoadingButton_clicked()
{
    // Adjust UI
    ui->LoadingButton->setText("Loading..");
    ui->StopLoadingButton->setEnabled(true);

    // Define default path for planning CT data
    lThread->path = MainWindow::lPath;

    // Start reading of data
    lThread->start();                       // starts LoadingThread::run()
    connect(lThread,SIGNAL(LoadingStopped()), this, SLOT(on_StopLoading()));
}


void MainWindow::on_StopLoading()
{
    // Adjust UI
    this->ui->LoadingButton->setText("Loaded");
    this->ui->StopLoadingButton->setEnabled(false);
    this->ui->StartReadingButton->setEnabled(true);

    // Stop reading of planning CT data
    lThread->terminate();
}


void MainWindow::on_StartPhaseRecognitionButton_clicked()
{
    // Adjust UI
    ui->StartPhaseRecognitionButton->setEnabled(false);

    // Close chart dialog if performance mode was chosen
    if (!ui->DebugModeRadioButton->isChecked()) {
        dThread->chartDialog->reject();
        ui->phaseLabel->setText("Performance");
    }

    // Enter while loop "Reading, Filtering and Phase Recognition"
    rThread->PhaseRecogStarted = true;
}


void MainWindow::on_SelectReadingFileButton_clicked()
{
    // Adjust UI
    ui->StartPhaseRecognitionButton->setEnabled(false);

    // Choose file in current folder manually
    QString rFullPath = QFileDialog::getOpenFileName(this, "Select Surface Data", QDir::currentPath()+"/SurfaceData", "All Files (*.*)");
    if (rFullPath.isEmpty()){
        rThread->logStream << "Error in MainWindow::on_SelectReadingFileButton_clicked()."
                              "Selecting surface data file was not successfull.";
    }
    else {
        QFileInfo rPathInfo(rFullPath);
        rPath = rPathInfo.fileName();
    }
    ui->ReadingFileLabel->setText(rPath);
}

void MainWindow::on_SelectLoadingFileButton_clicked()
{
    // Choose file in current folder manually
    QString lFullPath = QFileDialog::getOpenFileName(this, "Select 4D CT Data", QDir::currentPath()+"/SurfaceData", "All Files (*.*)");
    if (lFullPath.isEmpty()){
        rThread->logStream << "Error in MainWindow::on_SelectLoadingFileButton_clicked()."
                            "Selecting 4D CT data file was not successfull.";
    }
    else {
        QFileInfo lPathInfo(lFullPath);
        lPath = lPathInfo.fileName();
    }
    ui->LoadingFileLabel->setText(lPath);
}

void MainWindow::on_fc_slider_sliderReleased()
{
    // Apply new value for fc if slider was moved
    rThread->fc = ui->fc_slider->value() / 100.0;
    rThread->aValue = qSqrt( qPow( qCos(2*M_PI* rThread->fc/ rThread->fs), 2) - 4*qCos(2*M_PI* rThread->fc/ rThread->fs) + 3)
            + qCos(2*M_PI* rThread->fc/ rThread->fs) - 1;

//    double new_Amp = 0.0;
//    double convProd = 1.0;
//    int N = rThread->Amp1List.size();
//    if (rThread->Amp1List.size() >= rThread->windowSize) {
//        for(int convInd = 0; convInd < rThread->windowSize; ++convInd){
//            convProd = rThread->aValue * (rThread->Amp1List[N-1-convInd] * qPow((1-rThread->aValue), convInd));
//            new_Amp = convProd+new_Amp;
//        }
//    }

    // Show new fc in UI
    QString fc_text = QString::number(rThread->fc, 'f', 2);
    fc_text = fc_text + " Hz";
    ui->fc_label->setText(fc_text);

    rThread->logStream << "\nfcN" << rThread->fc << "\n";
}
