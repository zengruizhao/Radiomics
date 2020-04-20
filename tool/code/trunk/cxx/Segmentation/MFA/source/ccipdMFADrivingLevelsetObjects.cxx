


//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFADrivingLevelsetObjects.h"
#include "ccipdMFADrivingFeaturePreparer.h"
#include "ccipdMFAModel.h"
#include "ccipdMFAPruneDrivingFeatures.h"
#include "ccipdMFALevelsetExtractor.h"
#include "ccipdMFALevelsetsFromFeatures.h"

#include <Eigen/Core>

//////////////////////////////////////////////////////////////////////////////////////////

namespace ccipd
{


#ifndef __pcadClassifierBase_h
#define __pcadClassifierBase_h

//////////////////////////////////////////////////////////////////
// prostacad includes
#include "ccipdMatrixTypes.h"

// std includes
#include <memory> // for smart pointers
//////////////////////////////////////////////////////////////////

typedef unsigned int LabelValueType;
typedef EigenMatrixTypeF EigenMatrixType;

typedef Eigen::Matrix<
  LabelValueType,
  Eigen::Dynamic,
  +1,
  Eigen::ColMajor,
  Eigen::Dynamic,
  +1
> LabelVectorType; // easily define a dynamic eigen vector of labels

/**
 * \brief   Base class for classifiers.
 * \author  Rob Toth
 * \date    2013-03-28
 * \ingroup Classification
 */
class ClassifierBase
{

// constructor
//////////////////////////////////////////////////////////////////
protected:
  /**
   * \brief Constructor
   * 
   * Child classes should provide public static “New”
   * and “ConstNew” functions to create smart pointers
   * (see typedefs below) of our objects.
   * These can be easily provided by extending the
   * Factory class in pcadFactory.hxx
   */
  ClassifierBase() { }
public:
  virtual ~ClassifierBase() { } ///< Destructor (always virtual!)

  typedef ClassifierBase Self;
  typedef std::shared_ptr<       Self > Pointer;
  typedef std::shared_ptr< const Self > ConstPointer;
//////////////////////////////////////////////////////////////////

       public: // functions
//////////////////////////////////////////////////////////////////
/**
 * \brief Child classes must provide ability to classify.
 * 
 * \param[in] dataToClassify is a (instances x features) matrix.
 * \param[out] (instances x 1) vector
 */

virtual LabelVectorType Classify(
  const EigenMatrixType & dataToClassify ) const = 0;
//////////////////////////////////////////////////////////////////

}; // class ClassifierBase
#endif // __pcadClassifierBase_h


#ifndef __pcadTrainableClassifier_h
#define __pcadTrainableClassifier_h

//////////////////////////////////////////////////////////////////
// prostacad includes
//#include "pcadClassifierBase.h"
//////////////////////////////////////////////////////////////////

/**
 * \brief   Base class for classifiers which take in training data.
 * \author  Rob Toth
 * \date    2013-04-02
 * \ingroup Classification
 */
class TrainableClassifier : virtual public ClassifierBase
{

// constructor
//////////////////////////////////////////////////////////////////
protected:
  /**
   * \brief Constructor
   * 
   * Child classes should provide public static “New”
   * and “ConstNew” functions to create smart pointers
   * (see typedefs below) of our objects.
   * These can be easily provided by extending the
   * Factory class in pcadFactory.hxx
   */
  TrainableClassifier() { }
public:
  virtual ~TrainableClassifier() { } ///< Destructor (always virtual!)

  typedef TrainableClassifier Self;
  typedef std::shared_ptr<       Self > Pointer;
  typedef std::shared_ptr< const Self > ConstPointer;
//////////////////////////////////////////////////////////////////




public: // functions
//////////////////////////////////////////////////////////////////

  /**
   * \brief Child classes must provide ability to train.
   * 
   * \param[in] data is a (instances x features) matrix.
   * \param[in] labels is a (instances x 1) vector
   */
  static_assert(
    std::is_integral< LabelVectorType::Scalar >::value,
    "Labels should be an integer type." );

