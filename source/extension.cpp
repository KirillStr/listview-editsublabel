#include "extension.h"
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>


typedef struct tag_listviewedithdr
{
	HWND hwnd;
	int iItem;
	int iSubItem;
	BOOL bEscape;
	WNDPROC defProc;
} LISTVIEWEDITHDR;

LRESULT CALLBACK EditWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LISTVIEWEDITHDR* pHdr = (LISTVIEWEDITHDR*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch (message)
	{
	case WM_GETDLGCODE:
		return DLGC_WANTALLKEYS;
		//| CallWindowProc(pHdr->defProc, hwnd, message, wParam, lParam);

	//case WM_CHAR:
	case WM_KEYDOWN:
	{
		if (wParam == VK_ESCAPE || wParam == VK_RETURN || wParam == VK_TAB) {
			pHdr->bEscape = BOOL(wParam == VK_ESCAPE);
			SetFocus(GetParent(hwnd));
			return 0L;
		}
		break;
	}
	case WM_KILLFOCUS:
	{
		TCHAR pch[260];
		Edit_GetText(hwnd, pch, 260);

		if (!pHdr->bEscape) {
			LV_DISPINFO di = { 0 };
			di.hdr.hwndFrom = pHdr->hwnd;
			di.hdr.idFrom = GetDlgCtrlID(pHdr->hwnd);
			di.hdr.code = LVN_ENDLABELEDIT;
			di.item.mask = LVIF_TEXT;
			di.item.iItem = pHdr->iItem;
			di.item.iSubItem = pHdr->iSubItem;
			di.item.pszText = NULL;
			di.item.pszText = pch;
			di.item.cchTextMax = 260;
			SendMessage(GetParent(pHdr->hwnd), WM_NOTIFY,
				(WPARAM)GetDlgCtrlID(pHdr->hwnd), (LPARAM)&di);
		}

		DestroyWindow(hwnd);

		delete pHdr;
		return 0L;
	}
	}

	return CallWindowProc(pHdr->defProc, hwnd, message, wParam, lParam);
}

BOOL ListView_EditSubLabel(HWND hwnd, int id, int iItem, int iSubItem)
{
	HWND ctrl = GetDlgItem(hwnd, id);
	// cell rect
	RECT rt;
	ListView_GetSubItemRect(ctrl, iItem, iSubItem, LVIR_LABEL, &rt);
	InflateRect(&rt, 2, 2);
	// text alignment
	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	ListView_GetColumn(ctrl, iSubItem, &lvcol);
	DWORD dwAlign;
	if ((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT) {
		dwAlign = ES_LEFT;
	}
	else if ((lvcol.fmt & LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT) {
		dwAlign = ES_RIGHT;
	}
	else {
		dwAlign = ES_CENTER;
	}
	// edit text
	TCHAR txt[260];
	ListView_GetItemText(ctrl, iItem, iSubItem, txt, 260);
	// crete edit controll
	HWND edit = CreateWindow(
		WC_EDIT,
		txt,
		WS_BORDER | WS_CHILD | WS_VISIBLE | dwAlign,
		rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top,
		ctrl, NULL, GetModuleHandle(NULL), NULL);
	if (edit == NULL)
		return FALSE; // Could not create edit box

	SetWindowFont(edit, GetWindowFont(ctrl), TRUE);
	SetFocus(edit);
	Edit_SetSel(edit, 0, -1);
	
	LISTVIEWEDITHDR* phdr = new LISTVIEWEDITHDR;
	phdr->hwnd = ctrl;
	phdr->iItem = iItem;
	phdr->iSubItem = iSubItem;
	phdr->bEscape = FALSE;
	phdr->defProc = (WNDPROC)GetWindowLongPtr(edit, GWLP_WNDPROC);
	SetWindowLongPtr(edit, GWLP_USERDATA, (LONG_PTR)phdr);

	LONG w = SetWindowLongPtr(edit, GWLP_WNDPROC, (LONG_PTR)EditWndProc);

	return TRUE;
}
