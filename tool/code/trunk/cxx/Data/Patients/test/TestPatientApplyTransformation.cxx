//////////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "ccipdPatient.h"
#include "ccipdStudy.h"
#include "ccipdApplyTransformation.h"
#include "ccipdDuplicateImage.h"
#include "ccipdSaveImage.h"

#include "ccipdDisableWarningsMacro.h"
#include <itkAffineTransform.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
int main ( int argc, char * argv[] )
{



//////////////////////////////////////////////////////////////////////////////////////////
  if ( argc == 1 )
    {
    std::cout << std::endl << "Usage:";
    std::cout << argv[ 0 ] << " studyXML templateModality[ = studyXML] writeDirectory[= ./]" << std::endl;
    return EXIT_SUCCESS;
    } // 1 argument

  const std::string studyXML = argv[1],
                    templateXML( argc > 2 ? argv[ 2 ] : studyXML ),
                    writeDirectory( argc > 3 ? argv[ 3 ] : "./");
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
std::cout << " Begin loading in patient." << std::endl;

  const ccipd::Study::ConstPointer study = ccipd::Study::ConstNew( studyXML, false );

std::cout << "Done loading in patient." << std::endl;

// get template image
const ccipd::Study::ConstPointer templateStudy = ccipd::Study::ConstNew( templateXML, false );
if (!templateStudy )
  {
  std::cout << "Template study not loaded." << std::endl;
  return EXIT_FAILURE;
  }

if ( !templateStudy->GetVolume() )
  {
  std::cout << "Template image not present." << std::endl;
  return EXIT_FAILURE;
  }


// get studies with transformations
std::cout << "Begin transforming study." << std::endl;

ccipd::StudyPointer movedStudy = ccipd::ApplyTransform( *study, templateStudy->GetVolume() );
movedStudy->SaveContent( writeDirectory, "AffineStudy", "mha" );

// get studies with transformations
std::cout << "Done transforming studies." << std::endl;
//////////////////////////////////////////////////////////////////////////////////////////



return EXIT_SUCCESS;

}