/********************************************************************
	created:	Ahmad Algohary 20:5:2013   12:09
	filename: 	ccipdDAClassifier.h
	file path:	..\Classification\Core\include
	author:		
	purpose:	
*********************************************************************/
#ifndef __ccipdDAClassifier_h
#define __ccipdDAClassifier_h

//////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdTrainableClassifier.h"
#include "ccipdProbabilisticClassifier.h"

// std includes
#include <memory>				// for smart pointers
//////////////////////////////////////////////////////////////////

namespace ccipd
{
	/*
		brief     : Base class for Discriminant Analysis
		Created   : Gohary 5/20/2013 13:38
		Updated   : 
		Notes     : 
	 */
	class ccipdDAClassifier : virtual public ccipdTrainableClassifier, virtual public ccipdProbabilisticClassifier
	{
		public:
			/**
			* \brief Constructor
			*/
       ccipdDAClassifier(){m_nGroups = 2;};    
//       virtual ~ccipdDAClassifier(){}; ///< Destructor (always virtual!)
		
			typedef ccipdDAClassifier Self;
 			typedef std::shared_ptr< Self > Pointer;
 			typedef std::shared_ptr< const Self > ConstPointer;
			//////////////////////////////////////////////////////////////////
		public: // functions
			//virtual bool Train(const EigenMatrixTypeF & data, const EigenLabelVector & labels) = 0;
      using ccipdTrainableClassifier::Train;
      using ccipdProbabilisticClassifier::ClassifyProbabilistically;
			//virtual ProbabilisticOutputType ClassifyProbabilistically(const EigenMatrixTypeF & dataToClassify ) const = 0;
			//////////////////////////////////////////////////////////////////
		
      //virtual EigenVectorTypeU Classify(const EigenMatrixTypeF & dataToClassify ) const = 0;
      using ccipdProbabilisticClassifier::Classify;

      // For Direct access to Traning decision boundaries
      // This is useful when importing from or when traning is done outside
      void SetTrainingParams(EigenVectorTypeD prob, EigenVectorTypeD logDetSigma, EigenMatrixTypeD R, EigenMatrixTypeD gmeans)
      {
        m_prob        = prob;		   
        m_logDetSigma = logDetSigma;
        m_R           = R;			     
        m_gmeans      = gmeans;	
        m_nGroups = (unsigned int)m_prob.rows();
        m_group.resize(m_nGroups);
        for(unsigned int g = 0; g < m_nGroups; g++) m_group[g] = g;
        m_nFeatures = (unsigned int)m_gmeans.cols();
        m_bTrained = true;
      };

//	  void LoadFromINI(std::string iniFilePath)
//	  {
//
//	  };
      
		protected: //TODO: Apply PIMPL
			// Training Rule Elements
			EigenVectorTypeD m_prob;		     // Holding the prior probability of each class 
			EigenVectorTypeD m_logDetSigma;	 // Logarithm of the determinant of the Covariance Matrix 
			EigenMatrixTypeD m_R;			       // The upper triangular R matrix resulting from the QR decomposition of the covariance matrix
			EigenMatrixTypeD m_gmeans;		   // Means (c x n Matrix) of each group of observations in the given training/learning dataset, c is the number of groups, n is the number of attributes/features

			unsigned int	 m_nObsTraining; // Number of observations in the training/learning dataset
			EigenVectorTypeU m_group;		     // sorted vector labels corresponding to each observation in the training/learning dataset // TODO: need to discuss whether to remove it (in case it will not be needed for further use after training)
			unsigned int	 m_nGroups;		   // Number of groups/classes to which the observations in the passed training/learning dataset belong
			unsigned int	 m_nFeatures;	   // Number of features/attributes associated with each class/group
			EigenMatrixTypeD m_trainingSet;  // Training set sorted (grouped by class)
			bool			 m_bTrained;		 // boolean to tell whether training phase was successful (have to be checked in the beginning of classification stage)

	};	// class ccipdDAClassifier
}		// namespace ccipd
#endif	// __ccipdDAClassifier_h
