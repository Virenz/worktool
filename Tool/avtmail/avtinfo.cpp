#include "avtinfo.h"

void AvtInfo::setVirusName(std::string virusname)
{
	this->virusname = virusname;
}

std::string AvtInfo::getVirusName()
{
	// TODO: �ڴ˴����� return ���
	return virusname;
}

void AvtInfo::setJsonsInfo(const std::string & key, std::string & value)
{
	jsonsmap.insert(std::pair<std::string, std::string>(key, value));
}

std::multimap<const std::string, std::string>& AvtInfo::getJsonsInfo()
{
	// TODO: �ڴ˴����� return ���
	return jsonsmap;
}