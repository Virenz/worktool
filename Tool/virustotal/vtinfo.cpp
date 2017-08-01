#include "vtinfo.h"

VtInfo::VtInfo()
{
}

VtInfo::~VtInfo()
{
	vtsinfo.clear();
	std::multimap<std::string, ScanInfo*>::iterator iter = scansinfo.begin();
	while (iter != scansinfo.end()) //#1
	{
		//注意要先释放内存，在删除map元素，顺序不能颠倒。
		//释放内存
		delete iter->second;
		iter->second = NULL;
		//删除map元素
		scansinfo.erase(iter++); //#1
	}
	scansinfo.clear();
}

void VtInfo::set_scan_id(std::string scan_id)
{
	this->scan_id = scan_id;
}

std::string VtInfo::get_scan_id()
{
	return this->scan_id;
}

void VtInfo::set_sha1(std::string sha1)
{
	this->sha1 = sha1;
}

std::string VtInfo::get_sha1()
{
	return this->sha1;
}

void VtInfo::set_resource(std::string resource)
{
	this->resource = resource;
}

std::string VtInfo::get_resource()
{
	return this->resource;
}

void VtInfo::set_response_code(std::string response_code)
{
	this->response_code = response_code;
}

std::string VtInfo::get_response_code()
{
	return this->response_code;
}

void VtInfo::set_scan_date(std::string scan_date)
{
	this->scan_date = scan_date;
}

std::string VtInfo::get_scan_date()
{
	return this->scan_date;
}

void VtInfo::set_permalink(std::string permalink)
{
	this->permalink = permalink;
}

std::string VtInfo::get_permalink()
{
	return this->permalink;
}

void VtInfo::set_verbose_msg(std::string verbose_msg)
{
	this->verbose_msg = verbose_msg;
}

std::string VtInfo::get_verbose_msg()
{
	return this->verbose_msg;
}

void VtInfo::set_total(int total)
{
	this->total = total;
}

int VtInfo::get_total()
{
	return this->total;
}

void VtInfo::set_positives(int positives)
{
	this->positives = positives;
}

int VtInfo::get_positives()
{
	return this->positives;
}

void VtInfo::set_sha256(std::string sha256)
{
	this->sha256 = sha256;
}

std::string VtInfo::get_sha256()
{
	return this->sha256;
}

void VtInfo::set_md5(std::string md5)
{
	this->md5 = md5;
}

std::string VtInfo::get_md5()
{
	return this->md5;
}


void VtInfo::set_scans(std::string & key, ScanInfo* value)
{
	scansinfo.insert(std::pair<std::string, ScanInfo*>(key, value));
}

std::multimap<std::string, ScanInfo*>& VtInfo::get_scans()
{
	// TODO: 在此处插入 return 语句
	return this->scansinfo;
}

void VtInfo::setVirusName(std::string virusname)
{
	this->virusname = virusname;
}

std::string VtInfo::getVirusName()
{
	return this->virusname;
}

void VtInfo::set_vtsinfo(std::string & key, std::string & value)
{
	this->vtsinfo.insert(std::pair<std::string, std::string>(key, value));
}

std::multimap<std::string, std::string>& VtInfo::get_vtsinfo()
{
	return this->vtsinfo;
}
