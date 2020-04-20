#include "ccipdCore.h"
#include "ccipdHistogram.h"


int main(int argc, char *argv[])
{
  if(argc < 3)
  {
    cout << "Argument 1 is the .csv file from which you want to load the histogram\n"
         << "Argument 2 is the .csv file to which you want to write the histogram.";
    return EXIT_FAILURE;
  }

  ccipd::HistogramFPointer pH = ccipd::LoadHistogram<ccipd::HistogramTypeF>(argv[1]);

  ccipd::WriteHistogram<ccipd::HistogramTypeF>(pH, argv[2]);

  return EXIT_SUCCESS;
}