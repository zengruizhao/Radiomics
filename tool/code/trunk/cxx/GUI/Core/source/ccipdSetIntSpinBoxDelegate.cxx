


//////////////////////////////////////////////////////////////////////////////////////////
#include "ccipdSetIntSpinBoxDelegate.h"
//////////////////////////////////////////////////////////////////////////////////////////



namespace ccipd
{

  

//////////////////////////////////////////////////////////////////////////////////////////
class SetIntSpinBoxDelegate::SecretData
{

public:
  SecretData(); ///< Constructor

  SetIntSpinBoxDelegate::SpinValueType
    m_MinimumValue,
    m_MaximumValue,
    m_Step;

  bool m_HasStep;

}; // class SecretData
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SetIntSpinBoxDelegate::SetIntSpinBoxDelegate( QObject * const parent ) :
Superclass( parent ),
ccipdNewPIMPLMacro
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SetIntSpinBoxDelegate::SecretData::SecretData() :
m_MinimumValue(   0 ),
m_MaximumValue( 100 ),
m_HasStep( false )
{
} // constructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SetIntSpinBoxDelegate::~SetIntSpinBoxDelegate()
{
} // destructor
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
SetIntSpinBoxDelegate::SpinValueType
SetIntSpinBoxDelegate::GetStep() const
{
  if ( this->m_SecretData->m_HasStep )
    return this->m_SecretData->m_Step;
  else
    return this->Superclass::GetStep();
} // GetStep
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SetIntSpinBoxDelegate::SetStep( const SpinValueType variable )
{
  this->m_SecretData->m_HasStep = true;
  ccipdSetBodyMacro( Step )
} // SetStep
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
void SetIntSpinBoxDelegate::ClearStep()
{
  this->m_SecretData->m_HasStep = false;
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
ccipdGetConstImplementMacro( SetIntSpinBoxDelegate,
  MinimumValue, SetIntSpinBoxDelegate::SpinValueType )
ccipdGetConstImplementMacro( SetIntSpinBoxDelegate,
  MaximumValue, SetIntSpinBoxDelegate::SpinValueType )
ccipdSetByvalImplementMacro( SetIntSpinBoxDelegate,
  MinimumValue, SetIntSpinBoxDelegate::SpinValueType )
ccipdSetByvalImplementMacro( SetIntSpinBoxDelegate,
  MaximumValue, SetIntSpinBoxDelegate::SpinValueType )
//////////////////////////////////////////////////////////////////////////////////////////



} // namespace ccipd
