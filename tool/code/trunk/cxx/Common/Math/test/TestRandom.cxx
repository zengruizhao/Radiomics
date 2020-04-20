


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdUniformDistribution.h"
#include "ccipdUnivariateGaussianDistribution.h"
#include "ccipdMultivariateGaussianDistribution.h"
#include "ccipdGammaDistribution.h"
#include "ccipdDisableWarningsMacro.h"

// std includes
#include <iostream> // for cout
#include <cstdlib>  // for EXIT_SUCCESS
#include <cfloat>   // for FLT_MAX

// boost includes
#include <boost/timer/timer.hpp> // for timing functions


  // eigen includes
  #include <Eigen/Dense>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// using shortcuts
using std::cout;
using std::endl;
using ccipd::UniformDistribution;
using ccipd::UnivariateGaussianDistribution;
using ccipd::MultivariateGaussianDistribution;
using ccipd::GammaDistribution;
using boost::timer::auto_cpu_timer;
//////////////////////////////////////////////////////////////////////////////////////////



int main()
{

  // how many values will we generate?
  const unsigned int
#ifdef NDEBUG
    repeats    = 100,
    squareSide = 1000,
#else
    repeats    = 1,
    squareSide = 2,
#endif
    rows = squareSide,
    cols = squareSide;
  const std::size_t
    numberOfValues =
      static_cast< std::size_t >( rows ) * 
      static_cast< std::size_t >( cols ) *
      static_cast< std::size_t >( repeats );

  typedef float ValuesType;
  typedef ValuesType UniformValuesType;
  typedef std::pair< ValuesType, ValuesType > PairType;
  typedef std::vector< PairType > PairCollection;

  // first try a uniform distribution of random values
  typedef UniformDistribution< UniformValuesType > UniformDistributionType;

  // try different min and max's
  PairCollection uniformPairs;
  uniformPairs.push_back( PairType( -1.0f, +1.0f ) );
  uniformPairs.push_back( PairType(  0.0f, +1.0f ) );
  uniformPairs.push_back( PairType(  0.0f, +7.0f ) );

  for ( PairCollection::const_iterator pairIter = uniformPairs.cbegin();
    pairIter != uniformPairs.cend(); ++pairIter )
    { 
      
    const UniformValuesType
      minValue = pairIter->first,
      maxValue = pairIter->second;
  
    const UniformDistributionType::Pointer
      uniformDistributionPointer = UniformDistributionType::New();
    UniformDistributionType & uniformDistribution = *uniformDistributionPointer;
    uniformDistribution.SetMinimumValue( minValue );
    uniformDistribution.SetMaximumValue( maxValue );
    
    cout << "-------------------------" << endl;
    
    cout << "Min = " << minValue << endl;
    cout << "Max = " << maxValue << endl;

    { // timer

    auto_cpu_timer timer; // this will time the function

    cout << "Generating " << numberOfValues << " uniform random values:" << endl;
    for ( unsigned int repeat = 0; repeat < repeats; ++repeat )
      {
      const auto randomUniformMatrix =
        uniformDistribution.GetRandomMatrix( rows, cols );
      if ( randomUniformMatrix->coeff( 0, 0 ) > maxValue )
        cout << "Never reach this line!" << endl;
      }

    cout << "Generating " << numberOfValues << " uniform random values done." << endl;

    } // timer

    const UniformValuesType randomValue = uniformDistribution.GetRandomValue();
    cout << "Random value = " << randomValue << endl;
    cout << "PDF = " <<
      uniformDistribution.GetPDF( randomValue ) <<
      endl;
    cout << "CDF = " <<
      uniformDistribution.GetCDF( randomValue ) <<
      endl;
    cout << "~CDF = " <<
      uniformDistribution.GetCDFComplement( randomValue ) <<
      endl;
    cout << "-------------------------" << endl << endl;

    } // pairIter

  typedef ValuesType GammaValuesType;
  typedef GammaDistribution< GammaValuesType > GammaDistributionType;
  PairCollection gammaPairs;

  gammaPairs.push_back( PairType( 1.0f, 1.0f ) ); // standard gamma
  gammaPairs.push_back( PairType( 1.0f, 3.0f ) ); // wider
  gammaPairs.push_back( PairType( 3.0f, 1.0f ) ); // shifted

  for ( PairCollection::const_iterator pairIter = gammaPairs.cbegin();
    pairIter != gammaPairs.cend(); ++pairIter )
    {

    const GammaValuesType
      mode     = pairIter->first,
      variance = pairIter->second;
  
    const GammaDistributionType::Pointer
      gammaDistributionPointer = GammaDistributionType::New();
    GammaDistributionType & gammaDistribution = *gammaDistributionPointer;
    gammaDistribution.SetModeAndVariance( mode, variance );

    cout << "-------------------------" << endl;

    cout << "Mean            = " << gammaDistribution.GetMean          () << endl;
    cout << "Excess Kurtosis = " << gammaDistribution.GetExcessKurtosis() << endl;
    cout << "Skewness        = " << gammaDistribution.GetSkewness      () << endl;
    cout << "Mode            = " << gammaDistribution.GetMode          () << endl;
    cout << "Variance        = " << gammaDistribution.GetVariance      () << endl;

    { // timer
    auto_cpu_timer timer; // this will time the function

    cout << "Generating " << numberOfValues << " Gamma random values:" << endl;
    for ( unsigned int repeat = 0; repeat < repeats; ++repeat )
      {
      const auto randomUniformMatrix =
        gammaDistribution.GetRandomMatrix( rows, cols );
      if ( randomUniformMatrix->coeff( 0, 0 ) > FLT_MAX )
        cout << "Never reach this line!" << endl;
      }
    cout << "Generating " << numberOfValues << " Gamma random values done." << endl;
    } // timer

    const GammaValuesType
      randomValue = gammaDistribution.GetRandomValue();

    cout << "Random value = " << randomValue << endl;
    cout << "PDF = " <<
      gammaDistribution.GetPDF( randomValue ) << endl;
    cout << "CDF = " <<
      gammaDistribution.GetCDF( randomValue ) << endl;
    cout << "~CDF = " <<
      gammaDistribution.GetCDFComplement( randomValue ) << endl;

    const GammaValuesType
      anotherRandomValue = gammaDistribution.GetRandomValue();
    cout << "Another random value = " << anotherRandomValue << endl;
    cout << "Probability = " <<
      gammaDistribution.GetProbability( randomValue, anotherRandomValue ) << endl;

    cout << "-------------------------" << endl << endl;

    } // pairIter

  typedef ValuesType GaussianValuesType;
  typedef UnivariateGaussianDistribution< GaussianValuesType > UnivariateGaussianDistributionType;
  PairCollection gaussianPairs;

  gaussianPairs.push_back( PairType( 0.0f, +1.0f ) ); // standard normal
  gaussianPairs.push_back( PairType( 0.0f, +3.0f ) ); // wider
  gaussianPairs.push_back( PairType( 1.0f, +7.0f ) ); // shifted and wider

  for ( PairCollection::const_iterator pairIter = gaussianPairs.cbegin();
    pairIter != gaussianPairs.cend(); ++pairIter )
    {

    const GaussianValuesType
      mean  = pairIter->first,
      sigma = pairIter->second;
  
    const UnivariateGaussianDistributionType::Pointer
      gaussianDistributionPointer = UnivariateGaussianDistributionType::New();
    UnivariateGaussianDistributionType & gaussianDistribution = *gaussianDistributionPointer;
    gaussianDistribution.SetMean    ( mean );
    gaussianDistribution.SetVariance( sigma * sigma );

    cout << "-------------------------" << endl;
    
    cout << "Mean = " << mean << endl;
    cout << "Sigma = " << sigma << endl;

    { // timer
    auto_cpu_timer timer; // this will time the function

    cout << "Generating " << numberOfValues << " gaussian random values:" << endl;
    for ( unsigned int repeat = 0; repeat < repeats; ++repeat )
      {
      const auto randomUniformMatrix =
        gaussianDistribution.GetRandomMatrix( rows, cols );
      if ( randomUniformMatrix->coeff( 0, 0 ) > FLT_MAX )
        cout << "Never reach this line!" << endl;
      }
    cout << "Generating " << numberOfValues << " gaussian random values done." << endl;
    } // timer

    const GaussianValuesType
      randomValue = gaussianDistribution.GetRandomValue();

    cout << "Random value = " << randomValue << endl;
    cout << "PDF = " <<
      gaussianDistribution.GetPDF( randomValue ) << endl;
    cout << "CDF = " <<
      gaussianDistribution.GetCDF( randomValue ) << endl;
    cout << "~CDF = " <<
      gaussianDistribution.GetCDFComplement( randomValue ) << endl;

    const GaussianValuesType
      anotherRandomValue = gaussianDistribution.GetRandomValue();
    cout << "Another random value = " << anotherRandomValue << endl;
    cout << "Probability = " <<
      gaussianDistribution.GetProbability( randomValue, anotherRandomValue ) << endl;

    cout << "-------------------------" << endl << endl;

    } // pairIter


  { // timer

  cout << "-------------------------" << endl;

  // define a multi-variate gaussian
  const unsigned int gaussianDimensionality = 3;
  typedef MultivariateGaussianDistribution<
    GaussianValuesType, gaussianDimensionality
  > MultivariateGaussianDistributionType;

  const MultivariateGaussianDistributionType::Pointer
    gaussianDistributionPointer = MultivariateGaussianDistributionType::New();
  MultivariateGaussianDistributionType & gaussianDistribution =
    *gaussianDistributionPointer;

  // define the mean of the gaussian
  MultivariateGaussianDistributionType::MeanVectorPointer 
    mean ( new   MultivariateGaussianDistributionType::MeanVectorType);
  *mean << 1, 2, 3;
  gaussianDistribution.SetMean( mean );

  // define the covariance of the gaussian
  MultivariateGaussianDistributionType::CovarianceMatrixPointer
    covariance ( new MultivariateGaussianDistributionType::CovarianceMatrixType);
  *covariance << 4, 1, 1, 1, 8, 1, 1, 1, 7;
  gaussianDistribution.SetVariance( covariance );

  const MultivariateGaussianDistributionType::DistributionVectorPointer
    randomValue = gaussianDistribution.GetRandomVector();

  // display some output
  cout << "Mean = " << endl << *mean << endl;
  cout << "Covariance = " << endl << *covariance << endl;
  cout << "Random value = " << endl << *randomValue << endl;
  cout << "PDF = " << gaussianDistribution.GetPDF( *randomValue ) << endl;
  cout << "CDF = " << gaussianDistribution.GetCDF( *randomValue ) << endl;
  cout << "~CDF = " << gaussianDistribution.GetCDFComplement( *randomValue ) << endl;

  // estimate the mean and covariance from random samples
  cout << "Estimating mean and covariance:" << endl;
  const MultivariateGaussianDistributionType::Pointer
    sampledDistributionPointer = MultivariateGaussianDistributionType::New();
  MultivariateGaussianDistributionType & sampledDistribution =
    *sampledDistributionPointer;
  sampledDistribution.EstimateDistributionFromSamples(
    *gaussianDistribution.GetRandomVectors( squareSide * squareSide ) );
  cout << "Estimating mean and covariance done." << endl;

  cout << "Mean = "       << endl << *sampledDistribution.GetMean    () << endl;
  cout << "Covariance = " << endl << *sampledDistribution.GetVariance() << endl;

  // how many values are we generating?
  cout << "Generating " << numberOfValues << " multi-variate gaussian random values:" << endl;

  // time our function
  auto_cpu_timer timer;

  for ( unsigned int repeat = 0; repeat < repeats; ++repeat )
    {
    const MultivariateGaussianDistributionType::DistributionVectorsPointer
      randomValues = gaussianDistribution.GetRandomVectors( squareSide * squareSide );
#ifndef NDEBUG
    cout << "Random values = " << endl << *randomValues << endl;
#else
    // make sure we don't get optimized out
    if ( randomValues->coeff( 1, 1 ) == 4.552 )
      cout << "We should never reach this line." << endl;
#endif
    } // repeat
    cout << "Generating " << numberOfValues << " multi-variate gaussian random values done." << endl;

  } // timer
  cout << "-------------------------" << endl;

  return EXIT_SUCCESS;

} // main