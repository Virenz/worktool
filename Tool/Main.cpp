#include <Windows.h>
#include <windowsx.h>
#include "resource.h"
#include <commctrl.h>

#include "sophos\sophosparse.h"
#include "FileFunction.h"

#include "include\curl\curl.h"
#pragma comment(lib, "libcurl.lib") 

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
void Dlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void getVirustotalJson(HWND hDlg);
size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

int InitTreeControl();

HINSTANCE hgInst;
HWND m_tree;
TV_ITEM tvi = {0};
TCHAR buf[256] = { 0 };
HTREEITEM Selected;
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

	getVirustotalJson(hdlg);

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
		switch (LOWORD(wParam))
		{
		case IDOK:
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
	}
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
				TreeView_DeleteAllItems(m_tree);
				InitTreeControl();
			}
		}

		break;
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
	sophosInfos.clear();
	delete sophosParse;

	return 0;
}

void getVirustotalJson(HWND hDlg)
{
	CURL *curl;
	CURLcode res;

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;
	struct curl_slist *headerlist = NULL;
	static const char header_buf[] = "Expect:";
	long http_response_code = 0;
	CHAR *data = (CHAR*)malloc(600);
	memset(data, 0, 600);

	curl_global_init(CURL_GLOBAL_ALL);

	/* Fill in the file upload field */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "file",
		CURLFORM_FILE, "83fba62fbb414fa09c2110e2b426501e1859a046.apk",
		CURLFORM_END);

	/* Fill in the filename field */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "filename",
		CURLFORM_COPYCONTENTS, "83fba62fbb414fa09c2110e2b426501e1859a046.apk",
		CURLFORM_END);

	/* Fill in the submit field too, even if this is rarely needed */
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "apikey",
		CURLFORM_COPYCONTENTS, "63efab1f4d9bd879a836a9a38917d89265aedb261ea334df9637fef108512821",
		CURLFORM_END);

	curl = curl_easy_init();
	/* initalize custom header list (stating that Expect: 100-continue is not
	wanted */
	headerlist = curl_slist_append(headerlist, header_buf);
	if (curl) {
		/* what URL that receives this POST */

		curl_easy_setopt(curl, CURLOPT_URL, "http://www.virustotal.com/vtapi/v2/file/scan");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, header_buf);
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

		//通过write_data方法将联网返回数据写入到data中
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);
		/* Check for errors */
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
		else
		{
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
			if (http_response_code == 200) 
			{
				MessageBox(hDlg, StringToWchar_t(data), L"Example", MB_OK | MB_ICONINFORMATION);
			}
		}
		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
		/* free slist */
		curl_slist_free_all(headerlist);
	}
}

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
	size_t bytes = size * nmemb;  // total amount of data.
	CHAR* page_hand = (CHAR*)stream;
	stream = (CHAR*)realloc(page_hand, bytes);
	if (!page_hand)
	{
		return size * nmemb;
	}
	memcpy(page_hand, ptr, bytes);
	page_hand[bytes] = 0;

	return size * nmemb;
}