#pragma once
#include <fstream>
#include <optional>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <numbers>
#include <valarray>
#include <string_view>

#include "ScatteringOrderParameters.h"
#include "Utility.h"



class SingleSignalParametersParser {
protected:
	std::istringstream _parameters{};
public:
	virtual ~SingleSignalParametersParser() = default;

	virtual std::optional<ScatteringOrderParameters> parseSignalParameters(
		const ScatteringMode& iMode, 
		const std::string& iFilePath, 
		const double iThetaSca, 
		const double iUlp=10e11)
	{
		std::ifstream file{ iFilePath };
		ScatteringOrderParameters params;
		for (std::string buffer; std::getline(file, buffer, '\n'); ) {

			if (setParameters(params, buffer, iThetaSca, iMode, iUlp)) {
				break;
			}

		}

		if (!foundThetaScattering(params, iThetaSca)) {
			return std::nullopt;
		}

		params.mode = iMode;
		_parameters.clear();
		return params;
	}

	virtual std::vector<ScatteringOrderParameters> parseSignalParameters(
		const ScatteringMode& iMode, 
		const std::string& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11)
	{
		std::ifstream file{ iFilePath };
		std::vector<ScatteringOrderParameters> oParams(iThetaSca.size());

		auto itTheta = std::begin(iThetaSca);
		auto itParam = oParams.begin();

		for (std::string buffer; std::getline(file, buffer, '\n'); ) {

			
			if (setParameters(*itParam, buffer, *itTheta, iMode, iUlp)) {
				++itParam;
				++itTheta;
				_parameters.clear();
			}

			if(itParam == oParams.end()) {
				break;
			}


		}

		auto mode_setter = [&](auto& iParam) { iParam.mode = iMode; };
		std::for_each(oParams.begin(), oParams.end(), mode_setter);
		_parameters.clear();
		return oParams;
	}

	virtual std::optional<ScatteringOrderParameters> parseSignalParameters(
		const ScatteringMode& iMode, 
		const std::string_view& iFilePath, 
		double iThetaSca, 
		const double iUlp = 10e11)
	{
		return SingleSignalParametersParser::parseSignalParameters(iMode, std::string{ iFilePath.data() }, iThetaSca, iUlp);
	}
	virtual std::vector<ScatteringOrderParameters> parseSignalParameters(
		const ScatteringMode& iMode, 
		const std::string_view& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11)
	{
		return SingleSignalParametersParser::parseSignalParameters(iMode, std::string{ iFilePath.data() }, iThetaSca, iUlp);
	}

protected:

	virtual std::optional<ScatteringOrderParameters> parseSignalParameters(
		const std::string& iFilePath, 
		double iThetaSca, 
		const double iUlp = 10e11) { return {}; }
	virtual std::optional<ScatteringOrderParameters> parseSignalParameters(
		const std::string_view& iFilePath,
		double iThetaSca, 
		const double iUlp = 10e11) { return {}; }
	virtual std::vector<ScatteringOrderParameters> parseSignalParameters(
		const std::string& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11) { return {}; }
	virtual std::vector<ScatteringOrderParameters> parseSignalParameters(
		const std::string_view& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11) { return {}; }


private:
	bool setParameters(
		ScatteringOrderParameters& iParams, 
		const std::string& iBuffer, 
		const double iThetaSca, 
		const ScatteringMode& iMode, 
		const double iUlp = 10e11)
	{
		double thetaSca;
		_parameters.str(iBuffer);
		_parameters >> iParams.m;
		_parameters >> thetaSca;


		if (Utility::AlmostEqual(iThetaSca, thetaSca, iUlp)) {
			double theta;
			iParams.thetaSca.emplace(thetaSca);
			_parameters >> theta;

			if (iMode == ScatteringMode::P0) {
				theta *= -1;
			}

			iParams.theta = Utility::radians(theta);
			_parameters >> iParams.ampP1;
			_parameters >> iParams.ampP2;
			return true;
		}

		return false;
	}
	bool foundThetaScattering(const ScatteringOrderParameters& iParams, const double iThetaSca) {
		if (!iParams.thetaSca.has_value()) {
			std::cout << "<parse warning>: file with P" + std::to_string(Utility::to_underlying(iParams.mode)) + "doesn't have ";
			std::cout << std::setprecision(4);
			std::cout << iThetaSca;
			std::cout << " scattering angle" << std::endl;
			_parameters.clear();
			return false;
		}
		return true;
	}
	
};
