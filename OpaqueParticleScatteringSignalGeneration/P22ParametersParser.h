#pragma once
#include "SingleSignalParametersParser.h"


class P22ParametersParser : public SingleSignalParametersParser {
public:

	std::optional<ScatteringOrderParameters> parseSignalParameters(const std::string& iFilePath, double iThetaSca) override {
		return SingleSignalParametersParser::parseSignalParameters(ScatteringMode::P22, iFilePath, iThetaSca);
	}

	std::optional<ScatteringOrderParameters> parseSignalParameters(const std::string_view& iFilePath, double iThetaSca) override {
		return SingleSignalParametersParser::parseSignalParameters(ScatteringMode::P22, iFilePath, iThetaSca);
	}
};