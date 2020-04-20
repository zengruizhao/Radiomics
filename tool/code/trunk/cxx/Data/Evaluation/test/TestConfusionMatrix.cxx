#include "ccipdConfusionMatrix.h"
#include "ccipdVolumeTypes.h"
#include "ccipdLoadImage.h"


using namespace ccipd;
using namespace std;

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cerr << "Two arguments are required, the input image and the reference (ground truth) image." << endl;
		return EXIT_FAILURE;
	}


	VolumeImageType::Pointer pInImage = LoadITKImage<VolumeImageType>(argv[1]); // "G:\\Data\\Reduced\\coh833capRed.mhd");

	VolumeImageType::Pointer pRefImage = LoadITKImage<VolumeImageType>(argv[2]); // G:\\Data\\Reduced\\coh833red.mhd");

	ConfusionMatrix cm = CalculateConfusionMatrix<VolumeImageType>(pInImage, pRefImage, 0.0, 1.0);
		
	cout << "True Positives  = " << cm.TP << endl;
	cout << "False Positives = " << cm.TP << endl;
	cout << "True Negatives  = " << cm.TP << endl;
	cout << "False Negatives = " << cm.TP << endl;


	return EXIT_SUCCESS;
}