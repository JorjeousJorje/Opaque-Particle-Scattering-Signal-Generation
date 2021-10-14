#pragma once
#pragma once
#include <iostream>
#include <string>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>
#include <memory>

class Config;
using ConfigPtr = std::unique_ptr<Config>;
using config_t = std::unordered_map<std::string, std::variant<std::string, double>>;

class Config {
protected:
	const std::vector<std::string> _order;
	config_t config_;

public:
	Config(std::vector<std::string> iOrder)
	:	_order{ std::move(iOrder) }
	{
		for (auto&& name : _order) {
			config_[name] = {};
		}
	}
	virtual ~Config() = default;

	virtual void read(std::istream& iStream, char sep = '=') = 0;
	std::optional<std::variant<std::string, double>> look_up(const std::string& iName)
	{
		try {
			const auto& values = config_.at(iName);
			if (std::holds_alternative<double>(values)) {
				return { std::get<double>(values) };
			}
			return { std::get<std::string>(values) };
		}
		catch (const std::out_of_range&) {
			std::cout << "<config>: no such item: " + iName << std::endl;
			return std::nullopt;
		}
	}

};