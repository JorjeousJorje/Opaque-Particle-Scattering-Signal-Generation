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
	using Params = std::vector<ScatteringOrderParameters>;

public:
	virtual ~MultipleSignalParametersParser() = default;

	std::optional<ParameterHolder> parseSignalParameters(const Modes& iModes, const std::string& iFilePath, double iThetaSca) {
		std::ifstream file{ iFilePath };
		Params oParams(iModes.size());
		for (std::string buffer; std::getline(file, buffer, '\n'); ) {

			if (setParameters(oParams, buffer, iThetaSca)) {
				break;
			}

		}

		if (!foundThetaScattering(oParams, iThetaSca)) {
			return std::nullopt;
		}

		std::map<ScatteringMode, ScatteringOrderParameters> resultParams{};

		// TODO: fix this. It can be better!
		std::transform(std::begin(iModes), std::end(iModes), oParams.begin(), oParams.begin(),
			[&](const ScatteringMode& iMode, ScatteringOrderParameters& iParam) 
			{	iParam.mode = iMode;
				resultParams[iMode] = iParam;
				return iParam; });

		_parameters.clear();
		return ParameterHolder{ resultParams };
	}

	std::optional<ParameterHolder> parseSignalParameters(const Modes& iModes, const std::string_view& iFilePath, double iThetaSca) {
		return parseSignalParameters(iModes, std::string{ iFilePath.data() }, iThetaSca);
	}

protected:

	bool setParameters(Params& iParams, const std::string& iBuffer, const double iThetaSca) {
		double thetaSca;
		double m;
		_parameters.str(iBuffer);
		_parameters >> m;
		_parameters >> thetaSca;


		if (Utility::AlmostEqual(iThetaSca, thetaSca, 10e6)) {

			setAngles(iParams, thetaSca);
			setAmplitidesWithP1(iParams);
			setAmplitidesWithP2(iParams);
			setRefractiveIndex(iParams, m);
			return true;
		}

		return false;
	}
	bool foundThetaScattering(const Params& iParams, const double iThetaSca) {
		if (!iParams.front().thetaSca.has_value()) {
			std::cout << "<parse warning>: file with P0 doesn't have ";
			std::cout << std::setprecision(4);
			std::cout << iThetaSca;
			std::cout << " scattering angle" << std::endl;
			_parameters.clear();
			return false;
		}
		return true;
	}

	virtual std::optional<ParameterHolder> parseSignalParameters(const std::string& iFilePath, double iThetaSca) { return {}; }
	virtual std::optional<ParameterHolder> parseSignalParameters(const std::string_view& iFilePath, double iThetaSca) { return {}; }


	void skipTheta0() {
		_parameters.ignore(std::numeric_limits<std::streamsize>::max(), '\t');
	}

	void setAngles(Params& iParams, double iThetaSca) {
		// ignore '/t' before theta0
		_parameters.ignore(1);
		skipTheta0();
		for (auto& param : iParams) {
			param.thetaSca.emplace(iThetaSca);
			_parameters >> param.theta;
		}
	}
	void setAmplitidesWithP1(Params& iParams) {
		for (auto& param : iParams) {
			_parameters >> param.ampP1;
		}
	}

	void setAmplitidesWithP2(Params& iParams) {
		for (auto& param : iParams) {
			_parameters >> param.ampP2;
		}
	}

	void setRefractiveIndex(Params& iParams, const double iIndex) {
		std::for_each(iParams.begin(), iParams.end(), 
			[=](ScatteringOrderParameters& param) {param.m = iIndex; });
	}

};
