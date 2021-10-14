#pragma once
#include <fstream>
#include <optional>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string_view>

#include "ScatteringOrderParameters.h"
#include "Utility.h"



class SingleSignalParametersParser {
protected:
	std::istringstream _parameters{};
public:
	virtual ~SingleSignalParametersParser() = default;

	std::optional<ScatteringOrderParameters> parseSignalParameters(const ScatteringMode& iMode, const std::string& iFilePath, double iThetaSca) {
		std::ifstream file{ iFilePath };
		ScatteringOrderParameters params;
		for (std::string buffer; std::getline(file, buffer, '\n'); ) {

			if (setParameters(params, buffer, iThetaSca)) {
				break;
			}

		}

		if (!foundThetaScattering(params, iThetaSca)) {
			return std::nullopt;
		}

		params.mode = iMode;
		_parameters.clear();
		return params;
	};

	std::optional<ScatteringOrderParameters> parseSignalParameters(const ScatteringMode& iMode, const std::string_view& iFilePath, double iThetaSca) {
		return parseSignalParameters(iMode, std::string{ iFilePath.data() }, iThetaSca);
	}

protected:

	bool setParameters(ScatteringOrderParameters& iParams, const std::string& iBuffer, const double iThetaSca) {
		double thetaSca;
		_parameters.str(iBuffer);
		_parameters >> iParams.m;
		_parameters >> thetaSca;


		if (Utility::AlmostEqual(iThetaSca, thetaSca, 10e6)) {
			iParams.thetaSca.emplace(thetaSca);
			_parameters >> iParams.theta;
			_parameters >> iParams.ampP1;
			_parameters >> iParams.ampP2;
			return true;
		}

		return false;
	}
	bool foundThetaScattering(const ScatteringOrderParameters& iParams, const double iThetaSca) {
		if (!iParams.thetaSca.has_value()) {
			std::cout << "<parse warning>: file with P0 doesn't have ";
			std::cout << std::setprecision(4);
			std::cout << iThetaSca;
			std::cout << " scattering angle" << std::endl;
			_parameters.clear();
			return false;
		}
		return true;
	}

	virtual std::optional<ScatteringOrderParameters> parseSignalParameters(const std::string& iFilePath, double iThetaSca) { return {}; }
	virtual std::optional<ScatteringOrderParameters> parseSignalParameters(const std::string_view& iFilePath, double iThetaSca) { return {}; }
};
