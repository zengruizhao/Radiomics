

#include "ccipdDisableWarningsMacro.h"
	#include <itkImageFileWriter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{
	template<class TImageType>
	void WriteITKImage(typename TImageType::Pointer image, std::string fileName)
	{
	  typedef itk::ImageFileWriter<TImageType> WriterType;
	  typename WriterType::Pointer writer = WriterType::New();
	  writer->SetInput(image); 
	  writer->SetFileName(fileName);
	  try
	  {
	     writer->Update();
	  }
    catch ( itk::ExceptionObject &err )
    {
    	std::cerr << err << "Code: " << __FILE__ << ":"<< __LINE__ << std::endl;;
    }
    catch(...)
    {
      std::cerr << "Error(26-32): Unable to save Image to file: \"" << fileName << "\" ... \n" 
                << "Unknown Exception caught." << "\n" << __FILE__ << ":"<< __LINE__ << std::endl;
    }
	}

} // namespace
