


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdPatients.h"
#include "ccipdFactory.hxx"
#include "ccipdLoadableFactory.hxx"
#include "ccipdXMLBaseFactory.hxx"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< Patients >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
class LoadableFactory< Patients >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
class XMLBaseFactory< Patients >;
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
