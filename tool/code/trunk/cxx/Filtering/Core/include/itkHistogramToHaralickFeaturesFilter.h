
 #ifndef __HistogramToHaralickFeaturesFilterFilter_h
 #define __HistogramToHaralickFeaturesFilterFilter_h
 
#include "itkHistogram.h"
#include "itkMacro.h"
#include "itkProcessObject.h"
#include "itkSimpleDataObjectDecorator.h"
 
namespace itk 
{
namespace Statistics
{
 template <class THistogram>
 class ITK_EXPORT HistogramToHaralickFeaturesFilter : public ProcessObject
 {
 public:
  /** Standard typedefs */
  typedef HistogramToHaralickFeaturesFilter Self;
  typedef ProcessObject                     Superclass;
  typedef SmartPointer< Self >              Pointer;
  typedef SmartPointer< const Self >        ConstPointer;
 
  /** Run-time type information (and related methods). */
  itkTypeMacro(HistogramToHaralickFeaturesFilter, ProcessObject);
 
  /** standard New() method support */
  itkNewMacro(Self);
 
  typedef THistogram                                         HistogramType;
  typedef typename HistogramType::Pointer                    HistogramPointer;
  typedef typename HistogramType::ConstPointer               HistogramConstPointer;
  typedef typename HistogramType::MeasurementType            MeasurementType;
  typedef typename HistogramType::MeasurementVectorType      MeasurementVectorType;
  typedef typename HistogramType::IndexType                  IndexType;
  typedef typename HistogramType::AbsoluteFrequencyType      AbsoluteFrequencyType;
  typedef typename HistogramType::RelativeFrequencyType      RelativeFrequencyType;
  typedef typename HistogramType::TotalAbsoluteFrequencyType TotalAbsoluteFrequencyType;
  typedef typename HistogramType::TotalRelativeFrequencyType TotalRelativeFrequencyType;
  typedef typename HistogramType::ConstIterator              HistogramIterator;

  
  /** Method to Set/Get the input Histogram */
  using Superclass::SetInput;
  void SetInput(const HistogramType *histogram);

  const HistogramType * GetInput() const;

  /** Smart Pointer type to a DataObject. */
  typedef DataObject::Pointer DataObjectPointer;

  /** Type of DataObjects used for scalar outputs */
  typedef SimpleDataObjectDecorator< MeasurementType > MeasurementObjectType;   

   /** Return Mean texture value. */
   MeasurementType GetMean() const;
   const MeasurementObjectType* GetMeanOutput() const;
   
   /** Return Variance texture value. */
   MeasurementType GetVariance() const;
   const MeasurementObjectType* GetVarianceOutput() const;
   
   /** Return SumAverage texture value. */
   MeasurementType GetSumAverage() const;
   const MeasurementObjectType* GetSumAverageOutput() const;
   
   /** Return SumVariance texture value. */
   MeasurementType GetSumVariance() const;
   const MeasurementObjectType* GetSumVarianceOutput() const;
   
   /** Return SumEntropy texture value. */
   MeasurementType GetSumEntropy() const;
   const MeasurementObjectType* GetSumEntropyOutput() const;
   
   /** Return DifferenceEntropy texture value. */
   MeasurementType GetDifferenceEntropy() const;
   const MeasurementObjectType* GetDifferenceEntropyOutput() const;
   
   /** Return DifferenceVariance texture value. */
   MeasurementType GetDifferenceVariance() const;
   const MeasurementObjectType* GetDifferenceVarianceOutput() const;
   
   /** Return IMC1 texture value. */
   MeasurementType GetIMC1() const;
   const MeasurementObjectType* GetIMC1Output() const;
   
   /** Return IMC2 texture value. */
   MeasurementType GetIMC2() const;
   const MeasurementObjectType* GetIMC2Output() const;
   
   /** Texture feature types */
    typedef enum { Mean, 
                   Variance,
                   SumAverage, 
                   SumVariance, 
                   SumEntropy, 
                   DifferenceEntropy, 
                   DifferenceVariance, 
                   IMC1,
                   IMC2,
                   InvalidFeatureName
                 } ExtendedTextureFeatureName;
 
 /** convenience method to access the texture values */
  MeasurementType GetFeature(ExtendedTextureFeatureName name);
 protected:
  HistogramToHaralickFeaturesFilter();
  virtual ~HistogramToHaralickFeaturesFilter() {}
  void PrintSelf(std::ostream& os, Indent indent) const;
  /** Make a DataObject to be used for output output. */
  typedef ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;
  using Superclass::MakeOutput;
  virtual DataObjectPointer MakeOutput(DataObjectPointerArraySizeType);
  void GenerateData();
 

 
 private:
  HistogramToHaralickFeaturesFilter(const Self &); //purposely not implemented
  void operator =(const Self&);                    //purposely not implemented


}; // class
} // Statistics
} // itk
 
 #ifndef ITK_MANUAL_INSTANTIATION
 #include "itkHistogramToHaralickFeaturesFilter.hxx"
 #endif
 
 #endif

