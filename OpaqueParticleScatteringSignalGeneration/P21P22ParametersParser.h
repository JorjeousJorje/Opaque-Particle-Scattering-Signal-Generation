#pragma once
#include "MultipleSignalParametersParser.h"


class P21P22ParametersParser final: public MultipleSignalParametersParser {



public:


	ParameterHolder parseSignalParameters(
		const std::string& iFilePath,
		const double iThetaSca,
		const double iUlp = 10e11) override
	{
		return MultipleSignalParametersParser::parseSignalParameters({ ScatteringMode::P21, ScatteringMode::P22 }, iFilePath, iThetaSca, iUlp);
	}
	ParameterHolder parseSignalParameters(
		const std::string_view& iFilePath, 
		const double iThetaSca,
		const double iUlp = 10e11) override
	{
		return MultipleSignalParametersParser::parseSignalParameters({ ScatteringMode::P21, ScatteringMode::P22 }, iFilePath, iThetaSca, iUlp);
	}

	ParameterHolder parseSignalParameters(
		const std::string& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11) override
	{
		return MultipleSignalParametersParser::parseSignalParameters({ ScatteringMode::P21, ScatteringMode::P22 }, iFilePath, iThetaSca, iUlp);
	}
	ParameterHolder parseSignalParameters(
		const std::string_view& iFilePath, 
		const std::valarray<double>& iThetaSca, 
		const double iUlp = 10e11) override
	{
		return MultipleSignalParametersParser::parseSignalParameters({ ScatteringMode::P21, ScatteringMode::P22 }, iFilePath, iThetaSca, iUlp);
	}


private:

	ParameterHolder parseSignalParameters(
		const Modes& iModes, 
		const std::string& iFilePath, 
		const double iThetaSca,
		const double iUlp = 10e11) override
	{
		return ParameterHolder{{}};
	}

	ParameterHolder parseSignalParameters(
		const Modes& iModes, 
		const std::string_view& iFilePath, 
		const double iThetaSca, 
		const double iUlp = 10e11) override
	{
		return ParameterHolder{ {} };
	}
};
