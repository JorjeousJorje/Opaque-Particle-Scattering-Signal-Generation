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

		const auto paramsP0 = _parserP0.parseSignalParameters(_pathP0, iThetasScattering);
		const auto paramsP21P22 = _parserP21P22.parseSignalParameters(_pathP21P22, iThetasScattering);

		const auto paramsP21 = paramsP21P22(ScatteringMode::P21);
		const auto paramsP22 = paramsP21P22(ScatteringMode::P22);

		for(auto itP0 = paramsP0.begin(), itP21 = paramsP21.begin(), itP22 = paramsP22.begin(); itP0 != paramsP0.end(); ++itP0, ++itP21, ++itP22) {
			A13SignalGenerator signalGenerator{ {{*itP0, *itP21}, *itP22}, iLPParams };
			_generatedSignals.emplace_back(signalGenerator.generateSignal(iTime, _pol));
		}
	}

	void generateDataset(const valVec& iTime, const valVec& iDiameters, LaserParticleParameters& iLPParams, const double iThetaScattering) override {
		_generatedSignals.reserve(iDiameters.size());
		_signalLen = iTime.size();

		const auto paramsP0 = _parserP0.parseSignalParameters(_pathP0, iThetaScattering).value();
		const auto paramsP21P22 = _parserP21P22.parseSignalParameters(_pathP21P22, iThetaScattering);

		const auto paramsP21 = paramsP21P22(ScatteringMode::P21)[0];
		const auto paramsP22 = paramsP21P22(ScatteringMode::P22)[0];


		for(const auto diameter: iDiameters) {
			iLPParams.d = diameter;
			A13SignalGenerator signalGenerator{ {{paramsP0, paramsP21}, paramsP22}, iLPParams };
			_generatedSignals.emplace_back(signalGenerator.generateSignal(iTime, _pol));

		}
	}

	void generateDataset(const valVec& iTime, const valVec& iDiameters, const valVec& iVelocity, const valVec& iThetasScattering, LaserParticleParameters& iLPParams) override {
		_generatedSignals.reserve(iThetasScattering.size());
		_signalLen = iTime.size();

		const auto paramsP0 = _parserP0.parseSignalParameters(_pathP0, iThetasScattering);
		const auto paramsP21P22 = _parserP21P22.parseSignalParameters(_pathP21P22, iThetasScattering);

		const auto paramsP21 = paramsP21P22(ScatteringMode::P21);
		const auto paramsP22 = paramsP21P22(ScatteringMode::P22);

		for (auto itP0 = paramsP0.begin(), itP21 = paramsP21.begin(), itP22 = paramsP22.begin(); itP0 != paramsP0.end(); ++itP0, ++itP21, ++itP22) {

			for(const auto v : iVelocity) {
				iLPParams.v = v;
				iLPParams.sigma = iLPParams.w0 / v;
				for (const auto diameter : iDiameters) {
					iLPParams.d = diameter;
					A13SignalGenerator signalGenerator{ {{*itP0, *itP21}, *itP22}, iLPParams };
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

				const auto t0 = holder.getSignalTimePeak(ScatteringMode::P0);
				const auto t21 = holder.getSignalTimePeak(ScatteringMode::P21);
				const auto t22 = holder.getSignalTimePeak(ScatteringMode::P22);
				const auto a0 = holder.getSignalAmplitude(ScatteringMode::P0);
				const auto a21 = holder.getSignalAmplitude(ScatteringMode::P21);
				const auto a22 = holder.getSignalAmplitude(ScatteringMode::P22);
				const auto sigma = holder.getSigmaWidth(ScatteringMode::P0);

				file << i << ',';
				file << holder.signalsNum() << ',';
				file << std::setprecision(3);
				file << sigma << ',';
				file << std::fixed;
				file << std::abs(t21 - t0) << ',';
				file << std::abs(t22 - t21) << ',';
				file << a21 / a0 << ',';
				file << a22 / a0 << ',';
				file << a22 / a21 << ',';

				file << std::setprecision(0);

				std::copy(begin(signal), end(signal) - 1,std::ostream_iterator<double>(file, ","));
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
		iFStream << "delta_021" << ',';
		iFStream << "delta_2122" << ',';
		iFStream << "amplitude_ratio_210" << ',';
		iFStream << "amplitude_ratio_220" << ',';
		iFStream << "amplitude_ratio_2221" << ',';

		for(std::size_t i{}; i < _signalLen; ++i) {
			iFStream << "a" << i;

			if (i != _signalLen - 1)
				iFStream << ',';
		}

		iFStream << std::endl;
	}

};