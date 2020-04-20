/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

//  Software Guide : BeginCommandLineArgs
//    INPUTS:  {BrainProtonDensitySlice.png}
//    OUTPUTS: {DiscreteGaussianImageFilterOutput.png}
//    ARGUMENTS:    4 9
//  Software Guide : EndCommandLineArgs
//
//  Software Guide : BeginLatex
//
//  \begin{floatingfigure}[rlp]{6cm}
//    \centering
//    \includegraphics[width=6cm]{DiscreteGaussian.eps}
//    \caption[DiscreteGaussianImageFilter Gaussian diagram.]
//            {Discretized Gaussian.\label{fig:DiscretizedGaussian}
//  \end{floatingfigure}
//
//  The \doxygen{DiscreteGaussianImageFilter} computes the convolution of the
//  input image with a Gaussian kernel.  This is done in $ND$ by taking
//  advantage of the separability of the Gaussian kernel.  A one-dimensional
//  Gaussian function is discretized on a convolution kernel.  The size of the
//  kernel is extended until there are enough discrete points in the Gaussian
//  to ensure that a user-provided maximum error is not exceeded.  Since the
//  size of the kernel is unknown a priori, it is necessary to impose a limit to
//  its growth. The user can thus provide a value to be the maximum admissible
//  size of the kernel. Discretization error is defined as the difference
//  between the area under the discrete Gaussian curve (which has finite
//  support) and the area under the continuous Gaussian.
//
//  Gaussian kernels in ITK are constructed according to the theory of Tony
//  Lindeberg \cite{Lindeberg1994} so that smoothing and derivative operations
//  commute before and after discretization.  In other words, finite difference
//  derivatives on an image $I$ that has been smoothed by convolution with the
//  Gaussian are equivalent to finite differences computed on $I$ by convolving
//  with a derivative of the Gaussian.
//
//  \index{itk::DiscreteGaussianImageFilter}
//
//  Software Guide : EndLatex

#include "ccipdCore.h"

#include "ccipdDisableWarningsMacro.h"
  #include <itkImageFileReader.h>
  #include <itkImageFileWriter.h>

//#include "itkRescaleIntensityImageFilter.h"

//  Software Guide : BeginLatex
//
//  The first step required to use this filter is to include its header file.
//
//  \index{itk::DiscreteGaussianImageFilter!header}
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  #include <itkDiscreteGaussianImageFilter.h>
// Software Guide : EndCodeSnippet
#include "ccipdEnableWarningsMacro.h"


