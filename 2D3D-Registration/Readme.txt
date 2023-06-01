MainWindow class:

MainWindow(QWidget *parent)                     -> initialization of the application
                                                   e.g. RtSystemConfig, DRRs, masks, dropdown boxes, sliders, etc.

// SECTION: RADIATHERAPY SYSTEM CONFIGURATION
RtSystemsConfigInit()                           -> initialization of the available rt systems in the structure set
                                                   names, detector & source distance, pixel spacing, etc.

// SECTION: IMPORT VOLUME(S) AND SAVED SETTINGS
OnButtonVolumeLoad()                            -> load volume data (.nii or .avw file) into idf_vol/idf_vol_bs
ImportVolumeData(QString filePath)              -> load volume data into graphic memory (function is called from OnButtonVolumeLoad())
OnButtonSaveConfig()                            -> save entire settings and file paths to a file on disk
                                                   the config file (.fire) is used to restore all settings of the session
OnButtonLoadConfig()                            -> imports a configuration and updates the GUI elements (slider, drop down menus, ...)
UpdateVoxelFilterSliders()                      -> slides for voxel filtering are updated (min, max, current position) according to the volume data (idf_vol_bs)
                                                   e.g. when new volume data is loaded an update is triggered to avoid having old/illegal thresholds in the GUI

// SECTION: IMPORT AND SELECT X-RAYS
OnButtonXrayList1Load()                         -> opens a dialogbox/explorer to select multiple x-ray images (.pgm or .his), filepaths are inserted in x-ray listbox1
OnButtonXrayList2Load()                         -> opens a dialogbox/explorer to select multiple x-ray images (.pgm or .his), filepaths are inserted in x-ray listbox2
OnXrayList1SelectionChanged(QListWidgetItem* li, QListWidgetItem* li_prev)
                                                -> update of x-ray image 1 within GUI including bone suppression if enabled
OnXrayList2SelectionChanged(QListWidgetItem* li, QListWidgetItem* li_prev)
                                                -> update of x-ray image 2 within GUI including bone suppression if enabled

// SECTION: MASK MANAGEMENT
OnButtonActiveMask1Load()                       -> load MASK1 from .pgm file
OnButtonActiveMask2Load()                       -> load MASK2 from .pgm file
OnButtonActiveMask1Save()                       -> save MASK1 to .pgm file on disk 
OnButtonActiveMask2Save()                       -> save MASK2 to .pgm file on disk
OnRadioButtonSelectMask11AsActiveClicked()      -> selects MASK1 (ROI for rib registration) and updates DRR1 and XRAY1 within GUI
OnRadioButtonSelectMask12AsActiveClicked()      -> selects MASK2 (ROI for bone suppression) and updates DRR1 and XRAY1 within GUI
OnRadioButtonSelectMask21AsActiveClicked()      -> selects MASK1 (ROI for rib registration) and updates DRR2 and XRAY2 within GUI
OnRadioButtonSelectMask22AsActiveClicked()      -> selects MASK2 (ROI for rib registration) and updates DRR2 and XRAY1 within GUI
OnSliderActiveMask1ValueChanged()               -> update MASK1 settings (position x/y, width and height) and update DRR1 and XRAY1 within GUI
OnSliderActiveMask2ValueChanged()               -> update MASK2 settings (position x/y, width and height) and update DRR2 and XRAY1 within GUI

// SECTION: RAY CASTING
OnComboBoxRtSystemChanged(int sel)              -> activate predefined rt-system parameter set (x-ray source/detector position, pixel spacing and rotation)
                                                   and update all DRRs
OnComboBoxRegistrationModeChanged()             -> activate predefined registration mode (single/double axis mode = one or two imaging perspectives)
OnCam1SettingsChanged()                         -> obsolete function
OnCam2SettingsChanged()                         -> obsolete function
OnSliderIntensity1ValueChanged()                -> adjust intensity (brightness) of DRR1
OnSliderIntensity2ValueChanged()                -> adjust intensity (brightness) of DRR2
OnSliderVoxelFilter1ThresholdValueChanged()     -> update voxel filter settings and DRR1/XRAY1 including bone suppressoin if enabled
OnSliderVoxelFilter2ThresholdValueChanged()     -> update voxel filter settings and DRR2/XRAY2 including bone suppressoin if enabled
OnSliderBackgroundSaturationValueChanged()      -> update background (region out of ROI) attenuation, setting to zero allows for DRR ray-casting in real-time
OnSliderTransRotValueChanged()                  -> update translation/rotation matrix according to slider settings in GUI and update DRRs
OnButtonTransRotUp()                            -> get translation/rotation from status bar and update slider settings in GUI (e.g. after registration)
OnButtonTransRotDown()                          -> get translation/rotation from GUI sliders and update status bar
OnSliderWeightingFactorDrrValueChanged()        -> adjust weighting factor for bone suppression in DRRs and update DRRs
OnSliderWeightingFactorXrayValueChanged()       -> adjust weighting factor for bone suppression in XRAYs and update XRAYs
OnCheckBoxXrayBSClicked()                       -> enable/disable XRAY bone suppression
RenderDrr(DisplaySelector_T sel, ImageDataFile* idf)
                                                -> initiate ray-casting of DRRs
