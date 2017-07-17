#pragma once
#include <Windows.h>  
#include <stdio.h>

typedef unsigned char       u1;

void myFileDialogConfig(OPENFILENAME& ofn, HWND hwnd)
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
		hFile = CreateFile(filePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
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
		HANDLE hMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, FileSize.QuadPart, NULL);
		if (hMap)
		{
			pMapBuffer = (UCHAR*)MapViewOfFile(hMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, FileSize.QuadPart);
			OutputDebugString(TEXT("ReadFile success!\r\n"));

			CloseHandle(hMap);
			return pMapBuffer;
		}
	} while (true);

}