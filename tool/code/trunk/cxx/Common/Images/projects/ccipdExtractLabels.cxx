/**
 *   \author Mirabela Rusu
 *   \date 12/11/2012
 *   \brief Code to extract lables (of given value)
 *          the code is meant to process the data
 * ./ExtractLabels /home/mrusu/Dev/CxxModules/src/Data/data_files/BU_CG_PZ_Segmentation.xml
 */
//////////////////////////////////////////////////////////////////////////////////////////
// ccipd includes
#include "ccipdCore.h"
#include "ccipdSaveImage.h"      // for saving results
#include "ccipdLoadVolume.h"     // for loading images
#include "ccipdLoadDicomMRI.h"     // for loading images
#include "ccipdVolumeTypes.h"    // for image types
#include "ccipdStatistics.h"     // simple statistics Min
#include "ccipdXMLBase.h"        //read/write xml
#include "ccipdProcessLabels.h"

// std includes
#include <iostream>             // for cout
#include <fstream>              // for fout 
#include <string>               // for filenames

// boost includes
#include <boost/foreach.hpp>             // for loops
#include <boost/property_tree/ptree.hpp> // for xml trees
#include <boost/functional/hash.hpp>     // for hashing
#include <boost/filesystem.hpp>     // for hashing

#include "ccipdDisableWarningsMacro.h"
  // itk includes
  #include <itkImageSliceIteratorWithIndex.h>  // to access voxels
  #include <itkBinaryMorphologicalOpeningImageFilter.h>
  #include <itkBinaryBallStructuringElement.h>
  #include <itkBinaryImageToLabelMapFilter.h>
#include "ccipdEnableWarningsMacro.h"
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////
using std::cout;
using std::endl;
using std::cerr;
using std::string;
using std::vector;

using ccipd::VolumeMaskImagePointer;
using ccipd::VolumeMaskImageConstPointer;
using ccipd::VolumeImagePointer;
using ccipd::VolumeMaskImageType;
//////////////////////////////////////////////////////////////////////////////////////////
string GetPatientName(string path)
{
  boost::filesystem::path myPath(path);

  char numericPart[25] = "";
  string filename = myPath.filename().generic_string();
  
  int i = 0;
  int count = 0; 
  while (i<(int)filename.size())
  {
    char c = filename[i];
    if (isdigit(c))
    {
      count++;
      if (count>3) // don't add the first 2 numbers (01)
        sprintf(numericPart, "%s%c", numericPart, c);
    }
    i++;
  }

  char id[25];
  sprintf(id, "ProstateDx-01-%04d", atoi(numericPart) );
//  sprintf(id, "Prostate3T-01-%04d", atoi(numericPart) );

  return id;
}

void Opening(VolumeMaskImagePointer * inputImage, float radius)
{
/*  typedef itk::BinaryBallStructuringElement<
    ccipd::VolumeImageType::PixelType,3>                  StructuringElementType;
  StructuringElementType structuringElement;
  structuringElement.SetRadius(radius);
  structuringElement.CreateStructuringElement();
*/

  typedef itk::FlatStructuringElement<3> StructuringElementType;
  StructuringElementType::RadiusType elementRadius;
  elementRadius.Fill(
    static_cast< StructuringElementType::RadiusType::SizeValueType >( radius ));
  StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);
 
  typedef itk::BinaryMorphologicalOpeningImageFilter <ccipd::VolumeMaskImageType, ccipd::VolumeMaskImageType, StructuringElementType>
          BinaryMorphologicalOpeningImageFilterType;
  BinaryMorphologicalOpeningImageFilterType::Pointer openingFilter
          = BinaryMorphologicalOpeningImageFilterType::New();
  openingFilter->SetInput(*inputImage);
  openingFilter->SetKernel(structuringElement);
  openingFilter->SetBackgroundValue(0);
  openingFilter->SetForegroundValue(1);
  openingFilter->Update();

  *inputImage = openingFilter->GetOutput();
}

