



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdFEMMaterialsDialog.h"
#include "ccipdUniqueValuesTypes.h"   // the type of unique mask values
#include "ccipdExtractUniqueValues.h" // for getting the unique mask values
#include "ccipdDisableWarningsMacro.h"

// Qt includes
#include "ui_ccipdFEMMaterialsDialog.h"

// itk includes
#include <itkImage.h>
#include <itkFEMMaterialLinearElasticity.h> // our material

// boost includes
#include <boost/foreach.hpp>

#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  


//////////////////////////////////////////////////////////////////////////////////////////
/// PIMPL Interfact to ProstalignFEM
class FEMMaterialsDialog::SecretData
{

public: // to the FEMMaterialsDialog class

  /// Our UI of the encapsulating class.
  typedef Ui_FEMMaterialsDialog UserInterfaceType;

#ifdef _MSC_VER
  SecretData & operator=( const SecretData & other )
  {
    return const_cast< SecretData & >( other );
  }
#endif // _MSC_VER

  /// Constructor
  SecretData(
          FEMMaterialsDialog * const   encapsulatingClass,
    const UniqueMaskValuesCollection & uniqueMaskValues,
    const MaterialPointer            & initialDefaultMaterial = MaterialPointer(),
    const MaskMaterialMap            & initialMaterials       = MaskMaterialMap() );
  
  /// Our encapsulating class's ui
  UserInterfaceType * const m_EncapsulatingUI;

  /// What type of material will we use?
  typedef itk::fem::MaterialLinearElasticity LinearMaterialType;

  /// A smart pointer to our linear material
  typedef itk::SmartPointer< LinearMaterialType > LinearMaterialPointer;

  /// What should the combo box say for the "default" material?
  static const QString m_DefaultComboBoxString;

  /**
   * \brief Store the default material.
   */
  const LinearMaterialPointer
    m_DefaultMaterial;

  /// Stores whether or not a given mask value should use the default material.
  typedef std::unordered_map<
    MaskPixelType,
    bool
  > MaskDefaultMapType;

  /// Stores the **linear** material for each mask pixel.
  typedef std::unordered_map<
    MaskPixelType,
    LinearMaterialPointer
  > LinearMaterialMapType;

  /**
   * Store all the materials' properties
   * (even ones which will use the
   * default material) so that the GUI
   * can be updated and you
   * select a different material.
   */
  LinearMaterialMapType
    m_AllMaterials;

  /// Store whether each mask value should use the default material.
  MaskDefaultMapType
    m_MaskDefaultMap;

  /// Create a material with our desired default values.
  static LinearMaterialPointer CreateDefaultMaterial();

  /// Will determine which material is currently active from the combo box.
  LinearMaterialPointer GetCurrentlySelectedMaterial() const;

  /// Use an already-calculated mask value.
  LinearMaterialPointer GetCurrentlySelectedMaterial(
    const int maskValue ) const;

  /**
   * \brief Get the mask value of the currently selected material.
   * 
   * \returns -1 if the default material is selected, -2 on an error.
   */
  int GetCurrentlySelectedMaskValue() const;

  /// Is the currently selected material the default one?
  bool IsDefaultMaterialSelected() const;

