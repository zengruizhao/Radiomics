


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdLoadGUI.hxx"
#include "ccipdStudy.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  

//////////////////////////////////////////////////////////////////////////////////////////
template
std::pair< StudyPointer, std::string >
LoadObjectAndFileNameGUI< Study >(
        QWidget * const parent,
  const bool            verbose );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
StudyPointer LoadObjectGUI< Study >(
        QWidget * const parent,
  const bool            verbose );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
std::string LoadFileNameGUI< Study >(
        QWidget * const parent,
  const bool            verbose );
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