int main( int argc, char * argv[] )
{
  if( argc < 5 )
    {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << "  inputImageFile  outputImageFile  variance  maxKernelWidth " << std::endl;
    return EXIT_FAILURE;
    }


  //  Software Guide : BeginLatex
  //
  //  Types should be chosen for the pixels of the input and output images.
  //  Image types can be instantiated using the pixel type and dimension.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  //typedef    float    InputPixelType;
  //typedef    float    OutputPixelType;
  //typedef itk::Image< InputPixelType,  2 >   InputImageType;
  //typedef itk::Image< OutputPixelType, 2 >   OutputImageType;

  typedef    float    InputPixelType;
  typedef itk::CovariantVector<float,2>    VectorPixelType;
  typedef itk::Image< VectorPixelType,  2 >   InputImageType;
  typedef itk::Image< VectorPixelType, 2 >   VectorImageType;
  // Software Guide : EndCodeSnippet


  typedef itk::ImageFileReader< InputImageType >  ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  //  Software Guide : BeginLatex
  //
  //  The discrete Gaussian filter type is instantiated using the
  //  input and output image types.  A corresponding filter object is created.
  //
  //  \index{itk::DiscreteGaussianImageFilter!instantiation}
  //  \index{itk::DiscreteGaussianImageFilter!New()}
  //  \index{itk::DiscreteGaussianImageFilter!Pointer}
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef itk::DiscreteGaussianImageFilter<
                 VectorImageType, VectorImageType >  FilterType;

  FilterType::Pointer filter = FilterType::New();
  // Software Guide : EndCodeSnippet

    // Set up deformation field writer
  const unsigned int                                    Dimension = 2;
  typedef itk::Image< VectorPixelType, Dimension>       DeformationFieldType;
  typedef itk::ImageFileWriter< DeformationFieldType >   DeformationWriterType;



  //  Software Guide : BeginLatex
  //
  //  The input image can be obtained from the output of another
  //  filter. Here, an image reader is used as its input.
  //
  //  Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  filter->SetInput( reader->GetOutput() );
  // Software Guide : EndCodeSnippet


  const double gaussianVariance = atof( argv[3] );
  const unsigned int maxKernelWidth = atoi( argv[4] );


  //  Software Guide : BeginLatex
  //
  //  The filter requires the user to provide a value for the variance
  //  associated with the Gaussian kernel. The method \code{SetVariance()} is
  //  used for this purpose. The discrete Gaussian is constructed as a
  //  convolution kernel.  The maximum kernel size can be set by the user. Note
  //  that the combination of variance and kernel-size values may result in a
  //  truncated Gaussian kernel.
  //
  //  \index{itk::DiscreteGaussianImageFilter!SetVariance()}
  //  \index{itk::DiscreteGaussianImageFilter!SetMaximumKernelWidth()}
  //
  //  Software Guide : EndLatex


  // Software Guide : BeginCodeSnippet
  filter->SetVariance( gaussianVariance );
  filter->SetMaximumKernelWidth( maxKernelWidth );
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  Finally, the filter is executed by invoking the \code{Update()} method.
  //
  //  \index{itk::DiscreteGaussianImageFilter!Update()}
  //
  //  Software Guide : EndLatex


  // Software Guide : BeginCodeSnippet
  filter->Update();
  // Software Guide : EndCodeSnippet


  //  Software Guide : BeginLatex
  //
  //  If the output of this filter has been connected to other filters down
  //  the pipeline, updating any of the downstream filters would have
  //  triggered the execution of this one. For example, a writer could have
  //  been used after the filter.
  //
  //  Software Guide : EndLatex

  //////////////////////////////////////////////////////////////////////////////////////////

  //try to write the deformation field
  DeformationWriterType:: Pointer  deformationWriter = DeformationWriterType::New();

  deformationWriter -> SetFileName ( argv[2] );
  deformationWriter -> SetInput    ( filter->GetOutput() );
  deformationWriter -> UseCompressionOn();

  try
    {
      deformationWriter->Update();
      std::cout << "Done." << std::endl;
    }
  catch( itk::ExceptionObject & err )
    {
      std::cout << "failed." << std::endl;
      std::cerr << "exceptionobject caught !" << std::endl;
      std::cerr << err << std::endl;
      return EXIT_FAILURE;
    }


  //  Software Guide : BeginLatex
  //
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{BrainProtonDensitySlice.eps}
  // \includegraphics[width=0.44\textwidth]{DiscreteGaussianImageFilterOutput.eps}
  // \itkcaption[DiscreteGaussianImageFilter output]{Effect of the
  // DiscreteGaussianImageFilter on a slice from a MRI proton density image of
  // the brain.}
  // \label{fig:DiscreteGaussianImageFilterInputOutput}
  // \end{figure}
  //
  //  Figure~\ref{fig:DiscreteGaussianImageFilterInputOutput} illustrates the
  //  effect of this filter on a MRI proton density image of the brain.
  //
  //  Note that large Gaussian variances will produce large convolution kernels
  //  and correspondingly slower computation times.  Unless a high degree of
  //  accuracy is required, it may be more desirable to use the approximating
  //  \doxygen{RecursiveGaussianImageFilter} with large variances.
  //
  //  Software Guide : EndLatex

  return EXIT_SUCCESS;
}
