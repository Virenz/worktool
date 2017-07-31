#pragma once
#include <map>
#include <string>
#include "scaninfo.h"

class VtInfo
{
public:
	VtInfo();
	~VtInfo();

	void set_scan_id(std::string scan_id);
	std::string get_scan_id();

	void set_sha1(std::string sha1);
	std::string get_sha1();

	void set_resource(std::string resource);
	std::string get_resource();

	void set_response_code(std::string response_code);
	std::string get_response_code();

	void set_scan_date(std::string scan_date);
	std::string get_scan_date();

	void set_permalink(std::string permalink);
	std::string get_permalink();

	void set_verbose_msg(std::string verbose_msg);
	std::string get_verbose_msg();

	void set_total(std::string total);
	std::string get_total();

	void set_positives(std::string positives);
	std::string get_positives();

	void set_sha256(std::string sha256);
	std::string get_sha256();

	void set_md5(std::string md5);
	std::string get_md5();

	void set_scans(std::string& key, ScanInfo* value);
	std::multimap<std::string, ScanInfo*>& get_scans();

	void setVirusName(std::string virusname);
	std::string getVirusName();

	void set_vtsinfo(std::string& key, std::string& value);
	std::multimap<std::string, std::string>& get_vtsinfo();

private:
	std::multimap<std::string, std::string> vtsinfo;
	std::multimap<std::string, ScanInfo*> scansinfo;
	std::string virusname;
	/*"scan_id": "4c28fcdfde9e11e18ee60ab9132d4abf69777785047364ea9adf6dc5974a67af-1470133345",
	"sha1": "1eed2698fee72789c024a565d2cf3a663e8da48f",
	"resource": "1eed2698fee72789c024a565d2cf3a663e8da48f",
	"response_code": 1,
	"scan_date": "2016-08-02 10:22:25",
	"permalink": "https://www.virustotal.com/file/4c28fcdfde9e11e18ee60ab9132d4abf69777785047364ea9adf6dc5974a67af/analysis/1470133345/",
	"verbose_msg": "Scan finished, information embedded",
	"total": 53,
	"positives": 17,
	"sha256": "4c28fcdfde9e11e18ee60ab9132d4abf69777785047364ea9adf6dc5974a67af",
	"md5": "cb9552d65f53330e7d00b7f0601af364"*/

	std::string scan_id;
	std::string sha1;
	std::string resource;
	std::string response_code;
	std::string scan_date;
	std::string permalink;
	std::string verbose_msg;
	std::string total;
	std::string positives;
	std::string sha256;
	std::string md5;
};