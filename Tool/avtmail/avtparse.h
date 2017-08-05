#pragma once
#include "avtinfo.h"
#include <string>
#include <vector>
#include <regex>
#include "..\include\json\json.h"

class AvtParse
{
public:
	AvtParse();
	~AvtParse();

	std::vector<AvtInfo*>& getInfos();
	int readandparseJsonFromFile(const char* filename, const int length);
	std::vector<std::string> split(const std::string &s, const std::string &seperator);

private:
	std::vector<AvtInfo*> avtInfos;
};