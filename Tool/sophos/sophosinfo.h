#pragma once
#include <map>
#include <string>

class SophosInfo
{
public:
	SophosInfo();
	~SophosInfo();

	void setVirusName(std::string virusname);
	std::string getVirusName();

	void setJsonsInfo(std::string& key, std::string& value);
	std::multimap<std::string, std::string>& getJsonsInfo();

private:
	std::multimap<std::string, std::string> jsonsmap;
	std::string virusname;
};

