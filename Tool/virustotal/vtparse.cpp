#include "vtparse.h"

std::vector<VtInfo*>& VtParse::getVtInfos()
{
	// TODO: �ڴ˴����� return ���
	this->vtInfos;
}

int VtParse::readandparseJsonFromFile(char* vtdatas)
{
	Json::Reader reader;// ����json��Json::Reader   
	Json::Value root; // Json::Value��һ�ֺ���Ҫ�����ͣ����Դ����������͡���int, string, object, array         

	if (reader.parse(vtdatas, root, 0))
	{
		if (root.isObject())
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
