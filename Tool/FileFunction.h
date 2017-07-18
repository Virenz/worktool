#pragma once
#include <Windows.h>  
#include <stdio.h>
#include <string>
#include <locale>

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

// 使用windows api进行转换
// wchar_t to string
void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
	char *psText;  // psText为char*的临时数组，作为赋值给std::string的中间变量
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
	szDst = psText;// std::string赋值
	delete[]psText;// psText的清除
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

// 需包含locale、string头文件、使用setlocale函数。
wchar_t* StringToWchar_t(const std::string str)
{
	// string转wstring
	size_t converted = 0;
	unsigned len = str.size() * 2;// 预留字节数
	if (len == 0)
	{
		return NULL;
	}
	setlocale(LC_CTYPE, "");     //必须调用此函数
	wchar_t *p = new wchar_t[len];// 申请一段内存存放转换后的字符串
	mbstowcs_s(&converted, p, len, str.c_str(), _TRUNCATE);// 转换
	return p;
}

std::string WstringToString(const std::wstring str)
{
	// wstring转string
	size_t converted = 0;
	unsigned len = str.size() * 4;
	setlocale(LC_CTYPE, "");
	char *p = new char[len];
	wcstombs_s(&converted, p, len, str.c_str(), _TRUNCATE);
	std::string str1(p);
	delete[] p;
	return str1;
}