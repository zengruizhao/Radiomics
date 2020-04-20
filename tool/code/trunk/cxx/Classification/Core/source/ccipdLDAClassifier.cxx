#include "ccipdLDAClassifier.h"

namespace ccipd
{
	ccipdLDAClassifier::ccipdLDAClassifier() : ccipdDAClassifier()
	{
		m_bTrained = false;
	}

	ccipdLDAClassifier::~ccipdLDAClassifier()
	{

	}

	bool ccipdLDAClassifier::Train(const EigenMatrixTypeD & data, const EigenLabelVector & labels)
	{
		// Initializing
		bool bDataSorted = false;		// 
		m_nObsTraining = (unsigned int)data.rows();	// Length of the Training dataset
		m_nFeatures = (unsigned int)data.cols();		// Number of features in every class
		EigenVectorTypeU gsizes(1);		// Size of each class/group

// 		Check the consistency of the input data
  		if (m_nObsTraining != (unsigned int)labels.rows())
  		{
  			if (m_nObsTraining != labels.sum())
  			{
  				return false;
  			}
  			// given data observations is sorted, we know the number of classes in advance
  			m_nGroups = (unsigned int)labels.size(); 
  			gsizes = labels;
  			bDataSorted = true;
  		} 
 
		// Sort the trainingData according to the group coefficients passed 
  		m_trainingSet = data; 
  		m_group = labels;
  		if (!bDataSorted)
  		{
  			// Sort the Training dataset according to the group indexes
  			for (unsigned int i = 0; i < m_nObsTraining; i++) // m to be used
  			{
  				for (unsigned int j = i+1; j < m_nObsTraining; j++)
  				{
  					if (m_group[j] < m_group[i])
  					{
  						//Swap 
  						m_group.row(i).swap(m_group.row(j));
  						m_trainingSet.row(i).swap(m_trainingSet.row(j));
  					}
  				}
  			}
  
  			// Find the number of groups (classes) we have and the length of each group
  			m_nGroups = 1;
  			gsizes << (const unsigned int)m_nObsTraining;
  			for (unsigned int i = 1; i < m_nObsTraining; i++)
  			{
  				if(m_group[i] != m_group[i-1])
  				{
  					EigenVectorTypeU tempVec(++m_nGroups);
  					tempVec.topRows(m_nGroups-1) = gsizes ;
  					tempVec[m_nGroups - 1] = m_nObsTraining - i;
  					tempVec[m_nGroups - 2] -= tempVec[m_nGroups- 1];
  					gsizes = tempVec;
  				}
  			}
 		} 

      m_prob.resize(m_nGroups);
   		// Assign/Calculate the probability of each class (select one of the following two lines)
//		m_prob.cwise() = (gsizes / m_nObsTraining).cast<float>();	//(Empirically) 
      m_prob.fill(((double)1/m_nGroups));			//(Uniformly)
 
 		// Calculate the means of each class/group
  		m_gmeans.resize(m_nGroups, m_nFeatures);
  		EigenMatrixTypeD centered(m_nObsTraining, m_nFeatures); // centered will hold the difference between the original values and the mean values for observations in each class 
  		for (unsigned int i = 0; i < m_nGroups; i++)
  		{
  			// Get the matrix of each class separately
			  EigenMatrixTypeD classMat = m_trainingSet.middleRows(gsizes.topRows(i).sum(), gsizes[i]);
			  m_gmeans.row(i) = classMat.colwise().mean();
			  centered.middleRows(gsizes.topRows(i).sum(), gsizes[i]) = classMat.rowwise() - classMat.colwise().mean();
  		}
 
 		// Calculating the logarithm of the determinant of the covariance Matrix
    Eigen::HouseholderQR<EigenMatrixTypeD> qr(centered);
    EigenMatrixTypeD R = qr.matrixQR().triangularView<Eigen::Upper>();   // m_R = centered.qr().matrixR(); //Note: This has a nehative sign difference from Matlab
    m_R = -R.topRows(R.cols());
    
    
   	m_R /= sqrt((double)(m_nObsTraining - m_nGroups));
   	EigenMatrixTypeD s =  m_R.jacobiSvd().singularValues();  // m_R.svd().singularValues();
   	for (/*unsigned*/ int i = 0; i < s.size(); i++)
   	{//s = s.log();
		  s(i) = log(s(i));
   	}
		m_logDetSigma.resize(1);
   	m_logDetSigma(0) = 2 * s.sum();
  
 		// Setting m_Trained to true will enable classification by this object
 		return m_bTrained = true;
	}

	ProbabilisticOutputType ccipdLDAClassifier::ClassifyProbabilistically(const EigenMatrixTypeD & dataToClassify ) const
	{
		// Initializing
		unsigned int nObsSample = (unsigned int) dataToClassify.rows();
    unsigned int nFeatures = (unsigned int) dataToClassify.cols();
		ProbabilisticOutputType outclass;

		// Check if this classifier class was trained
		if (!m_bTrained || nObsSample < 1 || (nFeatures != (unsigned int)m_gmeans.cols()) || (nFeatures != m_nFeatures))
		{
			std::cout << "\nError in dimensions or class needs to be trained ...\n";
			outclass.first.resize(0, 0);
			outclass.second.resize(0);
			return outclass; 
		}

		// MVN relative log posterior density, by group, for each sample
		outclass.first.resize(nObsSample, m_nGroups);
    outclass.second.resize(m_nGroups);
		for (unsigned int i = 0; i < m_nGroups; i++)
		{
			EigenMatrixTypeD A = (dataToClassify.rowwise() - m_gmeans.row(i)) * m_R.inverse();      
      EigenVectorTypeD logProb(1);
      logProb << log(m_prob[i]);
      outclass.first.col(i) = ((- 0.5) * ((A.cwiseProduct(A).rowwise().sum()).rowwise() + m_logDetSigma.row(0))).rowwise() + logProb.row(0);
		}

    EigenMatrixTypeD D = outclass.first;
    EigenMatrixTypeD P(nObsSample, m_nGroups);
    EigenVectorTypeD sumP(nObsSample);

    for (unsigned int i = 0; i < nObsSample; i++)
    {
      double dc = (D.row(i).maxCoeff());
      for (unsigned int j = 0; j < m_nGroups/*D.cols*/; j++)
      {
        D(i, j) -= dc;
        P(i, j) = exp(D(i, j));
      }
      
      sumP(i) = P.row(i).sum();
      
      for (unsigned int j = 0; j < m_nGroups; j++)
      {
        outclass.first(i, j) = P(i, j) / sumP[i];
      }

    }
    outclass.second = MatrixUtils::GetUniqueValues(m_group);

    // Return the pair of Probabilities and Labels
		outclass.second = m_group;
		return outclass;
	}
	
}