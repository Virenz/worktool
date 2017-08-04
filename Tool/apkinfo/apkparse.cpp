#include "apkparse.h"

std::vector<ApkInfo*>& ApkParse::getInfos()
{
	// TODO: �ڴ˴����� return ���
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
		//ע��Ҫ���ͷ��ڴ棬��ɾ��vectorԪ�أ�˳���ܵߵ���
		//�ͷ��ڴ�
		delete *iter;
		*iter = NULL;
		iter++; //#1
	}
	apkInfos.clear();
}