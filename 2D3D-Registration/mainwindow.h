// Author: Ing. Ingo Gulyas, MMSc
// Date: 21.11.2019
// Medical University of Vienna / AKH Wien
// Department of Radiotherapy

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

#include "ImageDataFile.h"
#include "Registration.h"
#include "Mask.h"
#include "Matrix.h"
#include "CudaDrrRenderer.h"

#include <QListWidgetItem>
#include <QJsonArray>
#include <QJsonObject>

//Phase Recognition
#include "plottingthread.h"
#include "surfacedata.h"
#include "loadingthread.h"
#include "chartdialog.h"
#include "qcustomplot.h"
#include <armadillo>

namespace Ui {
class MainWindow;
}



typedef enum {DISP_DRR_1=BV(0),
              DISP_DRR_2=BV(1),
              DISP_DRR_ALL=(BV(0)|BV(1)),
              DISP_XRAY_1=BV(2),
              DISP_XRAY_2=BV(3),
              DISP_XRAY_ALL=(BV(2)|BV(3)),
              DISP_ALL=(BV(0)|BV(1)|BV(2)|BV(3))} DisplaySelector_T;



class ImagingSystemConfig
{
    public:
        QString name;
        float source_distance;
        float detector_distance;
        float pixel_spacing_ratio;
        Vector4_T rotation_ref;
};

class RtSystemConfig
{
    public:
        QString name;
        ImagingSystemConfig imgSys1;
        ImagingSystemConfig imgSys2;
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow *ui;

    TransRot_T drr_transrot;

    // since the optimizer can be configured to use only some of the 6 degrees of freedom
    // (translation X/Y/Z, rotation X/Y/Z) this array is used
    double opt_transrot[6];
    double drr_intensity1, drr_intensity2;

    RtSystemConfig RtSysConfig[3];
    RtSystemConfig* RtSysActive;

    ImageDataFile idf_vol;
    ImageDataFile idf_vol_bs;
    ImageDataFile idf_xray_1;
    ImageDataFile idf_xray_2;

    Mask_T mask_11;
    Mask_T mask_12;
    Mask_T mask_21;
    Mask_T mask_22;

    Mask_T* active_mask_1 = &mask_11;
    Mask_T* active_mask_2 = &mask_21;

    unsigned short active_mask_1_cuda_id = 0;
    unsigned short active_mask_2_cuda_id = 2;

    static const int drr_width = 410;
    static const int drr_height = 410;
    static const int drr_size = drr_width*drr_height;

    unsigned short drr_1[drr_height*drr_width];
    unsigned short drr_2[drr_height*drr_width];

    unsigned int background_saturation = 128;

    QString volume_dir;

    void RtSystemsConfigInit();
    bool ImportVolumeData(QString filePath);
    void UpdateVoxelFilterSliders();
    void RenderDrr(DisplaySelector_T sel, ImageDataFile* idf);
    void Rendering(RenderOption_T rdr_opt, ImageDataFile* idf, unsigned short cuda_mask_id, TransRot_T* transrot, ImagingSystemConfig* imgConf, unsigned short threshold_min, unsigned short threshold_max, float weighting, unsigned short windowing_min, unsigned short windowing_max, float intensity, unsigned short** buf1_out, unsigned short** buf2_out);
    void InsertJsonLogSettings(QJsonObject* j_obj);
    void UpdateView(DisplaySelector_T sel);
    void Init8BitImage(QImage* image, unsigned short data[]);
    int UpdateOptimizerTransRotAccordingSelection();
    int UpdateDrrTransRotAccordingSelection();
    void BatchRegistration2D3D();
    void BatchRegistration2D3DBS();
    void DisplayImage(unsigned short data[], unsigned int width, unsigned int height, QLabel* label, Mask_T* mask);
    void UpdateCam1Status();
    void UpdateCam2Status();
    void UpdateBSStatus();
    void UpdateStatusBar();


