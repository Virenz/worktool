#pragma once

#include "../include/curl/curl.h"
#pragma comment(lib, "libcurl_a.lib") 

#include <functional>

typedef	std::function<size_t (void*, size_t, size_t, void*)> WriteData_bc;//定义WriteData类型是一个指向函数的指针，格式就是libcurl的CURLOPT_WRITEFUNCTION回调函数
#define CC_CALLBACK_3(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, ##__VA_ARGS__)

typedef struct MemoryStruct {
	char *memory;
	size_t size;
}_MEMORYSTRUNCT;

class VtApi
{
public:
	VtApi();
	~VtApi();

	//virtual size_t WriteData(void* ptr, size_t size, size_t nmemb, void* stream);

	bool VtScanFile(const char* filepath, HWND m_progress);
	bool VtRescanFile(const char* sourses);
	bool VtReport(const char* sourses);
	char* getReportJson();
	void cleanChunk();

protected:
	WriteData_bc callback;
	_MEMORYSTRUNCT chunk;
};