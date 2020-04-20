#ifndef __ccipdCaPViewMainWindow_h
#define __ccipdCaPViewMainWindow_h

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

// vtk includes
#include <vtkObject.h>


#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// Forward Qt class declarations
class Ui_CaPViewMainWindow;
class QVTKWidget;
class vtkPropPointer;
//////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
namespace ccipd
{

/**
 * \brief  GUI window for CaPView.
 * 
 * \author  Gohary
 * \ingroup CaPView
 * \ingroup CaPView
 */
class CaPViewMainWindow : public QMainWindow
{

//////////////////////////////////////////////////////////////////////////////////////////
	Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////
public:

  enum VisualObject
  {
    T2_SLICES = 0,
    ADC_SLICES,
    //PROSTATE_SLICES,
    PROSTATE_SURFACE,
    CAP_SLICES,
    CAP_SURFACE
  };

  CaPViewMainWindow();
  virtual ~CaPViewMainWindow();
//////////////////////////////////////////////////////////////////////////////////////////
  void wheelEvent(QWheelEvent* event);


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

protected:

	void LoadImage_T2();
	void LoadImage_ADC();
	void LoadImage_PMask();
	void LoadImage_CaP();


  /** \brief  : checkes if file exits on HDD, it tries lnsions and returns the index of the existent one if  
    * \Created: Ahmad 1/24/2014
    * \Updated:  
    * \ingroup: 
    * \param  : 
    * \param  : 
    * \return : 
    */
	QString CheckFileExistence(const QString & filePath, const QStringList & extensio);

    void UpdateActionsEnabledState();


   

  /** \brief  : Retieves the Application Settings values 
    * \Created: axa667 9/29/2013
    * \Updated:  
    * \ingroup: 
    * \param  : 
    * \param  : 
    * \return : 
    */
	bool RetrieveSettings();
  /** \brief  : Saves the Application Settings values 
    * \Created: axa667 9/29/2013
    * \Updated:  
    * \ingroup: 
    * \param  : 
    * \param  : 
    * \return : 
    */
	bool SaveSettings();
	/////////////////////////////
	/////////////////////////////
	//void renderVolume(QVTKWidget* pQVTKWidget, RGBAVolumeConstPointer pVolumeImage, unsigned int sliceNumber = 0);
  
	void renderGraylevelVolume(VolumeImageConstPointer pVolumeImage, QVTKWidget* pQVTKWidget, vtkPropPointer & pProp, unsigned int sliceNumber = 0);

  
  void clearViewer(QVTKWidget* pQVTKWidget);

  /** \brief  : Sets the controls' ranges according to the loaded volume intensity values
    * \Created: axa667 7/25/2013
    * \Updated:  
    * \ingroup: 
    * \param  : 
    * \param  : 
    * \return : 
    */
  void initControlsValues(VisualObject whichObject = T2_SLICES); 
  /** \brief  : Gives the sliders and the spin boxes the default min values
    * \Created: axa667 7/31/2013
    * \Updated:  
    * \ingroup:
    * \param  : 
    * \param  : 
    * \return : 
    */
  void resetControlsValues();
 
  void initGUI();

  /// capture all events
  //virtual bool eventFilter(QObject * object, QEvent * event );

  //void DisplayErrorMessage(QMainWindow *pWindow, QString message, unsigned int blinkingTime = 2000);
  
public slots:
  


protected slots:
  void resetWidgetsVisibility();
  void ResetApp();
  void slot_mouseWheelForwardEvent(vtkObject*, unsigned long, void*, void*);
  void slot_mouseWheelBackwardEvent(vtkObject*, unsigned long, void*, void*);

  void on_qxtSpanSlider_Heatmap_lowerValueChanged(int min);

  void on_qxtSpanSlider_Heatmap_upperValueChanged(int range);


//////////////////////////////////////////////////////////////////////////////////////////
  void on_action_Show_Windowing_Controls_toggled( bool checked );
  
  void on_qxtCheckComboBox_Main_checkedItemsChanged( const QStringList & items );

  void setWindowWidthT2 (double value);
  void setWindowWidthT2 (int    value);
  void setWindowLevelT2 (int    value);
  void setWindowLevelT2 (double value);
  void setWindowWidthADC(double value);
  void setWindowWidthADC(int    value);
  void setWindowLevelADC(int    value);
  void setWindowLevelADC(double value);


  void on_action_LoadDatasetDir_triggered();

  void on_action_SetProstateSurfaceColor_triggered();
  void on_action_SetPZSurfaceColor_triggered();
  void on_action_SetCGSurfaceColor_triggered();
  void on_action_SetCancerSurfaceColor_triggered();
  void on_action_Load_T2_triggered();
  void on_action_Load_ADC_triggered();

  void on_actionOpen_DCE_Window_triggered();

  void on_action_Load_Mask_Prostate_triggered();
  void on_action_Load_Probabilities_triggered();
  
  void on_action_resetParameters_triggered();
   
  void on_action_ResetCamera_triggered();
  //void on_pushButton_ClearView_clicked();

  void on_horizontalSlider_currentSlice_valueChanged(int sliceNum);
  //void on_horizontalSlider_Window_valueChanged(int window);
  //void on_horizontalSlider_LevelColor_valueChanged(int level);
  void on_horizontalSlider_ProstateMaskSurface_Opacity_valueChanged(int Opacity);

  void on_horizontalSlider_VolumeOpacity_T2_valueChanged(int Opacity);
  void on_horizontalSlider_VolumeOpacity_ADC_valueChanged(int Opacity);


  void on_horizontalSlider_ProstateMaskOpacity_valueChanged(int Opacity);
  void on_horizontalSlider_CancerSurfaceOpacity_valueChanged(int Opacity);
  void on_horizontalSlider_OpacityHeatMapOverT2_valueChanged(int Opacity);
  void on_horizontalSlider_OpacityHeatMapOverADC_valueChanged(int Opacity);

  //void on_horizontalSlider_HeatmapRange_valueChanged(int range);
  //void on_horizontalSlider_HeatmapMinimum_valueChanged(int min);

  //void on_groupBox_CancerSurface_toggled(bool showLevelsetSurface);
  //void on_checkBox_HeatMapOverT2_toggled(bool showHeatmap);
  //void on_checkBox_HeatMapOverADC_toggled(bool showHeatmap);
  //void on_checkBox_Volume_T2_toggled(bool showT2Volume);
  //void on_checkBox_Volume_ADC_toggled(bool showT2Volume);

  //void on_groupBox_ProstateMask_toggled(bool showProstate);

  void on_doubleSpinBox_LevelSetValue_valueChanged(double value);

  void UpdateCancerHeatmap();

  void on_horizontalSlider_LevelSetValue_valueChanged(int levelset);

  void on_action_SaveT2ADCImage_triggered();


  void on_action_SaveAppSnapshot_triggered();
  void on_actionCaPView_Help_triggered();
  void on_actionAbout_CaPView_triggered();
  void on_actionAbout_CCIPD_triggered();

//////////////////////////////////////////////////////////////////////////////////////////
  void setLevelsetValue(int value);
  void setLevelsetValue(double value);

  private slots:
    //void blinkErrorMessage();

  private:
//////////////////////////////////////////////////////////////////////////////////////////
  // Designer form
  //Ui_CaPViewMainWindow * ui;

//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class CaPViewMainWindow
} // namespace ccipd
#endif // __ccipdCaPViewMainWindow_h
