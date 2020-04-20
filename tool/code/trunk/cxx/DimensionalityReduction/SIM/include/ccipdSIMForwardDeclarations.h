#ifndef __ccipdSIMForwardDeclarations_h
#define __ccipdSIMForwardDeclarations_h



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
// we are defining a SIM base for volume images here
template < class TPixelType, unsigned int Dimension >
class SIMStorage;

class SIMEstimator;
class SIMBoundingBoxExtractor;
class SIMFeatureExtractor;
class SIMStudiesAligner;
class SIMStudiesMaskAligner;
class SIMFeaturePreparerUsingExtractors;
class SIMFeaturePreparerInterface;
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
#endif //__ccipdSIMForwardDeclarations_h