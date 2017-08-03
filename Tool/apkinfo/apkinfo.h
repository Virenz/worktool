#pragma once
#include "apkinfo.h"
#include "sha1.h"
#include "md5.h"

class ApkInfo
{
public:
	ApkInfo();
	~ApkInfo();

	int GetFileMD5(const char* filePath, char* strMD5);
	int GetFileSHA1(const wchar_t * filePath, char* strSHA1);

private:

};