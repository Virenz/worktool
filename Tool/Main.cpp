#include <Windows.h>
#include "resource.h"
#include <commctrl.h>

#include "sophosparse.h"
#include "FileFunction.h"

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

int InitTreeControl();

HINSTANCE hgInst;
HWND m_tree;
SophosParse* sophosParse;

int WINAPI WinMain(HINSTANCE hThisApp, HINSTANCE hPrevApp, LPSTR lpCmd, int nShow)
{
	hgInst = hThisApp;
	//DialogBox(hgInst, MAKEINTRESOURCE(IDD_MY), NULL, DlgProc);
	HWND hdlg = CreateDialog(hThisApp, MAKEINTRESOURCE(IDD_DIALOG1), GetDesktopWindow(), (DLGPROC)DlgProc);
	if (!hdlg)
	{
		return 0;
	}
	ShowWindow(hdlg, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		// 设置对话框的图标   
		//SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hgInst, MAKEINTRESOURCE(IDI_APP)));
		return 0;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE)
		{
			PostQuitMessage(0);//退出     
		}
		return 0;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
			Dlg_OnCommand(hDlg, IDOK, GetDlgItem(hDlg, IDOK), BN_CLICKED);

	}

	return (INT_PTR)FALSE;
}

void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

	switch (id) {

	case IDCANCEL:
		EndDialog(hwnd, id);
		break;

	case IDOK:

		DWORD dwError = GetDlgItemInt(hwnd, IDC_FILEPATH, NULL, FALSE);

		OPENFILENAME  ofn;
		myFileDialogConfig(ofn, hwnd);
		if (GetOpenFileName(&ofn))
		{
			if (ofn.lpstrFile != NULL)
			{
				SetDlgItemText(hwnd, IDC_FILEPATH, ofn.lpstrFile);

				char* filedata = (char*)getFileInfo(ofn.lpstrFile);
				sophosParse = new SophosParse();
				sophosParse->readandparseJsonFromFile(filedata);

				UnmapViewOfFile(filedata);

				m_tree = GetDlgItem(hwnd, IDC_DATASHOW);
				InitTreeControl();
			}
		}

		
	}
}

int InitTreeControl()
{
	std::vector<SophosInfo*>& sophosInfos = sophosParse->getSophosInfos();
	for (SophosInfo* sp : sophosInfos)
	{
		TV_ITEM item;
		item.mask = TVIF_TEXT | TVIF_PARAM;
		item.cchTextMax = 2;
		item.pszText = StringToWchar_t(sp->getVirusName());

		TV_INSERTSTRUCT insert;
		insert.hParent = TVI_ROOT;
		insert.hInsertAfter = TVI_LAST;
		insert.item = item;

		HTREEITEM root1 = TreeView_InsertItem(m_tree, &insert);
		for (std::multimap<std::string, std::string>::iterator iter = sp->getJsonsInfo().begin(); iter != sp->getJsonsInfo().end(); ++iter) //遍历json成员
		{
			std::string str;
			str.append(iter->first);
			str.append(" : ");
			str.append(iter->second);
			wchar_t * wszUtf8 = StringToWchar_t(str);

			TV_ITEM item1;
			item1.mask = TVIF_TEXT | TVIF_PARAM | TVS_HASLINES | TVS_LINESATROOT;
			item1.cchTextMax = 2;
			item1.pszText = wszUtf8;

			TV_INSERTSTRUCT insert1;
			insert1.hParent = root1;
			insert1.hInsertAfter = TVI_LAST;
			insert1.item = item1;

			HTREEITEM root2 = TreeView_InsertItem(m_tree, &insert1);

			delete[] wszUtf8;
		}

	}
	sophosInfos.clear();
	delete sophosParse;

	return 0;
}