Rendering()                                     -> call ray-casting function with corresponding parameters (GUI)
RenderOption_T rdr_opt                          -> RDROPT_DIFF: voxel filtering in DRR (w=0), filtered values in diff
                                                   RDROPT_BONESUP: no filtering in DRR, values above threshold in diff (weighted)
ImageDataFile* idf                              -> volume data
unsigned short cuda_mask_id                     -> selected mask identifier
TransRot_T* transrot                            -> translation/rotation vector
ImagingSystemConfig* imgConf                    -> pointer to imaging system configuration
unsigned short threshold_min                    -> lower threshold value for e.g. bone suppression
unsigned short threshold_max                    -> upper threshold value for e.g. bone suppression
float weighting                                 -> weighting factor to be applied for values above upper threshold value
unsigned short windowing_min                    -> lower threshold for DRR windowing
unsigned short windowing_max                    -> upper threshold for DRR windowing
float intensity                                 -> adjust intensity (brightness) of DRR
unsigned short** buf1_out                       -> output memory of DRR
unsigned short** buf2_out                       -> output memory of bone suppressed/differential DRR
                                                
// SECTION: IMAGE DISPLAY
UpdateView(DisplaySelector_T sel)               -> update of DRR1/DRR2/XRAY1/XRAY2 and status bar according to selection
DisplayImage(unsigned short data[], unsigned int width, unsigned int height, QLabel* label, Mask_T* mask)
                                                -> draw image (DRR/XRAY) in GUI and apply mask
Init8BitImage(QImage* image, unsigned short data[])
                                                -> initializes QImage from unsigned short data (conversion)
OnSliderWindowing1ValueChanged()                -> update DRR according to windowing settings of GUI

// SECTION: 2D/3D REGISTRATION
UpdateOptimizerTransRotAccordingSelection()     -> create translation/rotation vector for optimizer according selected checkboxes (freedom) within the GUI
UpdateDrrTransRotAccordingSelection()           -> create translation/rotation vector for optimizer from translation/rotation vector of DRRs
OnButtonRegistration2D3D()                      -> execute 2D/3D registration
CallbackDrrRenderer(void* instance)             -> callback for 2D/3D registration to realize DRR update including a delay for visualization during runtime
OnButtonBatchRegistration2D3D()                 -> initiates 2D/3D batch registration with/without bone suppression accroding to checkbox state
BatchRegistration2D3D()                         -> execute 2D/3D batch registration for all XRAY images in the list including data logging of results
CallbackBatchDrrRenderer(void* instance)        -> callback for 2D/3D batch registration (memory update of GUI objects)
BatchRegistration2D3DBS()                       -> execute 2D/3D batch registration including bone suppression for all XRAY images in the list including data logging of results
CallbackBatchDrrRendererRibsBS(void* instance)  -> callback for 2D/3D batch registration of rib ROI (memory update of GUI objects)
CallbackBatchDrrRendererTumorBS(void* instance) -> callback for 2D/3D batch registration of tumour ROI (memory update of GUI objects)
SuppressBonesInXray()                           -> remove bones from XRAY image
  unsigned short threshold_min                    -> low threshold for bone-suppression
  unsigned short threshold_max                    -> high threshold for bone-suppression
  float weighting                                 -> weighting factor for bone-suppression in the XRAY image


// SECTION: DATA LOGGING
JsonLogger::JsonLogger(MainWindow* handle)      -> data logger class for storing batch-registration results and statistics in json format
void JsonLogger::AddSettings()                  -> inserts settings (rt-system, registration settings, cams, masks) to the logger object 
void JsonLogger::AddRecord()                    -> add a logger record to the logger object
  TransRot_T* transrot                            -> assessed translation/rotation vector
  int iterations                                  -> number of iterations required for 2D/3D registration
  int time_ms                                     -> elapsed time in milliseconds
  QString filepath_xray1                          -> filepath of XRAY1
  QString filepath_xray2                          -> filepath of XRAY2
