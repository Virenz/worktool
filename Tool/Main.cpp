#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <WinUser.h>
#include "resource.h"
#include <commctrl.h>
#include <tchar.h> 
#include <thread>

#include "sophos\sophosparse.h"
#include "FileFunction.h"
#include "vthttp\vtapi.h"
#include "virustotal\vtparse.h"
#include "apkinfo\apkparse.h"


INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);

VOID OnDropFiles(HWND hwnd, HDROP hDropInfo);								// 实现文件拖动进入TEXT框
void performActions(HWND hwnd);							// 获取单选框和编辑框进行对应的操作

template<class T>
int InitTreeControl(T *uidatas);											// 进行tree布局显示数据

HINSTANCE hgInst;
HWND m_tree;
HWND m_progress;
TV_ITEM tvi = {0};
TCHAR buf[256] = { 0 };
HTREEITEM Selected;

TCHAR title[20] = L"wlc-tool v0.1";											// 设置标题 

//对应
//1:SHA1/MD5/SHA256
//2:APK
//3:avt邮件
//4:sophos
//5:APKINFO
UINT m_type = 0;

int WINAPI WinMain(HINSTANCE hThisApp, HINSTANCE hPrevApp, LPSTR lpCmd, int nShow)
{
	hgInst = hThisApp;
	//DialogBox(hgInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);
	HWND hdlg = CreateDialog(hThisApp, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
	
	m_progress = GetDlgItem(hdlg, IDC_PROGRESSEXEC);
	DragAcceptFiles(hdlg,TRUE);
	if (!hdlg)
	{
		return 0;
	}
	ShowWindow(hdlg, SW_SHOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (msg.message == WM_KEYDOWN && GetKeyState('A') && GetKeyState(VK_CONTROL))
		{
			Edit_SetSel(GetDlgItem(hdlg, IDC_FILEPATH), 0, -1);
		}

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
	{
		// 设置对话框的图标 
		SetWindowText(hDlg, title);
		SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(hgInst, MAKEINTRESOURCE(IDI_ICON1)));
		return 0;
	}
	case WM_SYSCOMMAND:
	{
		if (wParam == SC_CLOSE)
		{
			PostQuitMessage(0);//退出     
		}
		return 0;
	}
	case WM_KEYDOWN:
	{
		switch (LOWORD(wParam))
		{
		case VK_CONTROL:
			int iState = GetKeyState('a');
			if (iState < 0)
			{
				Edit_SetSel(GetDlgItem(hDlg, IDC_FILEPATH), 0, -1);
				MessageBox(hDlg, L"全选", NULL, 0);
			}
			else
			{
				MessageBox(hDlg, L"为全选", NULL, 0);
			}
		}
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			EnableWindow(GetDlgItem(hDlg, IDOK), FALSE);
			Dlg_OnCommand(hDlg, IDOK, GetDlgItem(hDlg, IDOK), BN_CLICKED);
			break;
		case ID_COPY_COPY:
			Dlg_OnCommand(hDlg, ID_COPY_COPY, GetDlgItem(m_tree, ID_COPY_COPY), BN_CLICKED);
			break;
		}
		break;
	case WM_NOTIFY:
		{
			switch (LOWORD(wParam))
			{
			case IDC_DATASHOW:
				// if code == NM_CLICK - Single click on an item
				if (((LPNMHDR)lParam)->code == NM_RCLICK)
				{
					/*DWORD dwPos = GetMessagePos(); 
					POINT pt; 
					pt.x = LOWORD(dwPos); 
					pt.y = HIWORD(dwPos);
					ScreenToClient(m_tree, &pt);
					
					TVHITTESTINFO ht = { 0 }; 
					ht.pt = pt;
					ht.flags = TVHT_ONITEM; 
					HTREEITEM hItem = TreeView_HitTest(m_tree, &ht);

					TVITEM ti = { 0 }; 
					TCHAR buf[256] = { 0 };
					ti.mask = TVIF_HANDLE | TVIF_TEXT; 
					ti.cchTextMax = 256; 
					ti.pszText = buf; 
					ti.hItem = hItem;
					TreeView_GetItem(m_tree, &ti); 
					MessageBox(m_tree, buf, NULL, 0);*/
				
					tvi = { 0 };
					memset(&tvi, 0, sizeof(tvi));
					Selected = (HTREEITEM)SendDlgItemMessage(hDlg,
						IDC_DATASHOW, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)Selected);

					if (Selected == NULL)
					{
						MessageBox(hDlg, L"No Items in TreeView",
							L"Error", MB_OK | MB_ICONINFORMATION);
						break;
					}
					TreeView_EnsureVisible(hDlg, Selected);
					SendDlgItemMessage(hDlg, IDC_DATASHOW,
						TVM_SELECTITEM, TVGN_CARET, (LPARAM)Selected);
				
					
					tvi.mask = TVIF_HANDLE | TVIF_TEXT;
					tvi.cchTextMax = 256;
					tvi.pszText = buf;
					tvi.hItem = Selected;

					if (SendDlgItemMessage(hDlg, IDC_DATASHOW, TVM_GETITEM, TVGN_CARET, (LPARAM)&tvi))
					{
						/*MessageBox(hDlg, tvi.pszText,
						L"Example", MB_OK | MB_ICONINFORMATION);	*/
					}

					RECT rect;
					POINT pt;
					// 获取鼠标右击是的坐标     
					DWORD dwPos = GetMessagePos();
					pt.x = LOWORD(dwPos);
					pt.y = HIWORD(dwPos);
					//获取客户区域大小     
					GetClientRect(m_tree, &rect);
					//把屏幕坐标转为客户区坐标     
					ScreenToClient(m_tree, &pt);
					//判断点是否位于客户区域内     
					if (PtInRect(&rect, pt))
					{
						//加载菜单资源     
						HMENU hroot = LoadMenu((HINSTANCE)GetWindowLongPtr(m_tree, GWLP_HINSTANCE), MAKEINTRESOURCE(IDR_MENU1));
						if (hroot)
						{
							// 获取第一个弹出菜单     
							HMENU hpop = GetSubMenu(hroot, 0);
							// 把客户区坐标还原为屏幕坐标     
							ClientToScreen(m_tree, &pt);
							//显示快捷菜单     
							TrackPopupMenu(hpop,
								TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RIGHTBUTTON,
								pt.x,
								pt.y,
								0,
								hDlg,
								NULL);
							// 用完后要销毁菜单资源     
							DestroyMenu(hroot);
						}
					}
				}
			}
		}
		break;
	case WM_DROPFILES:
		OnDropFiles(hDlg, (HDROP)wParam);
		return 0;
	}
	//return DefWindowProc(hDlg, msg, wParam, lParam);
	return (INT_PTR)FALSE;
}

