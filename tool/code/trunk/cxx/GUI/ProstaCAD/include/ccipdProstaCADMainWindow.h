#ifndef __ccipdProstaCADMainWindow_h
#define __ccipdProstaCADMainWindow_h

//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMacros.h"
#include "ccipdVolumeTypes.h"
#include "ccipdRGBSliceTypes.h"


#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// qt includes
#include <QMainWindow>

// std includes
#include <memory> // for smart pointers

// itk includes
#include <itkImage.h>



#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Forward Qt class declarations
class Ui_ProstaCADMainWindow;
class QVTKWidget;
//////////////////////////////////////////////////////////////////////////////////////////


namespace ccipd
{

/**
 * \brief  GUI window for ProstaCAD.
 * 
 * \author  Gohary
 * \ingroup ProstaCAD
 * \ingroup ProstaCAD
 */
class ProstaCADMainWindow : public QMainWindow
{

//////////////////////////////////////////////////////////////////////////////////////////
	Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
public:
  ProstaCADMainWindow();
  virtual ~ProstaCADMainWindow();
//////////////////////////////////////////////////////////////////////////////////////////



public slots:
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////



protected:
  void ResetApp();
  void clearViewer(QVTKWidget* pQVTKWidget);
  void renderVolume(QVTKWidget* pQVTKWidget, VolumeImageConstPointer pVolumeImage, bool bClearViewerFirst = false, unsigned int sliceNumber = 0);
  void renderVolume(QVTKWidget* pQVTKWidget, RGBAVolumeConstPointer pVolumeImage, unsigned int sliceNumber = 0);
  
  void renderHeatMap();

  
  

  void renderSurface(QVTKWidget* pQVTKWidget, VolumeImageConstPointer pVolume, InternalPixelType levelsetValue = 0.5);
  /** \brief  : Sets the controls' ranges according to the loaded volume intensity values
    * \Created: axa667 7/25/2013
    * \Updated:  
    * \ingroup: 
    * \param  : 
    * \param  : 
    * \return : 
    */
  virtual void initControlsValues(int latestDisplayedVolume); 
  /** \brief  : Gives the sliders and the spin boxes the default min values
    * \Created: axa667 7/31/2013
    * \Updated:  
    * \ingroup:
    * \param  : 
    * \param  : 
    * \return : 
    */
  virtual void resetControlsValues();
 

  virtual void initGUI();
  virtual void hideAllFeatures();
  virtual void showFeatureGroupBoxByName(const QString & text);


  /// capture all events
  virtual bool eventFilter(QObject * object, QEvent * event );

  public slots:
    void UpdateStatusMessage(const QString & msg);

protected slots:
//////////////////////////////////////////////////////////////////////////////////////////
	void on_pushButton_test_clicked();
	void on_qxtCheckComboBox_Features_DCE_checkedItemsChanged(const QStringList & items);

  /// Exit the application.
  void SlotExit();

  void on_pushButton_AddMeanFeature_clicked();
  void on_pushButton_AddMedianFeature_clicked();
  void on_pushButton_AddGaborFeature_clicked();
  void on_pushButton_AddHaralickFeature_clicked();
  void on_pushButton_AddIntensityFeature_clicked();

  void on_action_LoadVolume_triggered();
  void on_action_LoadVolume_DCE_triggered();
  void on_action_SaveProbabilitiesVolumeImage_triggered();
  void on_action_SaveHeatmapVolume_triggered();
  
  // Volume tab
  void on_horizontalSlider_currentSlice_valueChanged(int sliceNum);
  void on_horizontalSlider_Window_valueChanged(int window);
  void on_horizontalSlider_Level_valueChanged(int level);
  void on_horizontalSlider_InputVolumeImageOpacity_valueChanged(int Opacity);
  
  void on_horizontalSlider_currentSlicePreprocessed_valueChanged(int sliceNum);
  void on_horizontalSlider_WindowPreprocessed_valueChanged(int window);
  void on_horizontalSlider_LevelPreprocessed_valueChanged(int level);
  void on_horizontalSlider_OpacityPreprocessed_valueChanged(int Opacity);

  void on_horizontalSlider_Opacity_DCE_valueChanged(int Opacity);
  void on_horizontalSlider_Opacity_DCE_Features_valueChanged(int Opacity);


  void on_horizontalSlider_currentSliceCurrentFeature_valueChanged(int sliceNum);
  //void on_horizontalSlider_WindowCurrentFeature_valueChanged(int window);
  //void on_horizontalSlider_LevelCurrentFeature_valueChanged(int level);
  void setWindowColorCurrentFeature(double value);
  void setWindowColorCurrentFeature(int value);
  void setLevelColorCurrentFeature (int value);
  void setLevelColorCurrentFeature (double value);
  void setWindowWidth_DCE(double value);
  void setWindowWidth_DCE(int value);
  void setWindowLevel_DCE(int value);
  void setWindowLevel_DCE(double value);

  void setWindowWidth_DCE_Features(double value);
  void setWindowWidth_DCE_Features(int value);
  void setWindowLevel_DCE_Features(int value);
  void setWindowLevel_DCE_Features(double value);

  void setTimePoint_DCE(int value);
  void setTimePoint_DCE(double value);

  void on_horizontalSlider_OpacityCurrentFeature_valueChanged(int Opacity);
  
