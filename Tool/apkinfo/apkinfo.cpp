#include "apkinfo.h"

void ApkInfo::setVirusName(std::string virusname)
{
	this->virusname = virusname;
}

std::string ApkInfo::getVirusName()
{
	// TODO: �ڴ˴����� return ���
	return virusname;
}

void ApkInfo::setJsonsInfo(const std::string & key, std::string & value)
{
	jsonsmap.insert(std::pair<std::string, std::string>(key, value));
}

std::multimap<const std::string, std::string>& ApkInfo::getJsonsInfo()
{
	// TODO: �ڴ˴����� return ���
	return jsonsmap;
}
