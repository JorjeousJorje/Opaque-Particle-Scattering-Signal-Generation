
#include "A13DatasetGenerator.h"
#include "A11DatasetGenerator.h"
#include "A6DatasetGenerator.h"
#include "PeakClassificationDatasetGenerator.h"
#include "Utility.h"
#include <thread>

using namespace Utility;

constexpr std::string_view filePathParametersP0{ "(amp)(p=0)(m=1.343).dat" };
constexpr std::string_view filePathParametersP21P22{ "(amp)(p=2.1 p=2.2)(m=1.343).dat" };

int main() {

	/*A13DatasetGenerator gen13{ filePathParametersP0 , filePathParametersP21P22 };
	A11DatasetGenerator gen11{ filePathParametersP0 , filePathParametersP21P22 };
	A6DatasetGenerator gen6{ filePathParametersP0 };*/

	LaserParticleParameters laserParameters{
		100.0,
		10.0,
		10.0,
	};

	PeakClassificationDatasetGenerator gen{ filePathParametersP0 , filePathParametersP21P22 };

	const auto thetas = GenerateLinspaceWithStep(165.0, 165.0, 1.0);
	const auto diameters = GenerateLinspaceWithStep(50, 110, 1.0);
	const auto velocities = GenerateLinspaceWithStep(2.0, 15.0, 1.0);
	const auto time = GenerateLinspace(-10.5, 35.0, 1000);

	/*const auto thetas = GenerateLinspaceWithStep(165.0, 165.0, 1.0);
	const auto diameters = GenerateLinspaceWithStep(80, 110, 0.5);
	const auto velocities = GenerateLinspaceWithStep(8.0, 10.0, 0.5);
	const auto time = GenerateLinspace(-3.5, 10.0, 1000);*/


	gen.generateDataset(time, diameters, velocities, thetas, laserParameters);
	gen.saveDataset("C:/Users/georg/Desktop/Last work moments/REFRESHED/Classification/data/dataset.txt");

	return 0;
}