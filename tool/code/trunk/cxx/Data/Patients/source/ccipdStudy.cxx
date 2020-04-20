


//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdStudy.h"
#include "ccipdVolumeTypes.h" // for Volume image types
#include "ccipdLoadVolume.h"  // for loading images
#include "ccipdLoadMRI.h"     // for loading images
#include "ccipdSaveImage.h"
#include "ccipdLoadTRUS.h"    // for loading images
#include "ccipdLoadTextLandmarks.h"
#include "ccipdFeatureCollection.h" // for feature collections
#include "ccipdHashImage.h"         // for hashing images
#include "ccipdHashLandmarks.h"     // for hashing landmarks
#include "ccipdPreprocessVolume.h"
#include "ccipdCohenMRIBiasCorrection.h" // for preprocessing
#include "ccipdPreprocessMRI.h"          // for preprocessing
#include "ccipdPreprocessGIPL.h"        // for preprocessing
#include "ccipdCloneableFactory.hxx"
#include "ccipdVolumeContainerToVolumeFeatures.h" // to concatenate features into vector image
#include "ccipdDuplicateImage.h"
#include "ccipdDuplicateTransform.h"
#include "ccipdDuplicateLandmarks.h"
#include "ccipdAddMissingSlicesToMask.h"

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageFileReader.h> // for loading masks
  #include <itkPointSet.h>
  #include <itkAffineTransform.h>
  #include <itkBSplineTransform.h>
  #include <itkVectorImage.h>
  #include <itkImageSliceIteratorWithIndex.h>


// std includes
#include <unordered_map> // for our collection of masks
#include <string>        // for file names
#include <iostream>      // for displaying output
#include <fstream>       // for saving files

// boost includes
#include <boost/foreach.hpp>             // for loops
#include <boost/functional/hash.hpp>     // for hashing
#include <boost/property_tree/ptree.hpp>
#include <boost/lexical_cast.hpp>        // for casting between strings/bools

#ifdef Boost_DATE_TIME_FOUND
  #include <boost/date_time/gregorian/gregorian.hpp> // for dates
  #include <boost/date_time/special_defs.hpp>        // for invalid dates
#endif // Boost_DATE_TIME_FOUND
//////////////////////////////////////////////////////////////////////////////////////////

#include "ccipdEnableWarningsMacro.h"


