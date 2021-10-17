#pragma once
#include "SingleSignalParametersParser.h"


class P21ParametersParser : public SingleSignalParametersParser {
public:

	std::optional<ScatteringOrderParameters> parseSignalParameters(
		const std::string& iFilePath, 
		double iThetaSca, 
		const double iUlp = 10e11) override
	{
		return SingleSignalParametersParser::parseSignalParameters(ScatteringMode::P21, iFilePath, iThetaSca, iUlp);
	}

	std::optional<ScatteringOrderParameters> parseSignalParameters(
		const std::string_view& iFilePath, 
		double iThetaSca, const double 
		iUlp = 10e11) override
	{
		return SingleSignalParametersParser::parseSignalParameters(ScatteringMode::P21, iFilePath, iThetaSca, iUlp);
	}

	std::vector<ScatteringOrderParameters> parseSignalParameters(
		const std::string& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11) override
	{
		return SingleSignalParametersParser::parseSignalParameters(ScatteringMode::P21, iFilePath, iThetaSca, iUlp);
	}

	std::vector<ScatteringOrderParameters> parseSignalParameters(
		const std::string_view& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11) override
	{
		return SingleSignalParametersParser::parseSignalParameters(ScatteringMode::P21, iFilePath, iThetaSca, iUlp);
	}

private:

	std::optional<ScatteringOrderParameters> parseSignalParameters(
		const ScatteringMode& iMode, 
		const std::string& iFilePath, 
		double iThetaSca, 
		const double iUlp = 10e11) override
	{
		return {};
	}

	std::optional<ScatteringOrderParameters> parseSignalParameters(
		const ScatteringMode& iMode, 
		const std::string_view& iFilePath, 
		double iThetaSca, 
		const double iUlp = 10e11) override
	{
		return {};
	}

	std::vector<ScatteringOrderParameters> parseSignalParameters(
		const ScatteringMode& iMode, 
		const std::string& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11) override
	{
		return {};
	}

	std::vector<ScatteringOrderParameters> parseSignalParameters(
		const ScatteringMode& iMode, 
		const std::string_view& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11) override
	{
		return {};
	}
};