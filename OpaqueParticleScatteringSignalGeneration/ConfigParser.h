#pragma once
#include <algorithm>


class ConfigParser final {
	bool end_{ false };

public:
	std::string parseLine(std::istream& iStream, const char sep) {
		std::string line;
		end_ = iStream.eof();
		std::getline(iStream.ignore(std::numeric_limits<std::streamsize>::max(), sep), line);

		removeWhitespaces(line);
		return line;
	}

	bool end() const {
		return end_;
	}

private:

	void removeWhitespaces(std::string& iStr) const {
		iStr.erase(std::remove_if(iStr.begin(), iStr.end(),
		[](char ch) { return std::isspace<char>(ch, std::locale::classic()); }), 
		iStr.end());
	}
};
