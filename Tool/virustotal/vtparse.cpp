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
	// TODO: �ڴ˴����� return ���
	return this->vtInfos;
}

int VtParse::readandparseJsonFromFile(char* vtdatas)
{
	cleanVtInfos();

	Json::Reader reader;// ����json��Json::Reader   
	Json::Value root; // Json::Value��һ�ֺ���Ҫ�����ͣ����Դ����������͡���int, string, object, array         

	if (reader.parse(vtdatas, root, 0))
	{
		if (root.isObject())	// ֻ��һ������
		{
			VtInfo* vtInfo = new VtInfo();

			if (root.get("response_code", "null").asInt() == 1)
			{
				// ��ȡscans����
				Json::Value valueList = root.get("scans", "null");
				if (valueList != "null")
				{
					Json::Value::Members scansMember = valueList.getMemberNames(); //scansMember�ǳ�Ա���Ƶļ���
					for (Json::Value::Members::iterator scans_id = scansMember.begin(); scans_id != scansMember.end(); ++scans_id) //����scans��Ա
					{
						Json::Value scans_name = valueList.get(*scans_id, "null");
						if (scans_name["detected"].asBool())
						{
							vtInfo->setJsonsInfo(*scans_id, scans_name["result"].asString());
						}
					}
					// ��ȡresource��positives/total
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
				// ��ȡresource��no found
				vtInfo->setVirusName(root.get("resource", "null").asString() + ":no found");
				vtInfo->set_resource(root.get("resource", "null").asString());
			}
			
			vtInfos.push_back(vtInfo);
		}
		else		//���ж�������
		{
			int file_size = root.size();  // �õ�"files"��������� 
			for (size_t i = 0; i < file_size; i++)
			{
				VtInfo* vtInfo = new VtInfo();
				if (root[i].get("response_code", "null").asInt() == 1)
				{
					// ��ȡscans����
					Json::Value valueList = root[i].get("scans", "null");
					if (valueList != "null")
					{
						Json::Value::Members scansMember = valueList.getMemberNames(); //scansMember�ǳ�Ա���Ƶļ���
						for (Json::Value::Members::iterator scans_id = scansMember.begin(); scans_id != scansMember.end(); ++scans_id) //����scans��Ա
						{
							Json::Value scans_name = valueList.get(*scans_id, "null");
							if (scans_name["detected"].asBool())
							{
								vtInfo->setJsonsInfo(*scans_id, scans_name["result"].asString());
							}
						}
						// ��ȡresource��positives/total
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
					// ��ȡresource��no found
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
		//ע��Ҫ���ͷ��ڴ棬��ɾ��vectorԪ�أ�˳���ܵߵ���
		//�ͷ��ڴ�
		delete (*iter);
		*iter = NULL;
		iter++;
	}
	vtInfos.clear();
}
