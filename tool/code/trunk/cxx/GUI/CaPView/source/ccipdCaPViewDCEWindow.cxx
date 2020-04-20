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
#include "ui_ccipdCaPViewDCEWindow.h"
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



//////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{
//////////////////////////////////////////////////////////////////////////////////////////
/// PIMPL Interface to CaPViewDCEWindow
class CaPViewDCEWindow::SecretData : public QThread, boost::noncopyable
{
  
public:
  SecretData(); ///< Constructor
  

  QSettings m_settings;
  bool      m_bVerbose;
  QString   m_lastVolumePathDCE;

  QString m_lastHeatmapPath;
  QString m_lastprobabilitiesImagePath;
  QString m_lastImagePath;
  QString m_lastAppSnapshotPath;
//////////////////////////////////////////////////////////////////////////////////////////

  VolumeImagePointer m_pVolumeDCE    ;

  RGBAVolumePointer  m_pHeatMap      ;

  RGBAVolumeContainer m_vecColoredFeatures_DCE;
  VolumeImagesContainer m_vecGrayscaleFeatures_DCE;


  unsigned int m_currentFeatureIndex;

  /**
   * We only apply this when saving the output
   * because VTK doesn't render this correctly.
   */
   VolumeImageBase::DirectionType m_OriginalDirection;
 
   // For Volumes as Slices
  vtkPropPointer   m_ImagePropDCE
                 , m_DCEFeatureHeatmapProp
                 ;


//////////////////////////////////////////////////////////////////////////
// Classification
  
  double           m_LevelSetValue;
  float            m_CancerOpacity;
/////////////////////////////////////////////////////////
  float            m_HeatMapMax;   // This is the min probability (which will bear the red color) in the heat map, its default value is 1
  float            m_HeatmapMin;   // This is the max probability which will be colored blue in the heat map, all values less than this value will all be colored blue too.
  unsigned int     m_currentSlice; // Number of slice (zero-based) shown in the viewer

  // Designer form
  Ui_CaPViewDCEWindow * ui;