void Dilate(VolumeMaskImagePointer * inputImage, float radius)
{
  typedef itk::BinaryBallStructuringElement<
    ccipd::VolumeImageType::PixelType,3>                  StructuringElementType;
  StructuringElementType structuringElement;
  structuringElement.SetRadius(
    static_cast< StructuringElementType::RadiusType::SizeValueType >( radius ) );
  structuringElement.CreateStructuringElement();

  typedef itk::BinaryDilateImageFilter <VolumeMaskImageType, VolumeMaskImageType, StructuringElementType>
          BinaryDilateImageFilterType;
 
  BinaryDilateImageFilterType::Pointer dilateFilter
          = BinaryDilateImageFilterType::New();
  dilateFilter->SetInput(*inputImage);
  dilateFilter->SetKernel(structuringElement);
  dilateFilter->SetBackgroundValue(0);
  dilateFilter->SetForegroundValue(1);
  dilateFilter->Update();

  *inputImage = dilateFilter->GetOutput();
}

//////////////////////////////////////////////////////////////////////////////////////////
void OutputToStream( std::ostream & stream, string patientId, string volume, std::vector<string> masks, std::vector<string> maskObject )
{
  stream  << "<patient>" << endl;
  stream  << "  <ignoreorientation>1</ignoreorientation>" << endl;
  stream  << "  <ignoreorigin>0</ignoreorigin>" << endl; 
  stream  << "  <bias_correction>0</bias_correction>" << endl;
  stream  << "  <patient_id>" << patientId << "</patient_id>" << endl;
  stream  << "  <study>" << endl;

  // display variables
  stream << "    <description> Nijmegen patient: "<< patientId  << "</description>" << endl;
  stream << "    <modality>"                << "MRI"                     << "</modality>"    << endl;
  stream << "    <volume>"                  << volume                    << "</volume>"      << endl;

  for (int i=0; i<(int)masks.size(); i++)
  {
    stream << "    <mask>" << endl;
    stream << "      <object>" << maskObject[i] << "</object>" << endl;
    stream << "      <file>" << masks[i] << "</file>" << endl;
    stream << "    </mask>" << endl;
  } // for each mask
  stream << "  </study>" << endl;
  stream << "</patient>" << endl << endl;

} // OutputToStream
//////////////////////////////////////////////////////////////////////////////////////////
int main( int argc, char * argv[] )
{

  if ( argc < 2 )
  {
    cout << "Usage:" << argv[ 0 ] << " input.xml output.xml" << endl;
    cout << "  Format of xml: see file \'/src/Data/Patients/data_files/BU_CG_PZ_Segmentation.xml\' " << endl;
    return EXIT_SUCCESS;
  }

  // extract the inputs
  const string xmlFileName = argv[1];
  const string outXmlFileName = argc<3?"output.xml":argv[2];

  const ccipd::XMLPointer rootNode = ccipd::GetXMLRootNode(xmlFileName);
  if (rootNode)
    cout << "Reading Data from XML" << xmlFileName <<  endl;
  else
  {
    cout << "Can't find XML file" << xmlFileName << endl;
    exit(1);
  }

  // extract the inputs
  string
    inputFileName,
    outputFileName,
    patientID,
    outputDir,
    outputDirTrain,
    inLabelStr,
    outLabelStr,
    tag,
    extension,
    MRIVolumePath;

  std::vector<VolumeMaskImageConstPointer>
    masks1Label,
    masksRealLabel;
 
  int 
    inLabel,
    outLabel = 1;
        
  vector<int> allLabels;
  std::vector<string> 
    maskFile, 
    maskObject;

  int count = 0;
  
  std::ofstream outputXml;
  outputXml.open (outXmlFileName);
  const auto patientNodes = ccipd::GetXMLChildNodes( *rootNode, "patient" );
  BOOST_FOREACH( const ccipd::XMLPointer & patientNode, patientNodes )
  {
    count++;

    maskFile.clear();
    maskObject.clear();
    cout << "####################################### " << count << " ########################################### " << endl;
    cout << "#### " << endl;
    cout << "#### " << endl;

   //parse patient
    string patientID;
    ccipd::GetXMLString( *patientNode, "patient_id", patientID      );

    
    auto studyNodes = ccipd::GetXMLChildNodes(*patientNode, "study");
    BOOST_FOREACH( const ccipd::XMLPointer & studyNode, studyNodes )
    {
      VolumeImagePointer MultiLabelVol;
      ccipd::GetXMLString( *studyNode, "volume",      MRIVolumePath  );

      VolumeImagePointer dicomVol  = ccipd::LoadVolume(MRIVolumePath);

      cout << "#### " ;
      cout << "Loading " << MRIVolumePath << " done. \n####  Range [" << int(ccipd::GetMin(dicomVol)) << " " << int(ccipd::GetMax(dicomVol)) << "]" << endl;

      masksRealLabel.clear();
      masks1Label.clear();
      auto maskNodes = ccipd::GetXMLChildNodes(*studyNode, "mask");
      BOOST_FOREACH( const ccipd::XMLPointer & maskNode, maskNodes )
      {
        ccipd::GetXMLString( *maskNode, "file",      inputFileName  );
        ccipd::GetXMLString( *maskNode, "outputDir", outputDir      );
        ccipd::GetXMLString( *maskNode, "outputDir2",outputDirTrain );
        ccipd::GetXMLString( *maskNode, "inlabel",   inLabelStr     );
        ccipd::GetXMLString( *maskNode, "outlabel",  outLabelStr    );
        ccipd::GetXMLString( *maskNode, "object",    tag            );
        ccipd::GetXMLString( *maskNode, "extension", extension      );


        if (tag[0]=='C' || tag[0]=='P') // exclude CG and PZ 
          continue; 

        inLabel  = atoi (inLabelStr.c_str());
        outLabel = atoi (outLabelStr.c_str());

        if (patientID.compare(0, 16, "ProstateDx-00-00")!=0) // if it does not start with ProstateDx then needs to be determined
          patientID = GetPatientName(MRIVolumePath);
    
        cout << "####\n#### reading "  << patientID << " from :" << inputFileName <<  " #####\n#### " << endl;

        
        VolumeImagePointer MultiLabelVol  = ccipd::LoadVolume( inputFileName ) ;
        cout << MultiLabelVol->GetLargestPossibleRegion().GetSize() << endl << dicomVol->GetLargestPossibleRegion().GetSize() << endl;
        if ( (MultiLabelVol->GetLargestPossibleRegion().GetSize()[0] != dicomVol->GetLargestPossibleRegion().GetSize()[0]) || 
             (MultiLabelVol->GetLargestPossibleRegion().GetSize()[1] != dicomVol->GetLargestPossibleRegion().GetSize()[1]) || 
             (MultiLabelVol->GetLargestPossibleRegion().GetSize()[2] != dicomVol->GetLargestPossibleRegion().GetSize()[2])
          )
        {
          cout << "Data doesn't have the same size!!!" << endl;    
          return EXIT_FAILURE;
        }
        if ( !MultiLabelVol )
        {
          cerr << "Loading " << inputFileName << " failed." <<endl;
          return EXIT_FAILURE;
        }
        cout << "#### Loading " << inputFileName << " done. Max = " << int(ccipd::GetMax(MultiLabelVol)) << endl;
        
        vector<int> inLabels;
        inLabels.push_back(inLabel);
        allLabels.push_back(inLabel);
        VolumeMaskImageConstPointer SingleLabelVol = ccipd::ExtractLabels(MultiLabelVol, inLabels, outLabel);

        masksRealLabel.push_back(SingleLabelVol); 
        VolumeMaskImageConstPointer SingleLabelVol1  = ccipd::ExtractLabels(MultiLabelVol, inLabels, 1);
        masks1Label.push_back(SingleLabelVol1); 
  

        if (outputDir.size()>0) // should save individual regions
        {
          //outputFileName = outputDir + "/" + patientID;
          //cout << "#### Creating folder" << outputFileName << endl; 
          //boost::filesystem::create_directory( boost::filesystem::path( outputFileName ) );
          //outputFileName = outputFileName + "/" + patientID + "_" + tag + "." + extension;
          
          outputFileName = outputDir + "/" + patientID + "_" + tag + "." + extension;

          // save the result
          cout << "#### Data range [" << int(ccipd::GetMin(SingleLabelVol1)) << "," <<  int(ccipd::GetMax(SingleLabelVol1)) << "] " << ccipd::GetVolume(SingleLabelVol1) <<  endl;
          bool savedSuccessfully = ccipd::SaveImage( SingleLabelVol1.GetPointer(), outputFileName, true );
          if ( !savedSuccessfully )
          {
            cerr << "Saving extraction result failed. Can't write to " << outputFileName << endl;
            return EXIT_FAILURE;
          }
        }
        maskFile.push_back(outputFileName);
        maskObject.push_back(tag);
      }

      //reunion of all masks
      /*
      VolumeMaskImageConstPointer MultiLabelVol2 = ccipd::MergeLabels(masksRealLabel[0], masksRealLabel[1]);
      outputFileName = outputDirTrain + "/" + patientID + ".nrrd";
      if (outputDirTrain.size()>0) // outputdir2 - write out output file should be wrote out
      {
        bool savedSuccessfully = ccipd::SaveImage( MultiLabelVol2.GetPointer(), outputFileName, true );
        if ( !savedSuccessfully )
        {
          cerr << "Saving extraction result failed. Can't write to " << outputFileName << endl;
          return EXIT_FAILURE;
        }
      }

      */
      

      if (masks1Label.size()>0)
      {
        VolumeMaskImageConstPointer MultiLabelVol3 = ccipd::MergeLabels(masks1Label[0], masks1Label[1]);

        cout << "#### Data range [" << int(ccipd::GetMin(MultiLabelVol3)) << "," <<  int(ccipd::GetMax(MultiLabelVol3)) << "] " << endl;
        double
          vol0 =   ccipd::GetVolume(masks1Label[0]),
          vol1 =   ccipd::GetVolume(masks1Label[1]),
          vol2 =   ccipd::GetVolume(MultiLabelVol3);
      
        cout << "####" << maskObject[0] << " : "  << vol0 << "/" << vol2 <<  " = " << vol0/vol2 <<  endl;
        cout << "####" << maskObject[1] << " : "  << vol1 << "/" << vol2 <<  " = " << vol1/vol2 <<  endl;

        if (vol0 == 0 || vol1 == 0 || vol2 == 0)
        {
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Probably no label found!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 " << endl;
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Probably no label found!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 " << endl;
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Probably no label found!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 " << endl;
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Probably no label found!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 " << endl;
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Probably no label found!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 " << endl;
            cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Probably no label found!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1 " << endl;
            return EXIT_FAILURE;
        }
      }

      /*

      if (outputDir.size()>0)
      {
        tag = "Pr";
        outputFileName = outputDir + "/" + patientID + "/" + patientID + "_" + tag + "." + extension;

        // save the result
       bool savedSuccessfully = ccipd::SaveImage(MultiLabelVol2.GetPointer(), outputFileName, true );
        if ( !savedSuccessfully )
        {
          cerr << "Saving extraction result failed. Can't write to " << outputFileName << endl;
          return EXIT_FAILURE;
        }
      }*/

      maskFile.push_back(outputFileName);
      maskObject.push_back(tag);

      OutputToStream(outputXml, patientID, MRIVolumePath, maskFile, maskObject);
    } 
    cout << "################################################################################## " << endl;
  }
  outputXml.close();
  
  return EXIT_SUCCESS;

} // main



