#ifndef __ccipdMFAAlgorithmTableWidget_h
#define __ccipdMFAAlgorithmTableWidget_h



//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdMacros.h"
#include "ccipdMFATypes.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// Qt includes
#include <QTableWidget>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



class MFAAlgorithmTableWidget : public QTableWidget
{



//////////////////////////////////////////////////////////////////////////////////////////
  Q_OBJECT
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////

  /// Constructor
  explicit MFAAlgorithmTableWidget( QWidget * const parent = 0 );

  /// Destructor
  virtual ~MFAAlgorithmTableWidget();

  typedef MFAAlgorithmTableWidget Self;
  typedef QTableWidget            Superclass;

  typedef int ColumnType;

  /// Which column represents the transform?
  static ColumnType GetTransformColumn();

  /// Which column represents the interpolator?
  static ColumnType GetInterpolatorColumn();
  
  /// Which column represents the % of pixels sampled?
  static ColumnType GetPercentPixelsColumn();
  
  /// Which column represents the UseNCC option?
  static ColumnType GetNCCColumn();
  
  /// Which column represents the # of particles?
  static ColumnType GetParticlesColumn();

  /// Which column represents the # of iterations?
  static ColumnType GetIterationsColumn();

  /// Which column represents the display % option?
  static ColumnType GetDisplayColumn();
  
  /// Which column represents the variance weights option?
  static ColumnType GetVarianceWeightsColumn();
  
  /// Which column represents the likelihood type?
  static ColumnType GetLikelihoodColumn();
  
  /// Which column represents the bounding box mantissa?
  static ColumnType GetBoundingBoxMantissaColumn();

  /// Which column represents the bounding box exponent?
  static ColumnType GetBoundingBoxExponentColumn();
  
  /// Which column represents the known points mantissa?
  static ColumnType GetKnownPointsMantissaColumn();

  /// Which column represents the known points exponent?
  static ColumnType GetKnownPointsExponentColumn();

  /// Which column represents the # of eigenvectors?
  static ColumnType GetMaxEigenvectorsColumn();
  
  /// Make sure the headings have the correct labels
  void UpdateColumnLabels();

//////////////////////////////////////////////////////////////////////////////////////////



private:
//////////////////////////////////////////////////////////////////////////////////////////
  //MFAAlgorithmTableWidget(); ///< Purposely not implemented
//////////////////////////////////////////////////////////////////////////////////////////



}; // class MFAALgorithmTableWidget
} // namespace ccipd
#endif // __ccipdMFAAlgorithmTableWidget_h
