#include "sophosinfo.h"

void SophosInfo::setVirusName(std::string virusname)
{
	this->virusname = virusname;
}

std::string SophosInfo::getVirusName()
{
	// TODO: 在此处插入 return 语句
	return virusname;
}

void SophosInfo::setJsonsInfo(std::string & key, std::string & value)
{
	jsonsmap.insert(std::pair<std::string, std::string>(key,value));
}

std::multimap<std::string, std::string>& SophosInfo::getJsonsInfo()
{
	// TODO: 在此处插入 return 语句
	return jsonsmap;
}

SophosInfo::SophosInfo()
{
	this->virusname = "";
}

SophosInfo::~SophosInfo()
{
}
