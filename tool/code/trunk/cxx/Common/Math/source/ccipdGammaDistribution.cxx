


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdGammaDistribution.hxx"
#include "ccipdFactory.hxx"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////
template
class GammaDistribution< float >;

template
class Factory< GammaDistribution< float > >;
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
template
class GammaDistribution< double >;

template
class Factory< GammaDistribution< double > >;
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
