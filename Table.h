// Table.h: interface for the CTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABLE_H__EAF4A1D6_1144_11DA_A17D_000103DD18CD__INCLUDED_)
#define AFX_TABLE_H__EAF4A1D6_1144_11DA_A17D_000103DD18CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


enum GAME {
	GAME_RANDOM,
	GAME_SINUCA_BRASILEIRA,
};

class CBall;
class CHole;


/////////////////////////////////////////////////////////////////////////////
// COffscreenDraw
class CClientRect : public CRect
{
public:
	CClientRect(HWND hWnd)
	{
		CWindow wnd(hWnd);
		wnd.GetClientRect((LPRECT)this);
	}
};

// thanks to http://www.viksoe.dk/code/index.htm for this class
// modified to use WTL's CMemoryDC
// To use it, derive from it and chain it in the message map.
template< class T >
class COffscreenDraw
{
public:
	BEGIN_MSG_MAP(COffscreenDraw)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
	END_MSG_MAP()

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		T* pT = static_cast<T*>(this);
		if (wParam != NULL)
		{
			CClientRect rc(pT->m_hWnd);
			CMemoryDC memdc((HDC)wParam, rc);
			pT->DoPaint(memdc.m_hDC);
		}
		else
		{
			RECT rc;
			pT->GetClientRect(&rc);
			CPaintDC dc(pT->m_hWnd);
			CMemoryDC memdc(dc.m_hDC, rc);
			pT->DoPaint(memdc.m_hDC);
		}
		return 0;
	}
	LRESULT OnEraseBackground(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 1; // handled; no need to erase background; do it in DoPaint();
	}
	void DoPaint(CDCHandle dc)
	{
		ATLASSERT(false); // must override this
	}
};


class CTable : public CWindowImpl<CTable>,public COffscreenDraw<CTable>
{
public:
	CTable();
	virtual ~CTable();

	DECLARE_WND_CLASS(_T("TableWndClass"))

	BEGIN_MSG_MAP(CTable)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		//MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLeftButtonDown)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
		MESSAGE_HANDLER(WM_GETDLGCODE, OnGetDlgCode)
		COMMAND_ID_HANDLER(IDC_BUTTON1, OnButton1)
		COMMAND_ID_HANDLER(IDC_BUTTON2, OnButton2)
		COMMAND_ID_HANDLER(IDC_BUTTON3, OnButton3)
		COMMAND_ID_HANDLER(IDC_BUTTON4, OnStartSinuca)
		COMMAND_ID_HANDLER(IDC_BUTTON5, OnStartRandom)
		CHAIN_MSG_MAP(COffscreenDraw<CTable>)
	END_MSG_MAP()

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetDlgCode(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnButton3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnStartSinuca(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnStartRandom(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void DoPaint(CDCHandle dc);
	
private:
	const int m_cBalls;
	const int m_cHoles;
	const int m_dt;
	CBall *m_balls;
	CHole *m_holes;
	int m_alfa;
	GAME m_game_type;
	bool m_bInit;

	void InitBalls(const CRect &r);
	void InitBallsSinuca(const CRect& r);
	void InitHoles(const CRect &r);
	void DrawHoles(HDC hdc);
	bool BallsStopped() const;
	void DrawCue(HDC hdc);
	bool IsInAim(CBall* b1, CBall* b2);
	void SetGameType(GAME game_type);
	void EnsureNotOverllaping();
};

#endif // !defined(AFX_TABLE_H__EAF4A1D6_1144_11DA_A17D_000103DD18CD__INCLUDED_)