  // DCE Tab
  void on_horizontalSlider_SliceNum_DCE_Features_valueChanged(int sliceNum);
  void on_horizontalSlider_SliceNum_DCE_valueChanged(int sliceNum);
  void on_groupBox_DCEVolumeControls_toggled(bool showDCE);
  //////////////////////////////////////////////////////////////////////////

  //void on_horizontalSlider_MaskedVolumeImageOpacity_valueChanged(int Opacity);
  void on_horizontalSlider_HeatMapVolumeImageOpacity_valueChanged(int Opacity);
  void on_horizontalSlider_HeatmapRange_valueChanged(int range);
  void on_horizontalSlider_HeatmapMinimum_valueChanged(int min);
  void on_checkBox_WhitenData_toggled(bool val);
  void on_action_File_Load_Histogram_triggered();

  //void on_comboBox_ExtractedFeatures_currentIndexChanged(int index);
  void on_listWidget_Features_currentRowChanged(int currentRow);
  
// Menu Bar Action
  // File Menu
  void on_action_LoadVolumeImage_triggered();
  void on_action_Load_Mask_Prostate_triggered();
  void on_actionFile_Load_StandardImage_triggered();
  // Tools Menu
  void on_action_showSettingsTab_triggered();
  void on_action_showOptionsTab_triggered();

  //virtual void on_pushButton_resetCamera_clicked();

  //void on_horizontalSlider_LevelSet_sliderMoved(int levelset);
  void on_pushButton_ExractFeatures_DCE_clicked();
  void on_pushButton_ExractKineticFeatures_DCE_clicked();
  void on_pushButton_ExtractFeatures_clicked();

  void on_pushButton_Classify_clicked();
  void on_comboBox_TextureFeatures_currentTextChanged(const QString & text);

  virtual void on_doubleSpinBox_LevelSetValue_valueChanged(double value);
  void UpdateCancerHeatmap();
  void on_pushButton_LoadFeatures_clicked();

  void on_pushButton_Train_clicked();


  void on_pushButton_Whiten_clicked();

  void on_horizontalSlider_LevelSetValue_valueChanged(int levelset);
  

//////////////////////////////////////////////////////////////////////////////////////////
  void setLevelsetValue(int value);
  void setLevelsetValue(double value);


  bool RetrieveSettings();
  void UpdateSettings();
  bool SaveSettings();

  //void DisplayErrorMessage(QMainWindow *pWindow, QString message, unsigned int blinkingTime = 2000);
  //void blinkErrorMessage();

  void on_groupBox_OriginalVolumeControls_toggled(bool showOriginalVolume);
  void on_groupBox_PreprocessedVolumeControls_toggled(bool showPreprocessedVolume);

  void on_pushButton_SaveNewSettings_clicked();
  void on_pushButton_CancelNewSettings_clicked();
  void on_pushButton_Preprocess_clicked();

  void on_checkBox_BiasFieldCorrection_toggled(bool correctBF);
  void on_checkBox_CropManually_toggled(bool cropM);
  void on_checkBox_CropAutomatically_toggled(bool cropA);
  void on_checkBox_ApplyStandardization_toggled(bool standardize);
  void on_checkBox_Normalize_toggled(bool normalize);
  void on_checkBox_Segment_toggled(bool bSegment);




  void on_doubleSpinBox_BiasSigma_valueChanged(double val);
  void on_doubleSpinBox_Padding_valueChanged(double val);
  void on_spinBox_RefHistogramBinsNum_valueChanged(int val);
  void on_spinBox_RefHistogramMatchingPointsNum_valueChanged(int val);

  void on_actionAbout_ProstaCAD_triggered();
  void on_actionAbout_CCIPD_triggered();
  void on_action_SavePreprocessedVolume_triggered();
 
  void on_action_SaveHistogramOriginalVolume_triggered();
  void on_action_SaveHistogramPreprocessedVolume_triggered();
  void on_action_SaveHistogramStandardVolume_triggered();


  
  void on_pushButton_RemoveCurrentFeature_clicked();
  void on_pushButton_RemoveAllFeatures_clicked();

  void on_groupBox_CancerSurface_toggled(bool showLevelsetSurface);
  void on_groupBox_HeatMap_toggled(bool showHeatmap);


  void on_action_SetCancerSurfaceColor_triggered();
  void on_action_SetProstateSurfaceColor_triggered();
  void on_action_SetPZSurfaceColor_triggered();
  void on_action_SetCGSurfaceColor_triggered();

  void on_pushButton_FastProcess_clicked();
  void on_qwtWheel_CropX_DCE_valueChanged(double value);
  void on_qwtWheel_CropY_DCE_valueChanged(double value);

  void on_pushButton_SaveDCEKineticFeatures_clicked();
  void on_pushButton_SaveCurrentTimePoint_DCE_clicked();
  void on_pushButton_SaveAllTimePoints_DCE_clicked();


private:
//////////////////////////////////////////////////////////////////////////////////////////
  // Designer form
  Ui_ProstaCADMainWindow * ui;
//////////////////////////////////////////////////////////////////////////////////////////
  enum LatestDisplayedVolumeImage
  {
	  ORIGINAL,
	  T2,
	  ADC,
	  DCE,
	  DCEFEATURE,
	  PREPROCESSED,
	  FEATURE
  };

//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////

  void DisplayFeaturesHierarchy();

}; // class ProstaCADMainWindow
} // namespace ccipd
#endif // __ccipdProstaCADMainWindow_h
