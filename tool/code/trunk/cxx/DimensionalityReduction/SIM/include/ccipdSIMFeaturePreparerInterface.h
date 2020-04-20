#ifndef __ccipdSIMFeaturePreparerInterface_h
#define __ccipdSIMFeaturePreparerInterface_h



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdPatientTypes.h"
#include "ccipdFeatureCollectionTypes.h"

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



class SIMFeaturePreparerInterface
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorEmptyDefinitionsMacro( SIMFeaturePreparerInterface )
//////////////////////////////////////////////////////////////////////////////////////////



public: // set/get
//////////////////////////////////////////////////////////////////////////////////////////

  /// Return all the feature ID's
  virtual FeatureIDConstCollection GetWhichFeatures() const = 0;
  
  /// Extract all features for a given study
  virtual FeatureCollectionPointer GetFeatures(
    const StudyConstPointer & study ) const = 0;

//////////////////////////////////////////////////////////////////////////////////////////

  

}; // class SIMFeaturePreparerInterface
} // namespace ccipd



#endif // __ccipdSIMFeaturePreparerInterface_h