  vtkQtConnectorPointer m_pVtkQtConnector;
}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
CaPViewDCEWindow::SecretData::SecretData()
{
    // Initialization for all secret data objects
	  m_bVerbose = true;
	  // Initialize members to default values
    m_lastVolumePathDCE          = ""  ;
    m_lastImagePath              = ""  ;
    m_lastAppSnapshotPath        = ""  ;
    m_pVolumeDCE                 = NULL;
    m_currentSlice               = 0   ;
    m_currentFeatureIndex        = 0   ;
    m_HeatMapMax                 = 1.0 ;
    m_HeatmapMin                 = 0.0 ;

    m_pVtkQtConnector = vtkQtConnectorPointer::New();
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
CaPViewDCEWindow::CaPViewDCEWindow(QStringList DCEFeaturesFilePaths, unsigned int nSlices) : ccipdNewPIMPLMacro
{
  THIS->m_currentSlice = nSlices;

  // Initialization for ONLY GUI objects
  initGUI();
  RetrieveSettings();

  LoadFeatures_DCE(DCEFeaturesFilePaths);

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////
void CaPViewDCEWindow::initGUI()
{
    THIS->ui = new Ui_CaPViewDCEWindow;
    THIS->ui->setupUi( this );


    THIS->ui->horizontalSlider_currentSlice->setMaximum(THIS->m_currentSlice);
    THIS->ui->spinBox_currentSlice->setMaximum(THIS->m_currentSlice);
    THIS->m_currentSlice = 0;


    connect(THIS->ui->qxtSpanSlider_Heatmap , SIGNAL(sliderReleased()), this, SLOT(renderCancerHeatmap()));

  //Load Settings

//////////////////////////////////////////////
    if(!initializeQVTKWidget(THIS->ui->qVTKWidget_DCE))
    {
      DisplayStatusMessage(  statusBar()
                           , "Error in initializing DCE 3D viewer"
                           , StatusMessageHandler::/*MessageType::*/ERR
                          );
    }
    else
    {
       THIS->m_pVtkQtConnector->Connect(THIS->ui->qVTKWidget_DCE->GetRenderWindow()->GetInteractor(),
       vtkCommand::MouseWheelForwardEvent, this, 
       SLOT(slot_mouseWheelForwardEvent(vtkObject*, unsigned long, void*, void*)));
      
       THIS->m_pVtkQtConnector->Connect(THIS->ui->qVTKWidget_DCE->GetRenderWindow()->GetInteractor(),
       vtkCommand::MouseWheelBackwardEvent, this, 
       SLOT(slot_mouseWheelBackwardEvent(vtkObject*, unsigned long, void*, void*)));
    }



///////////////////////////////////////////////

////////////////////////////////////////////////////////////
  clearViewer(THIS->ui->qVTKWidget_DCE);

  THIS->m_ImagePropDCE                             = NULL;

  THIS->m_DCEFeatureHeatmapProp                    = NULL;

  THIS->ui->action_Load_DCE->setEnabled(true);

}

//////////////////////////////////////////////////////////////////////////////////////////
CaPViewDCEWindow::~CaPViewDCEWindow()
{
	SaveSettings();
}
//////////////////////////////////////////////////////////////////////////////////////////
bool CaPViewDCEWindow::RetrieveSettings()
{
	THIS->m_lastVolumePathDCE = THIS->m_settings.value("Volume/DCEpath", "").toString();
  THIS->m_lastImagePath = THIS->m_settings.value("SnapshotImage/path", "").toString();
  THIS->m_lastAppSnapshotPath = THIS->m_settings.value("AppSnapshotImage/path", "").toString();

  // Restore GUI state
  restoreGeometry(THIS->m_settings.value("DCEWindow/Geometry").toByteArray());
  restoreState(THIS->m_settings.value("DCEWindow/WindowState").toByteArray());

	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
bool CaPViewDCEWindow::SaveSettings()
{
  // Save user settings

  THIS->m_settings.setValue("Volume/DCEpath"       , THIS->m_lastVolumePathDCE         );
	THIS->m_settings.setValue("SnapshotImage/path"   , THIS->m_lastImagePath             );
  THIS->m_settings.setValue("AppSnapshotImage/path", THIS->m_lastAppSnapshotPath       );

  // Save gui state settings
  THIS->m_settings.setValue("DCEWindow/Geometry"   , saveGeometry());
  THIS->m_settings.setValue("DCEWindow/WindowState", saveState   ());
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

 void CaPViewDCEWindow::ResetApp()
 {
   clearViewer(THIS->ui->qVTKWidget_DCE  );

   THIS->m_pVolumeDCE     = NULL;
                          
   THIS->m_pHeatMap       = NULL;
   
   THIS->m_LevelSetValue  = 0.8;
   THIS->m_CancerOpacity  = 0.5;
   THIS->m_HeatMapMax     = 1.0;
   THIS->m_HeatmapMin     = 0.0; 
   THIS->m_currentSlice   = 0  ;

  THIS->m_ImagePropDCE          = NULL;
  THIS->m_DCEFeatureHeatmapProp = NULL;

   // Reset GUI
   THIS->ui->action_Load_DCE->setEnabled(true);

 }

  void CaPViewDCEWindow::on_horizontalSlider_VolumeOpacity_DCE_valueChanged(int Opacity)
  {
    UpdateVTKPropTransparency(THIS->m_ImagePropDCE, (unsigned int)Opacity);
    THIS->ui->qVTKWidget_DCE->GetRenderWindow()->Render();
  }

/////////////////////////////////////////////////////////////////////////////////////////
  void CaPViewDCEWindow::on_horizontalSlider_currentSlice_valueChanged(int sliceNum)
  {
    THIS->m_currentSlice = sliceNum - 1;
	  if(THIS->m_ImagePropDCE) UpdateVTKImageSlice(THIS->m_ImagePropDCE, sliceNum - 1, THIS->m_bVerbose);
	  if(THIS->m_DCEFeatureHeatmapProp) UpdateVTKImageSlice(THIS->m_DCEFeatureHeatmapProp, sliceNum - 1, THIS->m_bVerbose);
	  THIS->ui->qVTKWidget_DCE->GetRenderWindow()->Render();
  }

void CaPViewDCEWindow::LoadFeatures_DCE(const QStringList & DCEFeaturesFilePaths)
{
  for(int i = 0; i < DCEFeaturesFilePaths.size(); i++)
	{
      THIS->m_vecGrayscaleFeatures_DCE.push_back(LoadITKImage<VolumeImageType>(DCEFeaturesFilePaths[i].toStdString()));
	    THIS->m_vecColoredFeatures_DCE.push_back(GetAsColormap(THIS->m_vecGrayscaleFeatures_DCE[i]));
	}

  // Adjust Heatmap Range
  UpdateHeatmapSpan();
	// Display the first feature in the list
  renderCancerHeatmap();

  DisplayStatusMessage(statusBar(), "Loading done ....", StatusMessageHandler/*::MessageType*/::SUCCESS);

}

void CaPViewDCEWindow::on_comboBox_Features_DCE_currentIndexChanged(int index)
{
  if(THIS->m_vecColoredFeatures_DCE.size() > index )
     THIS->m_currentFeatureIndex = index;

  // Adjust Heatmap Range
  UpdateHeatmapSpan();
  renderCancerHeatmap();
}

void CaPViewDCEWindow::UpdateHeatmapSpan()
{
// Get Histogram
   HistogramDPointer pHistogram = Get1DHistogram(THIS->m_vecGrayscaleFeatures_DCE[THIS->m_currentFeatureIndex], 101);
   vector<itk::SizeValueType> vec;
   for(unsigned int i = 0; i < pHistogram->GetSize()[0]; ++i)
     vec.push_back(pHistogram->GetFrequency(i));

   int indexOfMax = (int)(std::distance(vec.begin(), std::max_element(vec.begin(), vec.end())));
   
   THIS->ui->qxtSpanSlider_Heatmap->setSpan(0, 100);
   THIS->ui->qxtSpanSlider_Heatmap->setUpperPosition(indexOfMax <= 90 ? indexOfMax + 10 : 100);
   THIS->ui->qxtSpanSlider_Heatmap->setLowerPosition(indexOfMax >= 10 ? indexOfMax - 10 : 000);

}

void CaPViewDCEWindow::renderCancerHeatmap()
{
    THIS->m_vecColoredFeatures_DCE[THIS->m_currentFeatureIndex] = GetAsColormap(THIS->m_vecGrayscaleFeatures_DCE[THIS->m_currentFeatureIndex], THIS->m_HeatmapMin, THIS->m_HeatMapMax);
	  // Display the first feature in the list
    renderVolumeAsSlices<RGBAVolumeType>(THIS->m_vecColoredFeatures_DCE[THIS->m_currentFeatureIndex], THIS->ui->qVTKWidget_DCE, THIS->m_DCEFeatureHeatmapProp, THIS->m_currentSlice);
}

RGBAVolumePointer CaPViewDCEWindow::GetAsColormap(VolumeImagePointer pProbabilitiesImage, InternalPixelType minPercentage /*= 0.0*/, InternalPixelType maxPercentage /*= 1.0*/)
{
     // Security Checks
     if(!pProbabilitiesImage)
     {
       DisplayStatusMessage(statusBar(), "Error(1417-159): Probabilities volume image was not found ....", StatusMessageHandler/*::MessageType*/::ERR);
       return RGBAVolumePointer(NULL);
     }

     if(maxPercentage <= minPercentage)
     {
       DisplayStatusMessage(statusBar(), "Error(1417-161): Min/Max wrong values ....\n Min/Max will be reset to default values ...\n", StatusMessageHandler/*::MessageType*/::ERR);
       minPercentage = 0.0;
       maxPercentage = 1.0;
     }


     // Declaring and allocating the rgb image
     RGBAVolumeType::Pointer itkrgbImage = RGBAVolumeType::New();
     itkrgbImage->CopyInformation(pProbabilitiesImage);
     itkrgbImage->SetRegions(pProbabilitiesImage->GetBufferedRegion());
     itkrgbImage->Allocate();
       
     
     // Filling the image     
     itk::Index<3> start = pProbabilitiesImage->GetBufferedRegion().GetIndex();
     itk::Size<3> size = pProbabilitiesImage->GetBufferedRegion().GetSize();
     

     InternalPixelType inputMin   = GetMin(pProbabilitiesImage);
     InternalPixelType inputMax   = GetMax(pProbabilitiesImage);
     InternalPixelType inputRange = inputMax - inputMin;


	 InternalPixelType min   = inputMin + minPercentage * inputRange;
	 InternalPixelType max   = inputMin + maxPercentage * inputRange;
	 InternalPixelType range = max - min;


     for(unsigned int i = (unsigned int)start[0]; i < size[0]; i++)
     {
       for(unsigned int j = (unsigned int)start[1]; j < size[1]; j++)
       {
         for(unsigned int k = (unsigned int)start[2]; k < size[2]; k++)
         {
           itk::Index<3> idx = {{i, j, k}};
           RGBAPixelType rgbPixel;
           rgbPixel[3] = (unsigned char)(255); // All pixels are visible by default
           InternalPixelType pixVal = pProbabilitiesImage->GetPixel(idx);
           
		   if(pixVal < min) 
           {
             rgbPixel[0] = (unsigned char)(0)    ;
             rgbPixel[1] = (unsigned char)(0)    ;
             rgbPixel[2] = (unsigned char)(0)    ;
             rgbPixel[3] = (unsigned char)(0)    ; // invisible
           }
           else if(pixVal < (min + 0.0625 * range))
           {
             rgbPixel[0] = (unsigned char)(0)                     ;
             rgbPixel[1] = (unsigned char)(0)                     ;
             rgbPixel[2] = (unsigned char)(131.0 + 992.0 * pixVal);
           }
           else if(pixVal < (min + 0.1250 * range))
           {
             rgbPixel[0] = (unsigned char)(0);
             rgbPixel[1] = (unsigned char)(0);
             rgbPixel[2] = (unsigned char)(131.0 + 992.0 * pixVal);
           }
           else if(pixVal < (min + 0.375 * range))
           {
             rgbPixel[0] = (unsigned char)(0);
             rgbPixel[1] = (unsigned char)(-127.5 + 1020.0 * pixVal);
             rgbPixel[2] = (unsigned char)(255);
           }
           else if(pixVal < (min + 0.500 * range))
           {
             rgbPixel[0] = (unsigned char)(-382.5 + 1020.0 * pixVal);
             rgbPixel[1] = (unsigned char)(255);
             rgbPixel[2] = (unsigned char)(637.5 - 1020.0 * pixVal);
           }
           else if(pixVal < (min + 0.625 * range))
           {
             rgbPixel[0] = (unsigned char)(-382.5 + 1020.0 * pixVal);
             rgbPixel[1] = (unsigned char)(255);
             rgbPixel[2] = (unsigned char)(637.5 - 1020.0 * pixVal);
           }
           else if(pixVal < (min + 0.750 * range))
           {
             rgbPixel[0] = (unsigned char)(255);
             rgbPixel[1] = (unsigned char)(892.5 - 1020.0 * pixVal);
             rgbPixel[2] = (unsigned char)(0);
           }
           else if(pixVal < (min + 0.875 * range))
           {
             rgbPixel[0] = (unsigned char)(255);
             rgbPixel[1] = (unsigned char)(892.5 - 1020.0 * pixVal);
             rgbPixel[2] = (unsigned char)(0);
           }
           else if(pixVal < (min + 1.000 * range))
           {
             rgbPixel[0] = (unsigned char)(1144.0 - 1016.0 * pixVal);
             rgbPixel[1] = (unsigned char)(0);
             rgbPixel[2] = (unsigned char)(0);
           }
           else
           {
             rgbPixel[0] = (unsigned char)(0);
             rgbPixel[1] = (unsigned char)(0);
             rgbPixel[2] = (unsigned char)(0);
             rgbPixel[3] = (unsigned char)(0); // invisible - outside the range
           }

           itkrgbImage->SetPixel(idx, rgbPixel);
         }
       }
     }

     return itkrgbImage;
}



void CaPViewDCEWindow::on_action_Load_DCE_triggered()
{
  // Let the user select a directory to load the volume from 
      QString FilePath = QFileDialog::getOpenFileName(this,
        tr("Please, select a file to load the DCE volume from ...."), THIS->m_lastVolumePathDCE, tr("Image Files (*.mha *.mhd)"));

      // Check the returned directory path
      if (FilePath.isEmpty())
      {
        // write a warning on the status bar
        statusBar()->showMessage("Warning: Empty or invalid directory ....");
        return;
      }
 
	  THIS->m_lastVolumePathDCE = FilePath;

	  //TODO
    // If the user loaded a volume before, everything is reset
    if(THIS->m_pVolumeDCE /*&& !THIS->m_pProstateMask && !THIS->m_pProbabilitiesImage*/)
    {
      ResetApp();
    }

    // load the volume
    try
    {
      THIS->m_pVolumeDCE = CohenCorrectBiasField(ccipd::LoadITKImage<VolumeImageType>(FilePath.toStdString()));
    }
	catch (...)
    {
      DisplayStatusMessage(statusBar(), "Error(416-711): Not able to Load volume ...", StatusMessageHandler/*::MessageType*/::ERR);
      return;
    }

    if(!THIS->m_pVolumeDCE)
    {
      // write a warning on the status bar
      DisplayStatusMessage(statusBar(), "Error(116-701): Invalid Volume ....", StatusMessageHandler/*::MessageType*/::ERR);
      return;
    }

    this->setWindowTitle(QCoreApplication::applicationName() + "  -  " + FilePath);

    // Deals with the case if some other Volume Image type was loaded before loading the original volume
    //if(THIS->m_pProbabilitiesImage)
    //{
    //  ResizeImage(THIS->m_pVolumeDCE, THIS->m_pProbabilitiesImage, true);
    //}
    //else if(THIS->m_pProstateMask)
    //{
    //  ResizeImage(THIS->m_pVolumeDCE, THIS->m_pProstateMask, true);
    //}


    // Flip the Loaded volume over the x-axis
    THIS->m_pVolumeDCE = FlipImage<VolumeImageType>(THIS->m_pVolumeDCE, 1); // This will flip it over the y-axis
    
    // Now Display the loaded mask
    renderVolumeAsSlices<VolumeImageType>(THIS->m_pVolumeDCE, THIS->ui->qVTKWidget_DCE, THIS->m_ImagePropDCE, THIS->m_currentSlice);
    initControlsValues();

//    THIS->ui->checkBox_Volume_DCE->setChecked(true);

    // Prevent User from re-opening a volume image if any of the corresponding object has been loaded
    // User should press the Clear button first
    UpdateActionsEnabledState();
}

void CaPViewDCEWindow::initControlsValues()
{
  UpdateSliceSlider(THIS->m_pVolumeDCE, THIS->ui->horizontalSlider_currentSlice, THIS->ui->spinBox_currentSlice);
//  UpdateWindowLevelSliders(THIS->m_pVolumeDCE, THIS->ui->horizontalSlider_WindowWidth_DCE, THIS->ui->horizontalSlider_WindowLevel_DCE, THIS->ui->doubleSpinBox_WindowWidth_DCE, THIS->ui->doubleSpinBox_WindowLevel_DCE);
//  UpdateTransparencySlider(THIS->m_pVolumeDCE, THIS->ui->horizontalSlider_VolumeOpacity_DCE, THIS->ui->spinBox_VolumeOpacity_DCE);
}

void CaPViewDCEWindow::UpdateActionsEnabledState()
{
  // If only one object is loaded, then its corresponding action will be left true (all the others will be true because none of them is loaded yet)
  // If more than one object is loaded, only the remaining objects (which were not loaded) will be left enabled true

    THIS->ui->action_Load_DCE->setEnabled(true);

    


    //THIS->ui->action_SetProstateSurfaceColor->setEnabled(false);
    //THIS->ui->action_SetCancerSurfaceColor->setEnabled(false);

    if(THIS->m_pVolumeDCE)
    {
        THIS->ui->action_Load_DCE->setEnabled(false);
        //THIS->ui->groupBox_Viewer->setEnabled(false);
    }
}

void CaPViewDCEWindow::on_pushButton_ClearView_clicked()
{
  ResetApp();
}

void CaPViewDCEWindow::on_pushButton_ResetCamera_clicked()
{
	// extract out render window, reset and rerender
	vtkRenderWindowPointer renderWindow = THIS->ui->qVTKWidget_DCE->GetRenderWindow();
	vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();
	vtkCamera * cam = renderer->GetActiveCamera();
	
	//cam->Azimuth(150);
	cam->Elevation(180);

	renderer->ResetCamera();
	renderWindow->Render();

}

//////////////////////////////////////////////////////////////////////////
void CaPViewDCEWindow::clearViewer(QVTKWidget* pQVTKWidget)
{
  if (!pQVTKWidget) 
  {
    DisplayStatusMessage(statusBar(), "Error(944-48): Invalid QVTKWidget pointer ...", StatusMessageHandler/*::MessageType*/::ERR);
    return;
  }

  // extract the render window
  const vtkRenderWindowPointer renderWindow = pQVTKWidget->GetRenderWindow();
  if (!renderWindow) 
  {
    DisplayStatusMessage(statusBar(), "Error(1015-48): Invalid Render Window ...", StatusMessageHandler/*::MessageType*/::ERR);
    return;
  }

  // clear the render window
  const vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();
  if (!renderer) 
  {
    DisplayStatusMessage(statusBar(), "Error(1035-408): Invalid Renderer ...", StatusMessageHandler/*::MessageType*/::ERR);
    return;
  }
  renderer->RemoveAllViewProps();

  renderWindow->Render();
  renderWindow->Render();
}

void CaPViewDCEWindow::renderGraylevelVolume(VolumeImageConstPointer pVolumeImage, QVTKWidget* pQVTKWidget, vtkPropPointer & pProp, unsigned int sliceNumber/* = 0*/)
{
	if (!pQVTKWidget || !pVolumeImage || pProp)
	{
    statusBar()->showMessage("Invalid rendering parameters ...");
    return;
	}

   const vtkRenderWindowPointer renderWindow = pQVTKWidget->GetRenderWindow();
   const vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();
   renderer->SetBackground ( 0.0, 0.0, 0.0 );    //Define the background color of rendering window
   renderer->ResetCamera();

   UpdateVTKImageProp(renderWindow, pProp, pVolumeImage, sliceNumber);

}

void CaPViewDCEWindow::renderVolume(QVTKWidget* pQVTKWidget, RGBAVolumeConstPointer pVolumeImage, unsigned int /*sliceNumber*/)
{
   if (!pQVTKWidget || !pVolumeImage)
   {
     statusBar()->showMessage("Invalid rendering parameters ...");
     return;
   }
 
   const vtkRenderWindowPointer renderWindow = pQVTKWidget->GetRenderWindow();
   const vtkRendererPointer renderer = renderWindow->GetRenderers()->GetFirstRenderer();
   renderer->SetBackground ( 0.0, 0.0, 0.0 );    //Define the background color of rendering window
   renderer->ResetCamera();

   //UpdateVTKImageProp(renderWindow, THIS->m_HeatmapPropDCE, pVolumeImage, sliceNumber);
}




//void CaPViewDCEWindow::on_checkBox_Volume_DCE_toggled(bool showDCEVolume)
//{
//  if (!THIS->m_pVolumeDCE)
//	{
//		THIS->ui->checkBox_Volume_DCE->setChecked(false);
//		statusBar()->showMessage("Warning: No Volume Image Loaded ...");
//		return;
//	}
//	THIS->m_ImagePropDCE->SetVisibility(showDCEVolume);
//	THIS->ui->qVTKWidget_DCE->GetRenderWindow()->Render();
//}



void CaPViewDCEWindow::on_actionCaPView_Help_triggered()
{
  QDesktopServices::openUrl(QCoreApplication::applicationDirPath() + "/help/help.chm");
}



void CaPViewDCEWindow::on_action_SaveDCEImage_triggered()
{
  QString imagePath = QFileDialog::getSaveFileName(this, "Save DCE as ...", 
                             THIS->m_lastImagePath, "PNG(*.png);; TIFF(*.tiff *.tif);; JPEG(*.jpg *.jpeg)");
  if (imagePath.isEmpty())
  {
    // write a warning on the status bar
    statusBar()->showMessage("Warning: Empty or invalid File Path ....");
    return;
  }

  THIS->m_lastImagePath = imagePath;

  vtkWindowToImageFilter * windowToImage = vtkWindowToImageFilter::New();

  ///Capture Window
  windowToImage->SetInput(THIS->ui->qVTKWidget_DCE->GetRenderWindow());
  windowToImage->Update();

  ///Write/Save as PNG
  vtkPNGWriter *writer = vtkPNGWriter::New();
  writer->SetInputConnection(windowToImage->GetOutputPort()); //Cant group outside if's GCC compiler error
  writer->SetFileName(imagePath.toStdString().c_str());
  writer->Write();

  writer->Delete();
  windowToImage->Delete();
}

void CaPViewDCEWindow::on_action_SaveAppSnapshot_triggered()
{
  QString lastAppSnapshotPath = QFileDialog::getSaveFileName(this, "Save as ...", 
                             THIS->m_lastAppSnapshotPath, "PNG(*.png);; TIFF(*.tiff *.tif);; JPEG(*.jpg *.jpeg)");
  if (lastAppSnapshotPath.isEmpty())
  {
    // write a warning on the status bar
    statusBar()->showMessage("Warning: Empty or invalid File Path ...");
    return;
  }

  THIS->m_lastAppSnapshotPath = lastAppSnapshotPath;

  if (!QPixmap::grabWidget(this).save(lastAppSnapshotPath))
  {
    cerr << "Error(1188-5): Snapshot was not saved ...\n";
    statusBar()->showMessage("Error(1188-5): Snapshot was not saved ...");
  }
}

void CaPViewDCEWindow::on_actionAbout_CaPView_triggered()
{
    QMessageBox::about( this, tr( "About CaPView" ),
    tr( "CaPView is a C++ toolkit for 3D visualization and manipulation of Prostate Cancer.\n\
         \n\
         This is a trial version which is distributed AS IS.\n\
         Copyright (c) CCIPD and/or its subsidiary(-ies) and other contributors.\n\
         This program uses the toolkits: ITK, VTK, Qt, Boost, Eigen.\n\
         Author: Ahmad Algohary" ) );
}

  void CaPViewDCEWindow::on_actionAbout_CCIPD_triggered()
  {
          QMessageBox::about( this, tr( "About CCIPD" ),
      tr( "The Center of Computational Imaging and Personalized Diagnostics at Case Western Reserve University is involved in various different aspects of developing, evaluating and applying novel quantitative image analysis, computer vision, signal processing, segmentation, multi-modal co-registration tools, pattern recognition, and machine learning tools for disease diagnosis, prognosis, and theragnosis in the context of breast, prostate, head and neck, and brain tumors as also epilepsy and carotid plaque. Our group is also exploring the utility of these methods in studying correlations of disease markers across multiple scales, modalities, and functionalities -- from gene and protein expression to spectroscopy to digital pathology and to multi-parametric MRI.\n\
          \n\
          \n\
          For more information, please visit ccipd.case.edu" ) );
  }

  //void CaPViewDCEWindow::on_pushButton_ResetParams_clicked()
  //{
  //  THIS->ui->spinBox_currentSlice->setValue( THIS->ui->spinBox_currentSlice->maximum() / 2    );


  //  if(THIS->ui->checkBox_Volume_T2->isChecked())
  //  {
  //    //THIS->ui->spinBox_Window->setValue( THIS->ui->spinBox_Window->maximum() / 2    );
  //    //THIS->ui->spinBox_Level->setValue( THIS->ui->spinBox_Level->maximum() / 2    );
  //    //THIS->ui->spinBox_VolumeOpacity_T2->setValue( THIS->ui->spinBox_VolumeOpacity_T2->maximum() / 2    );
  //    initControlsValues();
  //  }


void CaPViewDCEWindow::on_qxtSpanSlider_Heatmap_upperValueChanged(int range)
{
  THIS->m_HeatMapMax = (range / 100.0f);
  QString rangeNum;
  THIS->ui->lineEdit_HeatmapLegendRange->setText(rangeNum.setNum(THIS->m_HeatMapMax, 'f', 2));
}

void CaPViewDCEWindow::on_qxtSpanSlider_Heatmap_lowerValueChanged(int min)
{
  THIS->m_HeatmapMin = (min / 100.0f);
  QString minNum;
  THIS->ui->lineEdit_HeatmapLegendMinimum->setText(minNum.setNum(THIS->m_HeatmapMin, 'f', 2));
}

void CaPViewDCEWindow::slot_mouseWheelForwardEvent(vtkObject*, unsigned long, void*, void*)
{
  if(THIS->m_currentSlice == (unsigned int)(THIS->ui->horizontalSlider_currentSlice->maximum() )) return;
  THIS->m_currentSlice++;
  THIS->ui->horizontalSlider_currentSlice->setValue(THIS->m_currentSlice);
  cout << "+\n";
}

void CaPViewDCEWindow::slot_mouseWheelBackwardEvent(vtkObject*, unsigned long, void*, void*)
{
  if (0 == THIS->m_currentSlice) return;
  THIS->m_currentSlice--;
  THIS->ui->horizontalSlider_currentSlice->setValue(THIS->m_currentSlice);
  cout << "-\n";
}

} // namespace ccipd

