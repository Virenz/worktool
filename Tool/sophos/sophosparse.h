#pragma once
#include "sophosinfo.h"
#include "..\include\json\json.h"
#include <string>
#include <vector>
/*!
 * \file sophosparse.h
 *
 * \author ruiwei
 * \date 八月 2017
 *
 * 作为配合UI显示，
 * SophosInfo必须提供getJsonsInfo返回std::vector<std::string, std::string>
 * SophosParse必须提供getInfos返回std::vector<Template>
 */

class SophosParse
{
public:
	SophosParse();
	~SophosParse();

	std::vector<SophosInfo*>& getInfos();
	int readandparseJsonFromFile(char* filename);

private:
	std::vector<SophosInfo*> sophosInfos;
};