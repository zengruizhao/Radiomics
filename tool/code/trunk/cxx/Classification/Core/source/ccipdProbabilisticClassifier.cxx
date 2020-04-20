#include "ccipdProbabilisticClassifier.h"

//////////////////////////////////////////////////////////////////
namespace ccipd
{
    ccipdProbabilisticClassifier::ccipdProbabilisticClassifier()
    {

    }

    ccipdProbabilisticClassifier::~ccipdProbabilisticClassifier()
    {

    }

	  EigenLabelVector ccipdProbabilisticClassifier::Classify(const EigenMatrixTypeD & dataToClassify ) const
    {
      int nObsSample = (int)dataToClassify.rows();
      EigenMatrixTypeD probalilitiesMatrix(nObsSample, dataToClassify.cols());
      probalilitiesMatrix = ClassifyProbabilistically(dataToClassify).first ;

      // Find nearest group to each observation in sample data
      EigenVectorTypeU outLabelVec(nObsSample);
      for (int i = 0; i < nObsSample; i++)
      {
        probalilitiesMatrix.row(i).maxCoeff( &outLabelVec[i] );
      }

      return outLabelVec;
    }

	//////////////////////////////////////////////////////////////////
    
    
    
    
    EigenVectorTypeD ccipdProbabilisticClassifier::Classify(const EigenMatrixTypeD & dataToClassify, unsigned int classNum, bool bWhiten) const
    {
      int nObsSample = (int)dataToClassify.rows();
      EigenMatrixTypeD probalilitiesMatrix(nObsSample, dataToClassify.cols());
      
      if(bWhiten)
        probalilitiesMatrix = ClassifyProbabilistically(MatrixUtils::WhitenMatrix(dataToClassify)).first;
      else
        probalilitiesMatrix = ClassifyProbabilistically(dataToClassify).first;
    
      return probalilitiesMatrix.col(classNum);
    }




	
}		// namespace ccipd