  /// Update the "enabled" state of all controls
  void UpdateControlsEnabled( const bool controlsEnabled ) const;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
// define our constants
const QString
  FEMMaterialsDialog::SecretData::m_DefaultComboBoxString = "(Default)";
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMMaterialsDialog::SecretData::SecretData(
        FEMMaterialsDialog * const   encapsulatingClass,
  const UniqueMaskValuesCollection & uniqueMaskValues,
  const MaterialPointer            & initialDefaultMaterial,
  const MaskMaterialMap            & initialMaterials ) :
m_EncapsulatingUI( encapsulatingClass->ui ), // our ui
m_DefaultMaterial(                          // if we have
  initialDefaultMaterial &&                 // an initial material
  dynamic_cast< LinearMaterialType * >(     // that we can cast
    initialDefaultMaterial.GetPointer() ) ? // as a linear material
  dynamic_cast< LinearMaterialType * >(     // then use it; otherwise
    initialDefaultMaterial.GetPointer() ) : // just create a
  CreateDefaultMaterial().GetPointer() )    // default one
{
  
  // extract the data
  MaskDefaultMapType    & defaultMap  = this->m_MaskDefaultMap;
  LinearMaterialMapType & materialMap = this->m_AllMaterials;
  UserInterfaceType     & ui          = *this->m_EncapsulatingUI;
  ui.setupUi( encapsulatingClass ); // we need to do this first
  QComboBox             & maskCombo   = *ui.m_MaskValueCombo;

  // now, set up the combo box
  maskCombo.clear();
  maskCombo.addItem( SecretData::m_DefaultComboBoxString );

  // clear the maps
  defaultMap .clear();
  materialMap.clear();

  // extract the end of the map
  const MaskMaterialMap::const_iterator
    initialMaterialsEnd = initialMaterials.cend();

  // loop through each material
  BOOST_FOREACH( const MaskPixelType uniqueMaskValue, uniqueMaskValues )
    {

    // see if we have an initial material for that mask value
    const MaskMaterialMap::const_iterator
      initialMaterialIterator = initialMaterials.find( uniqueMaskValue );

    const MaterialPointer & initialMaterial = 
      initialMaterialIterator == initialMaterialsEnd ?
      MaterialPointer() : // no material
      initialMaterialIterator->second; // first is the mask value

    // try to cast it as a linear material
    const LinearMaterialPointer initialLinearMaterial =
      initialMaterial ?
      dynamic_cast< LinearMaterialType * >( initialMaterial.GetPointer() ) :
      LinearMaterialPointer().GetPointer();

    // were we successful?
    const bool hasInitialLinearMaterial = initialLinearMaterial.IsNotNull();

    // if so, assign it (otherwise just create a default one)
    materialMap[ uniqueMaskValue ] =
      hasInitialLinearMaterial ?
        initialLinearMaterial : 
        CreateDefaultMaterial();

    // we aren't using the default material in that case
    defaultMap[ uniqueMaskValue ] = !hasInitialLinearMaterial;

    // add it to our combo box
    maskCombo.addItem( QString::number( uniqueMaskValue ) );

    } // for each unique mask value

} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMMaterialsDialog::SecretData::LinearMaterialPointer
FEMMaterialsDialog::SecretData::CreateDefaultMaterial()
{

  // initialize
  const LinearMaterialPointer material = LinearMaterialType::New();

  // set up the values (leave everything else as default)
  material->SetYoungsModulus( 40.0 );
  material->SetPoissonsRatio( 0.30 );

  // output
  return material;

} // CreateDefaultMaterial
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMMaterialsDialog::FEMMaterialsDialog(
  const VolumeMaskImageType * const mask,
  const MaterialPointer     & initialDefaultMaterial,
  const MaskMaterialMap     & initialMaterials ) :
ui( new Ui_FEMMaterialsDialog ), // set up the ui
m_Initialized( false ),          // not initialized yet
m_SecretData( new SecretData(    // set up our initial data
  this,
  ExtractUniqueValues( mask ),
  initialDefaultMaterial,
  initialMaterials ) )
{

  // okay, now we're done
  this->m_Initialized = true;

  // update the interface
  this->SlotMaskValueChanged();

} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMMaterialsDialog::~FEMMaterialsDialog()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MaskMaterialMap FEMMaterialsDialog::GetMaterials() const
{

  // typedefs
  typedef SecretData::LinearMaterialMapType LinearMaterialMapType;
  typedef SecretData::MaskDefaultMapType    MaskDefaultMapType;

  // extract the data
  const LinearMaterialMapType & allMaterials = this->m_SecretData->m_AllMaterials;
  const MaskDefaultMapType    & defaultMap   = this->m_SecretData->m_MaskDefaultMap;

  // prepare the output
  MaskMaterialMap outputMap;

  BOOST_FOREACH( const LinearMaterialMapType::value_type & linearMapPair, allMaterials )
    {

    // what is the mask value?
    const MaskPixelType maskValue = linearMapPair.first;

    // see if we are using the default
    const bool useDefault = defaultMap.at( maskValue );

    // if not, add it to our output collection
    if ( !useDefault )
      outputMap[ maskValue] = linearMapPair.second; // second contains the pointer

    } // for each linear map value

  // output the ones we've inserted
  return outputMap;

} // GetMaterials
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MaterialPointer FEMMaterialsDialog::GetDefaultMaterial() const
{
  return this->m_SecretData->m_DefaultMaterial.GetPointer();
} // GetDefaultMaterial
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool FEMMaterialsDialog::SecretData::IsDefaultMaterialSelected() const
{
  
  // extract the ui data
  const UserInterfaceType & ui = *this->m_EncapsulatingUI;

  // extract what is selected
  const QString selectedMaskValueText = ui.m_MaskValueCombo->currentText();

  // check if the user is working on the default material
  return selectedMaskValueText.compare( SecretData::m_DefaultComboBoxString ) == 0;

} // IsDefaultMaterialSelected
//////////////////////////////////////////////////////////////////////////////////////////
  
    
  
//////////////////////////////////////////////////////////////////////////////////////////
int FEMMaterialsDialog::SecretData::GetCurrentlySelectedMaskValue() const
{

  // first check if it's the default material
  if ( this->IsDefaultMaterialSelected() )
    return -1;

  // extract the ui data
  const UserInterfaceType & ui = *this->m_EncapsulatingUI;

  // extract what is selected
  const QString selectedMaskValueText = ui.m_MaskValueCombo->currentText();

  // now, we need the text as a mask value
  bool isAnInteger = false;
  const MaskPixelType maskValue =
    static_cast< MaskPixelType >(
      selectedMaskValueText.toInt( &isAnInteger ) );

  return isAnInteger ? maskValue : -2; // -2 on an error

} // GetCurrentlySelectedMaskValue
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
FEMMaterialsDialog::SecretData::LinearMaterialPointer
FEMMaterialsDialog::SecretData::GetCurrentlySelectedMaterial() const
{
  const int maskValue = this->GetCurrentlySelectedMaskValue();
  return this->GetCurrentlySelectedMaterial( maskValue );
}

FEMMaterialsDialog::SecretData::LinearMaterialPointer
FEMMaterialsDialog::SecretData::GetCurrentlySelectedMaterial(
  const int maskValue ) const
{

  if ( maskValue == -1 )
    return this->m_DefaultMaterial;
  else if ( maskValue >= 0 && maskValue <= std::numeric_limits< MaskPixelType >::max() )
    // output the material for that mask value
    return this->m_AllMaterials.at(
      static_cast< MaskPixelType >( maskValue ) );
  else
    return LinearMaterialPointer(); // error

} // GetCurrentlySelectedMaterial
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMMaterialsDialog::SlotMaskValueChanged()
{
  
  // make sure we're initialized
  if ( !this->m_Initialized ) return;

  const int
    currentMaskValue = this->m_SecretData->GetCurrentlySelectedMaskValue();

  // check for errors
  if ( currentMaskValue == -2 )
    {
    cerr << "Unable to extract the mask value." << endl;
    return;
    }

  // update all the materials
  const SecretData::LinearMaterialPointer
    materials = this->m_SecretData->GetCurrentlySelectedMaterial( currentMaskValue );

  // get all the values
  // so when we start updating them,
  // we remember what we want them to actually
  // be before they get changed
  const double
    youngsModulus = materials->GetYoungsModulus     (),
    poissons      = materials->GetPoissonsRatio     (),
    crossSection  = materials->GetCrossSectionalArea(),
    thickness     = materials->GetThickness         (),
    densityHeat   = materials->GetDensityHeatProduct();

  const double
    base = 10,
    youngsModulusExponent = floor( std::log( youngsModulus ) / std::log( base ) ),
    youngsModulusMantissa = youngsModulus / 
      std::pow( base, youngsModulusExponent );

  std::cout <<
    "Young's modulus = " << youngsModulus << std::endl <<
    "Young's modulus exponent = " << youngsModulusExponent << std::endl <<
    "Young's modulus mantissa = " << youngsModulusMantissa << std::endl;

  this->ui->m_YoungsModulusMantissaSpin->setValue( youngsModulusMantissa );
  this->ui->m_YoungsModulusExponentSpin->setValue( youngsModulusExponent );
  this->ui->m_PoissonsSpin      ->setValue( poissons      );
  this->ui->m_CrossSectionalSpin->setValue( crossSection  );
  this->ui->m_ThicknessSpin     ->setValue( thickness     );
  this->ui->m_DensityHeatSpin   ->setValue( densityHeat   );

  const bool
    isDefaultSelected = ( currentMaskValue == -1 ),
    useDefaultChecked = isDefaultSelected ||
      this->m_SecretData->m_MaskDefaultMap.at(
        static_cast< MaskPixelType >( currentMaskValue ) ),
    useDefaultEnabled = !isDefaultSelected,
    controlsEnabled   = isDefaultSelected || !useDefaultChecked;

  // update the "default materials" checkbox
  this->ui->m_DefaultMaterialCheck->setChecked( useDefaultChecked );
  this->ui->m_DefaultMaterialCheck->setEnabled( useDefaultEnabled );

  // now we might want to change the "enabled" state of the controls
  this->m_SecretData->UpdateControlsEnabled( controlsEnabled );

} // SlotMaskValueChanged
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMMaterialsDialog::SecretData::UpdateControlsEnabled(
  const bool controlsEnabled ) const
{

  UserInterfaceType & ui = *this->m_EncapsulatingUI;

  // update the enabled property of all other inputs
  ui.m_YoungsModulusMantissaSpin->setEnabled( controlsEnabled );
  ui.m_YoungsModulusExponentSpin->setEnabled( controlsEnabled );
  ui.m_PoissonsSpin             ->setEnabled( controlsEnabled );
  ui.m_CrossSectionalSpin       ->setEnabled( controlsEnabled );
  ui.m_ThicknessSpin            ->setEnabled( controlsEnabled );
  ui.m_DensityHeatSpin          ->setEnabled( controlsEnabled );

} // SlotMaskValueChanged
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMMaterialsDialog::SlotUseDefaultChanged( const bool useDefault )
{

  // update the "enabled" state of the controls
  const bool controlsEnabled = !useDefault;
  this->m_SecretData->UpdateControlsEnabled( controlsEnabled );

  // update the materials "default" state
  const int maskValue =
    this->m_SecretData->GetCurrentlySelectedMaskValue();

  // update the "use default" property
  if ( maskValue >= 0 )
    {
    this->m_SecretData->m_MaskDefaultMap[
      static_cast< MaskPixelType >( maskValue ) ] = useDefault;
    }

} // SlotUseDefaultChanged
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void FEMMaterialsDialog::SlotUpdateMaterial()
{

  // extract the material
  const SecretData::LinearMaterialPointer
    materialToUpdate = this->m_SecretData->GetCurrentlySelectedMaterial();

  // make sure there were no errors
  if ( !materialToUpdate )
    {
    cerr << "Error: No current material!" << endl;
    return;
    } // materialToUpdate

  const double
    base = 10.0,
    youngsModulusMantissa = this->ui->m_YoungsModulusMantissaSpin->value(),
    youngsModulusExponent = this->ui->m_YoungsModulusExponentSpin->value(),
    youngsModulus = youngsModulusMantissa * std::pow( base, youngsModulusExponent );

  // update the material's properties
  materialToUpdate->SetYoungsModulus     ( youngsModulus );
  materialToUpdate->SetPoissonsRatio     ( this->ui->m_PoissonsSpin      ->value() );
  materialToUpdate->SetCrossSectionalArea( this->ui->m_CrossSectionalSpin->value() );
  materialToUpdate->SetThickness         ( this->ui->m_ThicknessSpin     ->value() );
  materialToUpdate->SetDensityHeatProduct( this->ui->m_DensityHeatSpin   ->value() );

} // SlotUpdateMaterial
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
