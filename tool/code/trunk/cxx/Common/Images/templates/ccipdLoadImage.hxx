


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdDisableWarningsMacro.h"
	#include <itkImageFileReader.h>
  #include <itkImageSeriesReader.h>
  #include <itkGDCMImageIO.h>
#include "ccipdEnableWarningsMacro.h"

//////////////////////////////////////////////////////////////////////////
////////////////


namespace ccipd
{
	template <class TImageType>
	typename TImageType::Pointer LoadITKImage(std::string fileNamePath)
	{
	  typedef itk::ImageFileReader<TImageType> ReaderType;
	  typename ReaderType::Pointer reader = ReaderType::New();
	  reader->SetFileName(fileNamePath);
	  
    try
    {
      reader->Update();
    }
    catch ( itk::ExceptionObject &err )
    {
    	std::cerr << err << "Code: " << __FILE__ << ":"<< __LINE__ << std::endl;;
      return typename TImageType::Pointer(NULL);
    }
    catch(...)
    {
      std::cerr << "Error(26-29): Unable to Load Image from file: \"" << fileNamePath << "\" ... \n" 
                << "Unknown Exception caught." << "\n" << __FILE__ << ":"<< __LINE__ << std::endl;
      return typename TImageType::Pointer(NULL);
    }
    return reader->GetOutput();
	}


  template <class TImageType>
	typename TImageType::Pointer LoadDICOMImage(std::string fileNamePath)
	{
    typedef itk::ImageSeriesReader< TImageType > DicomReaderType;
    const typename DicomReaderType::Pointer dicomReader = DicomReaderType::New(); 
    dicomReader->SetImageIO( itk::GDCMImageIO::New() ); 
    dicomReader->SetFileName( fileNamePath );
    
    try
    {
      dicomReader->Update();
    }
    catch ( itk::ExceptionObject &err )
    {
    	std::cerr << err << "Code: " << __FILE__ << ":"<< __LINE__ << std::endl;;
      return typename TImageType::Pointer(NULL);
    }
    catch(...)
    {
      std::cerr << "Error(26-30): Unable to Load Image from file: \"" << fileNamePath << "\" ... \n" 
                << "Unknown Exception caught." << "\n" << __FILE__ << ":"<< __LINE__ << std::endl;
      return typename TImageType::Pointer(NULL);
    }

    return dicomReader->GetOutput();
	}













} // namespace
