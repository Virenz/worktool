#include "FileFunction.h"
#include "resource.h"

#include "include\json\json.h"
#include <map>
#include <commctrl.h>

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

int ReadJsonFromFile(LPWSTR filename);
int InitTreeControl();

std::multimap<std::string, std::string> jsonsmap;

HINSTANCE hgInst;
HWND m_tree;

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
			SetDlgItemText(hwnd, IDC_FILEPATH, ofn.lpstrFile);
		}

		ReadJsonFromFile(ofn.lpstrFile);
		m_tree = GetDlgItem(hwnd, IDC_DATASHOW);
		InitTreeControl();
	}
}

int ReadJsonFromFile(LPWSTR filename)
{
	Json::Reader reader;// 解析json用Json::Reader   
	Json::Value root; // Json::Value是一种很重要的类型，可以代表任意类型。如int, string, object, array         

	char* data = (CHAR*)getFileInfo(filename);
	//const char* str = "{\'AXML_BasicReceivers\': [\'com/mgb/safe/receiver/BootReceiver\'], \'AXML_BasicServices\' : [\'com/mgb/safe/service/ResidentService\', \'com/mgb/safe/service/CoreService\']}";
		//"{\"AXML_BasicReceivers\": [\"com/mgb/safe/receiver/BootReceiver\"], \"AXML_BasicServices\" : [\"com/mgb/safe/service/ResidentService\", \"com/mgb/safe/service/CoreService\"]}";

	// 进行切割
	const char *d = "\r\n";
	char *dataLine;
	char *buff;
	dataLine = strtok_s(data, d, &buff);
	while (dataLine)
	{
		if (reader.parse(dataLine, root, FALSE))
		{
			Json::Value::Members arrayMember = root.getMemberNames(); //ayyayMember是成员名称的集合，即name,age;
			for (Json::Value::Members::iterator iter = arrayMember.begin(); iter != arrayMember.end(); ++iter) //遍历json成员
			{
				Json::Value valueList = root.get(*iter, "null");
				int file_size = valueList.size();  // 得到"files"的数组个数 
				for (size_t i = 0; i < file_size; i++)
				{
					jsonsmap.insert(std::pair<std::string, std::string>(*iter, valueList[i].asString()));
				}
			}
		}
		else
		{

		}
		dataLine = strtok_s(NULL, d, &buff);
	}

	return 0;
}

int InitTreeControl()
{
	TV_ITEM item;
	item.mask = TVIF_TEXT | TVIF_PARAM;
	item.cchTextMax = 2;
	item.pszText = L"1";

	TV_INSERTSTRUCT insert;
	insert.hParent = TVI_ROOT;
	insert.hInsertAfter = TVI_LAST;
	insert.item = item;

	HTREEITEM root1 = TreeView_InsertItem(m_tree, &insert);
	for (std::multimap<std::string, std::string>::iterator iter = jsonsmap.begin(); iter != jsonsmap.end(); ++iter) //遍历json成员
	{
		//wchar_t treeStr[20];
		//swprintf(treeStr, "%s : %s", (*iter).first, (*iter).second);
		TV_ITEM item1;
		item1.mask = TVIF_TEXT | TVIF_PARAM | TVS_HASLINES | TVS_LINESATROOT;
		item1.cchTextMax = 2;
		item1.pszText = L"TEST";

		TV_INSERTSTRUCT insert1;
		insert1.hParent = root1;
		insert1.hInsertAfter = TVI_LAST;
		insert1.item = item1;

		HTREEITEM root2 = TreeView_InsertItem(m_tree, &insert1);
	}


	return 0;
}
