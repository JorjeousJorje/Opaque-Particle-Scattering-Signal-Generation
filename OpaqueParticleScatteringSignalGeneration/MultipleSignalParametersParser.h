#pragma once
#include <fstream>
#include <optional>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <vector>

#include "ParameterHolder.h"
#include "ScatteringOrderParameters.h"
#include "Utility.h"

class MultipleSignalParametersParser {
protected:
	std::istringstream _parameters{};

	using Modes = std::initializer_list<ScatteringMode>;
	using Params = std::unordered_map<ScatteringMode, ScatteringOrderParameters>;

public:
	virtual ~MultipleSignalParametersParser() = default;

	virtual ParameterHolder parseSignalParameters(const Modes& iModes, const std::string& iFilePath, double iThetaSca) {
		std::ifstream file{ iFilePath };
		Params oParams{};

		for(const auto& mode: iModes) {
			oParams[mode] = {};
		}

		for (std::string buffer; std::getline(file, buffer, '\n'); ) {

			if (setParameters(oParams, buffer, iThetaSca)) {
				break;
			}

		}

		if (!foundThetaScattering(oParams, iThetaSca)) {
			return ParameterHolder{{}};
		}

		_parameters.clear();
		return ParameterHolder{ oParams };
	}

	virtual ParameterHolder parseSignalParameters(const Modes& iModes, const std::string_view& iFilePath, double iThetaSca) {
		return parseSignalParameters(iModes, std::string{ iFilePath.data() }, iThetaSca);
	}

protected:

	bool setParameters(Params& iParams, const std::string& iBuffer, const double iThetaSca) {
		double thetaSca{};
		double m{};
		_parameters.str(iBuffer);
		_parameters >> m;
		_parameters >> thetaSca;


		if (Utility::AlmostEqual(iThetaSca, thetaSca, 10e6)) {
			// ignore '/t' before theta0
			_parameters.ignore(1);
			skipTheta0();

			setAnglesRefractiveIndex(iParams, thetaSca, m);
			setAmplitidesWithP1(iParams);
			setAmplitidesWithP2(iParams);
			return true;
		}

		return false;
	}
	bool foundThetaScattering(const Params& iParams, const double iThetaSca) {

		for(auto&& [mode, param] : iParams) {

			if (!param.thetaSca.has_value()) {
				std::cout << "<parse warning>: file with P" + std::to_string(Utility::to_underlying(param.mode)) + " doesn't have ";
				std::cout << std::setprecision(4);
				std::cout << iThetaSca;
				std::cout << " scattering angle" << std::endl;
				_parameters.clear();
				return false;
			}
		}
		return true;
	}

	virtual ParameterHolder parseSignalParameters(const std::string& iFilePath, double iThetaSca) { return ParameterHolder{ {} }; }
	virtual ParameterHolder parseSignalParameters(const std::string_view& iFilePath, double iThetaSca) { return ParameterHolder{ {} }; }


	void skipTheta0() {
		_parameters.ignore(std::numeric_limits<std::streamsize>::max(), '\t');
	}

	void setAnglesRefractiveIndex(Params& iParams, double iThetaSca, const double iIndex) {
		double theta;
		for (auto& [mode, param] : iParams) {
			param.thetaSca.emplace(iThetaSca);
			param.mode = mode;
			param.m = iIndex;
			_parameters >> theta;
			param.theta = Utility::radians(theta);
		}
	}
	void setAmplitidesWithP1(Params& iParams) {
		for (auto& [mode, param] : iParams) {
			_parameters >> param.ampP1;
		}
	}

	void setAmplitidesWithP2(Params& iParams) {
		for (auto& [mode, param] : iParams) {
			_parameters >> param.ampP2;
		}
	}

};