void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) {

	switch (id) {

	case IDCANCEL:
		EndDialog(hwnd, id);
		break;

	case ID_COPY_COPY:
		{
			//复制内容     
			//打开剪贴板     
			OpenClipboard(NULL);
			//清空剪贴板     
			EmptyClipboard();

			//分配内存     
			HGLOBAL hgl = GlobalAlloc(GMEM_MOVEABLE, 256 * sizeof(WCHAR));
			LPWSTR lpstrcpy = (LPWSTR)GlobalLock(hgl);
			memcpy(lpstrcpy, buf, 256 * sizeof(WCHAR));
			GlobalUnlock(hgl);
			SetClipboardData(CF_UNICODETEXT, lpstrcpy);
			//关闭剪贴板     
			CloseClipboard();
			break;
		}
	case IDOK:
		{
			std::thread action(performActions, hwnd);
			action.detach();
			//performActions(hwnd, fileName);
			break;
		}
	}
}

template<class T> 
int InitTreeControl(T *uidatas)
{
	auto infos = uidatas->getInfos();
	for (auto sp : infos)
	{
		TV_ITEM item;
		item.mask = TVIF_TEXT;
		item.cchTextMax = 10;
		item.pszText = StringToWchar_t(sp->getVirusName());

		TV_INSERTSTRUCT insert;
		insert.hParent = TVI_ROOT;
		insert.hInsertAfter = TVI_LAST;
		insert.item = item;

		Selected = TreeView_InsertItem(m_tree, &insert);
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
			insert1.hParent = Selected;
			insert1.hInsertAfter = TVI_LAST;
			insert1.item = item1;

			HTREEITEM root2 = TreeView_InsertItem(m_tree, &insert1);

			delete[] wszUtf8;
		}

	}

	return 0;
}

VOID OnDropFiles(HWND hwnd, HDROP hDropInfo)
{
	//UINT  nFileCount = DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);  //查询一共拖拽了几个文件  
	TCHAR szFileName[MAX_PATH] = TEXT("");
	DragQueryFile(hDropInfo, 0, szFileName, sizeof(szFileName));  //打开拖拽的第一个(下标为0)文件  
	HWND hEdit = GetDlgItem(hwnd, IDC_FILEPATH);
	SetWindowText(hEdit, szFileName);
	//完成拖入文件操作，系统释放缓冲区   
	DragFinish(hDropInfo);
}

