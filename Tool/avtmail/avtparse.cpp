#include "avtparse.h"

std::vector<AvtInfo*>& AvtParse::getInfos()
{
	// TODO: 在此处插入 return 语句
	return this->avtInfos;
}

std::vector<std::string> AvtParse::split(const std::string &s, const std::string &seperator)
{
	std::vector<std::string> result;
	typedef std::string::size_type string_size;
	string_size i = 0;

	while (i != s.size()) {
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0) {
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x]) {
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0) {
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x]) {
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j) {
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

int AvtParse::readandparseJsonFromFile(const char* filedata, const int length)
{
	Json::Reader reader;// 解析json用Json::Reader   
	Json::Value root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array  
	int index = 0;

	char* writable = new char[length + 1];
	strcpy_s(writable, length + 1, filedata);
	writable[length] = '\0'; // don't forget the terminating 0

	// 进行切割
	const char *d = "\r\n";
	std::string currentvirusname;
	char *dataLine = nullptr;
	char *buff = nullptr;
	dataLine = strtok_s(writable, d, &buff);
	while (dataLine)
	{
		AvtInfo* avtInfo = new AvtInfo();
		std::vector<std::string> v = split(dataLine, "\\t+"); //可按多个字符来分隔;
		if (v.size() != 1 && !(v[0].compare("log_time"))) {
			for (auto ss : v)
			{
				if (std::regex_match(ss, std::regex("^[a-z0-9]{32}$|^[a-z0-9]{32},[a-z0-9]{32}$")))
				{
					avtInfo->setJsonsInfo(std::string("apk_certmd5"), ss);
				}
				else if (std::regex_match(ss, std::regex("^[A-Za-z0-9]{40}$"))) 
				{
					avtInfo->setJsonsInfo(std::string("jiansha1"), ss);
				}
				else if (ss.compare("NOT_SAFE") || ss.compare("SAFE") || ss.compare("UN_KNOWN")) 
				{
					avtInfo->setJsonsInfo(std::string("scan_result"), ss);
					avtInfo->setVirusName(ss);
				}
				else if (std::regex_match(ss, std::regex("^((?!wlc)\\w+[.]\\w+)[.]*.*")))
				{	
					avtInfo->setJsonsInfo(std::string("apk_pkgname"), ss);

				}
				else if (ss.length() >= 40 && ss.find("{") == 0 ) {

					if (reader.parse(ss, root, 0))
					{
						Json::Value arr_certSha1 = root.get("certSha1s", "null");
						
						Json::Value arr_md5 = root.get("md5", "null");
						avtInfo->setJsonsInfo(std::string("md5"), arr_md5.asString());
						avtInfo->setJsonsInfo(std::string("certSha1s"), arr_certSha1[index].asString());
					}

				}
			}
			avtInfos.push_back(avtInfo);
		}
	}

	delete[] writable;
	return 0;
}

AvtParse::AvtParse()
{
}

AvtParse::~AvtParse()
{
	std::vector<AvtInfo*>::iterator iter = avtInfos.begin();
	while (iter != avtInfos.end()) //#1
	{
		//注意要先释放内存，在删除vector元素，顺序不能颠倒。
		//释放内存
		delete *iter;
		*iter = NULL;
		iter++; //#1
	}
	avtInfos.clear();
}