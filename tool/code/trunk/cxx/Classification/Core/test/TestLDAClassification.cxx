#include "ccipdLDAClassifier.h"

#include <iostream>


using namespace Eigen;
using namespace ccipd;
using namespace std;

int main()
{
  // Declare the QDA Classifier object
   ccipdLDAClassifier ldaClassifier;

   // Train the Classifier
   EigenMatrixTypeD trainingSet((unsigned int)9, (unsigned int)2);
   trainingSet << 8,7,4,2,2,4,2,3,4,4,9,10,6,8,9,5,10,8;
   EigenVectorTypeU group((unsigned int)9);
   group << 2, 1, 1, 1, 1, 2, 2, 2, 2; //, 2, 1, 1, 1, 1, 2, 2, 2, 2;

   ldaClassifier.Train(trainingSet, group);

   // OR Set the training parameters directly
   //EigenVectorTypeD prob(2);
   //prob << 0.5, 0.5;

   //EigenVectorTypeD logdetSigma(1);
   //logdetSigma << 16.576378212484870;
   //
   //EigenMatrixTypeD R(50, 50);
   //R << 2.066709, -0.491517, -2.735259, -2.188762, -1.889005, -1.857954, -2.034370, -2.054330, -2.092334, -2.353009, -1.040113, -0.966053, -1.036019, -1.343567, -1.724301, -1.576138, -1.234968, -1.123268, -0.933127, -0.889832, -0.986877, -1.272452, -1.576840, -1.473537, -1.168725, -1.039600, -0.796433, -0.783423, -0.886872, -1.109174, -1.353966, -1.298852, -1.032410, -0.902544, -0.625965, -0.614426, -0.620654, -0.766638, -1.025081, -0.880633, -0.701160, -0.673102, -0.316382, -0.330032, -0.347587, -0.504297, -0.656330, -0.643453, -0.441412, -0.364261, 0.000000, 1.273986, -0.807115, -0.447937, -0.358519, -0.120522, -0.089398, -0.121316, -0.331152, -0.437223, 0.132106, 0.129242, 0.185116, 0.311139, 0.356314, 0.318341, 0.224323, 0.147540, 0.247486, 0.239597, 0.279301, 0.398010, 0.439402, 0.417986, 0.346279, 0.269604, 0.318725, 0.291025, 0.341646, 0.455721, 0.495369, 0.511764, 0.418377, 0.337741, 0.395606, 0.412793, 0.433051, 0.596649, 0.822521, 0.724645, 0.471315, 0.426322, 0.408639, 0.425294, 0.370636, 0.613188, 0.813359, 0.745600, 0.450822, 0.408567, 0.000000, 0.000000, -4.726641, -2.569814, -0.922045, -0.167744, 0.188756, -0.231837, -0.933743, -2.739463, -1.002948, -0.486068, 0.036452, 0.292713, 0.437624, 0.245078, -0.019395, -0.609686, -0.592620, -0.270070, 0.098404, 0.301435, 0.415479, 0.260188, 0.038622, -0.360782, -0.463082, -0.211914, 0.091179, 0.269676, 0.346744, 0.229341, 0.040299, -0.280961, -0.431864, -0.232608, 0.012993, 0.174376, 0.250086, 0.139062, -0.023339, -0.298157, -0.281987, -0.224599, -0.065533, 0.069730, 0.116610, 0.035998, -0.080724, -0.229343, 0.000000, 0.000000, 0.000000, -2.377897, -1.581881, -0.366253, -0.104577, -0.300202, -0.267897, 0.301631, 0.205790, -0.618581, -0.546530, -0.090609, 0.108647, 0.099562, 0.190863, 0.396830, 0.183155, -0.347171, -0.331807, -0.034849, 0.103769, 0.115158, 0.200526, 0.337435, 0.142355, -0.267599, -0.253986, -0.017599, 0.088344, 0.110878, 0.190802, 0.297931, 0.063765, -0.214669, -0.214639, -0.045529, 0.054745, 0.074363, 0.151043, 0.225477, -0.019735, -0.126519, -0.162555, -0.071667, 0.005162, 0.039533, 0.094780, 0.079670, 0.000000, 0.000000, 0.000000, 0.000000, 1.893836, 1.428475, 0.570946, 0.410750, 0.369341, 0.525449, -0.284402, -0.233000, 0.496256, 0.592513, 0.202011, 0.013880, -0.073682, -0.078557, -0.284131, -0.230884, 0.203392, 0.306295, 0.103281, -0.040850, -0.123458, -0.153109, -0.262347, -0.204966, 0.110464, 0.215695, 0.082772, -0.053227, -0.117544, -0.158612, -0.190361, -0.127528, 0.073824, 0.155219, 0.090465, -0.005964, -0.068120, -0.126083, -0.086857, -0.049278, 0.061125, 0.100761, 0.070666, 0.029845, -0.035797, -0.067691, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.865611, 1.997974, 0.870843, 0.128795, -0.107574, -0.263906, -0.203624, -0.202013, 0.529007, 0.760240, 0.090655, -0.265120, -0.343814, -0.305258, -0.256093, -0.188724, 0.316650, 0.495317, 0.008675, -0.284664, -0.359775, -0.280069, -0.249462, -0.163427, 0.246532, 0.380266, -0.016963, -0.260922, -0.338458, -0.216046, -0.171319, -0.076259, 0.227977, 0.353473, 0.053123, -0.173559, -0.250736, -0.122735, -0.106619, -0.026963, 0.154701, 0.211565, 0.081487, -0.085071, -0.127318, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.715262, 1.549037, 0.343349, 0.043923, -0.149270, -0.204230, -0.212060, -0.197709, 0.570547, 0.555728, 0.057532, -0.071173, -0.186749, -0.221644, -0.254535, -0.157599, 0.366903, 0.341167, -0.026281, -0.135056, -0.186505, -0.213061, -0.238287, -0.137372, 0.277806, 0.270712, -0.029784, -0.138647, -0.156131, -0.164412, -0.151628, -0.030013, 0.244774, 0.238397, -0.002237, -0.107992, -0.104918, -0.102642, -0.069707, 0.014794, 0.152913, 0.158229, 0.016707, -0.072131, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.712003, 1.759193, 0.478120, -0.310684, -0.339116, -0.331502, -0.216527, 0.045770, 0.749851, 0.529590, -0.059038, -0.278863, -0.305701, -0.279929, -0.212912, 0.011643, 0.481453, 0.334622, -0.084652, -0.240806, -0.266820, -0.248911, -0.193983, 0.011483, 0.392028, 0.265620, -0.079699, -0.197737, -0.223764, -0.182795, -0.125677, 0.020794, 0.275163, 0.186854, -0.050717, -0.105861, -0.123729, -0.108405, -0.063769, 0.057698, 0.169513, 0.113902, -0.029172, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 2.021426, 1.780307, -0.069124, -0.121218, -0.112855, -0.124373, -0.185885, 0.003594, 0.746667, 0.514855, -0.074974, -0.134492, -0.128360, -0.114138, -0.169626, -0.036863, 0.434529, 0.308846, -0.069781, -0.119934, -0.122280, -0.103445, -0.127746, -0.035492, 0.314755, 0.235352, 0.002762, -0.072695, -0.077763, -0.066231, -0.093772, -0.034935, 0.206473, 0.200546, 0.067243, -0.003949, -0.056265, -0.048997, -0.046196, 0.028152, 0.139351, 0.128562, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.727495, -0.030880, -0.179780, -0.179999, -0.173777, -0.102076, -0.139960, 0.008262, 0.502247, -0.011085, -0.146245, -0.135515, -0.141409, -0.091326, -0.078860, 0.069528, 0.327436, 0.034885, -0.097475, -0.104211, -0.115915, -0.078854, -0.045027, 0.085866, 0.269978, 0.055105, -0.050770, -0.071618, -0.082548, -0.054862, -0.004524, 0.092552, 0.183200, 0.046563, -0.012511, -0.059421, -0.055743, -0.026982, 0.019595, 0.070822, 0.087983, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.534060, -0.785495, -0.195635, 0.048602, 0.142654, 0.050455, -0.179295, -0.812129, -1.153868, -0.676663, -0.154677, 0.118103, 0.210561, 0.126350, -0.142962, -0.694818, -0.669820, -0.451756, -0.121192, 0.123927, 0.200976, 0.144969, -0.080921, -0.448491, -0.404761, -0.224002, -0.037179, 0.147194, 0.259493, 0.210800, -0.036890, -0.335619, -0.167529, -0.128471, -0.029296, 0.169403, 0.221202, 0.149749, -0.004878, -0.155212, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.207001, -0.675023, -0.101513, 0.071805, 0.047497, 0.011168, 0.194415, -0.036614, -0.797351, -0.549624, -0.107695, 0.063393, 0.075601, 0.068420, 0.180494, -0.046538, -0.409790, -0.319412, -0.088324, 0.054839, 0.077721, 0.075060, 0.112307, -0.011967, -0.188726, -0.171370, -0.029413, 0.094148, 0.117341, 0.079735, 0.093447, 0.003297, -0.081064, -0.105140, 0.007308, 0.076126, 0.099647, 0.080158, 0.051206, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.167258, 0.667706, 0.099161, -0.029816, -0.005524, 0.089652, -0.025251, 0.018515, 0.707918, 0.506316, 0.078559, -0.069221, -0.027998, 0.037852, -0.030988, 0.038712, 0.326231, 0.286966, 0.042777, -0.086744, -0.047630, -0.011740, -0.011793, 0.027001, 0.112217, 0.105540, -0.014652, -0.103068, -0.079179, -0.021986, -0.020405, -0.006103, 0.037147, 0.030382, -0.013224, -0.055269, -0.048383, -0.030908, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.229750, 0.813474, 0.091878, -0.054360, -0.075657, -0.041689, -0.037730, 0.029680, 0.746141, 0.618680, 0.060465, -0.100775, -0.116291, -0.070220, -0.054038, 0.054252, 0.396550, 0.370870, 0.026337, -0.107714, -0.118689, -0.084929, -0.068379, 0.005625, 0.216693, 0.226052, 0.012092, -0.071286, -0.087949, -0.067795, -0.067798, 0.006964, 0.116779, 0.122719, 0.026868, -0.039468, -0.062593, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.314858, 0.763292, 0.084118, -0.008010, -0.059897, -0.056827, -0.037169, 0.103548, 0.873038, 0.576265, 0.053391, -0.049747, -0.095407, -0.094125, -0.058240, 0.133707, 0.521114, 0.385288, 0.040020, -0.085352, -0.116205, -0.103279, -0.067258, 0.105473, 0.346594, 0.205381, -0.015985, -0.097027, -0.111509, -0.094548, -0.052584, 0.085624, 0.188775, 0.122259, -0.012451, -0.109744, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.229535, 0.711611, 0.125793, -0.017388, -0.041576, -0.042918, -0.066324, 0.084141, 0.822735, 0.487817, 0.086859, -0.046031, -0.075700, -0.086679, -0.093507, 0.071015, 0.462878, 0.293282, 0.036410, -0.072834, -0.086965, -0.084406, -0.102309, -0.013954, 0.210842, 0.149494, -0.009569, -0.063833, -0.070043, -0.056510, -0.065190, 0.029282, 0.124402, 0.072679, -0.026765, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.193122, -0.647007, -0.019351, 0.042672, 0.034868, 0.023843, 0.041052, -0.129209, -0.808274, -0.451366, -0.033830, 0.029315, 0.031357, 0.041128, 0.050909, -0.091232, -0.400245, -0.279219, -0.045075, 0.003757, -0.001387, 0.029261, 0.073966, 0.019218, -0.165225, -0.164737, -0.036447, -0.019581, 0.010580, 0.037779, 0.045981, -0.003693, -0.080015, -0.069239, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.035733, -0.058702, 0.027562, -0.000758, 0.038588, 0.037169, 0.054664, -0.110247, -0.685935, -0.095789, 0.007666, -0.007834, 0.032124, 0.036652, 0.024280, -0.120481, -0.335565, -0.073072, 0.003276, 0.006340, 0.044968, 0.071258, 0.034977, -0.102793, -0.194301, -0.042033, 0.007900, 0.047617, 0.070179, 0.046446, 0.008808, -0.038240, -0.097784, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.087033, -0.491030, -0.092519, -0.009039, 0.031692, 0.012130, -0.082150, -0.510763, -0.968740, -0.491698, -0.133201, -0.009776, 0.036695, 0.020137, -0.096819, -0.536069, -0.339090, -0.208721, -0.055634, 0.036651, 0.088739, 0.074591, -0.025316, -0.240991, -0.118801, -0.077144, -0.024975, 0.040074, 0.062269, 0.040516, -0.047581, -0.124188, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.958125, -0.536910, -0.095829, -0.005364, 0.010211, 0.009851, 0.150439, -0.085391, -0.786417, -0.509221, -0.107511, 0.009859, 0.010641, -0.001818, 0.106277, -0.075885, -0.254149, -0.185155, -0.041763, 0.035105, 0.059729, 0.027987, 0.029304, -0.061162, -0.113244, -0.108957, -0.037816, 0.005552, 0.048640, 0.047977, 0.002759, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.997227, 0.562900, 0.132771, 0.045858, 0.034213, 0.065138, 0.017496, 0.106420, 0.822329, 0.558757, 0.148488, 0.053094, 0.050127, 0.057743, 0.026131, 0.154106, 0.271184, 0.206246, 0.076080, 0.006093, 0.002887, 0.011359, 0.009082, 0.061904, 0.119990, 0.093774, 0.037806, 0.008387, -0.000504, -0.005060, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.085364, -0.700616, -0.153291, -0.007812, 0.039924, 0.020183, 0.007969, -0.106222, -0.861579, -0.670631, -0.164035, 0.006308, 0.050191, 0.051347, 0.019000, -0.074363, -0.298381, -0.307733, -0.064941, 0.031762, 0.068301, 0.058214, 0.053433, -0.018947, -0.156873, -0.170036, -0.067355, 0.006759, 0.032620, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.122871, 0.661593, 0.067769, 0.007451, -0.014542, -0.008313, -0.004374, 0.123571, 0.895610, 0.624699, 0.093536, 0.000707, -0.056682, -0.054517, -0.025490, 0.175753, 0.463760, 0.331170, 0.097242, -0.024014, -0.129022, -0.142941, -0.112148, 0.075089, 0.229106, 0.178855, 0.036015, -0.082424, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.068881, 0.553680, 0.059709, -0.008994, -0.014384, 0.007586, 0.015200, 0.145612, 0.890003, 0.519487, 0.068116, -0.009633, -0.022707, -0.023148, -0.037072, 0.079243, 0.350109, 0.270406, 0.074385, -0.052104, -0.068214, -0.059765, -0.034513, 0.064001, 0.165758, 0.131136, 0.009241, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.024907, 0.487588, 0.048158, 0.008422, 0.018830, 0.007004, -0.019167, 0.130457, 0.830368, 0.486366, 0.084531, -0.001625, 0.009721, 0.020666, 0.020981, 0.063063, 0.247517, 0.248672, 0.049659, 0.015599, -0.021070, 0.004096, 0.010791, 0.052122, 0.104986, 0.077323, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.841741, 0.101074, -0.005838, -0.003577, -0.027022, 0.028015, -0.013801, 0.117194, 0.697208, 0.150588, 0.018839, 0.015484, 0.013286, 0.015579, 0.026344, 0.151099, 0.282441, 0.065225, 0.014475, -0.026727, -0.043020, -0.016673, 0.012511, 0.059143, 0.110228, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.013911, -0.474955, -0.119206, -0.048587, -0.005093, 0.004944, -0.088413, -0.480608, -0.677682, -0.427321, -0.121567, 0.014459, 0.086348, 0.013804, -0.156421, -0.503123, -0.287136, -0.222859, -0.062480, 0.012030, 0.048304, 0.011325, -0.116512, -0.260852, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.871871, -0.471921, -0.073538, -0.006530, -0.025673, 0.002981, 0.143776, -0.166168, -0.454861, -0.350928, -0.113324, -0.014987, 0.021772, 0.048993, 0.072901, -0.112301, -0.216385, -0.204651, -0.095182, 0.000481, 0.028901, 0.050842, 0.015483, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.895415, 0.469747, 0.061605, 0.047864, 0.038617, 0.050622, 0.025033, 0.158522, 0.418337, 0.298618, 0.085496, 0.028654, 0.043140, 0.025720, 0.010220, 0.103091, 0.198259, 0.166252, 0.049710, -0.022876, -0.059428, -0.037327, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.956604, -0.590895, -0.147439, -0.033291, 0.008462, 0.054906, 0.027420, -0.096950, -0.440628, -0.462170, -0.135691, 0.004183, 0.062224, 0.096048, 0.075690, -0.041681, -0.219660, -0.245371, -0.104449, 0.033919, 0.089737, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.990970, -0.626707, -0.099406, -0.029999, 0.021516, -0.001841, -0.021835, -0.167863, -0.568888, -0.461499, -0.178661, -0.046871, 0.067535, 0.078787, 0.060402, -0.098973, -0.295058, -0.253400, -0.069554, 0.017995, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.941055, -0.484654, -0.049657, -0.019673, 0.001199, 0.016332, 0.011010, -0.182010, -0.506364, -0.402679, -0.146582, -0.018686, 0.014188, 0.027191, -0.021323, -0.140722, -0.242740, -0.177738, -0.070251, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.903796, -0.448782, -0.130933, -0.010974, -0.015476, -0.025531, -0.019290, -0.089579, -0.436551, -0.403256, -0.094952, -0.018897, 0.023119, 0.013399, 0.020611, -0.071058, -0.189870, -0.167389, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.787272, -0.182135, -0.040920, 0.008057, 0.019180, 0.032885, 0.028163, -0.152745, -0.401118, -0.116563, -0.034240, 0.015351, 0.039267, 0.054482, -0.024592, -0.113204, -0.179208, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.523361, 0.850975, 0.223036, -0.062035, -0.143237, -0.072875, 0.220055, 0.944140, 0.729833, 0.575536, 0.204096, -0.008621, -0.123914, -0.007422, 0.257743, 0.599143, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.145418, 0.827462, 0.217999, 0.054334, -0.036640, -0.077005, -0.319434, 0.203227, 0.444090, 0.442017, 0.193625, 0.018593, -0.009905, -0.082831, -0.025493, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.082948, 0.707948, 0.175890, -0.009149, 0.010285, 0.103359, 0.078351, 0.142814, 0.308065, 0.292507, 0.088243, -0.023544, -0.049290, 0.040633, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.159295, -1.124483, -0.267649, 0.038125, 0.087826, 0.097969, 0.035776, -0.104745, -0.510023, -0.542664, -0.207596, 0.061063, 0.134342, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 1.240260, 0.936864, 0.175470, 0.044345, -0.167150, -0.171595, -0.134632, 0.160720, 0.491755, 0.387173, 0.026882, -0.107274, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.163272, -0.777745, -0.131811, 0.003892, 0.014994, 0.027179, 0.060063, -0.158193, -0.459859, -0.367049, -0.107570, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.154222, -0.690639, -0.206207, -0.048078, 0.053842, 0.038417, 0.024997, -0.095754, -0.435078, -0.439077, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.886746, -0.078642, 0.009322, -0.011791, 0.012919, -0.006622, -0.049828, -0.102945, -0.249901, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -1.220703, -0.947819, -0.365103, -0.152076, -0.005016, -0.069747, -0.278884, -0.792069, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.865066, -0.761221, -0.185410, 0.004248, 0.013823, 0.135947, 0.349382, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.938425, -0.615745, -0.094865, -0.007197, -0.124714, -0.140317, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.905758, 0.617248, 0.189716, -0.052760, -0.064273, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.838319, -0.728063, -0.091074, -0.058055, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.872991, -0.801078, -0.175314, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.895108, -0.475955, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, -0.626200;

   //EigenMatrixTypeD gmeans(2, 50);
   //gmeans << 0.114832, 0.094554, 0.110079, 0.107600, 0.082117, 0.072799, 0.075908, 0.070672, 0.067235, 0.085492, 0.096833, 0.089794, 0.090223, 0.102777, 0.104021, 0.088298, 0.075538, 0.080190, 0.098492, 0.104377, 0.105746, 0.114144, 0.111863, 0.099518, 0.088171, 0.087729, 0.104472, 0.120152, 0.114447, 0.116455, 0.105879, 0.092785, 0.089487, 0.093043, 0.099275, 0.097575, 0.088788, 0.078419, 0.102819, 0.076610, 0.059722, 0.077735, 0.069094, 0.071039, 0.043221, 0.034445, 0.051672, 0.031643, 0.027865, 0.050874, -0.893237, -0.715003, -0.674218, -0.679307, -0.583466, -0.601813, -0.654802, -0.564247, -0.471949, -0.518844, -0.654541, -0.688891, -0.686494, -0.756022, -0.787184, -0.669907, -0.554776, -0.534366, -0.715933, -0.781469, -0.795022, -0.858307, -0.899796, -0.765891, -0.634829, -0.624617, -0.756847, -0.846206, -0.799159, -0.865180, -0.883565, -0.757627, -0.664105, -0.679768, -0.718055, -0.695348, -0.640293, -0.634254, -0.841834, -0.596989, -0.424023, -0.561509, -0.551131, -0.572207, -0.344909, -0.290437, -0.443497, -0.263889, -0.194235, -0.400376;

   //qdaClassifier.SetTrainingParams(prob, logdetSigma, R, gmeans);
   
   // Use the Classifier
   //EigenMatrixTypeD dataToClassify(2/*observationsNumber*/, 50/*featuresNumber*/);
   //dataToClassify << 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25;

   EigenMatrixTypeD dataToClassify(3/*observationsNumber*/, 2/*featuresNumber*/);
   dataToClassify << 8.2, 7.6,4,  20,4.1, 2.1;

   EigenVectorTypeD classificationResult = ldaClassifier.Classify(dataToClassify, 0/*classNum*/);

   cout << classificationResult;



   return EXIT_SUCCESS;
}