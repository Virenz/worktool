#pragma once
#include <map>
#include <string>

class AvtInfo
{
public:
	void setVirusName(std::string virusname);
	std::string getVirusName();

	void setJsonsInfo(const std::string& key, std::string& value);
	std::multimap<const std::string, std::string>& getJsonsInfo();

private:
	std::multimap<const std::string, std::string> jsonsmap;
	std::string virusname;
};