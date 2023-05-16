// maindlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"
#include "table.h"
#include "maindlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	/*if (pMsg->message == WM_KEYUP || pMsg->message == WM_KEYDOWN) {
		m_table.PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return FALSE;
	}*/
	return IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	{
		CWindow wnd = GetDlgItem(IDC_TABLE);
		CRect r;
		wnd.GetWindowRect(&r);
		wnd.ShowWindow(SW_HIDE);
		ScreenToClient(&r);
		m_table.Create(m_hWnd,rcDefault,(LPCTSTR)0,WS_TABSTOP|WS_VISIBLE|WS_CHILD);
		m_table.MoveWindow(&r);
	}
	CWindow edit = GetDlgItem(IDC_EDIT1);
	edit.SetWindowTextW(L"500");
	m_spin = GetDlgItem(IDC_SPIN1);
	m_spin.SetRange32(0, 500);


	return TRUE;
}

LRESULT CMainDlg::OnKeyDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == 0x48/*h*/) {
		m_table.PostMessage(WM_KEYDOWN, wParam, lParam);
		bHandled = TRUE;
	}
	else
		bHandled = FALSE;
	return 0;
}

LRESULT CMainDlg::OnKeyUp(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == 0x48/*h*/) {
		m_table.PostMessage(WM_KEYUP, wParam, lParam);
		bHandled = TRUE;
	}
	else
		bHandled = FALSE;
	return 0;
}

LRESULT CMainDlg::OnGetDlgCode(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTALLKEYS;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}
