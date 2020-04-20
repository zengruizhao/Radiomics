#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkWarpImageFilter.h"
#include "itkDiffeomorphicDemonsRegistrationWithMaskFilter.h"
#include "itkVector.h"
#include "itkImageMaskSpatialObject.h"
#include "itkCommand.h"
#include "itkDisplacementFieldJacobianDeterminantFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkWarpHarmonicEnergyCalculator.h"


template <class TPixel = float, unsigned int VImageDimension = 3>
class CommandIterationUpdate : public itk::Command
  {
public:
  typedef  CommandIterationUpdate                      Self;
  typedef  itk::Command                                Superclass;
  typedef  itk::SmartPointer<Self>                     Pointer;

  typedef itk::Image<TPixel, VImageDimension>          InternalImageType;
  typedef itk::Vector<TPixel, VImageDimension>         VectorPixelType;
  typedef itk::Image<VectorPixelType, VImageDimension> DeformationFieldType;

  typedef itk::DiffeomorphicDemonsRegistrationWithMaskFilter<
    InternalImageType,
    InternalImageType,
    DeformationFieldType>   DiffeomorphicDemonsRegistrationWithMaskFilterType;

  typedef itk::DisplacementFieldJacobianDeterminantFilter<
    DeformationFieldType, TPixel, InternalImageType> JacobianFilterType;

  typedef itk::WarpHarmonicEnergyCalculator<DeformationFieldType>
  HarmonicEnergyCalculatorType;

  itkNewMacro( Self );
private:
  typename JacobianFilterType::Pointer m_JacobianFilter;
  typename HarmonicEnergyCalculatorType::Pointer m_HarmonicEnergyCalculator;
public:
  void Execute(itk::Object *caller, const itk::EventObject & event)
  {
    Execute( (const itk::Object *)caller, event );
  }

  void Execute(const itk::Object *object, const itk::EventObject & event)
  {
    if ( !( itk::IterationEvent().CheckEvent( &event ) ) )
      {
      return;
      }

    typename DeformationFieldType::Pointer deffield = 0;
    unsigned int iter = vcl_numeric_limits<unsigned int>::max();

   if ( const DiffeomorphicDemonsRegistrationWithMaskFilterType * filter
               = dynamic_cast<const  DiffeomorphicDemonsRegistrationWithMaskFilterType *>(
               object ) )
      {
      iter = filter->GetElapsedIterations() - 1;
      deffield = const_cast<DiffeomorphicDemonsRegistrationWithMaskFilterType *>
                 ( filter )->GetDeformationField();
      }
   else
   {
     return;
   }

      if ( deffield.IsNotNull() )
      {

      m_HarmonicEnergyCalculator->SetImage( deffield );
      m_HarmonicEnergyCalculator->Compute();
      const double harmonicEnergy
        = m_HarmonicEnergyCalculator->GetHarmonicEnergy();
      std::cout << iter << ": harmo. " << harmonicEnergy << " - ";

      m_JacobianFilter->SetInput( deffield );
      m_JacobianFilter->UpdateLargestPossibleRegion();

      const unsigned int numPix = m_JacobianFilter->
                                    GetOutput()->GetLargestPossibleRegion().
                                    GetNumberOfPixels();

      TPixel *pix_start = m_JacobianFilter->GetOutput()->GetBufferPointer();
      TPixel *pix_end = pix_start + numPix;

        // Get min an max jac
      const double minJac = *( std::min_element (pix_start, pix_end) );
      const double maxJac = *( std::max_element (pix_start, pix_end) );

        std::cout << "max|Jac| " << maxJac << " - "
                << "min|Jac| " << minJac << std::endl;
       }
  }

protected:
  CommandIterationUpdate() 
    {
    m_JacobianFilter = JacobianFilterType::New();
    m_JacobianFilter->SetUseImageSpacing( true );
    m_JacobianFilter->ReleaseDataFlagOn();
    m_HarmonicEnergyCalculator = HarmonicEnergyCalculatorType::New();
    }

  ~CommandIterationUpdate()
    {
    }
  };

