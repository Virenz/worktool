#pragma once
#include <Windows.h>  
#include <string>
#include <locale>
#include <regex>
#include <vector>

typedef unsigned char       u1;

extern void myFileDialogConfig(OPENFILENAME& ofn, HWND hwnd)
{
	static TCHAR  szFileName[MAX_PATH], szTitleName[MAX_PATH];
	static TCHAR szFilter[] = TEXT("All Files (*.*)\0*.*\0\0");
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = szTitleName;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrDefExt = TEXT("*");
	ofn.lpstrTitle = NULL;
	ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
	ofn.lStructSize = sizeof(OPENFILENAME);  // No OFN_ENABLEHOOK
	ofn.hwndOwner = hwnd;
}

const u1* getFileInfo(LPWSTR filePath)
{
	const u1* pMapBuffer;
	LARGE_INTEGER FileSize = {};
	HANDLE hFile = INVALID_HANDLE_VALUE;
	do
	{
		hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			OutputDebugString(TEXT("CreateFile fail!\r\n"));
			break;
		}
		
		if (!GetFileSizeEx(hFile, &FileSize) || FileSize.QuadPart == 0)
		{
			OutputDebugString(TEXT("FileSize error!\r\n"));
			break;
		}
		HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, FileSize.QuadPart, NULL);
		if (hMap)
		{
			pMapBuffer = (UCHAR*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, FileSize.QuadPart);
			OutputDebugString(TEXT("ReadFile success!\r\n"));
			CloseHandle(hMap);
			CloseHandle(hFile);
			FlushViewOfFile(pMapBuffer, FileSize.QuadPart);
			return pMapBuffer;
		}
	} while (FALSE);

}

// ʹ��windows api����ת��
// wchar_t to string
void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
	char *psText;  // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
	szDst = psText;// std::string��ֵ
	delete[]psText;// psText�����
}

// string to wstring
void StringToWstring(std::wstring szDst, std::string str)
{
	std::string temp = str;
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, NULL, 0);
	wchar_t * wszUtf8 = new wchar_t[len + 1];
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)temp.c_str(), -1, (LPWSTR)wszUtf8, len);
	szDst = wszUtf8;
	delete[] wszUtf8;
}

// �����locale��stringͷ�ļ���ʹ��setlocale������
wchar_t* StringToWchar_t(const std::string str)
{
	// stringתwstring
	size_t converted = 0;
	unsigned len = str.size() * 2;// Ԥ���ֽ���
	if (len == 0)
	{
		return NULL;
	}
	setlocale(LC_CTYPE, "");     //������ô˺���
	wchar_t *p = new wchar_t[len];// ����һ���ڴ���ת������ַ���
	mbstowcs_s(&converted, p, len, str.c_str(), _TRUNCATE);// ת��
	return p;
}

std::string WstringToString(const std::wstring str)
{
	// wstringתstring
	size_t converted = 0;
	unsigned len = str.size() * 4;
	setlocale(LC_CTYPE, "");
	char *p = new char[len];
	wcstombs_s(&converted, p, len, str.c_str(), _TRUNCATE);
	std::string str1(p);
	delete[] p;
	return str1;
}

// ������ʽƥ��sha1/md5/sha256
bool is_report_valid(const std::string& data, std::vector<std::string> *reportdata)
{
	int index = 0;
	std::string strbuf;
	const std::regex pattern("[a-z0-9]{40}");
	for (std::sregex_iterator it(data.begin(), data.end(), pattern), end;     //end��β���������regex_iterator��regex_iterator��string���͵İ汾
		it != end;
		++it, index++)
	{
		if (index % 4 == 0 && index != 0)
		{
			reportdata->push_back(strbuf.c_str());
			strbuf.clear();
		}
		strbuf.append(it->str());
		strbuf.append(",");
	}
	if (index == 0)
	{
		return false;
	}
	else
	{
		reportdata->push_back(strbuf.c_str());
		return true;
	}
	//std::match_results<std::string::const_iterator> result;
	//bool valid = std::regex_match(data, result, pattern);
	////�˴�result�������п��ޣ�result��һ���ַ������飬�����洢������ʽ�������ŵ����ݡ�
	//std::string strbuf;
	//if(valid&&(result.length()>0))
	//{
	//	for(int i = 0;i<result.length();i++)
	//	{
	//		if (i % 4 == 0 && i != 0)
	//		{
	//			reportdata.push_back(strbuf.c_str());
	//			strbuf.clear();
	//		}
	//		printf("%s", result[i]);
	//		strbuf.append(result[i]);
	//		strbuf.append(",");
	//	}
	//}
	//return valid;
}

// ƥ��apk�ļ�
int is_apk_valid(const std::string& data)
{
	int index = data.find("apk", 0);
	return index;
}