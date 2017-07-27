#pragma once
#include <string>

/*"detected": false,
"version": "1.3.0.8108",
"result": null,
"update": "20160802"*/

class ScanInfo {

private:
	bool detected;
	std::string version;
	std::string result;
	std::string update;

public:
	void set_detected(bool detected);
	bool get_detected();
	
	void set_version(std::string version);
	std::string get_version();

	void set_result(std::string result);
	std::string get_result();

	void set_update(std::string update);
	std::string get_update();
};