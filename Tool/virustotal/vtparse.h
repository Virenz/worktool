#pragma once
#include "vtinfo.h"
#include "..\include\json\json.h"
#include <string>
#include <vector>

#define SET(lpThis, key, value) lpThis##"->set_"##key(value)
#define VT_RESPONSE_JSON_SET(lpThis,member_name,value) lpThis->set_##member_name(value) 

class VtParse
{
public:
	std::vector<VtInfo*>& getVtInfos();
	int readandparseJsonFromFile(char* vtdatas);

private:
	std::vector<VtInfo*> vtInfos;
};