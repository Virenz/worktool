#include "sophosparse.h"

std::vector<SophosInfo*>& SophosParse::getSophosInfos()
{
	// TODO: �ڴ˴����� return ���
	return this->sophosInfos;
}

int SophosParse::readandparseJsonFromFile(char* filedata)
{
	Json::Reader reader;// ����json��Json::Reader   
	Json::Value root; // Json::Value��һ�ֺ���Ҫ�����ͣ����Դ����������͡���int, string, object, array         

	std::string data(filedata);
	char* writable = new char[data.size() + 1];
	strcpy_s(writable, data.size() + 1, data.c_str());
	writable[data.size()] = '\0'; // don't forget the terminating 0
	
	//const char* str = "{\'AXML_BasicReceivers\': [\'com/mgb/safe/receiver/BootReceiver\'], \'AXML_BasicServices\' : [\'com/mgb/safe/service/ResidentService\', \'com/mgb/safe/service/CoreService\']}";
	//"{\"AXML_BasicReceivers\": [\"com/mgb/safe/receiver/BootReceiver\"], \"AXML_BasicServices\" : [\"com/mgb/safe/service/ResidentService\", \"com/mgb/safe/service/CoreService\"]}";

	// �����и�
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
			Json::Value::Members arrayMember = root.getMemberNames(); //ayyayMember�ǳ�Ա���Ƶļ��ϣ���name,age;
			for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //����json��Ա
			{
				Json::Value valueList = root.get(*iter, "null");
				int file_size = valueList.size();  // �õ�"files"��������� 
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
		//ע��Ҫ���ͷ��ڴ棬��ɾ��vectorԪ�أ�˳���ܵߵ���
		//�ͷ��ڴ�
		delete *iter;
		*iter = NULL;
		//ɾ��vectorԪ��
		sophosInfos.erase(iter++); //#1
	}
	sophosInfos.clear();
}