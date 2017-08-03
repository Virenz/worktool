#include "apkinfo.h"

void ApkInfo::setVirusName(std::string virusname)
{
	this->virusname = virusname;
}

std::string ApkInfo::getVirusName()
{
	// TODO: 在此处插入 return 语句
	return virusname;
}

void ApkInfo::setJsonsInfo(const std::string & key, std::string & value)
{
	jsonsmap.insert(std::pair<std::string, std::string>(key, value));
}

std::multimap<const std::string, std::string>& ApkInfo::getJsonsInfo()
{
	// TODO: 在此处插入 return 语句
	return jsonsmap;
}
