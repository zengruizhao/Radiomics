



//////////////////////////////////////////////////////////////////////////////////////////

// ccipd includes
#include "ccipdMFASegmenterOptionsDialog.h"
#include "ccipdMFASegmenter.h"
#include "ccipdRegisterMFAAlgorithm.h"
#include "ccipdMFADrivingFeaturePreparer.h"
#include "ccipdMFALevelsetsFromFeatures.h"
#include "ccipdTexturesFromFeatures.h"
#include "ccipdTextureFeatureParser.h"
#include "ccipdTextureCollectionParser.h"
#include "ccipdMFAAlgorithmTableWidgetAdd.h"
#include "ccipdMFAAlgorithmTableWidgetSet.h"
#include "ccipdMFAAlgorithmTableWidgetGet.h"
#include "ccipdLoadGUI.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif // _MSC_VER

// Qt includes
#include "ui_ccipdMFASegmenterOptionsDialog.h"
#include <QTableWidget>
#include <QStandardItemModel>
#include <QListWidget>
#include <QInputDialog>

// boost includes
#include <boost/foreach.hpp>

// std includes
#include <iostream>

#ifdef _MSC_VER
#pragma warning( pop )
#endif // _MSC_VER

//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{
  


//////////////////////////////////////////////////////////////////////////////////////////
/// PIMPL Interfact to ProstalignFEM
class MFASegmenterOptionsDialog::SecretData
{

public: // to the MFASegmenterOptionsDialog class

  /// Our UI of the encapsulating class.
  typedef Ui_MFASegmenterOptionsDialog UserInterfaceType;
  
#ifdef _MSC_VER
  SecretData & operator=( const SecretData & other )
  {
    return const_cast< SecretData & >( other );
  }
#endif // _MSC_VER

  /// Constructor
  SecretData(
    MFASegmenterOptionsDialog * const encapsulatingClass,
    const MFASegmenter & segmenter );
  
  /// Our encapsulating class's ui
  UserInterfaceType * const m_EncapsulatingUI;

  MaskObjectCollectionType
    m_LevelsetObjects;

  /// Unique texture enum strings
  typedef std::set< std::string > TextureStringCollection;

  TextureStringCollection
    m_TextureStrings;

  /// Re-populate the levelset object list.
  void UpdateLevelsetList();

