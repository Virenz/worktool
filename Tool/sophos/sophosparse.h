#pragma once
#include "sophosinfo.h"
#include "..\include\json\json.h"
#include <string>
#include <vector>
/*!
 * \file sophosparse.h
 *
 * \author ruiwei
 * \date ���� 2017
 *
 * ��Ϊ���UI��ʾ��
 * SophosInfo�����ṩgetJsonsInfo����std::vector<std::string, std::string>
 * SophosParse�����ṩgetInfos����std::vector<Template>
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