int itkDiffeomorphicDemonsRegistrationWithMaskFilterTest(int argc, char **argv)
{
  if ( argc < 3 )
    {
    std::cerr << "Missing arguments" << std::endl;
    std::cerr << "Fixed Image File Name" << std::endl;
    std::cerr << "Moving Image File Name" << std::endl;
    std::cerr << "Deformed moving image File Name" << std::endl;
    std::cerr << "Fixed Mask File Name" << std::endl;
    std::cerr << "Moving Mask File Name" << std::endl;
    return EXIT_FAILURE;
    }

  typedef  float                     PixelType;
  const unsigned int dims = 3;
  typedef itk::Image<PixelType,dims> ImageType;
  typedef itk::Image<unsigned char, dims> MaskImageType;
  typedef itk::Image<itk::Vector<PixelType, dims>, dims> TDeformationField;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageFileReader<MaskImageType> MaskReaderType; 
  typedef itk::ImageFileWriter<ImageType> WriterType;
  typedef itk::SpatialObject<dims>  ImageMaskType;    

  ReaderType::Pointer fixedImageReader = ReaderType::New();
  fixedImageReader->SetFileName(argv[1]);
  fixedImageReader->Update();
  
  ReaderType::Pointer movingImageReader = ReaderType::New();
  movingImageReader->SetFileName(argv[2]);
  movingImageReader->Update();

  typedef itk::ImageMaskSpatialObject<dims> ImageMaskSpatialObjectType;
  typedef itk::DiffeomorphicDemonsRegistrationWithMaskFilter<
			ImageType, ImageType,
			TDeformationField>  RegistrationFilterType;
  RegistrationFilterType::Pointer filter = RegistrationFilterType::New();
  filter->SetFixedImage(fixedImageReader->GetOutput());
  filter->SetMovingImage(movingImageReader->GetOutput());

  if(argc > 4)
  {
    MaskReaderType::Pointer fixedMaskReader = MaskReaderType::New();
    fixedMaskReader->SetFileName(argv[4]);
    fixedMaskReader->Update();
    MaskImageType::Pointer fixedMaskImage ;
    fixedMaskImage = fixedMaskReader->GetOutput();
    fixedMaskImage->SetOrigin(fixedImageReader->GetOutput()->GetOrigin());

    ImageMaskSpatialObjectType::Pointer fixedMask  = ImageMaskSpatialObjectType::New();
    fixedMask->SetImage(fixedMaskImage);
    fixedMask->ComputeObjectToWorldTransform();

    filter->SetFixedImageMask(dynamic_cast< ImageMaskType * >( fixedMask.GetPointer()));
  }

  if(argc > 5)
  {
    MaskReaderType::Pointer movingMaskReader = MaskReaderType::New();
    movingMaskReader->SetFileName(argv[5]);
    movingMaskReader->Update();

    MaskImageType::Pointer movingMaskImage = MaskImageType::New();
    movingMaskImage = movingMaskReader->GetOutput();

    movingMaskImage->SetOrigin(movingImageReader->GetOutput()->GetOrigin());

    ImageMaskSpatialObjectType::Pointer movingMask  = ImageMaskSpatialObjectType::New();
    movingMask->SetImage(movingMaskImage);
    movingMask->ComputeObjectToWorldTransform();
    filter->SetMovingImageMask(dynamic_cast< ImageMaskType * >( movingMask.GetPointer()));
  }

//  filter->Print(std::cout,6);
  filter->SetIntensityDifferenceThreshold( 0.01 );
  filter->SetMaximumUpdateStepLength( 2.0 );
  filter->SetNumberOfIterations( 15 );
  filter->SetStandardDeviations(2.5  );
  filter->SetMaximumError( 0.1 );

  CommandIterationUpdate<float,
		  3>::Pointer observer
		  = CommandIterationUpdate<float, 3>::New();
		
  filter->AddObserver( itk::IterationEvent(), observer );

#if 0
  typedef DiffeomorphicDemonsShowProgressObject<RegistrationFilterType> ProgressType;

  ProgressType progressWatch(filter);
  itk::SimpleMemberCommand<ProgressType>::Pointer command;
  command = itk::SimpleMemberCommand<ProgressType>::New();
  command->SetCallbackFunction(&progressWatch,
                               &ProgressType::ShowProgress);
  filter->AddObserver( itk::ProgressEvent(), command);
#endif

  try
  {
    filter->Update();
  }
  catch( itk::ExceptionObject & err )
  {
    std::cout << "Caught expected error." << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
  }
  typedef itk::WarpImageFilter<ImageType, ImageType, TDeformationField> WarperType;
  WarperType::Pointer warper = WarperType::New();
  warper->SetInput(movingImageReader->GetOutput());
    warper->SetOutputSpacing( fixedImageReader->GetOutput()->GetSpacing() );
    warper->SetOutputOrigin( fixedImageReader->GetOutput()->GetOrigin() );
    warper->SetOutputDirection( fixedImageReader->GetOutput()->GetDirection() );
    warper->SetDeformationField( filter->GetOutput());
    try
    {
      warper->Update();
    }
    catch( itk::ExceptionObject & err )
    {
      std::cout << "Caught expected error." << std::endl;
      std::cout << err << std::endl;
      return EXIT_FAILURE;
    }
 
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(warper->GetOutput());
  writer->SetFileName(argv[3]);
  try
  {
    writer->Update();
  }

  catch( itk::ExceptionObject & err )
  {
    std::cout << "Caught expected error." << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
