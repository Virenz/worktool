#pragma once

#include "../include/curl/curl.h"
#pragma comment(lib, "libcurl.lib") 

class VtApi
{
public:
	VtApi();
	~VtApi();

	//size_t WriteData(void* ptr, size_t size, size_t nmemb, void* stream);

	void VtScanFile(void* getdata, char* filepath);
	void VtRescanFile(void* getdata, char* sourses);
	void VtReport(void* getdata, char* sourses);

private:

};