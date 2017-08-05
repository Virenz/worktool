#include "avtinfo.h"

void AvtInfo::setVirusName(std::string virusname)
{
	this->virusname = virusname;
}

std::string AvtInfo::getVirusName()
{
	// TODO: 在此处插入 return 语句
	return virusname;
}

void AvtInfo::setJsonsInfo(const std::string & key, std::string & value)
{
	jsonsmap.insert(std::pair<std::string, std::string>(key, value));
}

std::multimap<const std::string, std::string>& AvtInfo::getJsonsInfo()
{
	// TODO: 在此处插入 return 语句
	return jsonsmap;
}