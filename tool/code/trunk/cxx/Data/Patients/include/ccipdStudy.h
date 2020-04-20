#ifndef __ccipdStudy_h
#define __ccipdStudy_h



//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdVolumeTypes.h"   // for image and mask types
#include "ccipdLandmarkTypes.h" // for landmark types
#include "ccipdMacros.h"        // for commonly typed code
#include "ccipdPatientTypes.h"  // for typedef's
#include "ccipdHashable.h"      // hashing
#include "ccipdDataPatientsOptions.h"
#include "ccipdFeatureTypes.h"
#include "ccipdFeatureCollection.h" // for features
#include "ccipdXMLBaseFactory.h"
#include "ccipdEnableSharedFromThis.hxx"
#include "ccipdCloneableFactory.h"

// std includes 
#include <string>
#include <ostream>

#include "ccipdDisableWarningsMacro.h"
  // itk include
  #include <itkImage.h>
  #include <itkPointSet.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{



/**
 * \brief Store image and other information for a single study.
 * 
 * \see
 *   - ccipd::Study
 *   - ccipd::Patient
 * 
 * \date    2012-02-19
 * \author  Rob Toth
 * \ingroup PatientData
 */
class Study :
  public Hashable,
  public XMLBase,
  public XMLBaseFactory< Study >,
  public Copyable< Study >,
  public CloneableFactory< Study >
{



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdConstructorDeclarationMacro( Study )
public:
  typedef XMLBaseFactory< Study >         SuperclassFactory;
  ccipdFactoryTypedefsMacro

 typedef CloneableFactory< Self >         SuperclassCloneableFactory;
 using SuperclassCloneableFactory::New;                         
 using SuperclassCloneableFactory::ConstNew;
//////////////////////////////////////////////////////////////////////////////////////////



public:
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * For loading a study from an XML structure.
   *
   * <description>Description Text</description>
   * <modality>Modality</modality>
   * <date>YYYY-MM-DD</date>
   * <volume>volume_file_or_folder</volume>
   *
   * <mask>
   *   ... (see below)
   * </mask>
   * <mask>
   *   ... (another mask)
   * </mask>
   *
   * If the modality is "MRI", it will use LoadMRIVolume.
   * If the modality is "TRUS", it will use LoadTRUSVolume.
   * Otherwise, it will use LoadVolume.
   *
   */
  virtual bool LoadFromXML( const XMLNode & studyRoot );
//////////////////////////////////////////////////////////////////////////////////////////

  /** \brief  : Load study directly from a given directory
    * \Created: axa667 6/24/2013
    * \Updated:  
    * \ingroup: 
    * \param  : studyFolderPath: the full path of the study where images are located on the HDD
    * \param  : 
    * \return : bool: true of the study is loaded successfully, otherwise: false
    * \notes  : 
    * 
    * 
    * 
    */
  virtual bool LoadFromDirectory(const std::string & studyFolderPath = "");


//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * For loading a mask from an xml structure.
   *
   * <object>Mask Object Name (i.e. which organ)</object>
   * <file>MaskFile</file>
   */
  bool LoadMaskFromXML( const XMLNode     & maskRoot );
  bool LoadMaskFromXML( const std::string & xmlFile  );

  /**
   * Anntations are not always present for all slices in a data.
   * This function will interpolate the annotations for missing slices
   * specified by the user.
   */
  bool UpdateIncompleteMask (
    const MaskObjectType maskObject, 
    const std::string whichSlices);

  /**
   * This will attempt to load a mask from a file
   * and return true if successful.
   */
  bool LoadMask(
    const std::string    & fileName,
    const MaskObjectType & maskObject = "" );

  /**
   * This will replace, or insert, a mask
   * for the given object name.
   */
  void SetMask(
    const VolumeMaskImageType * mask,
    const MaskObjectType      & objectName = "" );

  /**
   * This will remove a mask for the given object name,
   * also will remove the mask file name.
   */
  void EraseMask(const MaskObjectType      & objectName = "" );

  /**
   * This will remove a mask for the given object name,
   * also will remove the mask file name.
   */
  void EraseMasks(const MaskObjectCollectionType & maskObjects );

  /**
   * This will remove a volume for the given object name,
   * also will remove the file name.
   */
  void EraseVolume( );

  /**
   * This will remove the time series volume for the given object name,
   * also will remove the file name.
   */
  void EraseTimeSeriesVolume( );

  /**
   * This will return an existing mask with
   * the object name give.
   */
  VolumeMaskImageConstPointer GetMask(
    const MaskObjectType & maskObject = "" ) const;

  /**
   * This is useful if you know your study only has one mask.
   */
  ccipdGetConstDeclareMacro( OneMask, VolumeMaskImageConstPointer )

  /**
   * This will return whether a given mask exists.
   */
  bool HasMask( const MaskObjectType & maskObject ) const;

  /**
   * This will return whether all the given masks exist.
   */
  bool HasMasks( const MaskObjectCollectionType & maskObjects ) const;

  /**
   * If a mask was successfully loaded, it will
   * return the file location.
   *
   * Otherwise, it will return an empty string.
   */
  std::string GetMaskFile( const MaskObjectType & maskObject = "" ) const;

  /**
   * Sets the file name for the mask
   *\param the maks object
   */
  void SetMaskFile(const std::string & filename, const MaskObjectType & maskObject);

  /**
   * If a volume was successfully loaded, it will
   * return the file (or folder) location.
   *
   * Otherwise, it will return an empty string.
   */
  ccipdSetByvalDeclareMacro( VolumeFileOrFolder, std::string )
  ccipdGetConstDeclareMacro( VolumeFileOrFolder, std::string )

 /**
   * Set/Get the volume for this study.
   */
  ccipdSetByvalDeclareMacro( Volume, VolumeImageType *       )
  ccipdGetConstDeclareMacro( Volume, VolumeImageConstPointer )

  /**
   * Whether or not we have a volume for this study.
   */
  bool HasVolume() const;

 /**
   * If a volume was successfully loaded, it will
   * return the file (or folder) location.
   *
   * Otherwise, it will return an empty string.
   */
  ccipdSetByvalDeclareMacro( TimeSeriesVolumeFileOrFolder, std::string )
  ccipdGetConstDeclareMacro( TimeSeriesVolumeFileOrFolder, std::string )

   /**
   * Set/Get the time series volume for this study. // in general this is a DCE like volume
   */
  ccipdSetByvalDeclareMacro( TimeSeriesVolume, VolumeVectorType *       )
  ccipdGetConstDeclareMacro( TimeSeriesVolume, VolumeVectorConstPointer )

  /**
   * Whether or not we have a time series volume for this study.
   */
  bool HasTimeSeriesVolume() const;
  
  /**
   * If a warp field volume was successfully loaded, it will
   * return the file location.
   *
   * Otherwise, it will return an empty string.
   */
  ccipdSetByvalDeclareMacro( WarpFieldFile, std::string )
  ccipdGetConstDeclareMacro( WarpFieldFile, std::string )


  /**
   * Whether or not we have a volume for this study.
   */
  bool HasWarpField() const;

  /**
   * Set/Get the warp field for this study.
   */
  ccipdSetByvalDeclareMacro( WarpField, VolumeWarpFieldType *       )
  ccipdGetConstDeclareMacro( WarpField, VolumeWarpFieldConstPointer )

  /**
   * Represents an affine transform associated with this study.
   * 
   * This was originally created to import atlas registration
   * results for each study.
   * 
   * \date 2013-03-07
   */
  bool HasAffineTransform() const;
  ccipdGetDeclareMacro     ( AffineTransform, VolumeAffineTransformConstPointer )
  ccipdGetConstDeclareMacro( AffineTransform, VolumeAffineTransformConstPointer )
  ccipdSetByvalDeclareMacro( AffineTransform, VolumeAffineTransformType *       )

  /**
   * Represents the B spline transform associated with this study.
   * 
   * \date 2013-06-08
   */
  bool HasBSplineTransform() const;
  ccipdGetDeclareMacro     ( BSplineTransform, VolumeBSplineTransformConstPointer)
  ccipdGetConstDeclareMacro( BSplineTransform, VolumeBSplineTransformConstPointer)
  ccipdSetByvalDeclareMacro( BSplineTransform, VolumeBSplineTransformType *      )

  /**
   * This will attempt to load an image
   * from a file and return true if successful.
   *
   * If it's a folder name, the folder
   * will be treated as a DICOM directory for MRI.
   *
   * You can explicitly decide what time of image to
   * try to load by calling the appropriate function.
   */
  bool LoadVolume         ( const std::string & fileOrFolderName );
  bool LoadMRIVolume      ( const std::string & fileOrFolderName );
  bool LoadTRUSVolume     ( const std::string & fileName         );
  bool LoadWarpFieldVolume( const std::string & fileName         );
  bool LoadTimeSeriesVolume( const std::string & fileOrFolderName );

   /**
   * If a set of landmarks was successfully loaded,
   * it will return the file (or folder) location.
   *
   * Otherwise, it will return an empty string.
   */
  ccipdSetByvalDeclareMacro( LandmarkFile, std::string )
  ccipdGetConstDeclareMacro( LandmarkFile, std::string )

  /**
   * Set/Get the landmark set for this study.
   */
  ccipdSetByvalDeclareMacro( LandmarkSet, LandmarkSetType *       )
  ccipdGetConstDeclareMacro( LandmarkSet, LandmarkSetConstPointer )

  /**
   * Whether or not we have a landmark set for this study.
   */
  bool HasLandmarkSet() const;

  /**
   * This will attempt to load a set of landmarks
   * from a file and return true if successful.
   */
  bool LoadLandmarks( const std::string & fileName );
 
  /**
   * Save all (at least a part of it) the data to the disk at the location
   * given by 
   * \param dir 
   * and using
   * \param filename_prefix as the begining of the filename 
   * and 
   * \parm extension as the file extension without the '.'
   */
  void SaveContent(const std::string dir, const std::string filename_prefix, const std::string extension);

  /**
   * Save all (at least a part of it) the data to the disk at the location
   * given by paths that have been already stored in the files
   */
  void SaveContent() const; 

  /**
   * Set/Get the modality for this study.
   */
  ccipdSetByrefDeclareMacro( Modality, ModalityType )
  ccipdGetConstDeclareMacro( Modality, ModalityType )

  /**
   * Set/Get the modality for this study.
   */
  ccipdSetByrefDeclareMacro( Plane, PlaneType )
  ccipdGetConstDeclareMacro( Plane, PlaneType )

  /**
   * Set/Get the annotator for this study.
   */
  ccipdSetByrefDeclareMacro( Annotator, AnnotatorType )
  ccipdGetConstDeclareMacro( Annotator, AnnotatorType )

  /**
   * Set/Get whether to ignore image orientation
   */
  ccipdSetByvalDeclareMacro( IgnoreOrientation, bool )
  ccipdGetConstDeclareMacro( IgnoreOrientation, bool )
  itkBooleanMacro          ( IgnoreOrientation )

  /**
   * Set/Get whether to ignore image origin
   */
  ccipdSetByvalDeclareMacro( IgnoreOrigin, bool )
  ccipdGetConstDeclareMacro( IgnoreOrigin, bool )
  itkBooleanMacro          ( IgnoreOrigin )

  /**
   * \brief Set/Get bias field correction.
   * 
   * If > 0.0, then bias correction
   * is performed using the method
   * by Cohen et al.
   * with the value as the given "sigma".
   * 
   * default = 0.0
   * 
   * \see CohenCorrectBiasField()
   * 
   * \date 2013-01-17
   */
  ccipdSetByvalDeclareMacro( BiasCorrectionSigma, float )
  ccipdGetConstDeclareMacro( BiasCorrectionSigma, float )
    /**
   * \brief Set/Get crop intensities correction.
   * 
   * If > 0.0, then intensities are cropped
   * to be within +/- standard deviations of the value set
   *
   * 
   * default = 0.0
   * 
   * \see CropIntensitiesVolume()
   * 
   * \date 2013-08-17
   */
  ccipdSetByvalDeclareMacro( CropIntensity, float )
  ccipdGetConstDeclareMacro( CropIntensity, float )
  /**
   * \brief Set/Get image normalization.
   * 
   * If true, the volume will
   * be normalized to have a
   * mean of 0 and variance of 1
   * immediately after loading.
   * 
   * default = false
   * 
   * \see NormalizeMRI()
   * 
   * \date 2013-01-17
   */
  ccipdSetByvalDeclareMacro( NormalizeVolume, bool )
  ccipdGetConstDeclareMacro( NormalizeVolume, bool )
  itkBooleanMacro          ( NormalizeVolume )

  /**
   * \brief Set/Get image denoising.
   * 
   * If true, the volume will
   * be slightly smoothed.
   * 
   * default = false
   * 
   * \see ApplyBilateralFilterMRI()
   * 
   * \date 2013-01-17
   */
  ccipdSetByvalDeclareMacro( DenoiseVolume, bool )
  ccipdGetConstDeclareMacro( DenoiseVolume, bool )
  itkBooleanMacro          ( DenoiseVolume )

  /**
   * \brief Set/Get maximum/minimum values to 
   * rescale pixel values.
   * 
   * Default is 0 for both values; At least one value must be 
   * set to rescale the intensities. 
   *
   * \see RescaleVolumeIntensities()
   * 
   * \date 2013-08-20
   */
  ccipdSetByvalDeclareMacro( MaximumIntensity, float )
  ccipdGetConstDeclareMacro( MaximumIntensity, float )


  ccipdSetByvalDeclareMacro( MinimumIntensity, float )
  ccipdGetConstDeclareMacro( MinimumIntensity, float )
  /**
   * Set/Get the description for this study.
   */
  ccipdSetByrefDeclareMacro( Description, DescriptionType )
  ccipdGetConstDeclareMacro( Description, DescriptionType )

  /**
   * Set/Get the date for this study.
   *
   * The string should be YYYY-MM-DD or YYYYMMDD
   * and will return true if successful.
   */
#ifdef Boost_DATE_TIME_FOUND
  bool SetDate( const std::string & dateString );
  ccipdSetByvalDeclareMacro( Date, DatePointer )
  ccipdGetConstDeclareMacro( Date, DatePointer )

  /**
   * Has the date been set to a valid date for this study?
   */
  bool HasDate() const;
#endif // Boost_DATE_TIME_FOUND

  /**
   * This will return how many masks exist for the study.
   */
  ccipdGetConstDeclareMacro( NumberOfMasks, unsigned int )

  /**
   * For getting all mask object names.
   */
  MaskObjectCollectionType GetMaskObjects() const;

  /**
   * This will return a feature collection.
   *
   * Note you must specifically set the volume
   * of this feature collection yourself.
   *
   */
  ccipdGetConstDeclareMacro( FeatureCollection, FeatureCollectionPointer )
  ccipdSetByvalDeclareMacro( FeatureCollection, FeatureCollectionPointer )

  /**
   * Whether or not we have a FeatureCollection set for this study.
   */
  bool HasFeatureCollection() const;

  /**
   * This will remove a feature from the feature collection
   */
  void EraseFeature(const FeatureIDConstPointer featureID);

  /**
   * This will remove a feature from the feature collection
   */
  void EraseFeatures(const FeatureIDConstCollection featureIDs);

  /**
   * If a feature collection was successfully create, it will
   * return the file where was saved from or loaded from.
   *
   * Otherwise, it will return an empty string.
   */
  ccipdSetByvalDeclareMacro( FeatureCollectionFile, std::string )
  ccipdGetConstDeclareMacro( FeatureCollectionFile, std::string )

  /**
   * This will clear all data from the study.
   */
  void ClearData();

  /**
   * Comparison with another Study.
   */
  virtual bool operator==( const Hashable & other ) const;

  /**
   * For outputting this class.
   */
  virtual void OutputToStream( std::ostream & stream ) const;
//////////////////////////////////////////////////////////////////////////////////////////



protected:
//////////////////////////////////////////////////////////////////////////////////////////
  /**
   * We want to be hashable.
   */
  virtual HashValueType ReHash() const;
//////////////////////////////////////////////////////////////////////////////////////////
 

//////////////////////////////////////////////////////////////////////////////////////////
virtual void CopyParametersInternal( const Self & other );
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
  ccipdPIMPLMacro
//////////////////////////////////////////////////////////////////////////////////////////



}; // class
} // namespace



#endif // __ccipdStudy_h