void performActions(HWND hwnd)
{
	if (IsDlgButtonChecked(hwnd, IDC_MD) == BST_CHECKED)
		m_type = 1;
	if (IsDlgButtonChecked(hwnd, IDC_APK) == BST_CHECKED)
		m_type = 2;
	if (IsDlgButtonChecked(hwnd, IDC_AVT) == BST_CHECKED)
		m_type = 3;
	if (IsDlgButtonChecked(hwnd, IDC_SOPHOS) == BST_CHECKED)
		m_type = 4;
	if (IsDlgButtonChecked(hwnd, IDC_APKINFO) == BST_CHECKED)
		m_type = 5;

	HWND editText = GetDlgItem(hwnd, IDC_FILEPATH);
	// 复制某一行到缓冲区; 
	WCHAR txContent[MAX_PATH];
	
	switch (m_type)
	{
	case 1:
	{
		std::vector<std::string> reportdata;
		std::string strbuf;
		bool m_erro;
		// 获得编辑器的行数;  
		int iCount = Edit_GetLineCount(editText);
		int index;
		for (index = 0; index < iCount; index++)
		{
			memset(txContent, 0, MAX_PATH);
			int iLength = Edit_GetLine(editText, index, txContent, MAX_PATH);
			txContent[iLength] = '\0';

			m_erro = is_reports_valid(WstringToString(txContent));
			if (m_erro)
			{
				if (index % 4 == 0 && index != 0)
				{
					reportdata.push_back(strbuf.c_str());
					strbuf.clear();
				}
				strbuf.append(WstringToString(txContent));
				strbuf.append(",");
			}
			else
			{
				MessageBox(hwnd, L"非SHA1/MD5/SHA256", NULL, 0);
			}
		}
		if (index == 0)
		{
			MessageBox(hwnd, L"输入文件为空", NULL, 0);
			// 唤醒执行 按钮
			EnableWindow(GetDlgItem(hwnd, IDOK), true);
			return;
		}
		else
		{
			reportdata.push_back(strbuf.c_str());
		}

		VtApi *vt_api = new VtApi();
		// 清除m_tree的界面
		m_tree = GetDlgItem(hwnd, IDC_DATASHOW);
		TreeView_DeleteAllItems(m_tree);

		for (auto reportdata_index : reportdata)
		{
			bool isSuccess = vt_api->VtReport(reportdata_index.c_str());
			if (isSuccess)
			{
				char* testjson = vt_api->getReportJson();
				VtParse* vtParse = new VtParse();
				vtParse->readandparseJsonFromFile(testjson);

				InitTreeControl(vtParse);

				delete vtParse;
				vt_api->cleanChunk();
			}
		}
		delete vt_api;
		SendMessage(m_progress, PBM_SETPOS, 100, 0L);
		
		// 唤醒执行 按钮
		EnableWindow(GetDlgItem(hwnd, IDOK), true);
	}
	break;
	case 2:
	{
		Edit_GetText(editText, txContent, MAX_PATH);
		int m_apk = is_apk_valid(WstringToString(txContent));
		if (m_apk > 0)
		{
			VtApi *vt_api = new VtApi();
			// 清除m_tree的界面
			m_tree = GetDlgItem(hwnd, IDC_DATASHOW);
			TreeView_DeleteAllItems(m_tree);

			bool isSuccess = vt_api->VtScanFile(WstringToString(txContent).c_str(), m_progress);
			if (isSuccess)
			{
				char* filescanjson = vt_api->getReportJson();
				VtParse* vtParse = new VtParse();
				vtParse->readandparseJsonFromFile(filescanjson);
				vt_api->cleanChunk();

				for (auto sp : vtParse->getInfos())
				{
					vt_api->VtRescanFile(sp->get_resource().c_str());
					vt_api->cleanChunk();

					bool isSuccess = vt_api->VtReport(sp->get_resource().c_str());
					if (isSuccess)
					{
						char* reportjson = vt_api->getReportJson();
						vtParse->readandparseJsonFromFile(reportjson);

						InitTreeControl(vtParse);
						SendMessage(m_progress, PBM_SETPOS, 100, 0L);

						delete vtParse;
						vt_api->cleanChunk();
					}
				}
			}
			delete vt_api;
		}
		else
		{
			MessageBox(hwnd, L"非apk后缀文件", NULL, 0);
		}
		// 唤醒执行 按钮
		EnableWindow(GetDlgItem(hwnd, IDOK), true);
	}
	break;
	case 3:
		break;
	case 4:
	{
		Edit_GetText(editText, txContent, MAX_PATH);
		char* filedata = (char*)getFileInfo(txContent);
		SophosParse* sophosParse = new SophosParse();
		sophosParse->readandparseJsonFromFile(filedata);

		UnmapViewOfFile(filedata);

		m_tree = GetDlgItem(hwnd, IDC_DATASHOW);
		TreeView_DeleteAllItems(m_tree);
		InitTreeControl(sophosParse);
		delete sophosParse;

		SendMessage(m_progress, PBM_SETPOS, 100, 0L);
		// 唤醒执行 按钮
		EnableWindow(GetDlgItem(hwnd, IDOK), true);
	}
	break;
	case 5:
	{
		Edit_GetText(editText, txContent, MAX_PATH);
		ApkParse* apkParse = new ApkParse();
		apkParse->readandparseJsonFromFile(WstringToString(txContent).c_str());

		m_tree = GetDlgItem(hwnd, IDC_DATASHOW);
		TreeView_DeleteAllItems(m_tree);
		InitTreeControl(apkParse);

		delete apkParse;

		SendMessage(m_progress, PBM_SETPOS, 100, 0L);
		// 唤醒执行 按钮
		EnableWindow(GetDlgItem(hwnd, IDOK), true);
	}
	break;
	default:
		MessageBox(hwnd, L"请选择类型", NULL, 0);
		// 唤醒执行 按钮
		EnableWindow(GetDlgItem(hwnd, IDOK), true);
		break;
	}

	/*OPENFILENAME  ofn;
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
	TreeView_DeleteAllItems(m_tree);
	InitTreeControl();
	}
	}*/
}