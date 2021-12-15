#pragma once
#include "SignalHolder.h"
struct LaserParticleParameters;

class DatasetGenerator {

protected:
	std::size_t _signalLen{};
	std::vector<SignalHolder> _generatedSignals{};

public:
	virtual ~DatasetGenerator() = default;
	virtual void generateDataset(const valVec&, const valVec&, const LaserParticleParameters&) = 0;
	virtual void generateDataset(const valVec&, const valVec&, const valVec&, const valVec&, LaserParticleParameters&) = 0;
	virtual void generateDataset(const valVec&, const valVec&, LaserParticleParameters&, double) = 0;
	virtual void saveDataset(const std::string&) = 0;

protected:
	virtual void setHeader(std::ofstream& iFStream) {
		iFStream << "Signal-ID" << ',';
		iFStream << "NumPeaks" << ',';
		iFStream << "Sigma" << ',';
	}
};
