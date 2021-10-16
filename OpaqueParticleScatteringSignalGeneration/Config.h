#pragma once
#pragma once
#include <filesystem>
#include <iostream>
#include <string>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>
#include <memory>

#include "ConfigParser.h"

class Config;
using ConfigPtr = std::unique_ptr<Config>;

enum class ValueId : uint16_t {
	Number,
	Range,
	PathToFile,
	PathToDir,
};

struct ConfigItem {
	const std::string name;
	const ValueId value;
};

class Config {
protected:

	using ConfigValues = std::variant<std::string, double, std::valarray<double>>;
	using config_t = std::unordered_map<std::string, ConfigValues>;
	const std::vector<ConfigItem> _order;
	config_t config_;


public:
	Config(std::vector<ConfigItem> iOrder)
	:	_order{ std::move(iOrder) }
	{
		for (auto&& configItem : _order) {
			config_[configItem.name] = {};
		}
	}
	virtual ~Config() = default;

	virtual void read(std::istream& iStream, char sep = '=') {
		if (emptyConfig(iStream)) {
			return;
		}

		ConfigParser parser{};
		for(auto&& [name, id]: _order) {

			if(id == ValueId::Number) {
				if (setNumericItems(iStream, parser, name, sep)) {
					continue;
				}
				return;
			}

			if(id == ValueId::PathToFile) {
				if (setPathToFile(iStream, parser, name, sep)) {
					continue;
				}
				return;
			}

			if (id == ValueId::PathToDir) {
				if (setPathToDir(iStream, parser, name, sep)) {
					continue;
				}
				return;
			}

			if (id == ValueId::Range) {
				if (setRange(iStream, parser, name, sep)) {
					continue;
				}
				return;
			}
		}

		std::cout << "<config>: received" << std::endl;
	}



	std::optional<ConfigValues> look_up(const std::string& iName)
	{
		try {
			const auto& values = config_.at(iName);
			if (std::holds_alternative<double>(values)) {
				return { std::get<double>(values) };
			}
			if(std::holds_alternative<std::valarray<double>>(values)) {
				return std::get<std::valarray<double>>(values);
			}
			return { std::get<std::string>(values) };
		}
		catch (const std::out_of_range&) {
			std::cout << "<config>: no such item: " + iName << std::endl;
			return std::nullopt;
		}
	}


private:
	bool emptyConfig(std::istream& iStream) {
		if (iStream.peek() == std::istream::traits_type::eof()) {
			std::cout << "<config>: empty config file" << std::endl;
			return true;
		}
		return false;
	}

	bool setNumericItems(std::istream& iStream, ConfigParser& iParser, const std::string& iItem, const char sep) {
		const auto line = iParser.parseLine(iStream, sep);

		if(emptyItem(line, iItem)) {
			return false;
		}

		try {
			double number = std::stod(line);
			config_.at(iItem) = number;
			return true;
		} catch (const std::invalid_argument&) {
			std::cout << "<config>: item '" + iItem << "' must be numerical!" << std::endl;
			return false;
		} catch (const std::out_of_range&) {
			std::cout << "<config>: item '" + iItem << "' is out of the range of the result type!" << std::endl;
			return false;
		}
	}

	bool setPathToFile(std::istream& iStream, ConfigParser& iParser, const std::string& iItem, const char sep) {
		config_.at(iItem) = iParser.parseLine(iStream, sep);
		const auto& pathToFile = std::get<std::string>(config_.at(iItem));

		if (emptyItem(pathToFile, iItem)) {
			return false;
		}

		if(!std::filesystem::directory_entry{ pathToFile }.exists()) {
			std::cout << "<config>: " << pathToFile << " doesn't exist." << std::endl;
			return false;
		}
		if (!std::filesystem::directory_entry{ pathToFile }.is_regular_file()) {
			std::cout << "<config>: " << pathToFile << " is not regular file." << std::endl;
			return false;
		}
		return true;
	}

	bool setPathToDir(std::istream& iStream, ConfigParser& iParser, const std::string& iItem, const char sep) {
		config_.at(iItem) = iParser.parseLine(iStream, sep);
		const auto& pathToDir = std::get<std::string>(config_.at(iItem));

		if (emptyItem(pathToDir, iItem)) {
			return false;
		}

		if (!std::filesystem::directory_entry{ pathToDir }.exists()) {
			std::cout << "<config>: " << pathToDir << " doesn't exist." << std::endl;
			return false;
		}
		if (!std::filesystem::directory_entry{ pathToDir }.is_directory()) {
			std::cout << "<config>: " << pathToDir << " is not directory." << std::endl;
			return false;
		}
		return true;
	}

	bool emptyItem(const std::string& iLine, const std::string& iItem) {
		if (iLine.empty()) {
			std::cout << "<config>: item '" + iItem << "' is empty" << std::endl;
			return true;
		}
		return false;
	}

	bool setRange(std::istream& iStream, ConfigParser& iParser, const std::string& iItem, const char sep) {
		/*config_.at(iItem) = iParser.parseLine(iStream, sep);
		const auto& pathToFile = std::get<std::string>(config_.at(iItem));

		if (emptyItem(pathToFile, iItem)) {
			return false;
		}

		if (!std::filesystem::directory_entry{ pathToFile }.exists()) {
			std::cout << "<config>: " << pathToFile << " doesn't exist." << std::endl;
			return false;
		}
		if (!std::filesystem::directory_entry{ pathToFile }.is_regular_file()) {
			std::cout << "<config>: " << pathToFile << " is not regular file." << std::endl;
			return false;
		}*/
		return true;
	}
};