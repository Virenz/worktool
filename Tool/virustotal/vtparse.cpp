#include "vtparse.h"

std::vector<VtInfo*>& VtParse::getVtInfos()
{
	// TODO: 在此处插入 return 语句
	this->vtInfos;
}

int VtParse::readandparseJsonFromFile(char * vtdatas)
{
	Json::Reader reader;// 解析json用Json::Reader   
	Json::Value root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array         

	if (reader.parse(vtdatas, root, 0))
	{
		VtInfo* vtInfo = new VtInfo();
		Json::Value::Members arrayMember = root.getMemberNames(); //ayyayMember是成员名称的集合，即name,age;
		for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //遍历json成员
		{
			Json::Value valueList = root.get(*iter, "null");
			std::string key("set_scan_id");
			int file_size = valueList.size();  // 得到"files"的数组个数 
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
