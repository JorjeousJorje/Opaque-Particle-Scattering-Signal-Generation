#pragma once
#include "MultipleSignalParametersParser.h"


class P21P22ParametersParser final: public MultipleSignalParametersParser {



public:



	ParameterHolder parseSignalParameters(const std::string& iFilePath, double iThetaSca) override {
		return MultipleSignalParametersParser::parseSignalParameters({ScatteringMode::P21, ScatteringMode::P22}, iFilePath, iThetaSca);
	}

	ParameterHolder parseSignalParameters(const std::string_view& iFilePath, double iThetaSca) override {
		return MultipleSignalParametersParser::parseSignalParameters({ ScatteringMode::P21, ScatteringMode::P22 }, iFilePath, iThetaSca);
	}


private:

	ParameterHolder parseSignalParameters(const Modes& iModes, const std::string& iFilePath, double iThetaSca) override {
		return ParameterHolder{{}};
	}

	ParameterHolder parseSignalParameters(const Modes& iModes, const std::string_view& iFilePath, double iThetaSca) override {
		return ParameterHolder{ {} };
	}
};
