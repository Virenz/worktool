#include "sophosinfo.h"

void SophosInfo::setVirusName(std::string virusname)
{
	this->virusname = virusname;
}

std::string SophosInfo::getVirusName()
{
	// TODO: �ڴ˴����� return ���
	return virusname;
}

void SophosInfo::setJsonsInfo(std::string & key, std::string & value)
{
	jsonsmap.insert(std::pair<std::string, std::string>(key,value));
}

std::multimap<std::string, std::string>& SophosInfo::getJsonsInfo()
{
	// TODO: �ڴ˴����� return ���
	return jsonsmap;
}

SophosInfo::SophosInfo()
{
	this->virusname = "";
}

SophosInfo::~SophosInfo()
{
}
