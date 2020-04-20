#ifndef __ccipdClassifierBase_h
#define __ccipdClassifierBase_h

//////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdMatrixTypes.h"	// for Eigen typedefs
#include "ccipdFactory.h"

// Eigen Includes
#include "ccipdDisableWarningsMacro.h"
#include <Eigen/Dense>

// std includes
#include <memory>				// for smart pointers
//////////////////////////////////////////////////////////////////
 
#include "ccipdEnableWarningsMacro.h"


namespace ccipd
{
/**
 * \brief    Base class for classifiers.
 * \Created  Rob Toth 03/28/2013
 * \Updated  Gohary   05/22/2013 
 * \ingroup Classification
 */

	class ccipdClassifierBase //: public Factory< ccipdClassifierBase >
	{
		public:
			// constructor
			//////////////////////////////////////////////////////////////////
			/**
			* \brief Constructor
			*
			* Child classes should provide public static “New”
			* and “ConstNew” functions to create smart pointers
			* (see typedefs below) of our objects.
			* These can be easily provided by extending the
			* Factory class in ccipdFactory.hxx
			*/
			ccipdClassifierBase(){};
			virtual ~ccipdClassifierBase(){}; ///< Destructor (always virtual!)
		
			typedef ccipdClassifierBase Self;
 			typedef std::shared_ptr< Self > Pointer;
 			typedef std::shared_ptr< const Self > ConstPointer;
			//////////////////////////////////////////////////////////////////
		public: // functions
			//////////////////////////////////////////////////////////////////////////
			/**
			* \brief:  Applies classification to the observations of the given sample dataset
			* \param[in]:  dataToClassify: is a (instances x features) matrix holding sample observations to be classified.
			* \param[out]:
            * \Return:     VectorXi: (instances x 1) vector in which each coefficient will hold a label (integer) representing 
			* 								the class/group to which the given observation belongs
			*			If classification was not successful a value of 0 is returned (1x1 vector holding 0)
			*/
			virtual EigenVectorTypeU Classify(const EigenMatrixTypeD & dataToClassify ) const = 0;
			//////////////////////////////////////////////////////////////////


	};  // class ClassifierBase
}       // namespace ccipd
#endif	// ccipdClassifierBase_h

