#pragma once
#pragma once
#include <fstream>

#include "A6SignalGenerator.h"
#include "DatasetGenerator.h"
#include "ParametersParsers.h"
#include "Utility.h"

class A6DatasetGenerator : public DatasetGenerator {
	P0ParametersParser _parserP0{};
	Polarization _pol{ Polarization::one };

	std::string _pathP0;

public:
	A6DatasetGenerator(std::string iPathP0)
		: _pathP0{ std::move(iPathP0) }
	{
	}

	A6DatasetGenerator(std::string_view iPathP0)
		: _pathP0{ iPathP0 }
	{
	}

	void generateDataset(const valVec& iTime, const valVec& iThetasScattering, const LaserParticleParameters& iLPParams) override {
		_generatedSignals.reserve(iThetasScattering.size());
		_signalLen = iTime.size();

		const auto paramsP0 = _parserP0.parseSignalParameters(_pathP0, iThetasScattering);


		for (auto itP0 = paramsP0.begin(); itP0 != paramsP0.end(); ++itP0) {
			A6SignalGenerator signalGenerator{ *itP0, iLPParams };
			_generatedSignals.emplace_back(signalGenerator.generateSignal(iTime, _pol));
		}
	}

	void generateDataset(const valVec& iTime, const valVec& iDiameters, LaserParticleParameters& iLPParams, const double iThetaScattering) override {
		_generatedSignals.reserve(iDiameters.size());
		_signalLen = iTime.size();

		const auto paramsP0 = _parserP0.parseSignalParameters(_pathP0, iThetaScattering).value();

		for (const auto diameter : iDiameters) {
			iLPParams.d = diameter;
			A6SignalGenerator signalGenerator{ paramsP0, iLPParams };
			_generatedSignals.emplace_back(signalGenerator.generateSignal(iTime, _pol));

		}
	}

	void generateDataset(const valVec& iTime, const valVec& iDiameters, const valVec& iVelocity, const valVec& iThetasScattering, LaserParticleParameters& iLPParams) override {
		_generatedSignals.reserve(iThetasScattering.size());
		_signalLen = iTime.size();

		const auto paramsP0 = _parserP0.parseSignalParameters(_pathP0, iThetasScattering);


		for (auto itP0 = paramsP0.begin(); itP0 != paramsP0.end(); ++itP0) {

			for (const auto v : iVelocity) {
				iLPParams.v = v;
				for (const auto diameter : iDiameters) {
					iLPParams.d = diameter;
					A6SignalGenerator signalGenerator{ *itP0, iLPParams };
					_generatedSignals.emplace_back(signalGenerator.generateSignal(iTime, _pol));

				}

			}
		}
	}


	void saveDataset(const std::string& iFilePath) override {

		if (!_generatedSignals.empty()) {
			std::ofstream file{ iFilePath };

			setHeader(file);

			for (std::size_t i{}; i < _generatedSignals.size(); ++i) {
				auto&& holder = _generatedSignals[i];
				const auto signal = holder.getResultSignal();
				const auto sigma = holder.getSigmaWidth(ScatteringMode::P0);

				file << i << ',';
				file << holder.signalsNum() << ',';
				file << std::setprecision(3);
				file << sigma << ',';

				file << std::fixed;
				file << std::setprecision(0);

				std::copy(begin(signal), end(signal) - 1, std::ostream_iterator<double>(file, ","));
				std::copy(end(signal) - 1, end(signal), std::ostream_iterator<double>(file));
				file << std::endl;
			}
			return;
		}
		std::cout << "Dataset wasn't generated" << std::endl;

	}


private:

	void setHeader(std::ofstream& iFStream) override {
		DatasetGenerator::setHeader(iFStream);

		for (std::size_t i{}; i < _signalLen; ++i) {
			iFStream << "a" << i;

			if (i != _signalLen - 1)
				iFStream << ',';
		}

		iFStream << std::endl;
	}

};