    bool OpenSingleFileDialog(QString title, QString path, QString filterStr, QString* filePath_out, bool updateWorkingDir);
    unsigned int OpenMultipleFilesDialog(QString title, QString pathk, QString filsterStr, QStringList* filePaths_out, bool updateWorkingDir);
    void FileInfo(QString filePath, QString& path, QString& name, QString& ext);
    QString GetDateTimeString();
    void GetMinMax(unsigned short data[], unsigned int length, unsigned short* minimum, unsigned short* maximum);
    void Windowing8Bit(unsigned short data[], unsigned int length, unsigned short minimum, unsigned short maximum);

    // Phase Recognition
    SurfaceData *rThread;
    LoadingThread *lThread;
    PlottingThread *dThread;

    QString rPath = "2021_11_30_Andi_RespSignal_20211130_171611_clean.csv";
    QString lPath = "BruMi_4D_CT_dummy.csv";

protected:
    bool eventFilter(QObject *obj, QEvent *event);

public slots:

    void OnButtonVolumeLoad();
    void OnButtonSaveConfig();
    void OnButtonLoadConfig();

    void OnButtonXrayList1Load();
    void OnButtonXrayList2Load();
    void OnXrayList1SelectionChanged(QListWidgetItem* li, QListWidgetItem* li_prev);
    void OnXrayList2SelectionChanged(QListWidgetItem* li, QListWidgetItem* li_prev);

    void OnButtonActiveMask1Load();
    void OnButtonActiveMask2Load();
    void OnButtonActiveMask1Save();
    void OnButtonActiveMask2Save();

    void OnRadioButtonSelectMask11AsActiveClicked();
    void OnRadioButtonSelectMask12AsActiveClicked();
    void OnRadioButtonSelectMask21AsActiveClicked();
    void OnRadioButtonSelectMask22AsActiveClicked();

    void OnSliderActiveMask1ValueChanged();
    void OnSliderActiveMask2ValueChanged();

    void OnComboBoxRtSystemChanged(int);
    void OnComboBoxRegistrationModeChanged();
    void OnCam1SettingsChanged();
    void OnCam2SettingsChanged();
    void OnSliderIntensity1ValueChanged();
    void OnSliderIntensity2ValueChanged();
    void OnSliderVoxelFilter1ThresholdValueChanged();
    void OnSliderVoxelFilter2ThresholdValueChanged();
    void OnSliderBackgroundSaturationValueChanged();
    void OnSliderTransRotValueChanged();
    void OnButtonTransRotUp();
    void OnButtonTransRotDown();
    void OnSliderWeightingFactorDrrValueChanged();
    void OnSliderWeightingFactorXrayValueChanged();
    void OnCheckBoxXrayBSClicked();
    void OnSliderWindowing1ValueChanged();

    void OnButtonRegistration2D3D();
    void OnButtonBatchRegistration2D3D();

    void OnButtonIntensity1Up();
    void OnButtonIntensity2Down();
    void OnButtonVoxelFilter1ThresholdUp();
    void OnButtonVoxelFilter1ThresholdSwap();
    void OnButtonVoxelFilter2ThresholdDown();
    void OnButtonVoxelFilter2ThresholdSwap();

    void OnButtonSave();
    void OnButtonTest();
    void OnButtonCopyTransRot();
    void SuppressBonesInXray(unsigned short threshold_min, unsigned short threshold_max, float weighting);

    // Phase Recognition
    void on_StartReadingButton_clicked();
    void on_StopReadingButton_clicked();
    void on_LoadingButton_clicked();
    void on_StopLoading();
    void on_StartPhaseRecognitionButton_clicked();
    void on_fc_slider_sliderReleased();
    void on_SelectReadingFileButton_clicked();
    void on_SelectLoadingFileButton_clicked();
};

class JsonLogger
{
public:
    JsonLogger(MainWindow* handle);
    void AddSettings();
    void AddRecord(TransRot_T* transrot, int iterations, int time_ms, QString filepath_xray1, QString filepath_xray2);
    void AddRecordBS(TransRot_T* transrot_ribs, TransRot_T* transrot_tumor, int iterations_ribs, int iterations_tumor, int time_ms_ribs, int time_ms_tumor, int time_ms_bs, QString filepath_xray1, QString filepath_xray2);
    void Save();

private:
    MainWindow* mwdw;
    QJsonArray j_reg_recs_arr;
    QJsonObject j_log_obj;
};


#endif // MAINWINDOW_H
