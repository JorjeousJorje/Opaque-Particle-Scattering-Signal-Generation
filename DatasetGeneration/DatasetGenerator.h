#pragma once
#include "SignalHolder.h"
struct LaserParticleParameters;

class DatasetGenerator {

protected:
	std::vector<SignalHolder> _generatedSignals{};

public:
	virtual ~DatasetGenerator() = default;
	virtual void generateDataset(const valVec&, const valVec&, const LaserParticleParameters&) = 0;
	virtual void saveDataset(const std::string&) = 0;
};
