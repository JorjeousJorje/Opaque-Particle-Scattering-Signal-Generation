#pragma once
#include <fstream>

#include "A13SignalGenerator.h"
#include "DatasetGenerator.h"
#include "ParametersParsers.h"
#include "Utility.h"

class A13DatasetGenerator: public DatasetGenerator {
	P0ParametersParser _parserP0{};
	P21P22ParametersParser _parserP21P22{};
	Polarization _pol{ Polarization::one };

	std::string _pathP0;
	std::string _pathP21P22;
	std::size_t _signalLen{};

public:
	A13DatasetGenerator(std::string iPathP0, std::string iPathP21P22)
		: _pathP0{std::move(iPathP0)}, _pathP21P22{ std::move(iPathP21P22)}
	{
		
	}

	A13DatasetGenerator(std::string_view iPathP0, std::string_view iPathP21P22)
		: _pathP0{ iPathP0 }, _pathP21P22{ iPathP21P22 }
	{

	}

	void generateDataset(const valVec& iTime, const valVec& iThetasScattering, const LaserParticleParameters& iLPParams) override {
		_generatedSignals.reserve(iThetasScattering.size());
		_signalLen = iTime.size();

		/*for (std::size_t i{}; i < iThetasScattering.size(); ++i) {
			const auto paramsP0 = _parserP0.parseSignalParameters(_pathP0, iThetasScattering[i]).value();
			const auto paramsP21P22 = _parserP21P22.parseSignalParameters(_pathP21P22, iThetasScattering[i]);

			const auto& paramsP21 = paramsP21P22[ScatteringMode::P21];
			const auto& paramsP22 = paramsP21P22[ScatteringMode::P22];

			A13SignalGenerator signalGenerator{ {paramsP0, paramsP21, paramsP22}, iLPParams };
			_generatedSignals.emplace_back(signalGenerator.generateSignal(iTime, _pol));
		}*/

		const auto paramsP0 = _parserP0.parseSignalParameters(_pathP0, iThetasScattering);
		const auto paramsP21P22 = _parserP21P22.parseSignalParameters(_pathP21P22, iThetasScattering);

		const auto paramsP21 = paramsP21P22(ScatteringMode::P21);
		const auto paramsP22 = paramsP21P22(ScatteringMode::P22);

		for(auto itP0 = paramsP0.begin(), itP21 = paramsP21.begin(), itP22 = paramsP22.begin(); itP0 != paramsP0.end(); ++itP0, ++itP21, ++itP22) {
			A13SignalGenerator signalGenerator{ {{*itP0, *itP21}, *itP22}, iLPParams };
			_generatedSignals.emplace_back(signalGenerator.generateSignal(iTime, _pol));
		}
	}


	void saveDataset(const std::string& iFilePath) override {

		if (!_generatedSignals.empty()) {
			std::ofstream file{ iFilePath };

			setHeader(file);


			for (std::size_t i{}; i < _generatedSignals.size(); ++i) {
				auto&& holder = _generatedSignals[i];
				const auto signal = holder.getResultSignal();
				const auto t0 = holder.getSignalPeak(ScatteringMode::P0);
				const auto t21 = holder.getSignalPeak(ScatteringMode::P21);
				const auto t22 = holder.getSignalPeak(ScatteringMode::P22);
				const auto sigma = holder.getSigmaWidth(ScatteringMode::P0);

				file << i << ',';
				file << holder.signalsNum() << ',';
				file << std::setprecision(3);
				file << sigma << ',';
				file << std::fixed;
				file << std::abs(t21 - t0) << ',';
				file << std::abs(t22 - t0) << ',';
				file << std::abs(t22 - t21) << ',';

				file << std::setprecision(0);
				std::ranges::copy(signal,std::ostream_iterator<double>(file, ","));
				file << std::endl;
			}
			return;
		}
		std::cout << "Dataset wasn't generated" << std::endl;

	}


private:

	void setHeader(std::ofstream& iFStream) {
		iFStream << "Signal-ID" << ',';
		iFStream << "NumPeaks" << ',';
		iFStream << "Sigma" << ',';
		iFStream << "delta_021" << ',';
		iFStream << "delta_022" << ',';
		iFStream << "delta_2122" << ',';

		for(std::size_t i{}; i < _signalLen; ++i) {
			iFStream << "a" << i << ',';
		}

		iFStream << std::endl;
	}

};