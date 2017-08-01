#include "vtparse.h"

VtParse::VtParse()
{
}

VtParse::~VtParse()
{
	std::vector<VtInfo*>::iterator iter = vtInfos.begin();
	while (iter != vtInfos.end()) //#1
	{
		//注意要先释放内存，在删除vector元素，顺序不能颠倒。
		//释放内存
		delete (*iter);
		*iter = NULL;
		iter++;
	}
	vtInfos.clear();
}

std::vector<VtInfo*>& VtParse::getVtInfos()
{
	// TODO: 在此处插入 return 语句
	return this->vtInfos;
}

int VtParse::readandparseJsonFromFile(char* vtdatas)
{
	Json::Reader reader;// 解析json用Json::Reader   
	Json::Value root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array         

	if (reader.parse(vtdatas, root, 0))
	{
		if (root.isObject())	// 只有一个数据
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
						Json::Value scans_name = valueList.get(*scans_id, "null");
						if (scans_name["detected"].asBool())
						{
							ScanInfo* scaninfo = new ScanInfo();
							scaninfo->set_result(scans_name["result"].asString());
							vtInfo->set_scans(*scans_id, scaninfo);
						}
					}
				}
				else if ((*iter) == "total")
				{
					vtInfo->set_total(valueList.asInt());
				}
				else if ((*iter) == "positives")
				{
					vtInfo->set_positives(valueList.asInt());
				}
				else if ((*iter) == "resource")
				{
					vtInfo->setVirusName(valueList.asString());
				}
				
			}
			vtInfos.push_back(vtInfo);
		}
		else		//含有多组数据
		{
			int file_size = root.size();  // 得到"files"的数组个数 
			for (size_t i = 0; i < file_size; i++)
			{
				VtInfo* vtInfo = new VtInfo();
				Json::Value::Members arrayMember = root[i].getMemberNames(); //ayyayMember是成员名称的集合
				for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //遍历json成员
				{
					Json::Value valueList = root[i].get(*iter, "null");
					if ((*iter) == "scans")
					{
						Json::Value::Members scansMember = valueList.getMemberNames(); //scansMember是成员名称的集合
						for (Json::Value::Members::iterator scans_id = scansMember.begin(); scans_id != scansMember.end(); ++scans_id) //遍历scans成员
						{
							Json::Value scans_name = valueList.get(*scans_id, "null");
							if (scans_name["detected"].asBool())
							{
								ScanInfo* scaninfo = new ScanInfo();
								scaninfo->set_result(scans_name["result"].asString());
								vtInfo->set_scans(*scans_id, scaninfo);
							}
						}
					}
					else if ((*iter) == "total")
					{
						vtInfo->set_total(valueList.asInt());
					}
					else if ((*iter) == "positives")
					{
						vtInfo->set_positives(valueList.asInt());
					}
					else if ((*iter) == "resource")
					{
						vtInfo->setVirusName(valueList.asString());
					}
				}
				vtInfos.push_back(vtInfo);
			}
		}
	}

	return 0;
}