//////////////////////////////////////////////////////////////////////////////////////////
using std::string;
using std::cerr;
using std::cout;
using std::endl;
using std::unordered_map;
using boost::lexical_cast;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{


//////////////////////////////////////////////////////////////////////////////////////////////////
template
class Factory< Study >;

template
class Copyable< Study >;

template
class Cloneable< Study >;

template
class CloneableFactory< Study >;
//////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
class Study::SecretData
{
public: // public to the Study class

  /// Constructor
  SecretData();

  /* Variables */
  typedef unordered_map<
    MaskObjectType,
    VolumeMaskImageConstPointer
  > MasksType;

  typedef unordered_map<
    MaskObjectType,
    string
  > MaskFilesType;

#ifdef Boost_DATE_TIME_FOUND
  DateType                     m_Date;
#endif
  MasksType                    m_Masks;
  MaskFilesType                m_MaskFiles;
  VolumeImageConstPointer      m_Volume;
  VolumeVectorConstPointer     m_TimeSeriesVolume;
  VolumeWarpFieldConstPointer  m_WarpField;
  VolumeAffineTransformConstPointer m_AffineTransform;
  VolumeBSplineTransformConstPointer m_BSplineTransform;
  string                       m_WarpFieldFile;
  string                       m_VolumeFileOrFolder;
  string                       m_TimeSeriesVolumeFileOrFolder;
  LandmarkSetConstPointer      m_LandmarkSet;
  string                       m_LandmarkFile;
  ModalityType                 m_Modality;
  PlaneType                    m_Plane;
  AnnotatorType                m_Annotator;
  DescriptionType              m_Description;
  FeatureCollectionPointer     m_FeatureCollection;
  string                       m_FeatureCollectionFile;
  bool                         m_IgnoreOrientation;
  bool                         m_IgnoreOrigin;
  float                        m_BiasCorrectionSigma; ///< If 0, no correction
  float                        m_CropIntensity;     ///< If 0, no correction
  float                        m_MaximumIntensity;
  float                        m_MinimumIntensity;
  bool                         m_NormalizeVolume;     ///< 0 mean, 1 var
  bool                         m_DenoiseVolume;       ///< Bilateral filter
  string                       m_StudyFolderPath;
};
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// constructors / destructors
Study::Study() :
ccipdNewPIMPLMacro
{
}
Study::~Study()
{
}
Study::SecretData::SecretData() :
m_FeatureCollection  ( FeatureCollection::New() ),
m_IgnoreOrientation  ( false ),
m_IgnoreOrigin       ( false ),
m_BiasCorrectionSigma( 0.0   ),
m_CropIntensity      ( 0.0   ),
m_MaximumIntensity   ( 0.0   ),
m_MinimumIntensity   ( 0.0   ),
m_NormalizeVolume    ( false ),
m_DenoiseVolume      ( false )
{
}

//ccipdClearDataMacro( Study )
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::operator==( const Hashable & other ) const
{

  // cast it to our type
  const Study & otherStudy =
    dynamic_cast< const Study & >( other );

  // we've compared to ourself!
  if ( &otherStudy == this )
    return true;

  // check the ignore orientation variable
  if ( this->GetIgnoreOrientation() != otherStudy.GetIgnoreOrientation() )
    return false;

  // check the ignore orientation variable
  if ( this->GetIgnoreOrigin() != otherStudy.GetIgnoreOrigin() )
    return false;

  // check the study description
  if ( this->GetDescription().compare( otherStudy.GetDescription() ) != 0 )
    return false;

  // check the modality
  if ( this->GetModality().compare( otherStudy.GetModality() ) != 0 )
    return false;
  
  // check the plane
  if ( this->GetPlane().compare( otherStudy.GetPlane() ) != 0 )
    return false;

  // check the modality
  if ( this->GetAnnotator().compare( otherStudy.GetAnnotator() ) != 0 )
    return false;

  // check the volume file location
  if ( this->GetVolumeFileOrFolder() != otherStudy.GetVolumeFileOrFolder() )
    return false;


 // check the volume file location
  if ( this->GetTimeSeriesVolumeFileOrFolder() != otherStudy.GetTimeSeriesVolumeFileOrFolder() )
    return false;

#ifdef Boost_DATE_TIME_FOUND
    if ( *( this->GetDate() ) != *( otherStudy.GetDate() ) )
      return false;
#endif // Boost_DATE_TIME_FOUND

  // each each object's mask
  const MaskObjectCollectionType
    thisObjectNames  = this->GetMaskObjects(),
    otherObjectNames = otherStudy.GetMaskObjects();
  const auto
    otherObjectNamesEnd = otherObjectNames.cend();

  // same number of mask objects?
  if ( thisObjectNames.size() != otherObjectNames.size() )
    return false;

  BOOST_FOREACH( const MaskObjectType & thisObjectName, thisObjectNames )
    {

    // make sure they also have the object
    if ( otherObjectNames.find( thisObjectName ) == otherObjectNamesEnd )
      return false;

    // check the mask file names
    const string
      thisMaskFile  = this     ->GetMaskFile( thisObjectName ),
      otherMaskFile = otherStudy.GetMaskFile( thisObjectName );
    if ( thisMaskFile.compare( otherMaskFile ) != 0 )
      return false;

    // the mask data itself is checked by hash

    } // each object

  //check the landmark file location
  if (this->GetLandmarkFile() != otherStudy.GetLandmarkFile() )
    return false;

  // check the image preprocessing variables
  if ( this->GetBiasCorrectionSigma() != otherStudy.GetBiasCorrectionSigma() )
    return false;

  if ( this->GetNormalizeVolume() != otherStudy.GetNormalizeVolume() )
    return false;

  if ( this->GetDenoiseVolume() != otherStudy.GetDenoiseVolume() )
    return false;

  // check the hash value
  if ( this->GetHash() != otherStudy.GetHash() )
    return false;

  // we've checked everything and haven't returned; we're equal.
  return true;

} // operator==
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
HashValueType Study::ReHash() const
{

  // shortcuts
  using boost::hash_combine;

  // initialize our hasher
  const HashValueType seed = 0;
  HashValueType hashValue = seed;

  // hash all the variables of the object
  hash_combine( hashValue, this->GetDescription() );
  hash_combine( hashValue, this->GetModality   () );
  hash_combine( hashValue, this->GetPlane      () );
  hash_combine( hashValue, this->GetAnnotator  () );

#ifdef Boost_DATE_TIME_FOUND
  hash_combine( hashValue,
    boost::gregorian::to_iso_extended_string(
      *( this->GetDate() ) ) );
#endif // Boost_DATE_TIME_FOUND

  // hash our variable
  hash_combine( hashValue, this->GetIgnoreOrientation  () );
  hash_combine( hashValue, this->GetIgnoreOrigin       () );
  hash_combine( hashValue, this->GetBiasCorrectionSigma() );
  hash_combine( hashValue, this->GetNormalizeVolume    () );
  hash_combine( hashValue, this->GetCropIntensity      () );
  hash_combine( hashValue, this->GetDenoiseVolume      () );

  // hash our image
  hash_combine( hashValue, HashImage( this->GetVolume().GetPointer() ) );

  // hash our warp field
  hash_combine( hashValue, HashImage( this->GetWarpField().GetPointer() ) );


  // hash each object's mask
  const MaskObjectCollectionType objectNames = this->GetMaskObjects();
  hash_combine( hashValue, objectNames.size() );

  BOOST_FOREACH( const MaskObjectType & objectName, objectNames )
    {

    // hash the object name
    hash_combine( hashValue, objectName );

    // hash the mask
    hash_combine( hashValue, HashImage( this->GetMask( objectName ).GetPointer() ) );

    } // each object

  //hash our landmark points
  hash_combine( hashValue, HashLandmarks( this->GetLandmarkSet().GetPointer() ) );

  // output
  return hashValue;

} // ReHash
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Study::OutputToStream( std::ostream & stream ) const
{
  std::string indent = "    ";
  
  // initialize
  const auto maskObjects = this->GetMaskObjects();

  const bool hasDate = this->HasDate();
  const string dateString =
    hasDate ?
    boost::gregorian::to_iso_extended_string( *( this->GetDate() ) ) :
    "";

  indent = "      ";
  // display variables
  stream << indent << "<description>" << this->GetDescription       () << "</description>" << endl;
  stream << indent << "<modality>"    << this->GetModality          () << "</modality>"    << endl;
  stream << indent << "<plane>"       << this->GetPlane             () << "</plane>"       << endl;
  stream << indent << "<annotator>"   << this->GetAnnotator         () << "</annotator>"   << endl;
  stream << indent << "<volume>"      << this->GetVolumeFileOrFolder() << "</volume>"      << endl;
  if (HasTimeSeriesVolume())
    stream << indent << "<timeseriesvolume>"  << this->GetTimeSeriesVolumeFileOrFolder() << "</timeseriesvolume>"  << endl;
  stream << indent << "<date>"        << dateString                    << "</date>"        << endl;
  stream << indent << "<BiasCorrectionSigma>" <<
    this->GetBiasCorrectionSigma() << "</BiasCorrectionSigma>" << endl;
  stream << indent << "<NormalizeVolume>" <<
    this->GetNormalizeVolume() << "</NormalizeVolume>" << endl;
  stream << indent << "<CropIntensity>" <<
    this->GetCropIntensity() << "</CropIntensity>" << endl;
  stream << indent << "<DenoiseVolume>" <<
    this->GetDenoiseVolume() << "</DenoiseVolume>" << endl;
  stream << indent << "<MaximumIntensity>" <<
    this->GetMaximumIntensity() << "</MaximumIntensity>" << endl;
  stream << indent << "<MinimumIntensity>" <<
    this->GetMinimumIntensity() << "</MinimumIntensity>" << endl;
  stream << indent << "<IgnoreOrientation>" <<
    this->GetIgnoreOrientation() << "</IgnoreOrientation>" << endl;
  stream << indent << "<IgnoreOrigin>" <<
    this->GetIgnoreOrigin() << "</IgnoreOrigin>" << endl;

  BOOST_FOREACH ( const MaskObjectType maskObject, maskObjects )
    {

    stream << indent << "<mask>" << endl;
    indent = "        ";
    stream << indent << "<object>" << maskObject << "</object>" << endl;

    // display the mask file if one exists
    const string maskFile = this->GetMaskFile( maskObject );
    if ( !maskFile.empty() )
      stream << indent << "<file>" << maskFile << "</file>" << endl;
    indent = "      ";
    stream << indent << "</mask>" << endl;

    } // for each mask

  if ( HasLandmarkSet() )
    stream << indent << "<landmarks>" << this->GetLandmarkFile()  << "</landmarks>" << endl;

  if ( HasWarpField() )  
    stream << indent << "<warpfield>"  << this->GetWarpFieldFile() << "</warpfield>" << endl;

  // the affine transform
  const VolumeAffineTransformConstPointer affine( this->GetAffineTransform() );

  if ( affine )
    {
    
    //check if affine is identity in which case it doesn't need to be wrote in the xml flie
    bool isAffineTranformEqualWithIdentity = true;
    VolumeAffineTransformPointer 
      identityTransform = VolumeAffineTransformType::New();
    identityTransform->SetIdentity();

    VolumeAffineTransformType::ParametersType 
      affineParams = affine->GetParameters();
    VolumeAffineTransformType::ParametersType 
      identityAffineParams = identityTransform->GetParameters();

    for (unsigned int index = 0; index< affineParams.size(); index++)
      {
      if (affineParams[index]!=identityAffineParams[index])
        isAffineTranformEqualWithIdentity = false;
      }

    if (!isAffineTranformEqualWithIdentity){
      stream << indent << "<Affine_Transform>" << endl;      
      const auto parameters = affine->GetParameters();
      const unsigned int numberOfParameters = 12;
      assert( parameters.size() == numberOfParameters );
        for ( unsigned int parameterIndex = 0; parameterIndex < VolumeDimension; ++parameterIndex )
        {

        // save this tag as Dimension0, Dimension1, ...
        string parameterTagName = string( "Dimension" ) + lexical_cast< string >( parameterIndex ),

        indent = "        ";
        // save it
        stream << indent << "<" << parameterTagName << ">" << endl;
        indent = "          ";
        stream << indent << "<Center>" << 
          affine->GetCenter()[parameterIndex] << "</Center>" << endl;
        stream << indent << "<Translation>" <<  
          affine->GetTranslation()[parameterIndex] 
          << "</Translation>" << endl;
        indent = "        ";
        stream << indent <<  "</" << parameterTagName << ">" << endl;

        } // for parameterIndex

      for ( unsigned int parameterIndex = 0; parameterIndex < numberOfParameters; ++parameterIndex )
        {

        // save this tag as param0, param1, ...
        const string
          parameterTagName = string( "param" ) +
            lexical_cast< string >( parameterIndex ),
          parameterTagValue = lexical_cast< string >( parameters[ parameterIndex ] );
        indent = "        ";
        // save it
        stream << indent << "<" << parameterTagName << ">" <<
          parameterTagValue << "</" << parameterTagName << ">" << endl;

        } // for parameterIndex
      indent = "      ";
      stream << indent << "</Affine_Transform>" << endl;
      }

    } // affine


   // the bSpline transform
  const VolumeBSplineTransformConstPointer  bSpline( this->GetBSplineTransform() );

  if ( bSpline )
    {
    
    //check if transform is identity so is not applied
    bool isBSplineTranformEqualWithIdentity = true;
    VolumeBSplineTransformType::ParametersType deformableParams = bSpline->GetParameters();
    for (unsigned int index = 0; index< deformableParams.size(); index++)
      {
      if (deformableParams[index]!=0.0)
        isBSplineTranformEqualWithIdentity = false;
      }

    if (!isBSplineTranformEqualWithIdentity)
      {
      stream << indent << "<BSpline_Transform>" << endl;
         
      const auto parameters = bSpline->GetParameters();
      const unsigned int numberOfParameters = parameters.size();

       for ( unsigned int parameterIndex = 0; parameterIndex < VolumeDimension; ++parameterIndex )
        {
          
        // save this tag as Dimension0, Dimension1, ...
        string
          parameterTagName = string( "Dimension" ) + lexical_cast< string >( parameterIndex ),

        indent = "        ";
        // save it
        stream << indent << "<" << parameterTagName << ">" << endl;
        indent = "          ";
        stream << indent << "<TransformDomainMeshSize>" << 
          bSpline->GetTransformDomainMeshSize()[parameterIndex] << "</TransformDomainMeshSize>" << endl;
        stream << indent << "<TransformDomainOrigin>" <<  bSpline->GetTransformDomainOrigin()[parameterIndex] 
          << "</TransformDomainOrigin>" << endl;
        stream << indent << "<TransformDomainPhysicalDimensions>" << 
          bSpline->GetTransformDomainPhysicalDimensions()[parameterIndex] << "</TransformDomainPhysicalDimensions>" << endl;
        indent = "        ";
        stream << indent <<  "</" << parameterTagName << ">" << endl;

        } // for parameterIndex

      for ( unsigned int parameterIndex = 0; parameterIndex < numberOfParameters; ++parameterIndex )
        {

        // save this tag as param0, param1, ...
        const string
          parameterTagName = string( "param" ) +
            lexical_cast< string >( parameterIndex ),
          parameterTagValue = lexical_cast< string >( parameters[ parameterIndex ] );
        indent = "        ";
        // save it
        stream << indent << "<" << parameterTagName << ">" <<
          parameterTagValue << "</" << parameterTagName << ">" << endl;

        } // for parameterIndex
      indent = "      ";
      stream << indent << "</BSpline_Transform>" << endl;
      }

    } // deformable

} // OutputToStream
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// XML functions
bool Study::LoadFromXML( const XMLNode & studyRoot )
{

  bool anythingLoaded = false;
  
  // we should first load in the variable
  // defining whether we should ignore the
  // orientation, before trying to
  // load any images

  // ignoreorientation
  string ignoreOrientationString;
  const bool loadedIgnoreOrientation =
    GetXMLString( studyRoot, "ignoreorientation", ignoreOrientationString );

  if ( loadedIgnoreOrientation )
    {

    // initialize
    bool ignoreOrientation = this->GetIgnoreOrientation();

    try 
      {
      ignoreOrientation = lexical_cast< bool >( ignoreOrientationString );
      anythingLoaded = true;
      } // try
    catch( ... ) { }

    this->SetIgnoreOrientation( ignoreOrientation );

    } // loadedIgnoreOrientation 
  
  // ignoreorigin
  string ignoreOriginString;
  const bool loadedIgnoreOrigin =
    GetXMLString( studyRoot, "ignoreorigin", ignoreOriginString );

  if ( loadedIgnoreOrigin )
    {

    // initialize
    bool ignoreOrigin = this->GetIgnoreOrigin();

    try 
      {
      ignoreOrigin = lexical_cast< bool >( ignoreOriginString );
      anythingLoaded = true;
      } // try
    catch( ... ) { }

    this->SetIgnoreOrigin( ignoreOrigin );

    } // loadedIgnoreorigin 

  // bias correction
  string biasCorrectionSigmaString;
  const bool loadedBiasCorrectionSigma =
    GetXMLString( studyRoot, "BiasCorrectionSigma", biasCorrectionSigmaString );
  if ( loadedBiasCorrectionSigma )
    {
      float biasCorrectionSigma = this->GetBiasCorrectionSigma();

      try
        {
        biasCorrectionSigma = lexical_cast< float >( biasCorrectionSigmaString );
        anythingLoaded = true;
        } // try
      catch( ... ) { }

      this->SetBiasCorrectionSigma( biasCorrectionSigma );

    } // loadedBiasCorrectionSigma 

  // normalizevolume
  string normalizeVolumeString;
  const bool loadedNormalizeVolume =
    GetXMLString( studyRoot, "NormalizeVolume", normalizeVolumeString );

  if ( loadedNormalizeVolume )
    {

    // initialize
    bool normalizeVolume = this->GetNormalizeVolume();

    try 
      {
      normalizeVolume = lexical_cast< bool >( normalizeVolumeString );
      anythingLoaded = true;
      } // try
    catch( ... ) { }

    this->SetNormalizeVolume( normalizeVolume );

    } // loadednormalizevolume 

   // intensity cropping
  string intensityCroppingString;
  const bool loadedIntensityCropping =
    GetXMLString( studyRoot, "CropIntensity", intensityCroppingString );
  if ( loadedIntensityCropping )
    {
      float intensityCropping = this->GetCropIntensity();

      try
        {
        intensityCropping = lexical_cast< float >( intensityCroppingString );
        anythingLoaded = true;
        } // try
      catch( ... ) { }

      this->SetCropIntensity( intensityCropping );

    } // CropIntensity 

  // denoisevolume
  string denoiseVolumeString;
  const bool loadedDenoiseVolume =
    GetXMLString( studyRoot, "DenoiseVolume", denoiseVolumeString );

  if ( loadedDenoiseVolume )
    {

    // initialize
    bool denoiseVolume = this->GetDenoiseVolume();

    try 
      {
      denoiseVolume = lexical_cast< bool >( denoiseVolumeString );
      anythingLoaded = true;
      } // try
    catch( ... ) { }

    this->SetDenoiseVolume( denoiseVolume );

    } // loadeddenoisevolume 

   // Maximum Intensity
  string maximumIntensityString;
  const bool loadedMaximumIntensity =
    GetXMLString( studyRoot, "MaximumIntensity", maximumIntensityString );
  if ( loadedMaximumIntensity )
    {
      float maximumIntensity = this->GetMaximumIntensity();

      try
        {
        maximumIntensity = lexical_cast< float >( maximumIntensityString );
        anythingLoaded = true;
        } // try
      catch( ... ) { }

      this->SetMaximumIntensity( maximumIntensity );

    } // MaximumIntensity 


   // Minimum Intensity
  string minimumIntensityString;
  const bool loadedMinimumIntensity =
    GetXMLString( studyRoot, "MinimumIntensity", minimumIntensityString );
  if ( loadedMinimumIntensity )
    {
      float minimumIntensity = this->GetMinimumIntensity();

      try
        {
        minimumIntensity = lexical_cast< float >( minimumIntensityString );
        anythingLoaded = true;
        } // try
      catch( ... ) { }

      this->SetMinimumIntensity( minimumIntensity );

    } // MinimumIntensity 
 
  // modality
  string modality;
  const bool loadedModality = GetXMLString( studyRoot, "modality", modality );
  if ( loadedModality )
    this->SetModality( modality );

  // plane
  string plane;
  const bool loadedPlane = GetXMLString( studyRoot, "plane", plane );
  if ( loadedPlane )
    this->SetPlane( plane );

  // annotator 
  string annotator;
  const bool loadedAnnotator = GetXMLString( studyRoot, "annotator", annotator );
  if ( loadedAnnotator )
    this->SetAnnotator( annotator );

  // volume
  string volumeFile;
  bool loadedVolume = GetXMLString( studyRoot, "volume", volumeFile );

  if ( loadedVolume )
    { 
    const volatile bool verbose = true;

    if (verbose)
      std::cout << "Loading volume "<< volumeFile << std::endl;
   
    // we have some modality-specific load functions
    if ( modality.compare( "MRI" ) == 0 )
      loadedVolume = this->LoadMRIVolume( volumeFile );
    else if ( modality.compare( "TRUS" ) == 0 )
      loadedVolume = this->LoadTRUSVolume( volumeFile );
    else
      loadedVolume = this->LoadVolume( volumeFile );
    }

  if ( loadedVolume )
    {
    std::cout << "Volume " << this->GetVolume()->GetLargestPossibleRegion().GetSize() << endl;
    
    anythingLoaded = true;

    const volatile bool verbose = true;

    // optionally preprocess
    const float biasCorrectionSigma = this->GetBiasCorrectionSigma();
    if ( biasCorrectionSigma > 0.0f )
      {
      if ( verbose )
        cout << "Correcting bias field with sigma = " << biasCorrectionSigma << endl;
      if ( modality.compare( "TRUS" ) == 0 ) // the bias correction is only relevant for TRUS!
        {
        this->SetVolume( CorrectBiasFieldGIPL( 
          this->GetVolume(), 
          biasCorrectionSigma ) );
        } // if ( modality.compare( "TRUS" ) == 0 )
        else
         //      if ( modality.compare( "MRI" ) == 0 ) // the bias correction is only relevant for MRI!
        {
        this->SetVolume( CohenCorrectBiasField(
          this->GetVolume(),
          biasCorrectionSigma ) );
        } // if ( modality.compare( "MRI" ) == 0 )

      if ( verbose ) 
        cout << "Correcting bias field done." << endl;
      }
    if ( this->GetNormalizeVolume() )
      {
      if ( verbose ) cout << "Normalizing:" << endl;
      this->SetVolume( NormalizeVolume( this->GetVolume() ) );
      if ( verbose ) cout << "Normalizing done." << endl;
      }

    // optionally preprocess
    const float cropIntensities = this->GetCropIntensity();
    if ( cropIntensities > 0.0f )
      {
      if ( verbose ) cout << "Cropping intensities above standard deviation = " << cropIntensities << endl;
      this->SetVolume( CropIntensitiesVolume( this->GetVolume(), -cropIntensities, cropIntensities )  ); 
      if ( verbose ) cout << "Cropping intensities done." << endl;
      }

    if ( this->GetDenoiseVolume() )
	  {
      if ( verbose ) cout << "Denoising:" << endl;   
		      if ( modality.compare( "TRUS" ) == 0 ) // the bias correction is only relevant for TRUS!
        {
          this->SetVolume( ApplyBilateralFilterVolume( this->GetVolume(), 0.1, 0.1 ) );
        } // if ( modality.compare( "TRUS" ) == 0 )
        else
          //      if ( modality.compare( "MRI" ) == 0 ) // the bias correction is only relevant for MRI!
          {
          this->SetVolume( ApplyBilateralFilterVolume( this->GetVolume() ) );
          } // if ( modality.compare( "MRI" ) == 0 )   
      if ( verbose ) cout << "Denoising done." << endl;      
      }

    // optionally preprocess
    float maximumIntensity = this->GetMaximumIntensity();
    float minimumIntensity = this->GetMinimumIntensity();
    if ( maximumIntensity!=0 || minimumIntensity!=0 )
      {
      if ( minimumIntensity > maximumIntensity ) // swap intensity values if they are backwards
        {  
        const float oldminimum = minimumIntensity;
        minimumIntensity = maximumIntensity;
        maximumIntensity = oldminimum;
        }
      if ( verbose ) cout << "Rescaling intensities within range " << minimumIntensity << " to " << maximumIntensity << "." << endl;
      this->SetVolume( RescaleIntensitiesVolume( this->GetVolume(), minimumIntensity, maximumIntensity )  );      
      if ( verbose ) cout << "Rescaling intensities done." << endl;   
    } //    if ( maximumIntensity!=0 || minimumIntensity!=0 )
  
  } // if loadedVolume

  // timeSeriesVolume
  string timeSeriesVolumeFile;
  loadedVolume = GetXMLString( studyRoot, "timeseriesvolume", timeSeriesVolumeFile );

  if ( loadedVolume )
    {
    loadedVolume = this->LoadTimeSeriesVolume( timeSeriesVolumeFile );
    } // loadedVolume


  // description
  string description;
  const bool loadedDescription = GetXMLString( studyRoot, "description", description );
  if ( loadedDescription )
    {
    anythingLoaded = true;

    this->SetDescription( description );
    }

#ifdef Boost_DATE_TIME_FOUND
  // date
  string date;
  const bool loadedDate = GetXMLString( studyRoot, "date", date );
  if ( loadedDate )
    {
    anythingLoaded = true;

    this->SetDate( date );
    }
#endif // Boost_DATE_TIME_FOUND

  // go through each child with the "mask" tag
  const auto maskNodes = GetXMLChildNodes( studyRoot, "mask" );
  BOOST_FOREACH( const auto & maskNode, maskNodes )
    {
    const bool loadedMask = this->LoadMaskFromXML( *maskNode );
    
    if ( loadedMask )
      anythingLoaded = true;

    } // for each mask

  // landmarks
  string landmarkFile;
  bool loadedLandmarks = GetXMLString( studyRoot, "landmarks", landmarkFile );
  if ( loadedLandmarks )
    {
    loadedLandmarks = this->LoadLandmarks( landmarkFile );
    
    if ( loadedLandmarks )
      anythingLoaded = true;

    }

  // warpfield
  string warpfieldFile;
  bool loadedWarpField = GetXMLString( studyRoot, "warpfield", warpfieldFile );
  if ( loadedWarpField )
    {
    loadedWarpField = this->LoadWarpFieldVolume( warpfieldFile );

    if ( loadedWarpField )
      anythingLoaded = true; 
    }

  // affine transform
  const XMLPointerCollectionType affineNodes =
    GetXMLChildNodes( studyRoot, "affine_transform" );

  if ( affineNodes.size() >= 1 )
    {
    
    if ( affineNodes.size() > 1 )
      std::cout << "Warning: Only using the first affine transform." << std::endl;

    const XMLPointer affineNode = *affineNodes.cbegin();
    if ( affineNode )
      {

      // create a default affine transform
      const VolumeAffineTransformPointer affineTransform =
        VolumeAffineTransformType::New();
      affineTransform->SetIdentity();

      VolumeAffineTransformType::CenterType       center;
      VolumeAffineTransformType::TranslationType  translation;

      // loop through each parameter that we have
      for ( unsigned int parameterIndex = 0; parameterIndex < VolumeDimension; ++parameterIndex )
      { 
      string 
        parameterTagName = string( "Dimension" ) + lexical_cast< string >(
        parameterIndex );
      
      const XMLPointerCollectionType dimensionNodes =
        GetXMLChildNodes( *affineNode, parameterTagName );

      if (dimensionNodes.size()>=1)
        {
        const XMLPointer dimensionNode = *dimensionNodes.cbegin();
        if (dimensionNode)
          {
          string CenterString;
          const bool loadedCenter =
           GetXMLString( *dimensionNode, "Center",   CenterString );
          if ( loadedCenter )
            {
            try
              {
              center[parameterIndex] = lexical_cast< double >( CenterString );
              anythingLoaded = true;
              } // try
            catch( ... ) { }
            }
          else
            {
             cerr << "Warning: unable to load center for affineTransform " << endl;
            }

          string TranslationString;
          const bool loadedTranslation =
          GetXMLString( *dimensionNode, "Translation", TranslationString );
          if ( loadedTranslation )
            {
            try
              {
              translation[parameterIndex] = lexical_cast< float >( TranslationString );
              anythingLoaded = true;
              } // try
            catch( ... ) { }
            }
          else
            {
            cerr << "Warning: unable to load Translation for affine " << endl;
            }
          }
        }
      }

      affineTransform->SetCenter( center );
      affineTransform->SetTranslation( translation );

      // loop through each parameter that we have
      typedef VolumeAffineTransformType::ParametersType AffineParametersType;
      AffineParametersType parameters = affineTransform->GetParameters();

      const unsigned int numberOfParameters = 12;
      assert( parameters.size() == numberOfParameters );
      for ( unsigned int parameterIndex = 0; parameterIndex < numberOfParameters; ++parameterIndex )
        {

        // try to load this tag as param0, param1, ...
        const string parameterTagName = string( "param" ) +
          lexical_cast< string >( parameterIndex );

        string parameterTagValue;
        const bool loadedParameterTag = GetXMLString(
          *affineNode, parameterTagName, parameterTagValue );

        if ( loadedParameterTag )
          {

          // try to cast it as a parameter value
          try 
            {
            const AffineParametersType::ValueType parameterValue =
              lexical_cast< AffineParametersType::ValueType >( parameterTagValue );

            // set it if successful
            parameters[ parameterIndex ] = parameterValue;

            } // try
          catch( ... ) {} // casting errors

          } // loadedParameterTag 

        } // for parameterIndex
      
      // set it
      anythingLoaded = true;
      affineTransform->SetParameters( parameters );
      this->SetAffineTransform( affineTransform );

      } // affineNode

    } // size > 1
      
  // deformable transform
  const XMLPointerCollectionType bSplineNodes =
    GetXMLChildNodes( studyRoot, "bspline_transform" );

  if ( bSplineNodes.size() >= 1 )
    {
    
    if ( bSplineNodes.size() > 1 )
      std::cout << "Warning: Only using the first bSpline transform." << std::endl;

    const XMLPointer bSplineNode = *bSplineNodes.cbegin();
    if (  bSplineNode )
      {

      // create a default bspline transform
      const VolumeBSplineTransformPointer bSplineTransform =
        VolumeBSplineTransformType::New();

      VolumeBSplineTransformType::OriginType               origin;
      VolumeBSplineTransformType::PhysicalDimensionsType   physicalDimensions;
      VolumeBSplineTransformType::MeshSizeType             meshSize;
 
      // loop through each parameter that we have
      unsigned int numberOfParameters = VolumeDimension;
      for ( unsigned int parameterIndex = 0; parameterIndex < VolumeDimension; ++parameterIndex )
      { 
      string 
        parameterTagName = string( "Dimension" ) + lexical_cast< string >(
        parameterIndex );
      
      const XMLPointerCollectionType dimensionNodes =
        GetXMLChildNodes( *bSplineNode, parameterTagName );


      if (dimensionNodes.size()>=1)
        {
        const XMLPointer dimensionNode = *dimensionNodes.cbegin();
        if (dimensionNode)
          {
          string TransformDomainMeshSizeString;
          const bool loadedTransformDomainMeshSize =
           GetXMLString( *dimensionNode, "TransformDomainMeshSize",   TransformDomainMeshSizeString );
          if ( loadedTransformDomainMeshSize )
            {
            try
              {
              meshSize[parameterIndex] = lexical_cast< itk::SizeValueType >( TransformDomainMeshSizeString );
              numberOfParameters = static_cast< unsigned int >(
                numberOfParameters * (meshSize[parameterIndex]+splineOrder) );
              anythingLoaded = true;
              } // try
            catch( ... ) { }
            }
          else
            {
             cerr << "Warning: unable to load TransformDomainMeshSize for bSplineTransform " << endl;
            }

          string TransformDomainOriginString;
          const bool loadedTransformDomainOrigin =
          GetXMLString( *dimensionNode, "TransformDomainOrigin", TransformDomainOriginString );
          if ( loadedTransformDomainOrigin )
            {
            try
              {
              origin[parameterIndex] = lexical_cast< float >( TransformDomainOriginString );
              anythingLoaded = true;
              } // try
            catch( ... ) { }
            }
          else
            {
             cerr << "Warning: unable to load TransformDomainOrigin for bSplineTransform " << endl;
            }

         string TransformDomainPhysicalDimensionsString;
          const bool loadedTransformDomainPhysicalDimensions =
          GetXMLString( *dimensionNode, "TransformDomainPhysicalDimensions", TransformDomainPhysicalDimensionsString );
          if ( loadedTransformDomainPhysicalDimensions )
            {
            try
              {
              physicalDimensions[parameterIndex] = lexical_cast< float >( TransformDomainPhysicalDimensionsString );
              anythingLoaded = true;
              } // try
            catch( ... ) { }
            }
          else
            {
             cerr << "Warning: unable to load TransformDomainPhysicalDimensions for bSplineTransform " << endl;
            }
          }
        }
      }
   
      VolumeBSplineTransformType::ParametersType parameters;
      parameters.SetSize( numberOfParameters );
      parameters.fill(0.0);


      bSplineTransform->SetTransformDomainOrigin               ( origin );
      bSplineTransform->SetTransformDomainPhysicalDimensions	 ( physicalDimensions );
      bSplineTransform->SetTransformDomainMeshSize             ( meshSize );


      for ( unsigned int parameterIndex = 0; parameterIndex < numberOfParameters; ++parameterIndex )
        {

        // try to load this tag as param0, param1, ...
        const string parameterTagName = string( "param" ) +
          lexical_cast< string >( parameterIndex );

        string parameterTagValue;
        const bool loadedParameterTag = GetXMLString(
          *bSplineNode, parameterTagName, parameterTagValue );

        if ( loadedParameterTag )
          {

          // try to cast it as a parameter value
          try 
            {
            const VolumeBSplineTransformType::ParametersType::ValueType parameterValue =
              lexical_cast< VolumeBSplineTransformType::ParametersType::ValueType >( parameterTagValue );

            // set it if successful
            parameters[ parameterIndex ] = parameterValue;

            } // try
          catch( ... ) {} // casting errors

          } // loadedParameterTag 

        } // for parameterIndex
      
      // set it
      anythingLoaded = true;
      bSplineTransform->SetParameters( parameters );
      this->SetBSplineTransform ( bSplineTransform );

      } // bsplineNode

    } // size > 1
  

  return anythingLoaded;
  
} // LoadStudyFromXML
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::LoadMaskFromXML( const XMLNode & maskRoot )
{

  // prepare our strings
  string 
    maskObject, 
    maskFile,
    correspondingSlicesFn;

  // extract the mask object
  GetXMLString( maskRoot, "object", maskObject );

  // extract the mask
  bool loadedMask = GetXMLString( maskRoot, "file", maskFile );

  // try to give the mask to the study
  loadedMask = loadedMask && this->LoadMask( maskFile, maskObject );

  // update our list of files (for outputting masks)
  if ( loadedMask )
    this->m_SecretData->m_MaskFiles[ maskObject ] = maskFile;
  else
    std::cout << "Trying to load mask \"" <<  maskFile << "\" failed!!!" << std::endl; 

  //the presence of this field means the file volume contains only slices with
  //annotations but not all slices. Read the vol and the corresponding slices
  //and create a real mask, same dimensions as the volume 

  // extract the mask
  bool loadedCorrepondingSlices = GetXMLString( maskRoot, "sliceCorrespondence", correspondingSlicesFn );

  if (loadedCorrepondingSlices)
  {
  // update our list of files (for outputting masks)
    UpdateIncompleteMask(maskObject, correspondingSlicesFn);
  }

  return loadedMask;

} // LoadMaskFromXML
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
/**
 * typically happening when disease annotations are done on some slices and
 * not all; then we have data on some slices, but not all; the param
 * slicesFn tells which slices are represented in the file
 */
bool Study::UpdateIncompleteMask (
  const MaskObjectType maskObject,
  const std::string slicesFn)
{

  if (!this->HasVolume() || !this->HasMask( maskObject ) )
    return false;
   

  VolumeImageType::RegionType 
    regionVol         = this->GetVolume()->GetLargestPossibleRegion(),
    regionLoadedMask  = this->GetMask(maskObject)->GetLargestPossibleRegion(); 

  if (regionVol.GetSize() == regionLoadedMask.GetSize()) // nothing to do as we don't replace slices info
    return false;
        
  //Step1: read slice numbers 
  cout << "Read slice info from: " << slicesFn << " for " <<  this->GetDescription() <<  endl;

  //FIXME: use C++ style to read files... 
  FILE * file = NULL;
  std::vector<int> slices;
  file = fopen(slicesFn.c_str(), "r");
  if (file==NULL) // 
    {
    std::cout << " Cant read slices from " << slicesFn << std::endl;
    return false;
    }
  

  int slice = 0;
  std::cout << "Reading slices " << std::endl;
  while (!feof(file))
  {
    if(fscanf(file, "%d", &slice)>0)// success reading
      {
      slices.push_back(slice);
      std::cout << slice << " " ;
     }
  }
  fclose(file);
  std::cout << std::endl;
  

  //Step 2: create new mask with the right size: same as orginal volume
  VolumeMaskImagePointer newMask = VolumeMaskImageType::New();
  newMask->SetRegions( this->GetVolume()->GetLargestPossibleRegion() );
  newMask->SetOrigin( this->GetVolume()->GetOrigin() );
  newMask->SetDirection( this->GetVolume()->GetDirection() );
  newMask->SetSpacing( this->GetVolume()->GetSpacing() );
  newMask->Allocate();
  newMask->FillBuffer(0);
 
  //Step 3: go through each slice
  typedef itk::ImageSliceIteratorWithIndex
    < VolumeMaskImageType > SliceIteratorType;
  typedef itk::ImageSliceIteratorWithIndex
    < const VolumeMaskImageType > SliceConstIteratorType;

  SliceIteratorType itNewMask ( newMask, newMask->GetLargestPossibleRegion() );
  SliceConstIteratorType itMask    ( (this->GetMask(maskObject)).GetPointer(),  
    this->GetMask(maskObject)->GetLargestPossibleRegion()    );

  itMask.SetFirstDirection(0); // x?
  itMask.SetSecondDirection(1); // y?
  itNewMask.SetFirstDirection(0); // x?
  itNewMask.SetSecondDirection(1); // y?
 
  itMask.GoToBegin();
  while( !itMask.IsAtEnd() )
  {
    itNewMask.GoToBegin();
    while( !itNewMask.IsAtEnd() &&  itNewMask.GetIndex()[2] != slices[ itMask.GetIndex()[2] ]-1  )
      itNewMask.NextSlice();

    while( !itMask.IsAtEndOfSlice() )
    {
      while( !itMask.IsAtEndOfLine() )
          {
            itNewMask.Set( itMask.Get() );
              ++itNewMask;
              ++itMask;
          }
        itMask.NextLine();
    }
    itMask.NextSlice();
  }

  //Step 4:
  AddMissingData(newMask, slices);
  
  //Step 5: replace the given mask with the new mask
  this->SetMask(newMask, maskObject);
  this->m_SecretData->m_MaskFiles[ maskObject ] = "NeedsNewName";

  return true; 
};


//////////////////////////////////////////////////////////////////////////////////////////
bool Study::LoadMaskFromXML( const string & xmlFile )
{

  return this->LoadMaskFromXML( *GetXMLRootNode( xmlFile ) );

} // LoadMaskFromXML
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_DATE_TIME_FOUND
void Study::SetDate( const DatePointer date )
{

  this->m_SecretData->m_Date = *date;
  this->ClearHash();

} // SetDate
#endif // Boost_DATE_TIME_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_DATE_TIME_FOUND
bool Study::HasDate() const
{

  return !( this->m_SecretData->m_Date.is_not_a_date() );

} // SetDate
#endif // Boost_DATE_TIME_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
#ifdef Boost_DATE_TIME_FOUND
DatePointer Study::GetDate() const
{

  return DatePointer( new DateType( this->m_SecretData->m_Date ) );

} // SetDate
#endif // Boost_DATE_TIME_FOUND
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Study::SetMask(
  const VolumeMaskImageType * mask,
  const MaskObjectType      & maskObject )
{

  // set the variable
  m_SecretData->m_Masks[ maskObject ] = mask;

  // uncache our hash value, so we have to recalculate it
  this->ClearHash();

} // SetMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Study::EraseMask( const MaskObjectType      & maskObject)
{
  if (HasMask(maskObject))
  {
    m_SecretData->m_Masks.erase(maskObject); // erase mask
    m_SecretData->m_MaskFiles.erase(maskObject);  //erase filename
  }

  // uncache our hash value, so we have to recalculate it
  this->ClearHash();

}; //EraseMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Study::EraseMasks(const MaskObjectCollectionType & maskObjects )
{
  BOOST_FOREACH(const auto maskObject, maskObjects )
    EraseMask(maskObject);
};//EraseMasks
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Study::EraseVolume( )
{

  this->SetVolume( NULL ); // delete volume
  this->SetVolumeFileOrFolder("");

}; //EraseVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Study::EraseTimeSeriesVolume( )
{

  this->SetTimeSeriesVolume( NULL ); // delete volume
  this->SetTimeSeriesVolumeFileOrFolder("");

}; //EraseTimeSeriesVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::LoadMask(
  const string         & fileName,
  const MaskObjectType & maskObject )
{

  const VolumeMaskImagePointer loadedMask =
    LoadVolumeMask( fileName, this->GetIgnoreOrientation() );

  if ( loadedMask )
    {

    if ( this->GetIgnoreOrigin() )
      {
      VolumeMaskImageType::PointType origin;
      origin.Fill( 0.0 );
      loadedMask->SetOrigin( origin );
      }

    // we've succeeded.  set it.
    this->SetMask( loadedMask, maskObject );
  
    // success!
    return true;

    } // loadedMask
  else
    return false;

} // LoadMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
string Study::GetMaskFile(
  const MaskObjectType & maskObject ) const
{

  if ( this->HasMask( maskObject ) )
    return this->m_SecretData->m_MaskFiles[ maskObject ];
  else
    return string();

} // GetMaskFile
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
void Study::SetMaskFile(const std::string & filename, const MaskObjectType & maskObject)
{
  if ( this->HasMask( maskObject ) )
    this->m_SecretData->m_MaskFiles[ maskObject ] =  filename;
};
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::HasMask( const MaskObjectType & maskObject ) const
{

  // try to find the given mask
  const auto maskIter = m_SecretData->m_Masks.find( maskObject );

  // did we find one?
  const bool found = ( maskIter != m_SecretData->m_Masks.end() );
  return found;

} // HasMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::HasMasks( const MaskObjectCollectionType & maskObjects ) const
{

  // go through each mask
  BOOST_FOREACH( const MaskObjectType & maskObject, maskObjects )
    {

    // try to find the given mask
    const auto maskIter = m_SecretData->m_Masks.find( maskObject );

    // did we find one?
    const bool found = ( maskIter != m_SecretData->m_Masks.end() );

    // if any aren't found, return false
    if ( !found )
      return false;

    } // for each mask object

  // if we've reached here, we've made sure all the masks exist
  return true;

} // HasMasks
//////////////////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImageConstPointer Study::GetOneMask() const
{

  const auto & masks = m_SecretData->m_Masks;

  // if we have a mask return it; otherwise, return NULL
  if ( masks.empty() )
    return NULL;
  else
    // return the first mask
    return masks.cbegin()->second;

} // GetOneMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeMaskImageConstPointer Study::GetMask(
  const MaskObjectType & maskObject ) const
{

  // if we have the mask return it; otherwise, return NULL
  if ( this->HasMask( maskObject ) )
    return m_SecretData->m_Masks[ maskObject ];
  else
    return NULL;

} // GetMask
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
unsigned int Study::GetNumberOfMasks() const
{

  return static_cast< unsigned int >( m_SecretData->m_Masks.size() );

} // GetNumberOfMasks
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::LoadVolume( const string & fileOrFolderName )
{

  // try to load the image
  const VolumeImagePointer volume(
    ccipd::LoadVolume( fileOrFolderName, this->GetIgnoreOrientation() ) );

  if ( volume )
    {

    if ( this->GetIgnoreOrigin() )
      {
      VolumeImageType::PointType origin;
      origin.Fill( 0.0 );
      volume->SetOrigin( origin );
      }

    this->SetVolume( volume );
    this->m_SecretData->m_VolumeFileOrFolder = fileOrFolderName;
    return true;
    }
  else // no image loaded
    {
      
    this->m_SecretData->m_VolumeFileOrFolder = fileOrFolderName; // this is for debugging purposed for eileen's code.
    return false;
    }

} // LoadVolume
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
bool Study::LoadTimeSeriesVolume( const string & fileOrFolderName )
{

  // try to load the image
  const VolumeVectorPointer volume(
    ccipd::LoadTimeSeries( fileOrFolderName, this->GetIgnoreOrientation() ) );

  if ( volume )
    {

    if ( this->GetIgnoreOrigin() )
      {
      VolumeImageType::PointType origin;
      origin.Fill( 0.0 );
      volume->SetOrigin( origin );
      }

    this->SetTimeSeriesVolume( volume );
    this->m_SecretData->m_TimeSeriesVolumeFileOrFolder = fileOrFolderName;
    return true;
    }
  else // no image loaded
    {
    return false;
    }

} // LoadVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::LoadMRIVolume( const string & fileOrFolderName )
{

  // try to load the image
  const VolumeImagePointer volume(
    ccipd::LoadMRI( fileOrFolderName, this->GetIgnoreOrientation() ) );

 
  if ( volume )
    {
    if ( this->GetIgnoreOrigin() )
      {
      VolumeImageType::PointType origin;
      origin.Fill( 0.0 );
      volume->SetOrigin( origin );
      }

    this->SetVolume( volume );
    this->m_SecretData->m_VolumeFileOrFolder = fileOrFolderName;

    return true;
    }
  else // no image loaded
    {
    return false;
    }

} // LoadMRIVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::LoadTRUSVolume( const string & fileName )
{

  // try to load the image
  const VolumeImagePointer volume(
    LoadTRUSFromFile( fileName, this->GetIgnoreOrientation() ) );

  if ( volume )
    {

    if ( this->GetIgnoreOrigin() )
      {
      VolumeImageType::PointType origin;
      origin.Fill( 0.0 );
      volume->SetOrigin( origin );
      }

    this->SetVolume( volume );
    this->m_SecretData->m_VolumeFileOrFolder = fileName;
    return true;
    }
  else // no image loaded
    {
    return false;
    }

} // LoadTRUSVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::LoadLandmarks( const std::string & fileName )
{

  // try to load the image
  const LandmarkSetConstPointer landmarks = LoadConstLandmarkSetFromTextFile( fileName );

  if ( landmarks )
    {
    this->SetLandmarkSet(landmarks);
    this->m_SecretData->m_LandmarkFile = fileName;
    return true;
    }
  else // no image loaded
    {
    return false;
    }

} // LoadLandmarks
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Study::SaveContent(const std::string dir, const std::string filename_prefix, const std::string extension) 
{
  //set filenames
  SetVolumeFileOrFolder( dir + "/"+filename_prefix+"_vol."+ extension );
  SetTimeSeriesVolumeFileOrFolder( dir + "/"+filename_prefix+"_timeSeries."+ extension );
  
  const MaskObjectCollectionType MaskObjects = GetMaskObjects();
  BOOST_FOREACH(const MaskObjectType MaskObject, MaskObjects )
    SetMaskFile( dir + "/"+filename_prefix+"_label_"+string(MaskObject)+"."+extension, MaskObject);

  SetWarpFieldFile( dir + "/"+filename_prefix+"_warpfield."+ extension );
  SetLandmarkFile ( dir + "/"+filename_prefix+"_landmarks.vtk" );
  SetFeatureCollectionFile ( dir + "/"+filename_prefix+"_features."+ extension );

  // now actually saved the data
  SaveContent();
};
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Study::SaveContent() const
{
  std::cout << "Saving Study: " << std::endl;
  if (HasVolume())
    {
    ccipd::SaveImage( this->m_SecretData->m_Volume.GetPointer(), 
      this->m_SecretData->m_VolumeFileOrFolder, true );
    std::cout << " Volume to: " <<  this->m_SecretData->m_VolumeFileOrFolder << std::endl;
    }
  
  if (HasTimeSeriesVolume())
    {
    ccipd::SaveImage( this->m_SecretData->m_TimeSeriesVolume.GetPointer(), 
      this->m_SecretData->m_TimeSeriesVolumeFileOrFolder, true );
    std::cout << " Time series to: " <<  this->m_SecretData->m_TimeSeriesVolumeFileOrFolder << std::endl;
    }
 
  const MaskObjectCollectionType MaskObjects = GetMaskObjects();
  BOOST_FOREACH(const MaskObjectType maskObject, MaskObjects )
    {
      if (HasMask(maskObject))
        { 
        ccipd::SaveImage(m_SecretData->m_Masks[ maskObject ].GetPointer(), 
          m_SecretData->m_MaskFiles[ maskObject ], true);
        }
    }

  if (HasLandmarkSet())
    {
    ccipd::SaveLandmarks(this->m_SecretData->m_LandmarkSet, this->m_SecretData->m_LandmarkFile, true);
    }

  if (HasWarpField())
    {
    ccipd::SaveImage(this->m_SecretData->m_WarpField.GetPointer(), this->m_SecretData->m_WarpFieldFile, true);
    }

  if (HasFeatureCollection())
    {
    ccipd::SaveImage( this->GetFeatureCollection()->GetFeatures( 
      GetFeatureCollection()->GetWhichFeatures() ).GetPointer(),
      GetFeatureCollectionFile(), true);
    }
   
  std::cout << "Done Saving!" << std::endl;

}; 
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::LoadWarpFieldVolume( const std::string & fileName )
{

  // try to load the image
  const VolumeWarpFieldPointer warpfield = LoadVolumeWarpField( fileName );

  if ( warpfield )
    {
    this->SetWarpField(warpfield);
    this->m_SecretData->m_WarpFieldFile = fileName;
    return true;
    }
  else // no image loaded
    {
    return false;
    }

} // LoadWarpFieldVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::HasWarpField() const
{

  // whether or not a volume exists
  return this->GetWarpField().IsNotNull();

} // HasWarpField
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MaskObjectCollectionType Study::GetMaskObjects() const
{

  // prepare our output
  MaskObjectCollectionType objectNames;

  // go through each mask
  BOOST_FOREACH( const auto & maskObject, m_SecretData->m_Masks )
	  {
	  objectNames.insert( maskObject.first );
	  }

  // output
  return objectNames;

} // GetMaskObjects
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::SetDate( const string & dateString )
{

  // try all the different known date formats

  try
    {
    using boost::gregorian::from_simple_string;
    this->SetDate( DatePointer( new DateType(from_simple_string( dateString ) ) ) );
    return true;
    }
  catch( ... )
    {
    }

  try
    {
    using boost::gregorian::from_undelimited_string;
    this->SetDate( DatePointer( new DateType(from_undelimited_string( dateString ) ) ) );
    return true;
    }
  catch( ... )
    {
    }

  return false;

} // SetDate
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::HasVolume() const
{

  // whether or not a volume exists
  return this->GetVolume().IsNotNull();

} // HasVolume
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::HasTimeSeriesVolume() const
{

  // whether or not a volume exists
  return this->GetTimeSeriesVolume().IsNotNull();

} // HasTimeSeriesVolume
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
bool Study::HasAffineTransform() const
{

  // whether or not an affine transform exists
  return this->GetAffineTransform().IsNotNull();

} // HasAffineTransform
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::HasBSplineTransform() const
{

  // whether or not an affine transform exists
  return this->GetBSplineTransform().IsNotNull();

};
// HasBSplineTransform
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::HasLandmarkSet() const
{

  // whether or not an affine transform exists
  return this->GetLandmarkSet().IsNotNull();

} // HasLandmarkSet
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool Study::HasFeatureCollection() const
{

  // whether or not a volume exists
  return this->GetFeatureCollection()->GetFeatures(this->GetFeatureCollection()->GetWhichFeatures()).IsNotNull();

} // HasFeatureCollection
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Study::EraseFeature(const FeatureIDConstPointer featureID)
{
  //clear the feature
  this->GetFeatureCollection()->UnsetFeature( featureID );

  this->ClearHash();

};//EraseFeature
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void Study::EraseFeatures(const FeatureIDConstCollection featureIDs)
{
  
  //clear all features are they need to be recomputed
  BOOST_FOREACH(const FeatureIDConstPointer featureID, featureIDs )
    EraseFeature( featureID );

}; // EraseFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeAffineTransformConstPointer Study::GetAffineTransform() const
{
  return VolumeAffineTransformConstPointer( this->m_SecretData->m_AffineTransform );
} // GetAffineTransform
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
VolumeBSplineTransformConstPointer Study::GetBSplineTransform() const
{
  return VolumeBSplineTransformConstPointer( this->m_SecretData->m_BSplineTransform );
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
void Study::CopyParametersInternal( const Self & other )
{
  MaskObjectCollectionType maskObjects = other.GetMaskObjects();
  BOOST_FOREACH( const MaskObjectType & maskObject, maskObjects )
    {
    SetMask( DuplicateImage(other.GetMask( maskObject ).GetPointer()), maskObject );
    this->m_SecretData->m_MaskFiles[ maskObject ] = other.GetMaskFile ( maskObject );
    }

  //recreating feature collection before recreating volume
  FeatureCollectionPointer featureCollection = this->GetFeatureCollection(); 
  FeatureIDConstCollection allFeatureIDs =
    featureCollection->GetWhichFeatures();
       
  //clear all features before they are added
  BOOST_FOREACH(const FeatureIDConstPointer featureID, allFeatureIDs )
    featureCollection->UnsetFeature(featureID);

  allFeatureIDs = other.GetFeatureCollection()->GetWhichFeatures();
       
  //clear all features as they need to be recomputed
  BOOST_FOREACH(const FeatureIDConstPointer featureID, allFeatureIDs )
    featureCollection->SetFeature( 
      featureID, 
      DuplicateImage(other.GetFeatureCollection()->GetFeature(featureID).at(0).GetPointer()) 
    );
  this->SetFeatureCollection(featureCollection);

  // direct setting of other values
  this->m_SecretData->m_WarpFieldFile       = other.GetWarpFieldFile();
  this->m_SecretData->m_VolumeFileOrFolder  = other.GetVolumeFileOrFolder();
  this->m_SecretData->m_LandmarkFile        = other.GetLandmarkFile();
  
   
  SetVolume               ( DuplicateImage(other.GetVolume().GetPointer())  );
  SetAffineTransform      ( dynamic_cast< VolumeAffineTransformType *>  ( 
    DuplicateTransform(other.GetAffineTransform() ).GetPointer())  );
  SetBSplineTransform     ( dynamic_cast< VolumeBSplineTransformType *> (
    DuplicateTransform(other.GetBSplineTransform()).GetPointer()) );
  if (other.HasLandmarkSet())
    SetLandmarkSet          ( DuplicateLandmarkSet( other.GetLandmarkSet() ) );
  if (other.HasWarpField())
    SetWarpField            ( other.GetWarpField()          ); //FIXME duplicate
  SetModality             ( other.GetModality()           );
  SetPlane                ( other.GetPlane()              ); 
  SetDescription          ( other.GetDescription()        );
  SetIgnoreOrientation    ( other.GetIgnoreOrientation()  );
  SetIgnoreOrigin         ( other.GetIgnoreOrigin()       );
  SetBiasCorrectionSigma  ( other.GetBiasCorrectionSigma());
  SetCropIntensity        ( other.GetCropIntensity()      );
  SetNormalizeVolume      ( other.GetNormalizeVolume()    );
  SetDenoiseVolume        ( other.GetDenoiseVolume()      );
  
#ifdef Boost_DATE_TIME_FOUND
  SetDate                 ( other.GetDate() );
#endif

} // CopyParametersInternal
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////
bool Study::LoadFromDirectory( const std::string & studyFolderPath/* = ""*/)
{
  if (studyFolderPath.empty())
  {
    if ( this->m_SecretData->m_StudyFolderPath.empty() )
    {
      // Write an error (verbose)
      return false;
    }
    else
    {
      //studyFolderPath = THIS->m_studyFolderPath;
    }
  }


  // Load Study Properties from Folder TODO
    // User has to specify if traversing the folder will be recursive or not
    // What are the needed tags (Patient ID, Study Description, .... etc.)
    // 

  // Load Volume
  // Already Implemented
  // LoadVolume();
  // Remember to check the rest of the function LoadFromXML after Loading the volume

  return true;
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// set/get functions
ccipdGetConstImplementMacro    ( Study, Volume             , VolumeImageConstPointer     )
ccipdSetByvalImplementHashMacro( Study, Volume             , VolumeImageType *           )
ccipdGetConstImplementMacro    ( Study, TimeSeriesVolume   , VolumeVectorConstPointer    )
ccipdSetByvalImplementHashMacro( Study, TimeSeriesVolume   , VolumeVectorType *          )
ccipdGetConstImplementMacro    ( Study, LandmarkSet        , LandmarkSetConstPointer     )
ccipdSetByvalImplementHashMacro( Study, LandmarkSet        , LandmarkSetType *           )
ccipdGetConstImplementMacro    ( Study, WarpField          , VolumeWarpFieldConstPointer )
ccipdSetByvalImplementHashMacro( Study, WarpField          , VolumeWarpFieldType *       )
ccipdSetByvalImplementMacro    ( Study, AffineTransform    , VolumeAffineTransformType * )
ccipdGetImplementMacro         ( Study, AffineTransform    , VolumeAffineTransformConstPointer )
ccipdSetByvalImplementMacro    ( Study, BSplineTransform   , VolumeBSplineTransformType *)
ccipdGetImplementMacro         ( Study, BSplineTransform   , VolumeBSplineTransformConstPointer )
ccipdSetByvalImplementHashMacro( Study, WarpFieldFile      , string                      )
ccipdGetConstImplementMacro    ( Study, WarpFieldFile      , string                      )
ccipdSetByrefImplementHashMacro( Study, Modality           , ModalityType                )
ccipdGetConstImplementMacro    ( Study, Modality           , ModalityType                )
ccipdSetByrefImplementHashMacro( Study, Plane              , PlaneType                   )
ccipdGetConstImplementMacro    ( Study, Plane              , PlaneType                   )
ccipdSetByrefImplementHashMacro( Study, Annotator          , AnnotatorType               )
ccipdGetConstImplementMacro    ( Study, Annotator          , AnnotatorType               )
ccipdSetByrefImplementHashMacro( Study, Description        , DescriptionType             )
ccipdGetConstImplementMacro    ( Study, Description        , DescriptionType             )
ccipdSetByvalImplementHashMacro( Study, VolumeFileOrFolder , string                      )
ccipdGetConstImplementMacro    ( Study, VolumeFileOrFolder , string                      )
ccipdSetByvalImplementHashMacro( Study, TimeSeriesVolumeFileOrFolder , string            )
ccipdGetConstImplementMacro    ( Study, TimeSeriesVolumeFileOrFolder , string            )
ccipdSetByvalImplementHashMacro( Study, LandmarkFile       , string                      )
ccipdGetConstImplementMacro    ( Study, LandmarkFile       , string                      )
ccipdGetConstImplementMacro    ( Study, FeatureCollection  , FeatureCollectionPointer    )
ccipdSetByvalImplementHashMacro( Study, FeatureCollection  , FeatureCollectionPointer    )
ccipdSetByvalImplementHashMacro( Study, FeatureCollectionFile, string                    )
ccipdGetConstImplementMacro    ( Study, FeatureCollectionFile, string                    )
ccipdSetByvalImplementHashMacro( Study, IgnoreOrientation  , bool                        )
ccipdGetConstImplementMacro    ( Study, IgnoreOrientation  , bool                        )
ccipdSetByvalImplementHashMacro( Study, IgnoreOrigin       , bool                        )
ccipdGetConstImplementMacro    ( Study, IgnoreOrigin       , bool                        )
ccipdSetByvalImplementHashMacro( Study, BiasCorrectionSigma, float                       )
ccipdGetConstImplementMacro    ( Study, BiasCorrectionSigma, float                       )
ccipdSetByvalImplementHashMacro( Study, NormalizeVolume    , bool                        )
ccipdSetByvalImplementHashMacro( Study, CropIntensity      , float                       )
ccipdGetConstImplementMacro    ( Study, CropIntensity      , float                       )
ccipdSetByvalImplementHashMacro( Study, MaximumIntensity   , float                       )
ccipdGetConstImplementMacro    ( Study, MaximumIntensity   , float                       )
ccipdSetByvalImplementHashMacro( Study, MinimumIntensity   , float                       )
ccipdGetConstImplementMacro    ( Study, MinimumIntensity   , float                       )
ccipdGetConstImplementMacro    ( Study, NormalizeVolume    , bool                        )
ccipdSetByvalImplementHashMacro( Study, DenoiseVolume      , bool                        )
ccipdGetConstImplementMacro    ( Study, DenoiseVolume      , bool                        )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace
