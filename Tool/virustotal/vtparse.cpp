#include "vtparse.h"

std::vector<VtInfo*>& VtParse::getVtInfos()
{
	// TODO: 在此处插入 return 语句
	this->vtInfos;
}

int VtParse::readandparseJsonFromFile(char* vtdatas)
{
	Json::Reader reader;// 解析json用Json::Reader   
	Json::Value root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array         

	if (reader.parse(vtdatas, root, 0))
	{
		if (root.isObject())
		{
			VtInfo* vtInfo = new VtInfo();
			Json::Value::Members arrayMember = root.getMemberNames(); //ayyayMember是成员名称的集合
			for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //遍历json成员
			{
				Json::Value valueList = root.get(*iter, "null");
				if ((*iter)=="scans")
				{
					Json::Value::Members scansMember = valueList.getMemberNames(); //scansMember是成员名称的集合
					for (Json::Value::Members::iterator scans_id = scansMember.begin(); scans_id != scansMember.end(); ++scans_id) //遍历scans成员
					{
						Json::Value scans_name = root.get(*scans_id, "null");
						if (scans_name["detected"].asBool())
						{
							ScanInfo* scaninfo = new ScanInfo();
							scaninfo->set_result(scans_name["result"].asString());
							vtInfo->set_scans(*scans_id, scaninfo);
						}
					}
				}
				else
				{
					vtInfo->set_vtsinfo(*iter, valueList.asString());
				}
				
			}
			vtInfos.push_back(vtInfo);
		}
		
	}

	return 0;
}
