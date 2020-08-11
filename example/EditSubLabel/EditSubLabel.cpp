// EditSubLabel.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include <windowsx.h>
#include <commdlg.h>
#include <commctrl.h>
#include "extension.h"
#include "EditSubLabel.h"

// Enable Visual Style
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#pragma endregion

// Global Variables:
HINSTANCE hInst;                                // current instance

// Forward declarations of functions included in this code module:
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

BOOL OnInitDialog(HWND, HWND, LPARAM);
void OnDestroy(HWND);
void OnCommand(HWND, int, HWND, UINT);
BOOL OnNotify(HWND, int, LPNMHDR);

//
// Write formatted data to a string
//
template<UINT Size>
LPTSTR TextFormat(TCHAR(&lpBuff)[Size], LPCTSTR frm, ...)
{
    va_list args;
    va_start(args, frm);
    _vstprintf_s(lpBuff, frm, args);
    va_end(args);
    return lpBuff;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITSUBLABEL));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateDialog(hInstance,
       MAKEINTRESOURCE(IDD_MAINDLG),
       nullptr,
       (DLGPROC)WndProc);

   if (!hWnd)
      return FALSE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hwnd, WM_NOTIFY, OnNotify);
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

// 
// WM_INITDIALOG message is sent to the dialog box procedure
// just before the dialog box appears on the screen
// 
BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    // Set the window icon
    HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON)); // load the icon from the resources
    if (hIcon != 0) {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
    }
    
    HWND ctrl = GetDlgItem(hwnd, IDC_GRID);
    ListView_SetExtendedListViewStyle(ctrl,
        ListView_GetExtendedListViewStyle(ctrl) | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    LVCOLUMN lvc = { 0 };
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
    lvc.cx = 100;

    lvc.fmt = LVCFMT_LEFT;
    lvc.pszText = (LPTSTR)_T("Left");
    ListView_InsertColumn(ctrl, 0, &lvc);

    lvc.fmt = LVCFMT_CENTER;
    lvc.pszText = (LPTSTR)_T("Center");
    ListView_InsertColumn(ctrl, 1, &lvc);

    lvc.fmt = LVCFMT_RIGHT;
    lvc.pszText = (LPTSTR)_T("Right");
    ListView_InsertColumn(ctrl, 2, &lvc);

    lvc.fmt = LVCFMT_RIGHT;
    lvc.pszText = (LPTSTR)_T("Empty");
    ListView_InsertColumn(ctrl, 3, &lvc);

    TCHAR buff[128];
    for (int i = 0; i < 50; i++) {
        LV_ITEM lvi = { 0 };
        lvi.iItem = i;
        ListView_InsertItem(ctrl, &lvi);

        ListView_SetItemText(ctrl, lvi.iItem, 0, TextFormat(buff, _T("Left   %02d"), i));
        ListView_SetItemText(ctrl, lvi.iItem, 1, TextFormat(buff, _T("Center %02d"), i));
        ListView_SetItemText(ctrl, lvi.iItem, 2, TextFormat(buff, _T("Right  %02d"), i));
        ListView_SetItemText(ctrl, lvi.iItem, 3, TextFormat(buff, _T("")));
    }

    return TRUE;
}

// 
// The WM_DESTROY message is sent when a window is destroyed.
// 
void OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDOK:
    case IDCANCEL:
        DestroyWindow(hwnd);
        break;
    }
}

BOOL OnNotify(HWND hwnd, int idCtrl, LPNMHDR pnm)
{
	switch (pnm->idFrom)
	{
	case IDC_GRID:
		switch (pnm->code)
		{
		case LVN_BEGINSCROLL:
		{
			HWND edit = FindWindowEx(pnm->hwndFrom, NULL, WC_EDIT, NULL);
			if (edit) {
				SendMessage(edit, WM_KEYDOWN, VK_ESCAPE, 0);
			}
			break;
		}
		case LVN_ENDLABELEDIT:
		{
			NMLVDISPINFO* pdi = (NMLVDISPINFO*)pnm;
			ListView_SetItemText(pnm->hwndFrom, pdi->item.iItem, pdi->item.iSubItem, pdi->item.pszText);
			break;
		}
		case NM_DBLCLK:
		{
			LPNMITEMACTIVATE pia = (LPNMITEMACTIVATE)pnm;
			if (pia->iItem < 0)
				break;

			if (ListView_GetItemState(pnm->hwndFrom, pia->iItem, LVIS_FOCUSED) & LVIS_FOCUSED) {
				ListView_EditSubLabel(hwnd, pnm->idFrom, pia->iItem, pia->iSubItem);
			}
			else {
				ListView_SetItemState(pnm->hwndFrom, pia->iItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			}
			break;
		}
		}
		break;
	}

	return FALSE;
}
