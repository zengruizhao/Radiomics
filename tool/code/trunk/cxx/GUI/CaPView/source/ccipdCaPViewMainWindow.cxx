//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdPreprocessVolume.h"
#include "ccipdBoundingBoxCreation.h"
#include "ccipdBoundingBoxCoordinates.h"
#include "ccipdBoundingBoxToActor.h" // for bounding box rendering
#include "ccipdCropImage.h"
#include "ccipdDisplayStatusMessage.h"
#include "ccipdDuplicateImage.h"
#include "ccipdExtractFeatureImage.hxx"
#include "ccipdFeatureCollection.h"
#include "ccipdFeatureID.h"
#include "ccipdFeatureTypes.h"
#include "ccipdFlipImage.h"
#include "ccipdGetLargestContiguousRegion.h"
#include "ccipdHistogram.h"
#include "ccipdImageToProp.hxx"
#include "ccipdIsosurfaceRendering.h"
#include "ccipdLandmarksToActor.h"
#include "ccipdLDAClassifier.h" 

#include "ccipdLevelsetToSurface.h"
#include "ccipdLoadableFactory.h"
#include "ccipdLoadImage.h"
#include "ccipdLoadVolume.h"
#include "ccipdLoadGUI.h"
#include "ccipdMaskToLevelset.h"
#include "ccipdQDAClassifier.h" 
#include "ccipdPickPointsOnVolume.h"
#include "ccipdCaPViewMainWindow.h"
#include "ccipdCaPViewDCEWindow.h"
#include "ccipdResizeImage.h"
#include "ccipdSaveGUI.h"
#include "ccipdSaveImage.h"
#include "ccipdSaveImageGUI.h"
#include "ccipdStandardizeImage.h"
#include "ccipdStatistics.h"
#include "ccipdStudy.h"
#include "ccipdTextureFeatureFirstOrderParameters.h"
#include "ccipdTextureFeatureGaborParameters.h"
#include "ccipdTextureFeatureMapParametersType.h"  
#include "ccipdTextureFeatureParser.h"
#include "ccipdUpdateVTKImageProp.h"
#include "ccipdUpdateVTKImageSlice.h"
#include "ccipdUpdateVTKImageWindowLevel.h"
#include "ccipdUpdateVTKPropColor.h"
#include "ccipdUpdateVTKPropTransparency.h"
#include "ccipdUpdateWindowLevelSliders.h"
#include "ccipdVTKForwardDeclarations.h"
#include "ccipdCohenMRIBiasCorrection.h" 
#include "ccipdVolumeContainerToVolumeFeatures.h"
#include "ccipdMaskImage.h"

#include "ccipdMatrixUtils.h"
#include "ccipdVTKUtils.h"

#include "ccipdWriteImage.h"

#include "ccipdDisableWarningsMacro.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// Qt includes
#include <QApplication>
#include <QFileDialog>
#include "ui_ccipdCaPViewMainWindow.h"
#include <QSettings>
#include <QMouseEvent>
#include <QColorDialog>
#include <QMessageBox>
#include <QThread>
#include <QProcess>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QDesktopServices>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER


// itk includes
#include <itkImage.h>
#include <itkImageBase.h>	 
#include <itkImageFileReader.h>	   
#include <itkImageFileWriter.h>
#include <itkStatisticsImageFilter.h>
#include <itkVectorImage.h>
#include <itkImageIterator.h>
#include <itkImageSliceIteratorWithIndex.h>
#include <itkRGBPixel.h>
#include <itkRGBAPixel.h>
#include <itkRescaleIntensityImageFilter.h>
#include <itkScalarToRGBColormapImageFilter.h>
#include "itkColormapFunction.h"
#include <itkCustomColormapFunction.h>


// VTK includes
#include <vtkCamera.h>
#include <vtkProp.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkActorCollection.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkProperty.h>
#include <vtkSmartPointer.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkTextProperty.h>
#include <vtkImageViewer2.h>


#include <vtkActor.h>
#include <vtkDelaunay2D.h>
#include <vtkLookupTable.h>
#include <vtkMath.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkDelaunay2D.h>
#include <vtkDelaunay3D.h>

#include <vtkCellArray.h>
#include <vtkProperty.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolygon.h>
#include <vtkSmartPointer.h>
#include <vtkMath.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCleanPolyData.h>

#include <vtkXMLPolyDataReader.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeRayCastMapper.h>

#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkEventQtSlotConnect.h>






// Boost includes
#include <boost/foreach.hpp>
#include <boost/math/constants/constants.hpp>


#include <math.h>
#include <stdio.h>
#include <float.h>

#include "ccipdEnableWarningsMacro.h"

///////////




