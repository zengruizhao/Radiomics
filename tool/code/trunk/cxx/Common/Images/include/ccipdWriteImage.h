#ifndef __ccipdWriteImage_h
#define __ccipdWriteImage_h

#include "ccipdWriteImage.hxx"

namespace ccipd
{
	/**
	This function Writes an Itk image of supported format to a specified file on HDD.
	 * 
	 * \author  Ahmad Algohary
	 * \date    2013-Sep-29
	 * \ingroup Common Images
	 */
	template <class TImageType>
	void WriteITKImage(typename TImageType::Pointer image, std::string fileName);

}

#endif