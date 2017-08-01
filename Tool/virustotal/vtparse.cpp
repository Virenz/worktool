#include "vtparse.h"

VtParse::VtParse()
{
}

VtParse::~VtParse()
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

std::vector<VtInfo*>& VtParse::getVtInfos()
{
	// TODO: �ڴ˴����� return ���
	return this->vtInfos;
}

int VtParse::readandparseJsonFromFile(char* vtdatas)
{
	Json::Reader reader;// ����json��Json::Reader   
	Json::Value root; // Json::Value��һ�ֺ���Ҫ�����ͣ����Դ����������͡���int, string, object, array         

	if (reader.parse(vtdatas, root, 0))
	{
		if (root.isObject())	// ֻ��һ������
		{
			VtInfo* vtInfo = new VtInfo();
			Json::Value::Members arrayMember = root.getMemberNames(); //ayyayMember�ǳ�Ա���Ƶļ���
			for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //����json��Ա
			{
				Json::Value valueList = root.get(*iter, "null");
				if ((*iter)=="scans")
				{
					Json::Value::Members scansMember = valueList.getMemberNames(); //scansMember�ǳ�Ա���Ƶļ���
					for (Json::Value::Members::iterator scans_id = scansMember.begin(); scans_id != scansMember.end(); ++scans_id) //����scans��Ա
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
		else		//���ж�������
		{
			int file_size = root.size();  // �õ�"files"��������� 
			for (size_t i = 0; i < file_size; i++)
			{
				VtInfo* vtInfo = new VtInfo();
				Json::Value::Members arrayMember = root[i].getMemberNames(); //ayyayMember�ǳ�Ա���Ƶļ���
				for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //����json��Ա
				{
					Json::Value valueList = root[i].get(*iter, "null");
					if ((*iter) == "scans")
					{
						Json::Value::Members scansMember = valueList.getMemberNames(); //scansMember�ǳ�Ա���Ƶļ���
						for (Json::Value::Members::iterator scans_id = scansMember.begin(); scans_id != scansMember.end(); ++scans_id) //����scans��Ա
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
