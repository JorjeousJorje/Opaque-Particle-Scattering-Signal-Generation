
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

	
	const auto thetasScattering = GenerateLinspaceWithStep(139.4, 167.5, 0.1);
	const auto time = GenerateLinspace(0.0, 15.0, 500);
	gen.generateDataset(time, thetasScattering, laserParameters);

	gen.saveDataset("result_pol1.txt");

	return 0;
}