  virtual void Train(
    const EigenMatrixType & data,
    const LabelVectorType & labels ) const = 0;

//////////////////////////////////////////////////////////////////

}; // class TrainableClassifier
#endif // __pcadTrainableClassifier_h



#ifndef __pcadProbabilisticClassifier_h
#define __pcadProbabilisticClassifier_h

//////////////////////////////////////////////////////////////////
// prostacad includes
//#include "pcadClassifierBase.h"
#include <utility> // for std::pair
//////////////////////////////////////////////////////////////////

/**
 * \brief   Base class for classifiers which output probabilities.
 * \author  Rob Toth
 * \date    2013-04-02
 * \ingroup Classification
 */
class ProbabilisticClassifier : virtual public ClassifierBase
{

// constructor
//////////////////////////////////////////////////////////////////
protected:
  /**
   * \brief Constructor
   * 
   * Child classes should provide public static “New”
   * and “ConstNew” functions to create smart pointers
   * (see typedefs below) of our objects.
   * These can be easily provided by extending the
   * Factory class in pcadFactory.hxx
   */
  ProbabilisticClassifier() { }
public:
  virtual ~ProbabilisticClassifier() { } ///< Destructor (always virtual!)

  typedef ProbabilisticClassifier Self;
  typedef std::shared_ptr<       Self > Pointer;
  typedef std::shared_ptr< const Self > ConstPointer;
//////////////////////////////////////////////////////////////////

public: // functions
//////////////////////////////////////////////////////////////////

  static_assert(
    std::is_integral< LabelVectorType::Scalar >::value,
    "Labels should be an integer type." );

  static_assert(
    std::is_floating_point< EigenMatrixType::Scalar >::value,
    "Probabilities should be floating types." );

  /**
   * The number of rows in the first output
   * should be the number of rows in dataToClassify.
   * The number of columns should be the number
   * of unique classes. In the second, the size()
   * should be the number of columns in the first,
   * and should indicate which class corresponds
   * to which column.
   */
  typedef std::pair<
    EigenMatrixType, LabelVectorType 
  > ProbabilisticOutputType;

  /**
   * Child classes must output
   * a probability (or, in actuality,
   * this could be a distance value)
   * for each instance belonging
   * to each class.
   * 
   * \param[in] data is a (instances x features) matrix.
   * 
   * \param[out]
   *   Each row should sum to one.
   *   The # of rows should be the rows in dataToClassify
   */
  virtual ProbabilisticOutputType ClassifyProbabilistically(
    const EigenMatrixType & dataToClassify ) const = 0;

  /**
   * This function should actually provide
   * the ability to discretely classify
   * each nstance, based on maximizing the probability.
   * 
   * In the future, possibly extend this to
   * use thresholds.
   */
  virtual LabelVectorType Classify(
    const EigenMatrixType & dataToClassify ) const;

//////////////////////////////////////////////////////////////////

}; // class ProbabilisticClassifier
#endif // __pcadProbabilisticClassifier_h



//////////////////////////////////////////////////////////////////////////////////////////
MaskObjectCollectionType GetDrivingLevelsetObjects(
  const MFADrivingFeaturePreparer & drivingFeatures )
{

  // extract the input model
  const MFAModelConstPointer
    model =  drivingFeatures.GetMFAModel();

  if ( !model )
    return MaskObjectCollectionType();

  // get the texture extractor
  const MFALevelsetExtractorConstPointer levelsetExtractor =
    model->GetLevelsetExtractor();

  if ( !levelsetExtractor )
    return MaskObjectCollectionType();

  // extract the input features and prune them
  const FeatureIDConstCollection
    inputLevelsetFeatures   = drivingFeatures.GetInputLevelsetFeatures(),
    drivingLevelsetFeatures = PruneDrivingFeatures(
      *levelsetExtractor, inputLevelsetFeatures );

  // now, just get the texture enums from them
  return GetLevelsetObjectsFromFeatures( drivingLevelsetFeatures );

} // GetDrivingLevelsetObjects
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