void JsonLogger::AddRecordBS()                  -> add a logger record with bone-suppression info to the logger object
  TransRot_T* transrot_ribs                       -> translation/rotation result of rib registration
  TransRot_T* transrot_tumor                      -> translation/rotation result of tumour registration
  int iterations_ribs                             -> number of iterations required for rib registration
  int iterations_tumor                            -> number of iterations required for tumour registration
  int time_ms_ribs                                -> runtime of rib registration
  int time_ms_tumor                               -> runtime of tumour registration
  int time_ms_bs                                  -> runtime of bone-suppression
  QString filepath_xray1                          -> filepath of XRAY1
  QString filepath_xray2                          -> filepath of XRAY2
void JsonLogger::Save()                         -> save logfile (json format) to disk

// SECTION: STATUS LABEL UPDATE WITHIN GUI
UpdateCam1Status()                              -> CAM1 status line in GUI is updated according to windowing, voxel-filtering and weighting settings (on slider motion)
UpdateCam2Status()                              -> CAM2 status line in GUI is updated according to windowing, voxel-filtering and weighting settings (on slider motion)
UpdateBSStatus()                                -> bone-suppression status in GUI is updated according to the weighting factor for the XRAYs
UpdateStatusBar()                               -> update status-bar according to translation/rotation vector (on trans/rot slider motion)

// SECTION: SWAP CAM1/CAM2 SETTINGS
OnButtonIntensity1Up()                          -> apply intensity of CAM2 on CAM1
OnButtonIntensity2Down()                        -> apply intensity of CAM1 on CAM2
OnButtonVoxelFilter1ThresholdUp()               -> copy voxel-filter settings of CAM2 to CAM1
OnButtonVoxelFilter1ThresholdSwap()             -> swap voxel-filter threshold low and high of CAM1
OnButtonVoxelFilter2ThresholdDown()             -> copy voxel-filter settings of CAM1 to CAM2
OnButtonVoxelFilter2ThresholdSwap()             -> swap voxel-filter threshold low and high of CAM2

// SECTION: KEYBOARD INPUT
eventFilter(QObject *obj, QEvent *event)        -> keyboard event handler
                                                   'del' with focus on x-ray listbox: removes selected XRAY image
                                                   UP/DOWN/LEFT/RIGHT with focus on DRR/XRAY: move active mask according to key function
                                                   'SHIFT+UP/DOWN/LEFT/RIGHT' with focus on DRR/XRAY: resize width and height of active mask

// SETCION: HELPER FUNCTIONS
OpenSingleFileDialog(QString title, QString path, QString filterStr, QString* filePath_out, bool updateWorkingDir)
                                                -> opens a file dialog to select a single file
                                                   filterStr: desired file extension (e.g.: .pgm, .nii)
                                                   updateWorkingDir: is used to start at the chosen directory on the next function call                                          
OpenMultipleFilesDialog(QString title, QString path, QString filterStr, QStringList* filePaths_out, bool updateWorkingDir)
                                                -> opens a file dialog and allows for selecting multiple files                                      
FileInfo(QString filePath, QString &path, QString &name, QString &ext)
                                                -> splits filepath into path, name and extension                                        
GetDateTimeString()                             -> get local time in string format 'yyyyMMdd-HHmmss'
GetMinMax(unsigned short data[], unsigned int length, unsigned short* minimum, unsigned short* maximum)
                                                -> get minimum and maximum value of data array                                       
Windowing8Bit(unsigned short data[], unsigned int length, unsigned short minimum, unsigned short maximum)
                                                -> apply windowing on the data array (minimum .. maximum -> 0 .. 255)                                       
OnButtonSave()                                  -> save DRRs and XRAYs to home path



Convolution Module:

implementation of convolution in CPU

Kernel_Init_Gauss()                             -> initialize coefficients for a gaussian kernel
  Kernel_T* kernel                                -> pointer to kernel memory
  unsigned short dim                              -> size of the kernel
  float sigma                                     -> sigma

Kernel_Conv()                                   -> convolution function
  unsigned short* img_in                          -> pointer to input image memory
  unsigned short* img_out                         -> pointer to output image memory
  unsigned short width                            -> width of input image
  unsigned short height                           -> height of input image
  Kernel_T* kernel                                -> pointer to kernel object

Kernel_Print()                                  -> print kernel coefficients

Kernel_Free()                                   -> free allocated kernel memory