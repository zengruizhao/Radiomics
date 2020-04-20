#ifndef __ccipdLoadImage_h
#define __ccipdLoadImage_h

#include "ccipdLoadImage.hxx"

namespace ccipd
{

/**
This function load an Itk image of supported format from an input file.
 * 
 * \author  Ahmad Algohary  2013-Aug-27
 * \updated Ahmad Algohary  09/30/2013    
 * \ingroup Common Images
 * \Note: Specializing this function will require to change its signature as overloads are not allowed to differ only by return type
 */

template <class TImageType>
typename TImageType::Pointer LoadITKImage(std::string fileNamePath);

template <class TImageType>
typename TImageType::Pointer LoadDICOMImage(std::string fileNamePath);

}

#endif