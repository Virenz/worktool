#pragma once
#include "vtinfo.h"
#include "..\include\json\json.h"
#include <string>
#include <vector>

/*!
 * \file vtparse.h
 *
 * \author ruiwei
 * \date 八月 2017
 *
 * 作为配合UI显示，
 * VtInfo必须提供getJsonsInfo返回std::vector<std::string, std::string>
 * VtParse必须提供getInfos返回std::vector<Template>
 */

#define SET(lpThis, key, value) lpThis##"->set_"##key(value)
#define VT_RESPONSE_JSON_SET(lpThis,member_name,value) lpThis->set_##member_name(value) 

class VtParse
{
public:
	VtParse();
	~VtParse();
	std::vector<VtInfo*>& getInfos();
	int readandparseJsonFromFile(char* vtdatas);
	void cleanVtInfos();

private:
	std::vector<VtInfo*> vtInfos;
};