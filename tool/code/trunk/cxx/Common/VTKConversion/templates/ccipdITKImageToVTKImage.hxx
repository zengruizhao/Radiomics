


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdITKImageToVTKImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkVTKImageExport.h> // for converting to VTK

  // vtk includes
  #include <vtkSmartPointer.h>   // for smart memory management
  #include <vtkImageData.h>      // the output type
  #include <vtkImageImport.h>    // for accepting an ITK image
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



// somewhat based on http://bit.ly/Ic7iHo



//////////////////////////////////////////////////////////////////////////////////////////
template< class TImageType >
vtkImageDataPointer ConvertITKImageToVTKImage(const TImageType * const image)
{
  // create a filter to export the ITK image
  typedef itk::VTKImageExport< TImageType > ExportType;
  const typename ExportType::Pointer exporter = ExportType::New();

  // create a filter to import an ITK image
  typedef vtkSmartPointer< vtkImageImport > ImportPointer;
  const ImportPointer importer = ImportPointer::New();

  // input our volume image
  exporter->SetInput( image );

  // set up the data to the pipeline
  importer->SetUpdateInformationCallback    ( exporter->GetUpdateInformationCallback    () );
  importer->SetPipelineModifiedCallback     ( exporter->GetPipelineModifiedCallback     () );
  importer->SetWholeExtentCallback          ( exporter->GetWholeExtentCallback          () );
  importer->SetSpacingCallback              ( exporter->GetSpacingCallback              () );
  importer->SetOriginCallback               ( exporter->GetOriginCallback               () );
  importer->SetScalarTypeCallback           ( exporter->GetScalarTypeCallback           () );
  importer->SetNumberOfComponentsCallback   ( exporter->GetNumberOfComponentsCallback   () );
  importer->SetPropagateUpdateExtentCallback( exporter->GetPropagateUpdateExtentCallback() );
  importer->SetUpdateDataCallback           ( exporter->GetUpdateDataCallback           () );
  importer->SetDataExtentCallback           ( exporter->GetDataExtentCallback           () );
  importer->SetBufferPointerCallback        ( exporter->GetBufferPointerCallback        () );
  importer->SetCallbackUserData             ( exporter->GetCallbackUserData             () );

  // import
  importer->Update();

  // output
  return importer->GetOutput();

} // ConvertITKImageToVTKImage
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
