#pragma once
#include <fstream>

#include "A11SignalGenerator.h"
#include "A13SignalGenerator.h"
#include "A6SignalGenerator.h"
#include "DatasetGenerator.h"
#include "ParametersParsers.h"
#include "Utility.h"

class PeakClassificationDatasetGenerator : public DatasetGenerator {
	P0ParametersParser _parserP0{};
	P21P22ParametersParser _parserP21P22{};
	Polarization _pol{ Polarization::one };

	std::string _pathP0;
	std::string _pathP21P22;

public:
	PeakClassificationDatasetGenerator(std::string iPathP0, std::string iPathP21P22)
		: _pathP0{ std::move(iPathP0) }, _pathP21P22{ std::move(iPathP21P22) }
	{

	}

	PeakClassificationDatasetGenerator(std::string_view iPathP0, std::string_view iPathP21P22)
		: _pathP0{ iPathP0 }, _pathP21P22{ iPathP21P22 }
	{

	}

	void generateDataset(const valVec& iTime, const valVec& iThetasScattering, const LaserParticleParameters& iLPParams) override {
	}

	void generateDataset(const valVec& iTime, const valVec& iDiameters, LaserParticleParameters& iLPParams, const double iThetaScattering) override {
	}

	void generateDataset(const valVec& iTime, const valVec& iDiameters, const valVec& iVelocity, const valVec& iThetasScattering, LaserParticleParameters& iLPParams) override {
		_generatedSignals.reserve(iThetasScattering.size() * iDiameters.size() * iVelocity.size());
		_signalLen = iTime.size();

		const auto paramsP0 = _parserP0.parseSignalParameters(_pathP0, iThetasScattering);
		const auto paramsP21P22 = _parserP21P22.parseSignalParameters(_pathP21P22, iThetasScattering);

		const auto paramsP21 = paramsP21P22(ScatteringMode::P21);
		const auto paramsP22 = paramsP21P22(ScatteringMode::P22);

		A13SignalGenerator signalGenerator13{ {}, iLPParams };
		A11SignalGenerator signalGenerator11{ {}, iLPParams };
		A6SignalGenerator signalGenerator6{ {}, iLPParams };

		for (auto itP0 = paramsP0.begin(), itP21 = paramsP21.begin(), itP22 = paramsP22.begin(); itP0 != paramsP0.end(); ++itP0, ++itP21, ++itP22) {

			signalGenerator13.setParams({ {*itP0, *itP21}, *itP22 });
			signalGenerator11.setParams({ *itP0, *itP21 });
			signalGenerator6.setParams(*itP0);

			for (const auto v : iVelocity) {
				iLPParams.v = v;
				iLPParams.sigma = iLPParams.w0 / v;
				for (const auto diameter : iDiameters) {
					iLPParams.d = diameter;

					signalGenerator6.setLaserParams(iLPParams);
					signalGenerator11.setLaserParams(iLPParams);
					signalGenerator13.setLaserParams(iLPParams);

					_generatedSignals.emplace_back(signalGenerator13.generateSignal(iTime, _pol));
					_generatedSignals.emplace_back(signalGenerator11.generateSignal(iTime, _pol));
					_generatedSignals.emplace_back(signalGenerator6.generateSignal(iTime, _pol));

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

				file << std::setprecision(2);

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