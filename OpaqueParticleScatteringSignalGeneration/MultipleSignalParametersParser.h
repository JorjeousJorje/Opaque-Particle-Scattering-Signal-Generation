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
	using Params = std::unordered_multimap<ScatteringMode, ScatteringOrderParameters>;

public:
	virtual ~MultipleSignalParametersParser() = default;

	virtual ParameterHolder parseSignalParameters(
		const Modes& iModes, 
		const std::string& iFilePath, 
		const double iThetaSca, 
		const double iUlp = 10e11)
	{
		std::ifstream file{ iFilePath };
		Params oParams{};

		for (std::string buffer; std::getline(file, buffer, '\n'); ) {

			if (setParameters(oParams, buffer, iThetaSca, iModes, iUlp)) {
				break;
			}

		}

		if (!foundThetaScattering(oParams, iThetaSca)) {
			return ParameterHolder{ {} };
		}

		_parameters.clear();
		return ParameterHolder{ oParams };
	}

	virtual ParameterHolder parseSignalParameters(
		const Modes& iModes, 
		const std::string& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11)
	{
		std::ifstream file{ iFilePath };
		Params oParams{};

		auto itTheta = std::begin(iThetaSca);
		for (std::string buffer; std::getline(file, buffer, '\n'); ) {

			if (setParameters(oParams, buffer, *itTheta, iModes, iUlp)) {
				++itTheta;
				_parameters.clear();
			}

		}

		if(oParams.empty()) {
			std::cout << "No angles from the input array were found!" << std::endl;
		}

		_parameters.clear();
		return ParameterHolder{ oParams };
	}

	virtual ParameterHolder parseSignalParameters(
		const Modes& iModes, 
		const std::string_view& iFilePath, 
		double iThetaSca, 
		const double iUlp = 10e11)
	{
		return MultipleSignalParametersParser::parseSignalParameters(iModes, std::string{ iFilePath.data() }, iThetaSca, iUlp);
	}
	virtual ParameterHolder parseSignalParameters(
		const Modes& iModes, 
		const std::string_view& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11)
	{
		return MultipleSignalParametersParser::parseSignalParameters(iModes, std::string{ iFilePath.data() }, iThetaSca, iUlp);
	}

protected:

	virtual ParameterHolder parseSignalParameters(const std::string& iFilePath, double iThetaSca, const double iUlp = 10e11) { return ParameterHolder{ {} }; }
	virtual ParameterHolder parseSignalParameters(const std::string_view& iFilePath, double iThetaSca, const double iUlp = 10e11) { return ParameterHolder{ {} }; }

	virtual ParameterHolder parseSignalParameters(const std::string& iFilePath, const std::valarray<double>& iThetaSca, const double iUlp = 10e11) { return ParameterHolder{ {} }; }
	virtual ParameterHolder parseSignalParameters(const std::string_view& iFilePath, const std::valarray<double>& iThetaSca, const double iUlp = 10e11) { return ParameterHolder{ {} }; }


private:
	bool setParameters(Params& iParams, const std::string& iBuffer, const double iThetaSca, const Modes& iModes, const double iUlp) {
		double thetaSca{};
		double m{};
		_parameters.str(iBuffer);
		_parameters >> m;
		_parameters >> thetaSca;


		if (Utility::AlmostEqual(iThetaSca, thetaSca, iUlp)) {
			// skip theta0
			skipColumn();

			auto theta_pos= _parameters.tellg();
			auto amplitude_pos= _parameters.tellg();
			ScatteringOrderParameters param;

			for (const auto& mode : iModes) {
				parseColumns(param, theta_pos, amplitude_pos);

				param.thetaSca.emplace(iThetaSca);
				param.mode = mode;
				param.m = m;
				param.theta = Utility::radians(param.theta);
				iParams.emplace(mode, param);
			}
			return true;
		}

		return false;
	}
	void skipColumn() {
		// ignore '/t' before column
		_parameters.ignore(1);
		_parameters.ignore(std::numeric_limits<std::streamsize>::max(), '\t');
	}


	void parseColumns(ScatteringOrderParameters& iParam, std::streampos& iThetaPos, std::streampos& iAmpPos) {
		_parameters.seekg(iThetaPos);
		_parameters >> iParam.theta;
		iThetaPos = _parameters.tellg();
		// skip theta of the next mode
		skipColumn();
		_parameters.seekg(iAmpPos > iThetaPos ? iAmpPos : _parameters.tellg());
		_parameters >> iParam.ampP1;
		_parameters >> iParam.ampP2;
		iAmpPos = _parameters.tellg();
	}
	bool foundThetaScattering(const Params& iParams, const double iThetaSca) {

		if (iParams.empty()) {
			std::cout << "<parse warning>: file doesn't have ";
			std::cout << std::setprecision(4);
			std::cout << iThetaSca;
			std::cout << " scattering angle" << std::endl;
			_parameters.clear();
			return false;
		}
		return true;
	}
};
