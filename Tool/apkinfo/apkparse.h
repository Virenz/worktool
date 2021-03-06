#pragma once

#include "apkinfo.h"
#include <string>
#include <vector>
#include "sha1.h"
#include "md5.h"

class ApkParse
{
public:
	ApkParse();
	~ApkParse();

	std::vector<ApkInfo*>& getInfos();
	int readandparseJsonFromFile(const char* filename);

private:
	std::vector<ApkInfo*> apkInfos;
};