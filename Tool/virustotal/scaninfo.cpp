#include "scaninfo.h"

void ScanInfo::set_detected(bool detected)
{
	this->detected = detected;
}

bool ScanInfo::get_detected()
{
	return this->detected;
}

void ScanInfo::set_version(std::string version)
{
	this->version = version;
}

std::string ScanInfo::get_version()
{
	return this->version;
}

void ScanInfo::set_result(std::string result)
{
	this->result = result;
}

std::string ScanInfo::get_result()
{
	return this->result;
}

void ScanInfo::set_update(std::string update)
{
	this->update = update;
}

std::string ScanInfo::get_update()
{
	return this->update;
}
