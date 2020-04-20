#ifndef __ccipdCaPViewDCEWindow_h
#define __ccipdCaPViewDCEWindow_h

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
class Ui_CaPViewDCEWindow;
class QVTKWidget;
class vtkPropPointer;
//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{

/**
 * \brief  GUI window for CaPView.
 * 
 * \author  Gohary
 * \ingroup CaPView
 * \ingroup CaPView
 */
class CaPViewDCEWindow : public QMainWindow
{

//////////////////////////////////////////////////////////////////////////////////////////
	Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////
public:
  enum DCE_FEATURE
  {
    MAX_UPTAKE    = 0,
    TIME_TO_PEAK     ,
    RATE_OF_UPTAKE   ,
    RATE_OF_WASHOUT  ,
    DCE_ENHANCEMENT  ,
    ENHANCEMENT_RATIO
  };


  CaPViewDCEWindow(QStringList DCEFeaturesFilePaths, unsigned int nSlices);
  virtual ~CaPViewDCEWindow();
//////////////////////////////////////////////////////////////////////////////////////////
public slots:
  void slot_mouseWheelForwardEvent(vtkObject*, unsigned long, void*, void*);
  void slot_mouseWheelBackwardEvent(vtkObject*, unsigned long, void*, void*);
  void on_qxtSpanSlider_Heatmap_upperValueChanged(int range);
  void on_qxtSpanSlider_Heatmap_lowerValueChanged(int min);
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

protected:
  /** \brief  : Converts a grayscale image to a colored while making the pixels outside the min-max range transparent
    * \Created: Ahmad 9/29/2013
    * \Updated: Ahmad 02/03/2014 
    * \ingroup: 
    * \param  : 
    * \param  : 
    * \return : 
    */
  RGBAVolumePointer GetAsColormap(VolumeImagePointer m_pProbabilitiesImage, InternalPixelType minPercentage = 0.0, InternalPixelType maxPercentage = 1.0);
  void LoadFeatures_DCE(const QStringList & DCEFeaturesFilePaths);
  void UpdateActionsEnabledState();
  void ResetApp();

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
	void renderVolume(QVTKWidget* pQVTKWidget, RGBAVolumeConstPointer pVolumeImage, unsigned int sliceNumber = 0);
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
  virtual void initControlsValues(); 

  //virtual void resetControlsValues();
 
  virtual void initGUI();

  

protected slots:
//////////////////////////////////////////////////////////////////////////////////////////

  void on_comboBox_Features_DCE_currentIndexChanged(int index);

  void on_action_Load_DCE_triggered();

  //void on_pushButton_ResetParams_clicked();
   
  void on_pushButton_ResetCamera_clicked();
  void on_pushButton_ClearView_clicked();

  void on_horizontalSlider_currentSlice_valueChanged(int sliceNum);
  void on_horizontalSlider_VolumeOpacity_DCE_valueChanged(int Opacity);

  void on_action_SaveDCEImage_triggered();
  void on_action_SaveAppSnapshot_triggered();
  void on_actionCaPView_Help_triggered();
  void on_actionAbout_CaPView_triggered();
  void on_actionAbout_CCIPD_triggered();
  //void on_checkBox_Volume_DCE_toggled(bool showDCEVolume);

  void renderCancerHeatmap();
  void UpdateHeatmapSpan();

  private slots:

  private:
//////////////////////////////////////////////////////////////////////////////////////////
  // Designer form
  //Ui_CaPViewDCEWindow * ui;
    
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////

}; // class CaPViewDCEWindow
} // namespace ccipd
#endif // __ccipdCaPViewDCEWindow_h
