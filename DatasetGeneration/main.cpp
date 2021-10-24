
#include "A13DatasetGenerator.h"
#include "Utility.h"

using namespace Utility;

constexpr std::string_view filePathParametersP0{ "(amp)(p=0)(m=1.343).dat" };
constexpr std::string_view filePathParametersP21P22{ "(amp)(p=2.1 p=2.2)(m=1.343).dat" };

int main() {

	A13DatasetGenerator gen{ filePathParametersP0 , filePathParametersP21P22 };

	LaserParticleParameters laserParameters{
		100.0,
		10.0,
		10.0,
	};

	
	const auto thetasScattering = 165.0;
	const auto diameters = GenerateLinspaceWithStep(90, 210, 0.5);
	const auto time = GenerateLinspace(-15.0, 15.0, 400);
	gen.generateDataset(time, diameters, laserParameters, thetasScattering);

	gen.saveDataset("C:/Users/georg/Desktop/Last work moments/Signals test/data/result_pol1.txt");

	return 0;
}