


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd
#include "ccipdMFABoundingBoxTraining.h"
#include "ccipdAffineDistribution.h"
#include "ccipdMultivariateGaussianDistribution.h"
#include "ccipdSIMBoundingBoxExtractor.h"
#include "ccipdStudyAffineAligner.h"
#include "ccipdRegisterBoundingBoxes.h"
#include "ccipdCombinedTransform.h"
#include "ccipdStudy.h"

// boost includes
#include <boost/foreach.hpp>

#include "ccipdSaveImage.h"
#include "ccipdBoundingBox.h"
#include <boost/lexical_cast.hpp>

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageBase.h>
  #include <itkAffineTransform.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



//////////////////////////////////////////////////////////////////////////////////////////
AffineDistributionPointer GetAffineDistribution(
  const StudyConstCollectionType & studies,
  const MFABoundingBoxExtractor  & extractor,
  const StudyAffineAligner       & aligner,
  const unsigned int               repeatsPerStudy )
{
  return GetAffineGaussianDistribution(
    studies, extractor, aligner, repeatsPerStudy );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
AffineDistributionPointer GetAffineDistribution(
  const StudyCollectionType      & studies,
  const MFABoundingBoxExtractor  & extractor,
  const StudyAffineAligner       & aligner,
  const unsigned int               repeatsPerStudy )
{
  StudyConstCollectionType constStudies( studies.cbegin(), studies.cend() );
  return GetAffineDistribution(
    constStudies, extractor, aligner, repeatsPerStudy );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
AffineGaussianDistributionPointer GetAffineGaussianDistribution(
  const StudyCollectionType      & studies,
  const MFABoundingBoxExtractor  & extractor,
  const StudyAffineAligner       & aligner,
  const unsigned int               repeatsPerStudy )
{
  StudyConstCollectionType constStudies( studies.cbegin(), studies.cend() );
  return GetAffineGaussianDistribution(
    constStudies, extractor, aligner, repeatsPerStudy );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
AffineGaussianDistributionPointer GetAffineGaussianDistribution(
  const StudyConstCollectionType & studies,
  const MFABoundingBoxExtractor  & extractor,
  const StudyAffineAligner       & aligner,
  const unsigned int               repeatsPerStudy )
{

  // prepare the affine collection

  VolumeAffineTransformConstCollection
    affineTransforms;

  // typedefs
  typedef MFABoundingBoxExtractor::BoundingBoxType
    BoundingBoxType;
  typedef StudyAffineAligner::LinearTransformConstPointer
    LinearTransformConstPointer;
  typedef StudyAffineAligner::AffineTransformConstPointer
    AffineTransformConstPointer;
  typedef StudyAffineAligner::AffineTransformPointer
    AffineTransformPointer;
  typedef StudyAffineAligner::AffineTransformType
    AffineTransformType;

  const BoundingBoxType templateBoundingBox = extractor.GetTemplateBoundingBox();
  const auto            templateVolume      = extractor.GetTemplateVolume();

  using boost::lexical_cast;
  using std::string;
  using std::cout;
  using std::endl;
  
  const VolumeMaskImageConstPointer templateBoundingBoxMask(
    CreateMaskFromBoundingBox(
      templateBoundingBox, templateVolume.GetPointer() ) );
  
  SaveImage( templateBoundingBoxMask.GetPointer(),
    "template_bounding_box.mha", true );

  if ( !templateVolume || templateBoundingBox.GetNumberOfPixels() == 0 )
    return AffineGaussianDistributionPointer();

  // loop through each study
  BOOST_FOREACH( const StudyConstPointer & study, studies )
    {

    if ( !study ) continue;

    // repeate the extraction multiple times
    #pragma omp parallel for
    for ( int repeat = 0; repeat <= static_cast< int >( repeatsPerStudy ); ++repeat )
      {

      // get the bounding box for this study
      const BoundingBoxType boundingBox = extractor.GetBoundingBox( study );
    
      // and its associated image
      const auto currentVolume =
        extractor.GetBoundingBoxImage( study );

      // get the total transform to be used
      // in this study which, when applied to
      // a point on the trained MFA, will return
      // the corresponding point on the
      // given study
      const AffineTransformConstPointer
        actualTransform( aligner.GetAffineTransform( study ) );
        /*
        actualInverseTransform(
          dynamic_cast< const AffineTransformType * >(
            actualTransform->GetInverseTransform().GetPointer() ) );
        */

      // make sure there is something
      if ( boundingBox.GetNumberOfPixels() == 0 || !currentVolume || !actualTransform )
        continue;

      /*
      const VolumeMaskImageConstPointer boundingBoxMask(
        CreateMaskFromBoundingBox(
          boundingBox, currentVolume.GetPointer() ) );
          
      SaveImage( boundingBoxMask.GetPointer(),
        string( "bounding_box_" ) +
        lexical_cast< string >( repeat ) +
        study->GetDescription() +
        string( ".mha" ),
        true );
        
      SaveImage( currentVolume.GetPointer(),
        string( "bounding_box_volume_" ) +
        lexical_cast< string >( repeat ) +
        study->GetDescription() +
        string( ".mha" ),
        true );
        */
        
      // get the transform mapping the bounding boxes
      const AffineTransformConstPointer boundingBoxTransform(
        GetTransform(
          boundingBox, templateBoundingBox,
          *currentVolume, *templateVolume ) );

      /*
      // only use translation
      auto identity = boundingBoxTransform->GetMatrix();
      identity.SetIdentity();
      boundingBoxTransform->SetMatrix( identity );
      */
     
      if ( !boundingBoxTransform ) continue;

      // now get the transform calculatedTransform such that
      // actualTransform( point ) =
      //   boundingBoxTransform( calculatedTransform( point ) )
      const LinearTransformConstPointer calculatedLinearTransform(
        GetTransformFirst( *actualTransform, *boundingBoxTransform ) );

      // convert it to an affine transform
      const AffineTransformPointer calculatedAffineTransform =
        AffineTransformType::New();

      calculatedAffineTransform->SetMatrix( calculatedLinearTransform->GetMatrix() );
      calculatedAffineTransform->SetOffset( calculatedLinearTransform->GetOffset() );

      // add it to our collection
      #pragma omp critical
        {
        
        affineTransforms.insert( calculatedAffineTransform.GetPointer() );
        
        /*
        cout << study->GetDescription() << endl;
        cout << "actualTransform = " << endl << actualTransform << endl;
        cout << "boundingBoxTransform = " << endl << boundingBoxTransform << endl;
        cout << "calculatedAffineTransform = " << endl << calculatedAffineTransform << endl;
        cout << "# of affine transforms = " << affineTransforms.size() << endl;
        */
        
        } // critial

      } // for repeat

    } // for each study

  // create a gaussian distribution from our affine transforms
  return GetAffineGaussianDistribution( affineTransforms );

} // GetAffineGaussianDistribution
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
