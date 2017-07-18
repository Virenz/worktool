#pragma once
#include "sophosinfo.h"
#include "include\json\json.h"
#include <string>
#include <vector>

class SophosParse
{
public:
	SophosParse();
	~SophosParse();

	std::vector<SophosInfo*>& getSophosInfos();
	int readandparseJsonFromFile(char* filename);

private:
	std::vector<SophosInfo*> sophosInfos;
};