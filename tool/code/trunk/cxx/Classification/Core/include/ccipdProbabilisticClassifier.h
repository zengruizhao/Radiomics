#ifndef __ccipdProbabilisticClassifier_h
#define __ccipdProbabilisticClassifier_h

//////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdClassifierBase.h"
#include "ccipdMatrixUtils.h"

// std includes
#include <utility> // for std::pair
//////////////////////////////////////////////////////////////////
namespace ccipd
{
	 /**
	   * The number of rows in the first output
	   * should be the number of rows in dataToClassify.
	   * The number of columns should be the number
	   * of unique classes. In the second, the size()
	   * should be the number of columns in the first,
	   * and should indicate which class corresponds
	   * to which column.
	   */
	  typedef std::pair<EigenMatrixTypeD, EigenLabelVector> ProbabilisticOutputType;






	/**
	 * \brief    Base class for classifiers which output probabilities.
	 * \author   Rob Toth 2013-04-02
	 * \Updated: Gohary 05/22/2013    
	 * \ingroup  Classification
	 */
	class ccipdProbabilisticClassifier : virtual public ccipdClassifierBase
	{

	// constructor
	//////////////////////////////////////////////////////////////////
	protected:
    ccipdProbabilisticClassifier();
	public:
    virtual ~ccipdProbabilisticClassifier(); ///< Destructor (always virtual!)

	  typedef ccipdProbabilisticClassifier Self;
	  typedef std::shared_ptr<       Self > Pointer;
	  typedef std::shared_ptr< const Self > ConstPointer;
	//////////////////////////////////////////////////////////////////

	public: // functions
	//////////////////////////////////////////////////////////////////
	  static_assert(std::is_integral< EigenLabelVector::Scalar >::value, "Labels should be an integer type." );

	  static_assert(std::is_floating_point< EigenMatrixTypeD::Scalar >::value, "Probabilities should be double types." );


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
	   *   Each row in the first output should sum to one.
	   *   The # of rows should be the rows in dataToClassify
	   */
	  virtual ProbabilisticOutputType ClassifyProbabilistically(const EigenMatrixTypeD & dataToClassify ) const = 0;

	  /**
	   * This function should actually provide
	   * the ability to discretely classify
	   * each instance, based on maximizing the probability
	   * and should call ClassifyProbabilistically.
	   * 
	   * In the future, possibly extend this to
	   * use thresholds.
	   * 
	   */
	  virtual EigenLabelVector Classify(const EigenMatrixTypeD & dataToClassify ) const;
    
    /** \brief  : This function does the probabilistic classification 
      *            and returns the probability by which each sample belongs to a specific class
      * \Created: Ahmad 7/19/2013
      * \Updated: Ahmad 9/10/2013 
      * \ingroup: Classification Core
      * \param  : dataToClassify: m x n matrix where m is the number of samples, n is the number of features
      * \param  : classNum: Number of the class (zero based) to which probability is calculated
      * \param  : bWhiten : boolean to tell whether to whiten the data (make mean = 0, and median absolute deviation = 1) before classification
      * \return : EigenVectorTypeD: This vector holds the probability that each sample (row) belongs to the class whose number is classNum.
      * \Notes  : 
      */
    EigenVectorTypeD Classify(const EigenMatrixTypeD & dataToClassify, unsigned int classNum, bool bWhiten = false) const;

	//////////////////////////////////////////////////////////////////

	};	// class ProbabilisticClassifier
}		// namespace ccipd
#endif	// __pcadProbabilisticClassifier_h

