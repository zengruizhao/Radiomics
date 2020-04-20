#ifndef __ccipdHashImage_hxx
#define __ccipdHashImage_hxx



//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdHashImage.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkTestingHashImageFilter.h> // for hashing images to string
#include "ccipdEnableWarningsMacro.h"

// std includes
#include <functional> // for std hash (from string to HashValueType)
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template< class TImage >
HashValueType HashImage( const TImage * const image )
{

  // first make sure we have an image
  if ( !image ) return 0;

  // use itk's hash type
  typedef itk::Testing::HashImageFilter< TImage > HasherType;
  typename HasherType::Pointer hasher = HasherType::New();

  // give it the inputs
  hasher->SetInput       ( image           );
  hasher->SetHashFunction( HasherType::MD5 );

  // calculate
  hasher->InPlaceOff(); // don't delete our image!
  hasher->Update    ();
  const std::string md5Hash = hasher->GetHash();

  // convert it to a HashValueType
  const HashValueType hash = std::hash< std::string >()( md5Hash );
  return hash;

} // HashImage
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
#endif // __ccipdHashImage_hxx
