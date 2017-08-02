#include "vtparse.h"

VtParse::VtParse()
{
}

VtParse::~VtParse()
{
	cleanVtInfos();
}

std::vector<VtInfo*>& VtParse::getInfos()
{
	// TODO: 在此处插入 return 语句
	return this->vtInfos;
}

int VtParse::readandparseJsonFromFile(char* vtdatas)
{
	cleanVtInfos();

	Json::Reader reader;// 解析json用Json::Reader   
	Json::Value root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array         

	if (reader.parse(vtdatas, root, 0))
	{
		if (root.isObject())	// 只有一个数据
		{
			VtInfo* vtInfo = new VtInfo();

			if (root.get("response_code", "null").asInt() == 1)
			{
				// 获取scans数据
				Json::Value valueList = root.get("scans", "null");
				if (valueList != "null")
				{
					Json::Value::Members scansMember = valueList.getMemberNames(); //scansMember是成员名称的集合
					for (Json::Value::Members::iterator scans_id = scansMember.begin(); scans_id != scansMember.end(); ++scans_id) //遍历scans成员
					{
						Json::Value scans_name = valueList.get(*scans_id, "null");
						if (scans_name["detected"].asBool())
						{
							vtInfo->setJsonsInfo(*scans_id, scans_name["result"].asString());
						}
					}
					// 获取resource：positives/total
					char vt_name[260];
					sprintf_s(vt_name, " : %d/%d", root.get("positives", "null").asInt(), root.get("total", "null").asInt());
					vtInfo->setVirusName(root.get("sha1", "null").asString() + vt_name);
				}
				else
				{
					vtInfo->setVirusName(root.get("sha1", "null").asString());
				}
				
				vtInfo->set_resource(root.get("resource", "null").asString());
			}
			else
			{
				// 获取resource：no found
				vtInfo->setVirusName(root.get("resource", "null").asString() + ":no found");
				vtInfo->set_resource(root.get("resource", "null").asString());
			}
			
			vtInfos.push_back(vtInfo);
		}
		else		//含有多组数据
		{
			int file_size = root.size();  // 得到"files"的数组个数 
			for (size_t i = 0; i < file_size; i++)
			{
				VtInfo* vtInfo = new VtInfo();
				if (root[i].get("response_code", "null").asInt() == 1)
				{
					// 获取scans数据
					Json::Value valueList = root[i].get("scans", "null");
					if (valueList != "null")
					{
						Json::Value::Members scansMember = valueList.getMemberNames(); //scansMember是成员名称的集合
						for (Json::Value::Members::iterator scans_id = scansMember.begin(); scans_id != scansMember.end(); ++scans_id) //遍历scans成员
						{
							Json::Value scans_name = valueList.get(*scans_id, "null");
							if (scans_name["detected"].asBool())
							{
								vtInfo->setJsonsInfo(*scans_id, scans_name["result"].asString());
							}
						}
						// 获取resource：positives/total
						char vt_name[260];
						sprintf_s(vt_name, " : %d/%d", root[i].get("positives", "null").asInt(), root[i].get("total", "null").asInt());
						vtInfo->setVirusName(root[i].get("sha1", "null").asString() + vt_name);
					}
					else
					{
						vtInfo->setVirusName(root[i].get("sha1", "null").asString());
					}
					
					vtInfo->set_resource(root[i].get("resource", "null").asString());
				}
				else
				{
					// 获取resource：no found
					vtInfo->setVirusName(root[i].get("resource", "null").asString() + ":no found");
					vtInfo->set_resource(root[i].get("resource", "null").asString());
				}
				vtInfos.push_back(vtInfo);
			}
		}
	}

	return 0;
}

void VtParse::cleanVtInfos()
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
