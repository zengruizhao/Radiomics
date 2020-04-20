


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdLoadGUI.hxx"
#include "ccipdMFAModel.h"// the actual MFA segmenter

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


  
//////////////////////////////////////////////////////////////////////////////////////////
template
std::pair< MFAModelPointer, std::string >
LoadObjectAndFileNameGUI< MFAModel >(
        QWidget * const parent,
  const bool            verbose );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
MFAModelPointer LoadObjectGUI< MFAModel >(
        QWidget * const parent,
  const bool            verbose );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
std::string LoadFileNameGUI< MFAModel >(
        QWidget * const parent,
  const bool            verbose );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