  /// Re-populate the texture string list.
  void UpdateTextureList();

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFASegmenterOptionsDialog::SecretData::SecretData(
  MFASegmenterOptionsDialog * const encapsulatingClass,
  const MFASegmenter & segmenter ) :
m_EncapsulatingUI( encapsulatingClass->ui ) // our ui
{
  
  // initialize the interface
  UserInterfaceType & ui = *this->m_EncapsulatingUI;
  ui.setupUi( encapsulatingClass ); // we need to do this first

  // extract the data
  ui.m_TrainedModelText->setText(
    tr( segmenter.GetTrainedMFAFile().c_str() ) );
  ui.m_SegmentationObjectText->setText(
    tr( segmenter.GetSegmentationObject().c_str() ) );
  ui.m_VerboseCheck->setChecked(
    segmenter.GetVerbose() );

  // update the registration algorithms
  SetAlgorithms(
    *ui.m_RegistrationOptionsTable,
    segmenter.GetRegistrationAlgorithms() );

  // now update the feature lists

  // extract the driving features
  const MFADrivingFeaturePreparerConstPointer
    drivingFeaturePreparer = segmenter.GetDrivingFeaturePreparer();

  if ( drivingFeaturePreparer )
    {

    // the extrapolation
    const auto extrapolation = drivingFeaturePreparer->GetExtrapolateInMM();
    ui.m_ExtrapolationSpin->setValue( extrapolation );
    
    // first the levelsets

    this->m_LevelsetObjects = GetLevelsetObjectsFromFeatures(
        drivingFeaturePreparer->GetInputLevelsetFeatures() );

    this->UpdateLevelsetList();

    // now the textures
    const TextureFeatureEnumCollection
      inputTextureEnums = GetTexturesFromFeatures(
        drivingFeaturePreparer->GetInputTextureFeatures() );

    BOOST_FOREACH( const TextureFeatureEnum & textureEnum, inputTextureEnums )
      {
      this->m_TextureStrings.insert( 
        TextureFeatureParser::GetTextureFeatureString( textureEnum ) );
      } // textureEnum

    this->UpdateTextureList();

    } // drivingFeaturePreparer


} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFASegmenterOptionsDialog::MFASegmenterOptionsDialog(
  const MFASegmenter & segmenter ) :
ui( new Ui_MFASegmenterOptionsDialog ), // set up the ui
m_Initialized( false ),          // not initialized yet
m_SecretData( new SecretData(    // set up our initial data
  this, segmenter ) )
{

  // okay, now we're done
  this->m_Initialized = true;

  // update the interface
  //this->SlotMaskValueChanged();

} // Constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MFASegmenterOptionsDialog::~MFASegmenterOptionsDialog()
{
} // Destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterOptionsDialog::SlotAddTexture()
{

  // we need a list of available texture features to add
  const auto textureFeatureStrings = TextureFeatureParser::GetAllTextureFeatures();

  QStringList textureFeatureQStrings;

  // populate the list
  BOOST_FOREACH( const std::string & textureFeatureString, textureFeatureStrings )
    {
    textureFeatureQStrings.push_back( tr( textureFeatureString.c_str() ) );
    } // for each texture feature string

  // ask the user to select a texture enum string from the list
  
  bool okay = false;
  const bool editable = false;

  // get the object name
  const QString textureFeatureString =
    QInputDialog::getItem( this, tr( "Input Texture Feature" ),
      tr( "Please input the texture feature name for a new driving texture feature." ),
      textureFeatureQStrings, 0, editable, &okay );

  // did the user press cancel?
  if ( !okay )
    return;

  // add it to our list
  this->m_SecretData->m_TextureStrings.insert( textureFeatureString.toStdString() );

  // update the texture list
  this->m_SecretData->UpdateTextureList();

} // SlotAddTexture
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterOptionsDialog::SlotAddLevelset()
{

  // we will input the levelset object from the user
  bool okay = false;

  // get the object name
  const QString levelsetObjectName =
    QInputDialog::getText( this, tr( "Input Levelset" ),
      tr( "Please input the mask object name for a new driving levelset feature." ),
      QLineEdit::Normal, QString(), &okay );

  // did the user press cancel?
  if ( !okay || levelsetObjectName.isEmpty() )
    return;

  // insert it into our collection
  this->m_SecretData->m_LevelsetObjects.insert( levelsetObjectName.toStdString() );

  // update our list widget
  this->m_SecretData->UpdateLevelsetList();

} // SlotAddLevelset
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterOptionsDialog::SlotRemoveLevelset()
{

  // get the currently selected list item
  const QListWidgetItem * const currentItem =
    this->ui->m_LevelsetList->currentItem();

  // is anything selected?
  if ( !currentItem )
    return;

  // get the currently selected levelset object
  const QString currentText = currentItem->text();

  // remove it from our collection
  this->m_SecretData->m_LevelsetObjects.erase( currentText.toStdString() );

  // update our list
  this->m_SecretData->UpdateLevelsetList();

} // SlotRemoveLevelset
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterOptionsDialog::SlotRemoveTexture()
{

  // get the currently selected list item
  const QListWidgetItem * const currentItem =
    this->ui->m_TextureList->currentItem();

  // is anything selected?
  if ( !currentItem )
    return;

  // get the currently selected levelset object
  const QString currentText = currentItem->text();

  // remove it from our collection
  this->m_SecretData->m_TextureStrings.erase( currentText.toStdString() );

  // update our list
  this->m_SecretData->UpdateTextureList();

} // SlotRemoveTexture
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterOptionsDialog::SlotAddAlgorithm()
{

  ::ccipd::AddDefaultAlgorithm(
    *this->ui->m_RegistrationOptionsTable );

} // SlotAddAlgorithm
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterOptionsDialog::SlotRemoveAlgorithm()
{

  // get the currently selected list item
  const int currentRow =
    this->ui->m_RegistrationOptionsTable->currentRow();

  if ( currentRow >= 0 )
    this->ui->m_RegistrationOptionsTable->removeRow( currentRow );
  
} // SlotRemoveAlgorithm
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterOptionsDialog::SlotOpenModel()
{

  const bool verbose = true;
  const std::string fileName = LoadFileNameGUI< MFAModel >( this, verbose );

  // if nothing was loaded, return
  if ( fileName.empty() )
    return;

  // update the text
  this->ui->m_TrainedModelText->setText( fileName.c_str() );

} // SlotOpenModel
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterOptionsDialog::SecretData::UpdateLevelsetList()
{

  QListWidget & levelsetList = *this->m_EncapsulatingUI->m_LevelsetList;

  // clear the existing list
  levelsetList.clear();

  // loop through each item
  BOOST_FOREACH( const MaskObjectType & levelsetObject, this->m_LevelsetObjects )
    {
    // add it to the list
    levelsetList.addItem( levelsetObject.c_str() );
    }

} // UpdateLevelsetList
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void MFASegmenterOptionsDialog::SecretData::UpdateTextureList()
{

  QListWidget & textureList = *this->m_EncapsulatingUI->m_TextureList;

  // clear the existing list
  textureList.clear();

  // loop through each item
  BOOST_FOREACH( const std::string & textureString, this->m_TextureStrings )
    {
    // add it to the list
    textureList.addItem( textureString.c_str() );
    }

} // UpdateTextureList
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
RegisterMFAAlgorithmCollection MFASegmenterOptionsDialog::GetAlgorithms() const
{
  return ::ccipd::GetAlgorithms( *this->ui->m_RegistrationOptionsTable );
} // GetAlgorithms
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
TextureFeatureEnumCollection
MFASegmenterOptionsDialog::GetTextureFeatures() const
{
  return GetTextureEnums( this->m_SecretData->m_TextureStrings );
} // GetTextureFeatures
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
MaskObjectType MFASegmenterOptionsDialog::GetSegmentationObject() const
{
  return this->ui->m_SegmentationObjectText->text().toStdString();
} // GetSegmentationObject
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
std::string MFASegmenterOptionsDialog::GetTrainedMFAFile() const
{
  return this->ui->m_TrainedModelText->text().toStdString();
} // GetTrainedMFAFile
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
bool MFASegmenterOptionsDialog::GetVerbose() const
{
  return this->ui->m_VerboseCheck->isChecked();
} // GetVerbose
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
float MFASegmenterOptionsDialog::GetExtrapolation() const
{
  return static_cast< float >( this->ui->m_ExtrapolationSpin->value() );
} // GetVerbose
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdGetConstImplementMacro( MFASegmenterOptionsDialog,
  LevelsetObjects, MaskObjectCollectionType )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
