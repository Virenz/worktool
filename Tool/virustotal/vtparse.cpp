#include "vtparse.h"

std::vector<VtInfo*>& VtParse::getVtInfos()
{
	// TODO: �ڴ˴����� return ���
	this->vtInfos;
}

int VtParse::readandparseJsonFromFile(char * vtdatas)
{
	Json::Reader reader;// ����json��Json::Reader   
	Json::Value root; // Json::Value��һ�ֺ���Ҫ�����ͣ����Դ����������͡���int, string, object, array         

	if (reader.parse(vtdatas, root, 0))
	{
		VtInfo* vtInfo = new VtInfo();
		Json::Value::Members arrayMember = root.getMemberNames(); //ayyayMember�ǳ�Ա���Ƶļ��ϣ���name,age;
		for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //����json��Ա
		{
			Json::Value valueList = root.get(*iter, "null");
			std::string key("set_scan_id");
			int file_size = valueList.size();  // �õ�"files"��������� 
			for (size_t i = 0; i < file_size; i++)
			{
				VT_RESPONSE_JSON_SET(vtInfo, key.c_str(), valueList.asString());
				sophosinfo->setJsonsInfo(*iter, valueList[i].asString());
			}
		}
		sophosInfos.push_back(sophosinfo);
	}
		
	
	



	return 0;
}
