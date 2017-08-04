#include "apkparse.h"

std::vector<ApkInfo*>& ApkParse::getInfos()
{
	// TODO: 在此处插入 return 语句
	return this->apkInfos;
}

int ApkParse::readandparseJsonFromFile(const char* filedata)
{
	ApkInfo * apkinfo = new ApkInfo();
	apkinfo->setVirusName(filedata);

	CSHA1 sha1;
	sha1.HashFile(filedata);
	sha1.Final();
	std::string chSha1 = sha1.GetHashStr();
	apkinfo->setJsonsInfo("sha1", chSha1);

	MD5 md5;
	md5.md5_file(filedata);
	std::string result = md5.md5();
	apkinfo->setJsonsInfo("md5", result);

	/*CRC32 crc32;
	std::string str_crc32 = crc32.GetCRC32FromFile(filedata);
	apkinfo->setJsonsInfo("crc32", str_crc32);*/

	this->apkInfos.push_back(apkinfo);
	
	return 0;
}

ApkParse::ApkParse()
{
}

ApkParse::~ApkParse()
{
	std::vector<ApkInfo*>::iterator iter = apkInfos.begin();
	while (iter != apkInfos.end()) //#1
	{
		//注意要先释放内存，在删除vector元素，顺序不能颠倒。
		//释放内存
		delete *iter;
		*iter = NULL;
		iter++; //#1
	}
	apkInfos.clear();
}