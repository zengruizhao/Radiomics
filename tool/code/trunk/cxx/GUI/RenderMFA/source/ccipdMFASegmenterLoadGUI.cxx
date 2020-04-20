


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdLoadGUI.hxx"
#include "ccipdMFASegmenterCorrection.h"// the actual MFA segmenter

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  

//////////////////////////////////////////////////////////////////////////////////////////
template
std::pair< MFASegmenterPointer, std::string >
LoadObjectAndFileNameGUI< MFASegmenter >(
        QWidget * const parent,
  const bool            verbose );

template
MFASegmenterPointer LoadObjectGUI< MFASegmenter >(
        QWidget * const parent,
  const bool            verbose );

template
std::string LoadFileNameGUI< MFASegmenter >(
        QWidget * const parent,
  const bool            verbose );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
std::pair< MFASegmenterCorrection::Pointer, std::string >
LoadObjectAndFileNameGUI< MFASegmenterCorrection >(
        QWidget * const parent,
  const bool            verbose );

template
MFASegmenterCorrection::Pointer LoadObjectGUI< MFASegmenterCorrection >(
        QWidget * const parent,
  const bool            verbose );

template
std::string LoadFileNameGUI< MFASegmenterCorrection >(
        QWidget * const parent,
  const bool            verbose );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
