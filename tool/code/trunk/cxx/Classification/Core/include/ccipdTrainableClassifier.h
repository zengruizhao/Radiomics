#ifndef __ccipdTrainableClassifier_h
#define __ccipdTrainableClassifier_h

//////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdClassifierBase.h"
//////////////////////////////////////////////////////////////////
namespace ccipd
{
	/**
	 * \brief   Base class for classifiers which take in training data.
	 * \author  Rob Toth 04/02/2013
	 * \Updated Gohary 05/22/2013    
	 * \ingroup Classification
	 */
	class ccipdTrainableClassifier : virtual public ccipdClassifierBase
	{

	// constructor
	//////////////////////////////////////////////////////////////////
	protected:
    ccipdTrainableClassifier(){};
	public:
    virtual ~ccipdTrainableClassifier(){}; ///< Destructor (always virtual!)

	  typedef ccipdTrainableClassifier Self;
	  typedef std::shared_ptr<       Self > Pointer;
	  typedef std::shared_ptr< const Self > ConstPointer;
	//////////////////////////////////////////////////////////////////

	public: // functions
	//////////////////////////////////////////////////////////////////

	  static_assert(std::is_integral< EigenLabelVector::Scalar >::value, "Labels should be an integer type.");

    using ccipdClassifierBase::Classify;


	  /**
	   * \brief Child classes must provide ability to train.
	   * 
	   * \param[in] data is a (instances x features) matrix.
	   * \param[in] labels is a (instances x 1) vector
	   */
	  virtual bool Train(const EigenMatrixTypeD & data, const EigenLabelVector & labels) = 0;

	//////////////////////////////////////////////////////////////////

    

	}; // class TrainableClassifier
}      // namespace ccipd
#endif // __pcadTrainableClassifier_h