#ifdef _MSC_VER
#pragma warning( disable: 4127 )
#endif
//////////////////////////////////////////////////////////////////////////////////////////

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{
//////////////////////////////////////////////////////////////////////////////////////////
/// PIMPL Interface to CaPViewMainWindow
class CaPViewMainWindow::SecretData : public QThread, boost::noncopyable
{
  
public:
  SecretData(); ///< Constructor



  QStringList m_DCEFeaturesFilePaths;

  QSettings m_settings;

  bool    m_bVerbose;
  QString m_lastDatasetDir;
  QString m_lastVolumePathT2;
  QString m_lastVolumePathADC;


  QString m_lastProstateMaskPath;
  QString m_lastHeatmapPath;
  QString m_lastprobabilitiesImagePath;
  QString m_lastImagePath;
  QString m_lastAppSnapshotPath;

  QStringList m_supportedImageFileExtensions;

//////////////////////////////////////////////////////////////////////////////////////////
  VolumeImagePointer m_pVolumeT2     ;
  VolumeImagePointer m_pVolumeADC    ;


  VolumeImagePointer m_pProstateMask      ;
  VolumeImagePointer m_pPZMask            ;
  VolumeImagePointer m_pCGMask            ;
  VolumeImagePointer m_pProbabilitiesImage; // This will hold the output of classifcation (probabilities for either cancer or not in whole prostate)
  
  RGBAVolumePointer  m_pHeatMap           ;

  /**
   * We only apply this when saving the output
   * because VTK doesn't render this correctly.
   */
   VolumeImageBase::DirectionType m_OriginalDirection;
 
   // For Volumes as Slices
  vtkPropPointer   m_ImagePropT2	          /*< Store the vtk prop for the originally loaded image */
                 , m_ImagePropADC
                 , m_ProstateMaskProp_T2
				         , m_ProstateMaskProp_ADC
                 , m_PZMaskProp
                 , m_CGMaskProp
                 , m_HeatmapPropT2 
                 , m_HeatmapPropADC 
                 ;

  // For Surfaces
	vtkActorPointer   m_pProstateLevelsetSurfaceActor_T2
                  , m_pProstateLevelsetSurfaceActor_ADC
                  , m_pPZLevelsetSurfaceActor
                  , m_pCGLevelsetSurfaceActor
                  , m_pCancerLevelsetSurfaceActor_overT2
					        , m_pCancerLevelsetSurfaceActor_overADC
                  ;
   

//////////////////////////////////////////////////////////////////////////
// Classification
  
  double           m_LevelSetValue;
  float            m_CancerOpacity;
/////////////////////////////////////////////////////////
  float            m_HeatMapRange;   // This is the min probability (which will bear the red color) in the heat map, its default value is 1
  float            m_HeatmapMinimum; // This is the max probability which will be colored blue in the heat map, all values less than this value will all be colored blue too.
  unsigned int     m_currentSlice;   // Number of slice (zero-based) shown in the viewer

  // Designer form
  Ui_CaPViewMainWindow * ui;

  //QLabel *m_pErrorMessageLabel;
  //QTimer *m_pErrorMessageTimer;


  vtkQtConnectorPointer m_pVtkQtConnector;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
CaPViewMainWindow::SecretData::SecretData()
{
  // Initialization for all secret data objects
	m_bVerbose = true;
	m_supportedImageFileExtensions << "mha" << "vtk" << "mhd";

  m_pVtkQtConnector = vtkQtConnectorPointer::New();

  m_ImagePropT2              = NULL;
  m_ImagePropADC             = NULL;

  m_ProstateMaskProp_T2      = NULL;
  m_ProstateMaskProp_ADC     = NULL;
  m_PZMaskProp               = NULL;
  m_CGMaskProp               = NULL;
  m_HeatmapPropT2            = NULL;
  m_HeatmapPropADC           = NULL;
  
  m_pProstateLevelsetSurfaceActor_T2     = NULL;
  m_pProstateLevelsetSurfaceActor_ADC    = NULL;
  m_pPZLevelsetSurfaceActor              = NULL;
  m_pCGLevelsetSurfaceActor              = NULL;
  m_pCancerLevelsetSurfaceActor_overT2   = NULL;
  m_pCancerLevelsetSurfaceActor_overADC  = NULL;

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
CaPViewMainWindow::CaPViewMainWindow() : ccipdNewPIMPLMacro
{

// Initialize members to default values
	
THIS->m_lastDatasetDir             = "";
THIS->m_lastVolumePathT2           = "";
THIS->m_lastVolumePathADC          = "";

THIS->m_lastProstateMaskPath       = "";
THIS->m_lastHeatmapPath            = "";
THIS->m_lastprobabilitiesImagePath = "";
THIS->m_lastImagePath              = "";
THIS->m_lastAppSnapshotPath        = "";
//////////////////////////////////////////////////////////////////////////////////////////
THIS->m_pVolumeT2                  = NULL;
THIS->m_pVolumeADC                 = NULL;

THIS->m_pProstateMask              = NULL;
THIS->m_pPZMask                    = NULL;
THIS->m_pCGMask                    = NULL;
THIS->m_pProbabilitiesImage        = NULL;
THIS->m_pHeatMap                   = NULL;


//////////////////////////////////////////////////////////////////////////
// Classification
  THIS->m_LevelSetValue  = 0.8;
  THIS->m_CancerOpacity  = 0.5;
  THIS->m_HeatMapRange   = 1.0;
  THIS->m_HeatmapMinimum = 0.0; 
  THIS->m_currentSlice   = 0  ;

  // Initialization for ONLY GUI objects
  
  RetrieveSettings();
  initGUI();
  
  
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////
void CaPViewMainWindow::initGUI()
{
    // Setting Up Window
    THIS->ui = new Ui_CaPViewMainWindow;
    THIS->ui->setupUi( this );
    // Initializing Visibility of QWidgets
    resetWidgetsVisibility();

    connect(THIS->ui->horizontalSlider_LevelSetValue, SIGNAL(valueChanged(int   )), this, SLOT(setLevelsetValue(int   )));
    connect(THIS->ui->doubleSpinBox_LevelSetValue   , SIGNAL(valueChanged(double)), this, SLOT(setLevelsetValue(double)));

	  connect(THIS->ui->qxtSpanSlider_Heatmap           , SIGNAL(sliderReleased()), this, SLOT(UpdateCancerHeatmap()));


	  connect(THIS->ui->horizontalSlider_WindowWidth_T2 , SIGNAL(valueChanged(int   )), this, SLOT(setWindowWidthT2(int   )));
    connect(THIS->ui->doubleSpinBox_WindowWidth_T2    , SIGNAL(valueChanged(double)), this, SLOT(setWindowWidthT2(double)));
    connect(THIS->ui->horizontalSlider_WindowLevel_T2 , SIGNAL(valueChanged(int   )), this, SLOT(setWindowLevelT2(int   )));
    connect(THIS->ui->doubleSpinBox_WindowLevel_T2    , SIGNAL(valueChanged(double)), this, SLOT(setWindowLevelT2(double)));

	  connect(THIS->ui->horizontalSlider_WindowWidth_ADC, SIGNAL(valueChanged(int   )), this, SLOT(setWindowWidthADC(int   )));
    connect(THIS->ui->doubleSpinBox_WindowWidth_ADC   , SIGNAL(valueChanged(double)), this, SLOT(setWindowWidthADC(double)));
    connect(THIS->ui->horizontalSlider_WindowLevel_ADC, SIGNAL(valueChanged(int   )), this, SLOT(setWindowLevelADC(int   )));
    connect(THIS->ui->doubleSpinBox_WindowLevel_ADC   , SIGNAL(valueChanged(double)), this, SLOT(setWindowLevelADC(double)));

    connect(THIS->ui->action_resetApp   , SIGNAL(triggered()), this, SLOT(ResetApp()));
    
//////////////////////////////////////////////
    // Initialize QVTKWidgets
    if(!initializeQVTKWidget(THIS->ui->qVTKWidget_T2))
    {
      DisplayStatusMessage(  statusBar()
                           , "Error in initializing T2 3D viewer"
                           , StatusMessageHandler/*::MessageType*/::ERR
                          );
    }
    else
    {
       THIS->m_pVtkQtConnector->Connect(THIS->ui->qVTKWidget_T2->GetRenderWindow()->GetInteractor(),
       vtkCommand::MouseWheelForwardEvent, this, 
       SLOT(slot_mouseWheelForwardEvent(vtkObject*, unsigned long, void*, void*)));
      
       THIS->m_pVtkQtConnector->Connect(THIS->ui->qVTKWidget_T2->GetRenderWindow()->GetInteractor(),
       vtkCommand::MouseWheelBackwardEvent, this, 
       SLOT(slot_mouseWheelBackwardEvent(vtkObject*, unsigned long, void*, void*)));
    }

    if(!initializeQVTKWidget(THIS->ui->qVTKWidget_ADC))
    {
      DisplayStatusMessage(  statusBar()
                           , "Error in initializing ADC 3D viewer"
                           , StatusMessageHandler/*::MessageType*/::ERR
                          );
    }
    else
    {
       THIS->m_pVtkQtConnector->Connect(THIS->ui->qVTKWidget_ADC->GetRenderWindow()->GetInteractor(),
       vtkCommand::MouseWheelForwardEvent, this, 
       SLOT(slot_mouseWheelForwardEvent(vtkObject*, unsigned long, void*, void*)));
      
       THIS->m_pVtkQtConnector->Connect(THIS->ui->qVTKWidget_ADC->GetRenderWindow()->GetInteractor(),
       vtkCommand::MouseWheelBackwardEvent, this, 
       SLOT(slot_mouseWheelBackwardEvent(vtkObject*, unsigned long, void*, void*)));
    }

  clearViewer(THIS->ui->qVTKWidget_T2);
  clearViewer(THIS->ui->qVTKWidget_ADC);
  
  

  THIS->ui->action_Load_T2->setEnabled(true);
  THIS->ui->action_Load_ADC->setEnabled(true);


  THIS->ui->action_Load_Mask_Prostate->setEnabled(true);
  THIS->ui->action_Load_Probabilities->setEnabled(true);




}

void CaPViewMainWindow::resetWidgetsVisibility()
{
  // VTK Widgets
  THIS->ui->qVTKWidget_T2->setVisible(true);
  THIS->ui->qVTKWidget_ADC->setVisible(true);

  // Legend ColorBar
  THIS->ui->lineEdit_HeatmapLegendRange->setVisible(true);
  THIS->ui->graphicsView_HeatmapLegend->setVisible(true);
  THIS->ui->lineEdit_HeatmapLegendMinimum->setVisible(true);
  THIS->ui->qxtSpanSlider_Heatmap->setVisible(true);

  // ComboBox and Slices Slider
  THIS->ui->qxtCheckComboBox_Main->setVisible(true);
  THIS->ui->label_Slice->setVisible(true);
  THIS->ui->horizontalSlider_currentSlice->setVisible(true);
  THIS->ui->spinBox_currentSlice->setVisible(true);

  // T2 and ADC Opacity Bars
  THIS->ui->label_VolumeOpacity_T2->setVisible(false);
  THIS->ui->horizontalSlider_VolumeOpacity_T2->setVisible(false);
  THIS->ui->spinBox_VolumeOpacity_T2->setVisible(false);
  THIS->ui->label_VolumeOpacity_ADC->setVisible(false);
  THIS->ui->horizontalSlider_VolumeOpacity_ADC->setVisible(false);
  THIS->ui->spinBox_VolumeOpacity_ADC->setVisible(false);

  // Prostate Slice and Surface Opacity
  THIS->ui->label_ProstateMaskSlice_Opacity->setVisible(false);
  THIS->ui->horizontalSlider_ProstateMaskOpacity->setVisible(false);
  THIS->ui->spinBox_ProstateMaskOpacity->setVisible(false);
  THIS->ui->label_ProstateMaskSurface_Opacity->setVisible(false);
  THIS->ui->horizontalSlider_ProstateMaskSurface_Opacity->setVisible(false);
  THIS->ui->spinBox_ProstateMaskSurface_Opacity->setVisible(false);

  // CaP Heatmap Opacity
  THIS->ui->label_OpacityHeatMapOverT2->setVisible(true);
  THIS->ui->horizontalSlider_OpacityHeatMapOverT2->setVisible(true);
  THIS->ui->spinBox_OpacityHeatMapOverT2->setVisible(true);
  THIS->ui->label_OpacityHeatMapOverADC->setVisible(true);
  THIS->ui->horizontalSlider_OpacityHeatMapOverADC->setVisible(true);
  THIS->ui->spinBox_OpacityHeatMapOverADC->setVisible(true);

  // Cap Surface Opacity and Levelset
  THIS->ui->label_CancerSurfaceOpacity->setVisible(false);
  THIS->ui->horizontalSlider_CancerSurfaceOpacity->setVisible(false);
  THIS->ui->spinBox_CancerSurfaceOpacity->setVisible(false);
  THIS->ui->label_LevelSet->setVisible(false);
  THIS->ui->horizontalSlider_LevelSetValue->setVisible(false);
  THIS->ui->doubleSpinBox_LevelSetValue->setVisible(false);

  // Windowing (Windth and Center)
  THIS->ui->label_WindowWidth_T2->setVisible(false);
  THIS->ui->horizontalSlider_WindowWidth_T2->setVisible(false);
  THIS->ui->doubleSpinBox_WindowWidth_T2->setVisible(false);
  THIS->ui->label_WindowLevel_T2->setVisible(false);
  THIS->ui->horizontalSlider_WindowLevel_T2->setVisible(false);
  THIS->ui->doubleSpinBox_WindowLevel_T2->setVisible(false);
      
  THIS->ui->label_WindowWidth_ADC->setVisible(false);
  THIS->ui->horizontalSlider_WindowWidth_ADC->setVisible(false);
  THIS->ui->doubleSpinBox_WindowWidth_ADC->setVisible(false);
  THIS->ui->label_WindowLevel_ADC->setVisible(false);
  THIS->ui->horizontalSlider_WindowLevel_ADC->setVisible(false);
  THIS->ui->doubleSpinBox_WindowLevel_ADC->setVisible(false);

}


void CaPViewMainWindow::setWindowWidthT2(int value)
{
	disconnect(THIS->ui->doubleSpinBox_WindowWidth_T2, SIGNAL(valueChanged(double)), this, SLOT(setWindowWidthT2(double)));
	double newVal = (double)(THIS->ui->doubleSpinBox_WindowWidth_T2->minimum() + (((double)value) * (THIS->ui->doubleSpinBox_WindowWidth_T2->maximum() - THIS->ui->doubleSpinBox_WindowWidth_T2->minimum()) / (100.0f)));
	THIS->ui->doubleSpinBox_WindowWidth_T2->setValue(newVal);
	UpdateVTKImageWindow(THIS->m_ImagePropT2, newVal);
	THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
	connect(THIS->ui->doubleSpinBox_WindowWidth_T2, SIGNAL(valueChanged(double)), this, SLOT(setWindowWidthT2(double)));
}

void CaPViewMainWindow::setWindowWidthT2(double value)
{
	disconnect(THIS->ui->horizontalSlider_WindowWidth_T2, SIGNAL(valueChanged(int)), this, SLOT(setWindowWidthT2(int)));
	int newVal = (int)(100.00f * (value - THIS->ui->doubleSpinBox_WindowWidth_T2->minimum()) / (THIS->ui->doubleSpinBox_WindowWidth_T2->maximum() - THIS->ui->doubleSpinBox_WindowWidth_T2->minimum()));
	THIS->ui->horizontalSlider_WindowWidth_T2->setValue(newVal);
	UpdateVTKImageWindow(THIS->m_ImagePropT2, value);
	THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
	connect(THIS->ui->horizontalSlider_WindowWidth_T2, SIGNAL(valueChanged(int)), this, SLOT(setWindowWidthT2(int)));
}

void CaPViewMainWindow::setWindowLevelT2(int value)
{
	disconnect(THIS->ui->doubleSpinBox_WindowLevel_T2, SIGNAL(valueChanged(double)), this, SLOT(setWindowLevelT2(double)));
	double newVal = (double)(THIS->ui->doubleSpinBox_WindowLevel_T2->minimum() + (((double)value) * (THIS->ui->doubleSpinBox_WindowLevel_T2->maximum() - THIS->ui->doubleSpinBox_WindowLevel_T2->minimum()) / (100.0f)));
	THIS->ui->doubleSpinBox_WindowLevel_T2->setValue(newVal);
	UpdateVTKImageLevel(THIS->m_ImagePropT2, newVal);
	THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
	connect(THIS->ui->doubleSpinBox_WindowLevel_T2, SIGNAL(valueChanged(double)), this, SLOT(setWindowLevelT2(double)));
}

void CaPViewMainWindow::setWindowLevelT2(double value)
{
	disconnect(THIS->ui->horizontalSlider_WindowLevel_T2, SIGNAL(valueChanged(int)), this, SLOT(setWindowLevelT2(int)));
	int newVal = (int)(100.00f * (value - THIS->ui->doubleSpinBox_WindowLevel_T2->minimum()) / (THIS->ui->doubleSpinBox_WindowLevel_T2->maximum() - THIS->ui->doubleSpinBox_WindowLevel_T2->minimum()));
	THIS->ui->horizontalSlider_WindowLevel_T2->setValue(newVal);
	UpdateVTKImageLevel(THIS->m_ImagePropT2, value);
	THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
	connect(THIS->ui->horizontalSlider_WindowLevel_T2, SIGNAL(valueChanged(int)), this, SLOT(setWindowLevelT2(int)));
}

void CaPViewMainWindow::setWindowWidthADC(int value)
{
	disconnect(THIS->ui->doubleSpinBox_WindowWidth_ADC, SIGNAL(valueChanged(double)), this, SLOT(setWindowWidthADC(double)));
	double newVal = (double)(THIS->ui->doubleSpinBox_WindowWidth_ADC->minimum() + (((double)value) * (THIS->ui->doubleSpinBox_WindowWidth_ADC->maximum() - THIS->ui->doubleSpinBox_WindowWidth_ADC->minimum()) / (100.0f)));
	THIS->ui->doubleSpinBox_WindowWidth_ADC->setValue(newVal);
	UpdateVTKImageWindow(THIS->m_ImagePropADC, newVal);
	THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
	connect(THIS->ui->doubleSpinBox_WindowWidth_ADC, SIGNAL(valueChanged(double)), this, SLOT(setWindowWidthADC(double)));
}

void CaPViewMainWindow::setWindowWidthADC(double value)
{
	disconnect(THIS->ui->horizontalSlider_WindowWidth_ADC, SIGNAL(valueChanged(int)), this, SLOT(setWindowWidthADC(int)));
	int newVal = (int)(100.00f * (value - THIS->ui->doubleSpinBox_WindowWidth_ADC->minimum()) / (THIS->ui->doubleSpinBox_WindowWidth_ADC->maximum() - THIS->ui->doubleSpinBox_WindowWidth_ADC->minimum()));
	THIS->ui->horizontalSlider_WindowWidth_ADC->setValue(newVal);
	UpdateVTKImageWindow(THIS->m_ImagePropADC, value);
	THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
	connect(THIS->ui->horizontalSlider_WindowWidth_ADC, SIGNAL(valueChanged(int)), this, SLOT(setWindowWidthADC(int)));
}

void CaPViewMainWindow::setWindowLevelADC(int value)
{
	disconnect(THIS->ui->doubleSpinBox_WindowLevel_ADC, SIGNAL(valueChanged(double)), this, SLOT(setWindowLevelADC(double)));
	double newVal = (double)(THIS->ui->doubleSpinBox_WindowLevel_ADC->minimum() + (((double)value) * (THIS->ui->doubleSpinBox_WindowLevel_ADC->maximum() - THIS->ui->doubleSpinBox_WindowLevel_ADC->minimum()) / (100.0f)));
	THIS->ui->doubleSpinBox_WindowLevel_ADC->setValue(newVal);
	UpdateVTKImageLevel(THIS->m_ImagePropADC, newVal);
	THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
	connect(THIS->ui->doubleSpinBox_WindowLevel_ADC, SIGNAL(valueChanged(double)), this, SLOT(setWindowLevelADC(double)));
}

void CaPViewMainWindow::setWindowLevelADC(double value)
{
	disconnect(THIS->ui->horizontalSlider_WindowLevel_ADC, SIGNAL(valueChanged(int)), this, SLOT(setWindowLevelADC(int)));
	int newVal = (int)(100.00f * (value - THIS->ui->doubleSpinBox_WindowLevel_ADC->minimum()) / (THIS->ui->doubleSpinBox_WindowLevel_ADC->maximum() - THIS->ui->doubleSpinBox_WindowLevel_ADC->minimum()));
	THIS->ui->horizontalSlider_WindowLevel_ADC->setValue(newVal);
	UpdateVTKImageLevel(THIS->m_ImagePropADC, value);
	THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
	connect(THIS->ui->horizontalSlider_WindowLevel_ADC, SIGNAL(valueChanged(int)), this, SLOT(setWindowLevelADC(int)));
}



//////////////////////////////////////////////////////////////////////////////////////////
CaPViewMainWindow::~CaPViewMainWindow()
{
	SaveSettings();
}
//////////////////////////////////////////////////////////////////////////////////////////
bool CaPViewMainWindow::RetrieveSettings()
{
	THIS->m_lastDatasetDir = THIS->m_settings.value("DatasetDir", "").toString();
	THIS->m_lastVolumePathT2 = THIS->m_settings.value("Volume/T2path", "").toString();
	THIS->m_lastVolumePathADC = THIS->m_settings.value("Volume/ADCpath", "").toString();


  THIS->m_lastProstateMaskPath = THIS->m_settings.value("PMask/path", "").toString();
  THIS->m_lastHeatmapPath = THIS->m_settings.value("Heatmap/path", "").toString();
  THIS->m_lastprobabilitiesImagePath = THIS->m_settings.value("ProbImage/path", "").toString();
  THIS->m_lastImagePath = THIS->m_settings.value("SnapshotImage/path", "").toString();
  THIS->m_lastAppSnapshotPath = THIS->m_settings.value("AppSnapshotImage/path", "").toString();

  // Restore GUI state
  restoreGeometry(THIS->m_settings.value("MainWindow/Geometry").toByteArray());
  restoreState(THIS->m_settings.value("MainWindow/WindowState").toByteArray());

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
bool CaPViewMainWindow::SaveSettings()
{
    // Save user settings
	
    THIS->m_settings.setValue("DatasetDir"            , THIS->m_lastDatasetDir            );
    THIS->m_settings.setValue("Volume/T2path"         , THIS->m_lastVolumePathT2          );
    THIS->m_settings.setValue("Volume/ADCpath"        , THIS->m_lastVolumePathADC         );

    THIS->m_settings.setValue("PMask/path"            , THIS->m_lastProstateMaskPath      );
    THIS->m_settings.setValue("Heatmap/path"          , THIS->m_lastHeatmapPath           );
    THIS->m_settings.setValue("ProbImage/path"        , THIS->m_lastprobabilitiesImagePath);
    THIS->m_settings.setValue("SnapshotImage/path"    , THIS->m_lastImagePath             );
    THIS->m_settings.setValue("AppSnapshotImage/path" , THIS->m_lastAppSnapshotPath       );

    // Save gui state settings
    THIS->m_settings.setValue("MainWindow/Geometry"   , saveGeometry()                    );
    THIS->m_settings.setValue("MainWindow/WindowState", saveState   ()                    );
    
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

 void CaPViewMainWindow::ResetApp()
 {
   //QProcess::startDetached(QApplication::applicationFilePath());
   //exit(12);
   
   clearViewer(THIS->ui->qVTKWidget_T2);
   clearViewer(THIS->ui->qVTKWidget_ADC  );

   THIS->m_pVolumeT2                = NULL;
   THIS->m_pVolumeADC               = NULL;


   THIS->m_pProstateMask                 = NULL;
   THIS->m_pPZMask                       = NULL;
   THIS->m_pCGMask                       = NULL;
   THIS->m_pProbabilitiesImage           = NULL;
   THIS->m_pHeatMap                      = NULL;
   
   THIS->m_LevelSetValue  = 0.8;
   THIS->m_CancerOpacity  = 0.5;
   THIS->m_HeatMapRange   = 1.0;
   THIS->m_HeatmapMinimum = 0.0; 
   THIS->m_currentSlice   = 0  ;

  THIS->m_ImagePropT2             = NULL;
  THIS->m_ImagePropADC            = NULL;


  THIS->m_ProstateMaskProp_T2                  = NULL;
  THIS->m_ProstateMaskProp_ADC                 = NULL;
  THIS->m_PZMaskProp                           = NULL;
  THIS->m_CGMaskProp                           = NULL;
  THIS->m_HeatmapPropT2                        = NULL;
  THIS->m_HeatmapPropADC                       = NULL;
  
  THIS->m_pProstateLevelsetSurfaceActor_T2     = NULL;
  THIS->m_pPZLevelsetSurfaceActor              = NULL;
  THIS->m_pCGLevelsetSurfaceActor              = NULL;
  THIS->m_pCancerLevelsetSurfaceActor_overT2   = NULL;
  THIS->m_pCancerLevelsetSurfaceActor_overADC  = NULL;
  
  // Reset GUI

   THIS->ui->action_Load_T2->setEnabled(true);
   THIS->ui->action_Load_ADC->setEnabled(true);


   THIS->ui->action_Load_Mask_Prostate->setEnabled(true);
   THIS->ui->action_Load_Probabilities->setEnabled(true);
   
   for(int i = 0; i < 6; i++)
      THIS->ui->qxtCheckComboBox_Main->setItemCheckState(i, Qt::Unchecked );

   
////////////////////////////////////////////////////////

//
//   // For default values
//   // slice num
//   // opacity and so
   THIS->ui->horizontalSlider_WindowWidth_T2->setValue(1);
   THIS->ui->horizontalSlider_WindowLevel_T2->setValue(1);
   THIS->ui->horizontalSlider_VolumeOpacity_T2->setValue(100);

   THIS->ui->horizontalSlider_WindowWidth_ADC->setValue(1);
   THIS->ui->horizontalSlider_WindowLevel_ADC->setValue(1);
   THIS->ui->horizontalSlider_VolumeOpacity_ADC->setValue(100);

 }

 void CaPViewMainWindow::on_horizontalSlider_LevelSetValue_valueChanged(int /*levelset*/)
 {
	 {
	   // Remove the actor
	   const vtkRenderWindowPointer renderWindow = THIS->ui->qVTKWidget_T2->GetRenderWindow();
	   const vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	   renderer->RemoveActor(THIS->m_pCancerLevelsetSurfaceActor_overT2);
	   // Update, Render and assign the newly created actor to m_pCancerLevelsetSurfaceActor_overT2
	   THIS->m_pCancerLevelsetSurfaceActor_overT2 = renderVolumeAsSurface<VolumeImageType>(THIS->m_pProbabilitiesImage, THIS->ui->qVTKWidget_T2, false, (const float)THIS->m_LevelSetValue, THIS->m_CancerOpacity, 10.0, false, false, 0.95, 0.20, 0.20);
	 }
	 {
     // Remove the actor
	 const vtkRenderWindowPointer renderWindow = THIS->ui->qVTKWidget_ADC->GetRenderWindow();
	 const vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	 renderer->RemoveActor(THIS->m_pCancerLevelsetSurfaceActor_overADC);
	 // Update, Render and assign the newly created actor to m_pCancerLevelsetSurfaceActor_overADC
	 THIS->m_pCancerLevelsetSurfaceActor_overADC = renderVolumeAsSurface<VolumeImageType>(THIS->m_pProbabilitiesImage, THIS->ui->qVTKWidget_ADC, false, (const float)THIS->m_LevelSetValue, THIS->m_CancerOpacity, 10.0, false, false, 0.95, 0.20, 0.20);
	 }
 }

/////////////////////////////////////////////////////////////////////////////////////////
  void CaPViewMainWindow::on_horizontalSlider_currentSlice_valueChanged(int sliceNum)
  {
    THIS->m_currentSlice = sliceNum - 1;
    
    if(THIS->m_ImagePropT2)
      UpdateVTKImageSlice(THIS->m_ImagePropT2, sliceNum - 1, THIS->m_bVerbose);
    
	if(THIS->m_ImagePropADC)
      UpdateVTKImageSlice(THIS->m_ImagePropADC, sliceNum - 1, THIS->m_bVerbose);

    if(THIS->m_ProstateMaskProp_T2)
	    UpdateVTKImageSlice(THIS->m_ProstateMaskProp_T2, sliceNum - 1, THIS->m_bVerbose );

	if(THIS->m_ProstateMaskProp_ADC)
	    UpdateVTKImageSlice(THIS->m_ProstateMaskProp_ADC, sliceNum - 1, THIS->m_bVerbose );

    if(THIS->m_HeatmapPropT2)
      UpdateVTKImageSlice(THIS->m_HeatmapPropT2, sliceNum - 1, THIS->m_bVerbose );
        
    if(THIS->m_HeatmapPropADC)
      UpdateVTKImageSlice(THIS->m_HeatmapPropADC, sliceNum - 1, THIS->m_bVerbose );

    THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
	  THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
  }

  //void CaPViewMainWindow::on_horizontalSlider_Window_valueChanged(int window)
  //{
  //  UpdateVTKImageWindow(THIS->m_ImagePropT2, window);
  //  THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
  //}

  //void CaPViewMainWindow::on_horizontalSlider_LevelColor_valueChanged(int level)
  //{
  //  UpdateVTKImageLevel(THIS->m_ImagePropT2, level);
  //  THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
  //}

  void CaPViewMainWindow::on_horizontalSlider_VolumeOpacity_T2_valueChanged(int Opacity)
  {
    UpdateVTKPropTransparency(THIS->m_ImagePropT2, (unsigned int)Opacity);
    THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
  }

  void CaPViewMainWindow::on_horizontalSlider_VolumeOpacity_ADC_valueChanged(int Opacity)
  {
    UpdateVTKPropTransparency(THIS->m_ImagePropADC, (unsigned int)Opacity);
    THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
  }

  void CaPViewMainWindow::on_horizontalSlider_ProstateMaskOpacity_valueChanged(int Opacity)
  {
    UpdateVTKPropTransparency(THIS->m_ProstateMaskProp_T2, (unsigned int)Opacity);
    THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
    UpdateVTKPropTransparency(THIS->m_ProstateMaskProp_ADC, (unsigned int)Opacity);
    THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
  }

  void CaPViewMainWindow::on_horizontalSlider_ProstateMaskSurface_Opacity_valueChanged(int Opacity)
  {
	  UpdateVTKPropTransparency(THIS->m_pProstateLevelsetSurfaceActor_T2, (unsigned int)Opacity);
	  THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();

	  UpdateVTKPropTransparency(THIS->m_pProstateLevelsetSurfaceActor_ADC, (unsigned int)Opacity);
	  THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
  }

  void CaPViewMainWindow::on_horizontalSlider_CancerSurfaceOpacity_valueChanged(int Opacity)
  {
	  UpdateVTKPropTransparency(THIS->m_pCancerLevelsetSurfaceActor_overT2, (unsigned int)Opacity);
	  THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
	  
	  UpdateVTKPropTransparency(THIS->m_pCancerLevelsetSurfaceActor_overADC, (unsigned int)Opacity);
	  THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
	  
	  THIS->m_CancerOpacity = (float)Opacity / 100.0f;
  }
  
  void CaPViewMainWindow::on_horizontalSlider_OpacityHeatMapOverT2_valueChanged(int Opacity)
  {
    UpdateVTKPropTransparency(THIS->m_HeatmapPropT2, (unsigned int)Opacity);
    THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
  }

  void CaPViewMainWindow::on_horizontalSlider_OpacityHeatMapOverADC_valueChanged(int Opacity)
  {
    UpdateVTKPropTransparency(THIS->m_HeatmapPropADC, (unsigned int)Opacity);
    THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
  }




void CaPViewMainWindow::on_action_Load_T2_triggered()
{
  // Let the user select a directory to load the volume from 
  QString FilePath = QFileDialog::getOpenFileName(this,
    tr("Please, select a file to load the T2 volume from ...."), 
    THIS->m_lastVolumePathT2, tr("Image Files (*.mha *.mhd *.vtk)"));

  // Check the returned directory path
  if (FilePath.isEmpty())
  {
    // write a warning on the status bar
    DisplayStatusMessage(  statusBar()
                         , "Warning: Empty or invalid directory ..."
                         , StatusMessageHandler/*::MessageType*/::WARNING
                        );
    return;
  }
 
	THIS->m_lastVolumePathT2 = FilePath;

	LoadImage_T2();
}

void CaPViewMainWindow::LoadImage_T2()
{
    // If the user loaded a volume before, everything is reset
    if(THIS->m_pVolumeT2 && !THIS->m_pProstateMask && !THIS->m_pProbabilitiesImage)
    {
      ResetApp();
    }

    // load the volume
    try
    {
      THIS->m_pVolumeT2 = CohenCorrectBiasField(ccipd::LoadITKImage<VolumeImageType>(THIS->m_lastVolumePathT2.toStdString()));
    }
	  catch (...)
    {
      DisplayStatusMessage(statusBar(), 
        "Error(453-7): Not able to Load T2 volume ...", StatusMessageHandler/*::MessageType*/::ERR);
      return;
    }

    if(!THIS->m_pVolumeT2)
    {
      // write a warning on the status bar
      DisplayStatusMessage(statusBar(), 
        "Invalid Volume ...", StatusMessageHandler/*::MessageType*/::WARNING);
      return;
    }

    this->setWindowTitle(QCoreApplication::applicationName() + "  -  " + THIS->m_lastVolumePathT2);

    // Deals with the case if some other Volume Image type was loaded before loading the original volume
    //if(THIS->m_pProbabilitiesImage)
    //{
    //  ResizeImage(THIS->m_pVolumeT2, THIS->m_pProbabilitiesImage, true);
    //}
    //else if(THIS->m_pProstateMask)
    //{
    //  ResizeImage(THIS->m_pVolumeT2, THIS->m_pProstateMask, true);
    //}

    // Flip the Loaded volume over the x-axis
    THIS->m_pVolumeT2 = FlipImage<VolumeImageType>(THIS->m_pVolumeT2, 1); // This will flip it over the y-axis

    // Now Display the loaded mask
    renderVolumeAsSlices<VolumeImageType>(THIS->m_pVolumeT2, THIS->ui->qVTKWidget_T2, THIS->m_ImagePropT2, THIS->m_currentSlice);
    // Set the default transparency
    UpdateVTKPropTransparency(THIS->m_ImagePropT2, (unsigned int)60);
    
    initControlsValues(T2_SLICES);

    // Prevent User from re-opening a volume image if any of the corresponding object has been loaded
    // User should press the Clear button first
    UpdateActionsEnabledState();
}

void CaPViewMainWindow::on_action_Load_ADC_triggered()
{
  // Let the user select a directory to load the volume from 
      QString FilePath = QFileDialog::getOpenFileName(this,
        tr("Please, select a file to load the ADC volume from ...."), THIS->m_lastVolumePathADC, tr("Image Files (*.mha *.mhd *.vtk)"));

      // Check the returned directory path
      if (FilePath.isEmpty())
      {
        // write a warning on the status bar
        DisplayStatusMessage(  statusBar()
                             , "Warning: Empty or invalid directory ..."
                             , StatusMessageHandler/*::MessageType*/::WARNING
                            );
        return;
      }
 
	  THIS->m_lastVolumePathADC = FilePath;

    LoadImage_ADC();
}

void CaPViewMainWindow::LoadImage_ADC()    
{
    // If the user loaded a volume before, everything is reset
    if(THIS->m_pVolumeADC && !THIS->m_pProstateMask && !THIS->m_pProbabilitiesImage)
    {
      ResetApp();
    }

    // load the volume
    try
    {
      THIS->m_pVolumeADC = ccipd::LoadITKImage<VolumeImageType>(THIS->m_lastVolumePathADC.toStdString());
    }
	  catch (...)
    {
      DisplayStatusMessage(statusBar(), 
        "Error(450-74): Not able to Load ADC volume ...", 
        StatusMessageHandler/*::MessageType*/::ERR);
      return;
    }

    if(!THIS->m_pVolumeADC)
    {
      // write a warning on the status bar
      DisplayStatusMessage(statusBar(), 
        "Warning(150-14): Invalid ADC Volume ....", 
        StatusMessageHandler/*::MessageType*/::WARNING);
      return;
    }

    this->setWindowTitle(QCoreApplication::applicationName() + "  -  " + THIS->m_lastVolumePathADC);

    // Deals with the case if some other Volume Image type was loaded before loading the original volume
    //if(THIS->m_pProbabilitiesImage)
    //{
    //  ResizeImage(THIS->m_pVolumeADC, THIS->m_pProbabilitiesImage, true);
    //}
    //else if(THIS->m_pProstateMask)
    //{
    //  ResizeImage(THIS->m_pVolumeADC, THIS->m_pProstateMask, true);
    //}

    // Flip the Loaded volume over the y-axis
    THIS->m_pVolumeADC = FlipImage<VolumeImageType>(THIS->m_pVolumeADC, 1); // This will flip it over the y-axis

    // Now Display the loaded mask
    renderVolumeAsSlices<VolumeImageType>(THIS->m_pVolumeADC, THIS->ui->qVTKWidget_ADC, THIS->m_ImagePropADC, THIS->m_currentSlice);
    // Set the default transparency
    UpdateVTKPropTransparency(THIS->m_ImagePropADC, (unsigned int)65);
    
    
    initControlsValues(ADC_SLICES);


    // Prevent User from re-opening a volume image if any of the corresponding object has been loaded
    // User should press the Clear button first
    UpdateActionsEnabledState();
}

void CaPViewMainWindow::on_action_Load_Mask_Prostate_triggered()
{
  QString prostateMaskFileName = QFileDialog::getOpenFileName(this, tr( "Please, select a file to load the prostate mask from ...."), 
                                                      THIS->m_lastProstateMaskPath, tr("Files (*.mha *.mhd *.vtk)"));   
  
  if(prostateMaskFileName.isEmpty())
  {
     DisplayStatusMessage(  statusBar()
                          , "Warning: Empty or invalid Prostate File path ..."
                          , StatusMessageHandler/*::MessageType*/::WARNING
                         );
	  return;
  }
  
  THIS->m_lastProstateMaskPath = prostateMaskFileName;

  LoadImage_PMask();
}

void CaPViewMainWindow::LoadImage_PMask()
{
  // If the user loaded a volume before, everything is reset
  if(THIS->m_pProstateMask && !THIS->m_pVolumeT2 && !THIS->m_pProbabilitiesImage)
  {
    ResetApp();
  }

  // Load Mask
  THIS->m_pProstateMask = LoadITKImage<VolumeImageType>(THIS->m_lastProstateMaskPath.toStdString());

  if (!THIS->m_pProstateMask)
  {
    DisplayStatusMessage(statusBar(), 
      "Error(563-29): Unable to load Mask file ...", StatusMessageHandler/*::MessageType*/::ERR);
    return;
  }

  this->setWindowTitle(QCoreApplication::applicationName() + "  -  " + THIS->m_lastProstateMaskPath);

  //THIS->m_pProstateMask->CopyInformation(THIS->m_pVolumeT2);
  //if(THIS->m_pVolumeT2)
  //{
  //    ResizeImage(THIS->m_pProstateMask, THIS->m_pVolumeT2, true);
  //    this->setWindowTitle(QCoreApplication::applicationName() + "  -  " + THIS->m_lastVolumePathT2);
  //}
  //else if(THIS->m_pProbabilitiesImage)
  //{
  //    ResizeImage(THIS->m_pProstateMask, THIS->m_pProbabilitiesImage, true);
  //}

  // Flip the Loaded mask over the x-axis
  THIS->m_pProstateMask = FlipImage<VolumeImageType>(THIS->m_pProstateMask, 1); // This will flip it over the y-axis

  // The default now is not to display the prostate mask in the beginning

  //// Now Display the loaded mask
  //renderVolumeAsSlices<VolumeImageType>(THIS->m_pProstateMask, THIS->ui->qVTKWidget_T2, THIS->m_ProstateMaskProp_T2, THIS->m_currentSlice, true);
  //renderVolumeAsSlices<VolumeImageType>(THIS->m_pProstateMask, THIS->ui->qVTKWidget_ADC, THIS->m_ProstateMaskProp_ADC, THIS->m_currentSlice, true);
  //initControlsValues(PROSTATE_SLICES);  
  
  //// Display the Prostate Surface from the mask
  //THIS->m_pProstateLevelsetSurfaceActor_T2  = renderVolumeAsSurface<VolumeImageType>(THIS->m_pProstateMask, THIS->ui->qVTKWidget_T2, true, 0.0, 0.5);
  //THIS->m_pProstateLevelsetSurfaceActor_ADC = renderVolumeAsSurface<VolumeImageType>(THIS->m_pProstateMask, THIS->ui->qVTKWidget_ADC, true, 0.0, 0.5);
  //initControlsValues(PROSTATE_SURFACE);

  // Prevent User from re-opening a volume image if any of the corresponding object has been loaded
  // User should press the Clear button first
  UpdateActionsEnabledState();

}

void CaPViewMainWindow::on_action_Load_Probabilities_triggered()
{
	// Let the user select the file to open
	QString probabilitiesImagePath = QFileDialog::getOpenFileName(this, tr("Please, select a file to load the probabilities Image from ...."),
														THIS->m_lastprobabilitiesImagePath, tr("Files (*.mha *.mhd *.vtk)"));

	// Check if the selected path is correct
	if (probabilitiesImagePath.isEmpty())
	{
     DisplayStatusMessage(  statusBar()
                          , "Warning: Empty Probabilities File path ..."
                          , StatusMessageHandler::WARNING
                         );
		return;
	}

	// Save the path for the next time
	THIS->m_lastprobabilitiesImagePath = probabilitiesImagePath;

  LoadImage_CaP();

}

void CaPViewMainWindow::LoadImage_CaP()
{
    // If the user loaded a volume before, everything is reset
    if(THIS->m_pProbabilitiesImage && !THIS->m_pVolumeT2 && !THIS->m_pProstateMask)
    {
      ResetApp();
    }

	// Load the file at the seleceted path
	THIS->m_pProbabilitiesImage = ccipd::LoadITKImage<VolumeImageType>(THIS->m_lastprobabilitiesImagePath.toStdString());

    if (!THIS->m_pProbabilitiesImage)
    {
      DisplayStatusMessage(statusBar(), "Error(643-5): Unable to load Probabilities file ...", StatusMessageHandler::ERR);
      return;
    }

    this->setWindowTitle(QCoreApplication::applicationName() + "  -  " + THIS->m_lastprobabilitiesImagePath);


    //THIS->m_pProstateMask->CopyInformation(THIS->m_pVolumeT2);
    //if(THIS->m_pVolumeT2)
    //{
    //    ResizeImage(THIS->m_pProbabilitiesImage, THIS->m_pVolumeT2, true);
    //    this->setWindowTitle(QCoreApplication::applicationName() + "  -  " + THIS->m_lastVolumePathT2);
    //}
    //else if(THIS->m_pProstateMask)
    //{      
    //    ResizeImage(THIS->m_pProbabilitiesImage, THIS->m_pProstateMask, true);
    //}
    
    // Flip the Loaded mask over the x-axis
    THIS->m_pProbabilitiesImage = FlipImage<VolumeImageType>(THIS->m_pProbabilitiesImage, 1); // This will flip it over the y-axis
    

	// Calulate and Show Heatmap
	UpdateCancerHeatmap(); // This includes Calculating and rendering the heatmap

  initControlsValues(CAP_SLICES);


    // Display the cancer surface from the probabilities file
	//THIS->m_pCancerLevelsetSurfaceActor_overT2  = renderVolumeAsSurface<VolumeImageType>(THIS->m_pProbabilitiesImage, THIS->ui->qVTKWidget_T2 , false, (const float)THIS->m_LevelSetValue, 0.9, 6.0, false, true, 0.95, 0.20, 0.20);
	//THIS->m_pCancerLevelsetSurfaceActor_overADC = renderVolumeAsSurface<VolumeImageType>(THIS->m_pProbabilitiesImage, THIS->ui->qVTKWidget_ADC, false, (const float)THIS->m_LevelSetValue, 0.9, 6.0, false, true, 0.95, 0.20, 0.20);
 // initControlsValues(CAP_SURFACE);


    // Prevent User from re-opening a volume image if any of the corresponding object has been loaded
    // User should press the Clear button first
    UpdateActionsEnabledState();
}

void CaPViewMainWindow::on_action_SetCancerSurfaceColor_triggered()
{
	UpdateVTKPropColorGUI(THIS->m_pCancerLevelsetSurfaceActor_overT2);
	THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();

	UpdateVTKPropColorGUI(THIS->m_pCancerLevelsetSurfaceActor_overADC);
	THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
}

void CaPViewMainWindow::on_action_SetProstateSurfaceColor_triggered()
{
	UpdateVTKPropColorGUI(THIS->m_pProstateLevelsetSurfaceActor_T2);
	THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();

	UpdateVTKPropColorGUI(THIS->m_pProstateLevelsetSurfaceActor_ADC);
	THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
}

void CaPViewMainWindow::on_action_SetPZSurfaceColor_triggered()
{
	UpdateVTKPropColorGUI(THIS->m_pPZLevelsetSurfaceActor);
	THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
}

void CaPViewMainWindow::on_action_SetCGSurfaceColor_triggered()
{
	UpdateVTKPropColorGUI(THIS->m_pCGLevelsetSurfaceActor);
	THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();
}

//void CaPViewMainWindow::on_pushButton_ClearView_clicked()
//{
//  ResetApp();
//}
void CaPViewMainWindow::on_action_ResetCamera_triggered()
{
	// extract out render window, reset and rerender
	vtkRenderWindowPointer renderWindow = THIS->ui->qVTKWidget_T2->GetRenderWindow();
	vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	vtkCamera * cam = renderer->GetActiveCamera();
	
	//cam->Azimuth(150);
	cam->Elevation(180);

	renderer->ResetCamera();
	renderWindow->Render();

}

//void CaPViewMainWindow::on_pushButton_FlipImage_clicked()
//{
//  
//}

void CaPViewMainWindow::UpdateCancerHeatmap()
{
     // Declaring and allocating the image
     if(!THIS->m_pProbabilitiesImage)
     {
       DisplayStatusMessage(statusBar(), 
         "Error(1417-159): Probabilities volume image was not found ....", StatusMessageHandler::ERR);
       return;
     }

     // Declaring and allocating the rgb image
     RGBAVolumeType::Pointer itkrgbImage = RGBAVolumeType::New();
     itkrgbImage->CopyInformation(THIS->m_pProbabilitiesImage);
     itkrgbImage->SetRegions(THIS->m_pProbabilitiesImage->GetBufferedRegion());
     itkrgbImage->Allocate();
        

     
     // Filling the image     
     itk::Index<3> start = THIS->m_pProbabilitiesImage->GetBufferedRegion().GetIndex();
     itk::Size<3> size = THIS->m_pProbabilitiesImage->GetBufferedRegion().GetSize();
     
     float heatmapSpan = THIS->m_HeatMapRange - THIS->m_HeatmapMinimum;

     if(heatmapSpan < 0.075) // just wanted to make sure it is less than 0.08
     {
       DisplayStatusMessage(  statusBar()
		                    , "Error(1439-96): Check Probability Ranges ..."
							, StatusMessageHandler::ERR);
       return;
     }

     for(unsigned int i = (unsigned int)start[0]; i < size[0]; i++)
     {
       for(unsigned int j = (unsigned int)start[1]; j < size[1]; j++)
       {
         for(unsigned int k = (unsigned int)start[2]; k < size[2]; k++)
         {
           itk::Index<3> idx = {{i, j, k}};
           RGBAPixelType rgbPixel;
           rgbPixel[3] = (unsigned char)(255); // All pixels are visible by default
           InternalPixelType pixVal = THIS->m_pProbabilitiesImage->GetPixel(idx);
           
		   if(pixVal < 0.00) // -1.0f which means it is outside the prostate
           {
             rgbPixel[0] = (unsigned char)(0);
             rgbPixel[1] = (unsigned char)(0);
             rgbPixel[2] = (unsigned char)(0);
             rgbPixel[3] = (unsigned char)(0); // invisible (transparent)
           }
		   else if(pixVal < THIS->m_HeatmapMinimum) // -1.0f which means it is outside the prostate
           {
             rgbPixel[0] = (unsigned char)(0);
             rgbPixel[1] = (unsigned char)(0);
             rgbPixel[2] = (unsigned char)(131.0);
           }
           else if(pixVal < (THIS->m_HeatmapMinimum + 0.0625 * heatmapSpan))
           {
             rgbPixel[0] = (unsigned char)(0);
             rgbPixel[1] = (unsigned char)(0);
             rgbPixel[2] = (unsigned char)(131.0 + 992.0 * pixVal);
           }
           else if(pixVal < (THIS->m_HeatmapMinimum + 0.1250 * heatmapSpan))
           {
             rgbPixel[0] = (unsigned char)(0);
             rgbPixel[1] = (unsigned char)(0);
             rgbPixel[2] = (unsigned char)(131.0 + 992.0 * pixVal);
           }
           else if(pixVal < (THIS->m_HeatmapMinimum + 0.375 * heatmapSpan))
           {
             rgbPixel[0] = (unsigned char)(0);
             rgbPixel[1] = (unsigned char)(-127.5 + 1020.0 * pixVal);
             rgbPixel[2] = (unsigned char)(255);
           }
           else if(pixVal < (THIS->m_HeatmapMinimum + 0.500 * heatmapSpan))
           {
             rgbPixel[0] = (unsigned char)(-382.5 + 1020.0 * pixVal);
             rgbPixel[1] = (unsigned char)(255);
             rgbPixel[2] = (unsigned char)(637.5 - 1020.0 * pixVal);
           }
           else if(pixVal < (THIS->m_HeatmapMinimum + 0.625 * heatmapSpan))
           {
             rgbPixel[0] = (unsigned char)(-382.5 + 1020.0 * pixVal);
             rgbPixel[1] = (unsigned char)(255);
             rgbPixel[2] = (unsigned char)(637.5 - 1020.0 * pixVal);
           }
           else if(pixVal < (THIS->m_HeatmapMinimum + 0.750 * heatmapSpan))
           {
             rgbPixel[0] = (unsigned char)(255);
             rgbPixel[1] = (unsigned char)(892.5 - 1020.0 * pixVal);
             rgbPixel[2] = (unsigned char)(0);
           }
           else if(pixVal < (THIS->m_HeatmapMinimum + 0.875 * heatmapSpan))
           {
             rgbPixel[0] = (unsigned char)(255);
             rgbPixel[1] = (unsigned char)(892.5 - 1020.0 * pixVal);
             rgbPixel[2] = (unsigned char)(0);
           }
           else
           {
             rgbPixel[0] = (unsigned char)(1144.0 - 1016.0 * pixVal);
             rgbPixel[1] = (unsigned char)(0);
             rgbPixel[2] = (unsigned char)(0);
           }

           itkrgbImage->SetPixel(idx, rgbPixel);
         }
       }
     }

     THIS->m_pHeatMap = itkrgbImage;

     // Render RGB Image T2,  
	 renderVolumeAsSlices<RGBAVolumeType>(THIS->m_pHeatMap, THIS->ui->qVTKWidget_T2, THIS->m_HeatmapPropT2, THIS->m_currentSlice);
	 // Render RGB Image ADC,  
	 renderVolumeAsSlices<RGBAVolumeType>(THIS->m_pHeatMap, THIS->ui->qVTKWidget_ADC, THIS->m_HeatmapPropADC, THIS->m_currentSlice);
}


//////////////////////////////////////////////////////////////////////////

void CaPViewMainWindow::setLevelsetValue(int value)
{
  THIS->m_LevelSetValue = value/100.0f;
  disconnect(THIS->ui->doubleSpinBox_LevelSetValue, SIGNAL(valueChanged(double)), this, SLOT(setLevelsetValue(double)));
  THIS->ui->doubleSpinBox_LevelSetValue->setValue(THIS->m_LevelSetValue);
  connect(THIS->ui->doubleSpinBox_LevelSetValue, SIGNAL(valueChanged(double)), this, SLOT(setLevelsetValue(double)));
}

void CaPViewMainWindow::setLevelsetValue(double value)
{
  THIS->m_LevelSetValue = value;
  disconnect(THIS->ui->horizontalSlider_LevelSetValue, SIGNAL(valueChanged(int)), this, SLOT(setLevelsetValue(int)));
  THIS->ui->horizontalSlider_LevelSetValue->setValue((int)(value * 100.0));
  connect(THIS->ui->horizontalSlider_LevelSetValue, SIGNAL(valueChanged(int)), this, SLOT(setLevelsetValue(int)));
}

void CaPViewMainWindow::on_qxtSpanSlider_Heatmap_lowerValueChanged(int min)
{
  THIS->m_HeatmapMinimum = (min / 100.0f);
  QString minNum;
  THIS->ui->lineEdit_HeatmapLegendMinimum->setText(minNum.setNum(THIS->m_HeatmapMinimum, 'f', 2));
}

void CaPViewMainWindow::on_qxtSpanSlider_Heatmap_upperValueChanged(int range)
{
  THIS->m_HeatMapRange = (range / 100.0f);
  QString rangeNum;
  THIS->ui->lineEdit_HeatmapLegendRange->setText(rangeNum.setNum(THIS->m_HeatMapRange, 'f', 2));
}

void CaPViewMainWindow::clearViewer(QVTKWidget* pQVTKWidget)
{
  if (!pQVTKWidget) 
  {
    DisplayStatusMessage(  statusBar()
		                 , "Error(944-48): Invalid QVTKWidget pointer ..."
						 , StatusMessageHandler::ERR);
    return;
  }

  // extract the render window
  const vtkRenderWindowPointer renderWindow = pQVTKWidget->GetRenderWindow();
  if (!renderWindow) 
  {
    DisplayStatusMessage(  statusBar()
		                 , "Error(1015-48): Invalid Render Window ..."
						 , StatusMessageHandler::ERR);
    return;
  }

  // clear the render window
  const vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();
  if (!renderer) 
  {
    DisplayStatusMessage(  statusBar()
		                 , "Error(1035-408): Invalid Renderer ..."
						 , StatusMessageHandler::ERR);
    return;
  }
  renderer->RemoveAllViewProps();

  renderWindow->Render();
  renderWindow->Render();
}

void CaPViewMainWindow::on_doubleSpinBox_LevelSetValue_valueChanged(double value)
{
  THIS->m_LevelSetValue = value;
}

void CaPViewMainWindow::initControlsValues(VisualObject whichObject)
{
  switch(whichObject)
  {
    case T2_SLICES:
      UpdateSliceSlider(THIS->m_pVolumeT2, THIS->ui->horizontalSlider_currentSlice, THIS->ui->spinBox_currentSlice);
      UpdateWindowLevelSliders(THIS->m_pVolumeT2, THIS->ui->horizontalSlider_WindowWidth_T2, THIS->ui->horizontalSlider_WindowLevel_T2, THIS->ui->doubleSpinBox_WindowWidth_T2, THIS->ui->doubleSpinBox_WindowLevel_T2);
      THIS->ui->qxtCheckComboBox_Main->setItemCheckState(T2_SLICES, Qt::PartiallyChecked );
      break;

    case ADC_SLICES:
      UpdateSliceSlider(THIS->m_pVolumeADC, THIS->ui->horizontalSlider_currentSlice, THIS->ui->spinBox_currentSlice);
      UpdateWindowLevelSliders(THIS->m_pVolumeADC, THIS->ui->horizontalSlider_WindowWidth_ADC, THIS->ui->horizontalSlider_WindowLevel_ADC, THIS->ui->doubleSpinBox_WindowWidth_ADC, THIS->ui->doubleSpinBox_WindowLevel_ADC);
      THIS->ui->qxtCheckComboBox_Main->setItemCheckState(ADC_SLICES, Qt::PartiallyChecked );
      break;

    //case PROSTATE_SLICES:
    //  UpdateSliceSlider(THIS->m_pProstateMask, THIS->ui->horizontalSlider_currentSlice, THIS->ui->spinBox_currentSlice);
    //  THIS->ui->qxtCheckComboBox_Main->setItemCheckState(PROSTATE_SLICES, Qt::PartiallyChecked );
    //  break;

    case PROSTATE_SURFACE:
      THIS->ui->qxtCheckComboBox_Main->setItemCheckState(PROSTATE_SURFACE, Qt::PartiallyChecked );
      break;

    case CAP_SLICES:
      UpdateSliceSlider(THIS->m_pProbabilitiesImage, THIS->ui->horizontalSlider_currentSlice, THIS->ui->spinBox_currentSlice);
      THIS->ui->qxtCheckComboBox_Main->setItemCheckState(CAP_SLICES, Qt::Checked );
      break;

    case CAP_SURFACE:
      THIS->ui->qxtCheckComboBox_Main->setItemCheckState(CAP_SURFACE, Qt::PartiallyChecked );
      break;

    default:
     break;

  }
}

void CaPViewMainWindow::resetControlsValues()
{
  THIS->ui->horizontalSlider_WindowWidth_T2->setValue(50);
  THIS->ui->horizontalSlider_WindowLevel_T2->setValue(50);
  THIS->ui->horizontalSlider_VolumeOpacity_T2->setValue(100);
}

void CaPViewMainWindow::UpdateActionsEnabledState()
{
  // If only one object is loaded, then its corresponding action will be left true (all the others will be true because none of them is loaded yet)
  // If more than one object is loaded, only the remaining objects (which were not loaded) will be left enabled true

    THIS->ui->action_Load_T2->setEnabled(true);
    THIS->ui->action_Load_ADC->setEnabled(true);

    THIS->ui->action_Load_Mask_Prostate->setEnabled(true);
    THIS->ui->action_Load_Probabilities->setEnabled(true);
    
    //THIS->ui->action_SetProstateSurfaceColor->setEnabled(false);
    //THIS->ui->action_SetCancerSurfaceColor->setEnabled(false);

    if(THIS->m_pProstateMask && THIS->m_pVolumeT2 && THIS->m_pProbabilitiesImage)
    {
        THIS->ui->action_Load_T2->setEnabled(false);
        THIS->ui->action_Load_Mask_Prostate->setEnabled(false);
        THIS->ui->action_Load_Probabilities->setEnabled(false);
        
    }
    else if (THIS->m_pProstateMask && THIS->m_pVolumeT2 && !THIS->m_pProbabilitiesImage)
    {
        THIS->ui->action_Load_T2->setEnabled(false);
        THIS->ui->action_Load_Mask_Prostate->setEnabled(false);
    }
    else if (THIS->m_pProstateMask && !THIS->m_pVolumeT2 && THIS->m_pProbabilitiesImage)
    {
        THIS->ui->action_Load_Mask_Prostate->setEnabled(false);
        THIS->ui->action_Load_Probabilities->setEnabled(false);
    }
    else if (!THIS->m_pProstateMask && THIS->m_pVolumeT2 && THIS->m_pProbabilitiesImage)
    {
        THIS->ui->action_Load_T2->setEnabled(false);
        THIS->ui->action_Load_Probabilities->setEnabled(false);
    }
}

void CaPViewMainWindow::on_actionCaPView_Help_triggered()
{
  QDesktopServices::openUrl(QCoreApplication::applicationDirPath() + "/help/help.chm");
}

void CaPViewMainWindow::on_action_SaveT2ADCImage_triggered()
{
  QString imagePath = QFileDialog::getSaveFileName(this, "Save T2 as ...", 
                             THIS->m_lastImagePath, "PNG(*.png);; TIFF(*.tiff *.tif);; JPEG(*.jpg *.jpeg)");
  if (imagePath.isEmpty())
  {
    // write a warning on the status bar
    DisplayStatusMessage(  statusBar()
                         , "Warning: Empty or invalid File Path ..."
                         , StatusMessageHandler::WARNING
                        );
    return;
  }

  THIS->m_lastImagePath = imagePath;

  vtkWindowToImageFilter * windowToImage = vtkWindowToImageFilter::New();

  ///Capture Window
  windowToImage->SetInput(THIS->ui->qVTKWidget_T2->GetRenderWindow());
  windowToImage->Update();

  ///Write/Save as PNG
  vtkPNGWriter *writer = vtkPNGWriter::New();
  writer->SetInputConnection(windowToImage->GetOutputPort()); //Cant group outside if's GCC compiler error
  writer->SetFileName(imagePath.toStdString().c_str());
  writer->Write();

  writer->Delete();
  windowToImage->Delete();
}

void CaPViewMainWindow::on_action_SaveAppSnapshot_triggered()
{
  QString lastAppSnapshotPath = QFileDialog::getSaveFileName(this, "Save as ...", 
                             THIS->m_lastAppSnapshotPath, "PNG(*.png);; TIFF(*.tiff *.tif);; JPEG(*.jpg *.jpeg)");
  if (lastAppSnapshotPath.isEmpty())
  {
    // write a warning on the status bar
    DisplayStatusMessage(  statusBar()
                         , "Warning: Empty or invalid File Path ..."
                         , StatusMessageHandler::WARNING
                        );
    return;
  }

  THIS->m_lastAppSnapshotPath = lastAppSnapshotPath;

  if (!QPixmap::grabWidget(this).save(lastAppSnapshotPath))
  {
    DisplayStatusMessage(  statusBar()
		                 , "Error(1188-5): Snapshot was not saved ..."
						 , StatusMessageHandler::ERR);
  }
}

void CaPViewMainWindow::on_actionAbout_CaPView_triggered()
{
    QMessageBox::about( this, tr( "About CaPView" ),
    tr( "CaPView is a C++ toolkit for 3D visualization and manipulation of Prostate Cancer.\n\
         \n\
         This is a trial version which is distributed AS IS.\n\
         Copyright (c) CCIPD and/or its subsidiary(-ies) and other contributors.\n\
         This program uses the toolkits: ITK, VTK, Qt, Boost, Eigen.\n\
         Author: Ahmad Algohary" ) );
}

  void CaPViewMainWindow::on_actionAbout_CCIPD_triggered()
  {
          QMessageBox::about( this, tr( "About CCIPD" ),
      tr( "The Center of Computational Imaging and Personalized Diagnostics at Case Western Reserve University is involved in various different aspects of developing, evaluating and applying novel quantitative image analysis, computer vision, signal processing, segmentation, multi-modal co-registration tools, pattern recognition, and machine learning tools for disease diagnosis, prognosis, and theragnosis in the context of breast, prostate, head and neck, and brain tumors as also epilepsy and carotid plaque. Our group is also exploring the utility of these methods in studying correlations of disease markers across multiple scales, modalities, and functionalities -- from gene and protein expression to spectroscopy to digital pathology and to multi-parametric MRI.\n\
          \n\
          \n\
          For more information, please visit ccipd.case.edu" ) );
  }

  void CaPViewMainWindow::on_action_resetParameters_triggered()
  {
    THIS->ui->spinBox_currentSlice->setValue( THIS->ui->spinBox_currentSlice->maximum() / 2    );

    THIS->ui->doubleSpinBox_WindowWidth_T2->setValue( THIS->ui->doubleSpinBox_WindowWidth_T2->maximum() / 2    );
    THIS->ui->doubleSpinBox_WindowLevel_T2->setValue( THIS->ui->doubleSpinBox_WindowLevel_T2->maximum() / 2    );
    THIS->ui->spinBox_VolumeOpacity_T2->setValue( THIS->ui->spinBox_VolumeOpacity_T2->maximum() / 2    );
    initControlsValues(T2_SLICES);

    THIS->ui->doubleSpinBox_WindowWidth_ADC->setValue( THIS->ui->doubleSpinBox_WindowWidth_ADC->maximum() / 2    );
    THIS->ui->doubleSpinBox_WindowLevel_ADC->setValue( THIS->ui->doubleSpinBox_WindowLevel_ADC->maximum() / 2    );
    THIS->ui->spinBox_VolumeOpacity_ADC->setValue( THIS->ui->spinBox_VolumeOpacity_ADC->maximum() / 2    );
    initControlsValues(ADC_SLICES);
    

    THIS->ui->spinBox_ProstateMaskOpacity->setValue( THIS->ui->spinBox_ProstateMaskOpacity->maximum() / 2    );
    THIS->ui->spinBox_ProstateMaskSurface_Opacity->setValue( THIS->ui->spinBox_ProstateMaskSurface_Opacity->maximum() / 2    );
    

    THIS->ui->spinBox_CancerSurfaceOpacity->setValue( THIS->ui->spinBox_CancerSurfaceOpacity->maximum() / 2    );
    THIS->ui->doubleSpinBox_LevelSetValue->setValue(  THIS->ui->doubleSpinBox_LevelSetValue->maximum() / 2    );
  
  }

  void CaPViewMainWindow::on_actionOpen_DCE_Window_triggered()
  {
	  if(THIS->m_DCEFeaturesFilePaths.size() < 1)
	  {
      DisplayStatusMessage(  statusBar()
                           , "Warning: No DCE Features Loaded ..."
                           , StatusMessageHandler::WARNING
                          );
		  return;
	  }

    ccipd::CaPViewDCEWindow * dceGUI = new ccipd::CaPViewDCEWindow(THIS->m_DCEFeaturesFilePaths, THIS->ui->horizontalSlider_currentSlice->maximum());
    dceGUI->showNormal();
  }


QString CaPViewMainWindow::CheckFileExistence(const QString & filePath, const QStringList & extensions)
{
	QFile file(filePath);
	unsigned int numberOfExtensions = extensions.size();
	for(unsigned int i = 0; i < numberOfExtensions; i++)
	{
		QString path = filePath + "." + extensions[i];
		if(QFile(path).exists())	
			return path;
	}

	return QString();
}

void CaPViewMainWindow::on_action_LoadDatasetDir_triggered()
{
  // Let the user select a directory to load the volume from 
  QString dirPath = QFileDialog::getExistingDirectory(this, 
                 tr( "Please, select the dataset directory ...."), THIS->m_lastDatasetDir);
  
  // Check the returned directory path
  if (dirPath.isEmpty())
  {
    // write a warning on the status bar
    DisplayStatusMessage(  statusBar()
		                 , "Warning(626-417): Empty or invalid directory ...."
						 , StatusMessageHandler::WARNING);
    return;
  }
  
  THIS->m_lastDatasetDir = dirPath;
  
  // Show the path on the title bar
  this->setWindowTitle(QCoreApplication::applicationName() + THIS->m_lastDatasetDir);
  
  // Load Prostate MAsk
  THIS->m_lastProstateMaskPath = CaPViewMainWindow::CheckFileExistence(THIS->m_lastDatasetDir + "/PMask", THIS->m_supportedImageFileExtensions);
  LoadImage_PMask();

  // Load CaP probabilities Image
  THIS->m_lastprobabilitiesImagePath = CaPViewMainWindow::CheckFileExistence(THIS->m_lastDatasetDir + "/CaP", THIS->m_supportedImageFileExtensions);
  LoadImage_CaP();

  // Prepare the DCE Features file paths, but they will not be loaded here
  THIS->m_DCEFeaturesFilePaths.push_back(THIS->m_lastDatasetDir + "/DCE_MaxUptake.mha"       );
  THIS->m_DCEFeaturesFilePaths.push_back(THIS->m_lastDatasetDir + "/DCE_TimeToPeak.mha"      );
  THIS->m_DCEFeaturesFilePaths.push_back(THIS->m_lastDatasetDir + "/DCE_RateOfUptake.mha"    );
  THIS->m_DCEFeaturesFilePaths.push_back(THIS->m_lastDatasetDir + "/DCE_RateOfWashout.mha"   );
  THIS->m_DCEFeaturesFilePaths.push_back(THIS->m_lastDatasetDir + "/DCE_Enhancement.mha"     );
  THIS->m_DCEFeaturesFilePaths.push_back(THIS->m_lastDatasetDir + "/DCE_EnhancementRatio.mha");

  // Load T2
  THIS->m_lastVolumePathT2 = CaPViewMainWindow::CheckFileExistence(THIS->m_lastDatasetDir + "/T2", THIS->m_supportedImageFileExtensions);
  LoadImage_T2();

  // Load ADC
  THIS->m_lastVolumePathADC = CaPViewMainWindow::CheckFileExistence(THIS->m_lastDatasetDir + "/ADC", THIS->m_supportedImageFileExtensions);
  LoadImage_ADC();

}

void CaPViewMainWindow::slot_mouseWheelForwardEvent(vtkObject*, unsigned long, void*, void*)
{
  if(THIS->m_currentSlice == (unsigned int)(THIS->ui->horizontalSlider_currentSlice->maximum() )) return;
  THIS->m_currentSlice++;
  THIS->ui->horizontalSlider_currentSlice->setValue(THIS->m_currentSlice);
  cout << "+\n";
}

void CaPViewMainWindow::slot_mouseWheelBackwardEvent(vtkObject*, unsigned long, void*, void*)
{
  if (0 == THIS->m_currentSlice) return;
  THIS->m_currentSlice--;
  THIS->ui->horizontalSlider_currentSlice->setValue(THIS->m_currentSlice);
  cout << "-\n";
}


void CaPViewMainWindow::wheelEvent(QWheelEvent* /*event*/)
{
  //THIS->ui->horizontalSlider_currentSlice->setValue(THIS->m_currentSlice++);
  //cout << "-\n";
  //on_horizontalSlider_currentSlice_valueChanged(THIS->m_currentSlice++);
}

void CaPViewMainWindow::on_action_Show_Windowing_Controls_toggled( bool checked )
{
    THIS->ui->label_WindowWidth_T2->setVisible(checked);
    THIS->ui->horizontalSlider_WindowWidth_T2->setVisible(checked);
    THIS->ui->doubleSpinBox_WindowWidth_T2->setVisible(checked);
    THIS->ui->label_WindowLevel_T2->setVisible(checked);
    THIS->ui->horizontalSlider_WindowLevel_T2->setVisible(checked);
    THIS->ui->doubleSpinBox_WindowLevel_T2->setVisible(checked);
        
    THIS->ui->label_WindowWidth_ADC->setVisible(checked);
    THIS->ui->horizontalSlider_WindowWidth_ADC->setVisible(checked);
    THIS->ui->doubleSpinBox_WindowWidth_ADC->setVisible(checked);
    THIS->ui->label_WindowLevel_ADC->setVisible(checked);
    THIS->ui->horizontalSlider_WindowLevel_ADC->setVisible(checked);
    THIS->ui->doubleSpinBox_WindowLevel_ADC->setVisible(checked);
}

void CaPViewMainWindow::on_qxtCheckComboBox_Main_checkedItemsChanged( const QStringList & /*items*/ )
{
  // T2_SLICES
  if(THIS->ui->qxtCheckComboBox_Main->itemCheckState (T2_SLICES) != Qt::PartiallyChecked)
  {
    if(THIS->m_ImagePropT2) 
      THIS->m_ImagePropT2->SetVisibility(THIS->ui->qxtCheckComboBox_Main->itemCheckState (T2_SLICES));
    
    THIS->ui->label_VolumeOpacity_T2->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (T2_SLICES));
    THIS->ui->horizontalSlider_VolumeOpacity_T2->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (T2_SLICES));
    THIS->ui->spinBox_VolumeOpacity_T2->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (T2_SLICES));
  }

  // ADC_SLICES
  if(THIS->ui->qxtCheckComboBox_Main->itemCheckState (ADC_SLICES) != Qt::PartiallyChecked)
  {
    if(THIS->m_ImagePropADC) 
      THIS->m_ImagePropADC->SetVisibility(THIS->ui->qxtCheckComboBox_Main->itemCheckState (ADC_SLICES));
    
    THIS->ui->label_VolumeOpacity_ADC->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (ADC_SLICES));
    THIS->ui->horizontalSlider_VolumeOpacity_ADC->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (ADC_SLICES));
    THIS->ui->spinBox_VolumeOpacity_ADC->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (ADC_SLICES));
  }

  //// Prostate Mask Slices
  //if(THIS->ui->qxtCheckComboBox_Main->itemCheckState (PROSTATE_SLICES) != Qt::PartiallyChecked)
  //{
  //  if(THIS->m_ProstateMaskProp_T2) 
  //    THIS->m_ProstateMaskProp_T2->SetVisibility(THIS->ui->qxtCheckComboBox_Main->itemCheckState(PROSTATE_SLICES));
  //  if(THIS->m_ProstateMaskProp_ADC) 
  //    THIS->m_ProstateMaskProp_ADC->SetVisibility(THIS->ui->qxtCheckComboBox_Main->itemCheckState(PROSTATE_SLICES));
  //  
  //  THIS->ui->label_ProstateMaskSlice_Opacity->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState(PROSTATE_SLICES));
  //  THIS->ui->horizontalSlider_ProstateMaskOpacity->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState(PROSTATE_SLICES));
  //  THIS->ui->spinBox_ProstateMaskOpacity->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState(PROSTATE_SLICES));
  //}

  // PROSTATE_SURFACE
  if(THIS->ui->qxtCheckComboBox_Main->itemCheckState(PROSTATE_SURFACE) != Qt::PartiallyChecked)
  {
    if((!THIS->m_pProstateLevelsetSurfaceActor_T2 || !THIS->m_pProstateLevelsetSurfaceActor_ADC) && (THIS->ui->qxtCheckComboBox_Main->itemCheckState(PROSTATE_SURFACE) == Qt::Checked)) 
    {
      THIS->m_pProstateLevelsetSurfaceActor_T2  = renderVolumeAsSurface<VolumeImageType>(THIS->m_pProstateMask, THIS->ui->qVTKWidget_T2, true, 0.0, 0.5);
      THIS->m_pProstateLevelsetSurfaceActor_ADC = renderVolumeAsSurface<VolumeImageType>(THIS->m_pProstateMask, THIS->ui->qVTKWidget_ADC, true, 0.0, 0.5);
      initControlsValues(PROSTATE_SURFACE);
    }
    else
    {
      if(THIS->m_pProstateLevelsetSurfaceActor_T2)
        THIS->m_pProstateLevelsetSurfaceActor_T2->SetVisibility(THIS->ui->qxtCheckComboBox_Main->itemCheckState (PROSTATE_SURFACE));
      if(THIS->m_pProstateLevelsetSurfaceActor_ADC)
        THIS->m_pProstateLevelsetSurfaceActor_ADC->SetVisibility(THIS->ui->qxtCheckComboBox_Main->itemCheckState (PROSTATE_SURFACE));
      
      THIS->ui->label_ProstateMaskSurface_Opacity->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (PROSTATE_SURFACE));
      THIS->ui->horizontalSlider_ProstateMaskSurface_Opacity->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (PROSTATE_SURFACE));
      THIS->ui->spinBox_ProstateMaskSurface_Opacity->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (PROSTATE_SURFACE));

    }
  }

  // CAP_SLICES
  if(THIS->ui->qxtCheckComboBox_Main->itemCheckState(CAP_SLICES) != Qt::PartiallyChecked)
  {
    if(THIS->m_HeatmapPropT2)
      THIS->m_HeatmapPropT2->SetVisibility(THIS->ui->qxtCheckComboBox_Main->itemCheckState(CAP_SLICES));
    if(THIS->m_HeatmapPropADC)
      THIS->m_HeatmapPropADC->SetVisibility(THIS->ui->qxtCheckComboBox_Main->itemCheckState(CAP_SLICES));

      THIS->ui->label_OpacityHeatMapOverT2->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SLICES));
      THIS->ui->horizontalSlider_OpacityHeatMapOverT2->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SLICES));
      THIS->ui->spinBox_OpacityHeatMapOverT2->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SLICES));
      THIS->ui->label_OpacityHeatMapOverADC->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SLICES));
      THIS->ui->horizontalSlider_OpacityHeatMapOverADC->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SLICES));
      THIS->ui->spinBox_OpacityHeatMapOverADC->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SLICES));
  }

  // CAP_SURFACE
  if(THIS->ui->qxtCheckComboBox_Main->itemCheckState(CAP_SURFACE) != Qt::PartiallyChecked)
  {
    if((!THIS->m_pCancerLevelsetSurfaceActor_overT2 || !THIS->m_pCancerLevelsetSurfaceActor_overADC) && (THIS->ui->qxtCheckComboBox_Main->itemCheckState(CAP_SURFACE) == Qt::Checked)) 
    {
      THIS->m_pCancerLevelsetSurfaceActor_overT2  = renderVolumeAsSurface<VolumeImageType>(THIS->m_pProbabilitiesImage, THIS->ui->qVTKWidget_T2 , false, (const float)THIS->m_LevelSetValue, 0.9, 6.0, false, true, 0.95, 0.20, 0.20);
      THIS->m_pCancerLevelsetSurfaceActor_overADC = renderVolumeAsSurface<VolumeImageType>(THIS->m_pProbabilitiesImage, THIS->ui->qVTKWidget_ADC, false, (const float)THIS->m_LevelSetValue, 0.9, 6.0, false, true, 0.95, 0.20, 0.20);
      initControlsValues(CAP_SURFACE);
    }
    else
    {
      if(THIS->m_pCancerLevelsetSurfaceActor_overT2) 
        THIS->m_pCancerLevelsetSurfaceActor_overT2->SetVisibility(THIS->ui->qxtCheckComboBox_Main->itemCheckState(CAP_SURFACE));
      if(THIS->m_pCancerLevelsetSurfaceActor_overADC) 
        THIS->m_pCancerLevelsetSurfaceActor_overADC->SetVisibility(THIS->ui->qxtCheckComboBox_Main->itemCheckState(CAP_SURFACE));

      THIS->ui->label_CancerSurfaceOpacity->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SURFACE));
      THIS->ui->horizontalSlider_CancerSurfaceOpacity->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SURFACE));
      THIS->ui->spinBox_CancerSurfaceOpacity->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SURFACE));
      THIS->ui->label_LevelSet->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SURFACE));
      THIS->ui->horizontalSlider_LevelSetValue->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SURFACE));
      THIS->ui->doubleSpinBox_LevelSetValue->setVisible(THIS->ui->qxtCheckComboBox_Main->itemCheckState (CAP_SURFACE));

      THIS->ui->action_SetCancerSurfaceColor->setEnabled(THIS->ui->qxtCheckComboBox_Main->itemCheckState(CAP_SURFACE));
    }
  }

///////////////////////Render
  THIS->ui->qVTKWidget_ADC->GetRenderWindow()->Render();
  THIS->ui->qVTKWidget_T2->GetRenderWindow()->Render();

}




} // namespace ccipd

