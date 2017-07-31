#include "sophosparse.h"

std::vector<SophosInfo*>& SophosParse::getSophosInfos()
{
	// TODO: 在此处插入 return 语句
	return this->sophosInfos;
}

int SophosParse::readandparseJsonFromFile(char* filedata)
{
	Json::Reader reader;// 解析json用Json::Reader   
	Json::Value root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array         

	std::string data(filedata);
	char* writable = new char[data.size() + 1];
	strcpy_s(writable, data.size() + 1, data.c_str());
	writable[data.size()] = '\0'; // don't forget the terminating 0
	
	//const char* str = "{\'AXML_BasicReceivers\': [\'com/mgb/safe/receiver/BootReceiver\'], \'AXML_BasicServices\' : [\'com/mgb/safe/service/ResidentService\', \'com/mgb/safe/service/CoreService\']}";
	//"{\"AXML_BasicReceivers\": [\"com/mgb/safe/receiver/BootReceiver\"], \"AXML_BasicServices\" : [\"com/mgb/safe/service/ResidentService\", \"com/mgb/safe/service/CoreService\"]}";

	// 进行切割
	const char *d = "\r\n";
	std::string currentvirusname;
	char *dataLine = nullptr;
	char *buff = nullptr;
	dataLine = strtok_s(writable, d, &buff);
	while (dataLine)
	{
		if (reader.parse(dataLine, root, 0))
		{
			SophosInfo* sophosinfo = new SophosInfo();
			sophosinfo->setVirusName(currentvirusname);
			Json::Value::Members arrayMember = root.getMemberNames(); //ayyayMember是成员名称的集合，即name,age;
			for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //遍历json成员
			{
				Json::Value valueList = root.get(*iter, "null");
				int file_size = valueList.size();  // 得到"files"的数组个数 
				for (size_t i = 0; i < file_size; i++)
				{
					sophosinfo->setJsonsInfo(*iter, valueList[i].asString());
				}
			}
			sophosInfos.push_back(sophosinfo);
		}
		else
		{
			currentvirusname = dataLine;
		}
		dataLine = strtok_s(NULL, d, &buff);
	}

	delete[] writable;

	return 0;
}

SophosParse::SophosParse()
{
}

SophosParse::~SophosParse()
{
	std::vector<SophosInfo*>::iterator iter = sophosInfos.begin();
	while (iter != sophosInfos.end()) //#1
	{
		//注意要先释放内存，在删除vector元素，顺序不能颠倒。
		//释放内存
		delete *iter;
		*iter = NULL;
		//删除vector元素
		sophosInfos.erase(iter++); //#1
	}
	sophosInfos.clear();
}