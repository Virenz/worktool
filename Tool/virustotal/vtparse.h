#pragma once
#include "vtinfo.h"
#include "..\include\json\json.h"
#include <string>
#include <vector>

/*!
 * \file vtparse.h
 *
 * \author ruiwei
 * \date ���� 2017
 *
 * ��Ϊ���UI��ʾ��
 * VtInfo�����ṩgetJsonsInfo����std::vector<std::string, std::string>
 * VtParse�����ṩgetInfos����std::vector<Template>
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