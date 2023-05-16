// Table.cpp: implementation of the CTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "./NO5TL/Color.h"
#include "Table.h"
#include "ball.h"

enum {
	BALL_WHITE = 0,
	BALL_RED,
	BALL_YELLOW,
	BALL_GREEN,
	BALL_BROWN,
	BALL_BLUE,
	BALL_PINK,
	BALL_BLACK,
};

class CHole
{
public:
	int m_hole;
	Vector m_c;
	static LONG m_r;
	void draw(HDC hdc);
	bool is_ball_inside(const CBall &b);
};
LONG CHole::m_r = 0;
int CBall::m_friction = 10;	// 5
int CBall::m_wall_friction = 20; // 15


	


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTable::CTable():m_cBalls(8),m_dt(100),m_cHoles(6)
{
	CWndClassInfo &ci = GetWndClassInfo();
	static CBrush br;

	br.CreateSolidBrush(0x00aa00);
	ci.m_wc.hbrBackground = HBRUSH(br);

	m_balls = new CBall[m_cBalls];
	m_holes = new CHole[m_cHoles];
	m_alfa = 0;
	m_bInit = true;
}

CTable::~CTable()
{
	delete[] m_balls;
	delete[] m_holes;
}

void CTable::SetGameType(GAME game_type)
{
	CClientRect r(m_hWnd);

	m_game_type = game_type;
	switch (m_game_type) {
	case GAME_RANDOM:
		InitBalls(r);
		break;
	case GAME_SINUCA_BRASILEIRA:
		InitBallsSinuca(r);
		m_bInit = true;
		break;
	default:
		ATLASSERT(0);
		break;
	}

}


LRESULT CTable::OnCreate(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
{
	this->SetFocus();
	return 0;
}

LRESULT CTable::OnSize(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
{
	CClientRect r(m_hWnd);
	static bool bInit = true;

	if(bInit && r.Width() > 0){
		bInit = false;
		InitHoles(r);
	}
	else{
		for(int i=0;i<m_cBalls;i++){
			m_balls[i].SetBox(r);
		}
	}
	return 0;
}

LRESULT CTable::OnPaint(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
{
	CPaintDC dc(m_hWnd);
	CClientRect r(m_hWnd);
		
	for(int i=0;i<m_cBalls;i++){
		if(!m_balls[i].m_gone){
			m_balls[i].draw(dc);
			//m_balls[i].draw_arrow(dc,false);
			if (m_balls[i].m_drawcue)
				m_balls[i].draw_cue(dc,m_alfa);
		}
	}
	DrawHoles(dc);
	return 0;
}

void CTable::DoPaint(CDCHandle dc)
{
	static CBrush br;
	CClientRect rc(m_hWnd);

	if(br.IsNull())
		br.CreateSolidBrush(0x00aa00);
	HBRUSH hbr = dc.SelectBrush(br);
	dc.Rectangle(rc);
	dc.SelectBrush(hbr);
	for (int i = 0; i < m_cBalls; i++) {
		if (!m_balls[i].m_gone) {
			m_balls[i].draw(dc);
			//m_balls[i].draw_arrow(dc,false);
			if (m_balls[i].m_drawcue)
				m_balls[i].draw_cue(dc, m_alfa);
		}
	}
	DrawHoles(dc);
	//if(g_ghost.m_bDrawGhostVel)
		g_ghost.draw_ghost_vel(dc);
}

LRESULT CTable::OnTimer(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
{
	CClientDC dc(m_hWnd);
	CRect r;

	for(int i=0;i<m_cBalls;i++){
		if(!m_balls[i].m_gone){
			// erase ball
			m_balls[i].GetRect(r);
			InvalidateRect(&r,TRUE);
			//m_balls[i].draw_arrow(dc,true);
			// update balls position
			m_balls[i].Move(m_dt);

			// do chocks
			/*
			for(int j=0;j<m_cBalls;j++){
				for(int k=j+1;k<m_cBalls;k++){
					if(m_balls[j].is_shock(m_balls[k]))
						m_balls[j].do_shock(m_balls[k]);
				}
			}
			*/
			for(int j=0;j<m_cBalls;j++){
				if(i != j && !m_balls[j].m_gone){
					if (m_balls[i].is_shock(m_balls[j])) {
						m_balls[i].do_shock(m_balls[j]);
						EnsureNotOverllaping();
					}
				}
			}
			// check if it was sucked up by the hole
			for(int m=0;m<m_cHoles;m++){
				if (m_holes[m].is_ball_inside(m_balls[i])) {
					m_balls[i].m_gone = true;
				}
				/*else if (BallsStopped()) {
					if (m_game_type == GAME_SINUCA_BRASILEIRA && m_pGame) {
						m_pGame->ball_out_hole(m_balls[i].m_id);
					}
				}*/
			}

			// draw ball
			m_balls[i].GetRect(r);
			InvalidateRect(&r,m_balls[i].m_gone ? TRUE : FALSE);
			//InvalidateRect(&r,TRUE);
		}
		CWindow wnd = GetParent();
		if (BallsStopped()) {
			wnd.SetWindowText(_T("stopped."));
			InvalidateRect(NULL);
		}
		else {
			wnd.SetWindowText(_T("moving..."));
		}		
	}
	UpdateWindow();
	return 0;
}

LRESULT CTable::OnLeftButtonDown(UINT uMsg,WPARAM wParam,LPARAM lParam,BOOL &bHandled)
{
	CPoint ptCursor(lParam);

	if (true/*BallsStopped()*/) {
		for (int i = 0; i < m_cBalls; i++) {
			if (m_balls[i].is_point_over(ptCursor.x, ptCursor.y)) {
					m_balls[i].m_drawcue = true;
					m_alfa = 0;
			}
			else
				m_balls[i].m_drawcue = false;
		}
	}
	InvalidateRect(NULL);
	GetFocus();
	
	return 0;
}

LRESULT CTable::OnKeyDown(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == VK_LEFT || wParam == VK_RIGHT) {
		CClientDC dc(m_hWnd);
		wParam == VK_LEFT ? m_alfa += 1 : m_alfa -= 1;
		DrawCue(dc);
		return 0;
	}
	else
		bHandled = FALSE;
	return 0;
}

LRESULT CTable::OnKeyUp(UINT, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == 0x48/*h*/) {
		bHandled = TRUE;
		CWindow edit = ::GetDlgItem(GetParent(), IDC_EDIT1);
		CString s;

		edit.GetWindowText(s);
		for (int i = 0; i < m_cBalls; i++) {
			if (m_balls[i].m_cue.modulo() != 0) {
				m_balls[i].m_v = m_balls[i].m_cue * _wtoi(LPCTSTR(s));	
				m_balls[i].m_cue *= 0;
				m_balls[i].m_drawcue = false;
				InvalidateRect(NULL);
				break;
			}
		}
	}
	else
		bHandled = FALSE;
	return 0;
}

LRESULT CTable::OnGetDlgCode(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return DLGC_WANTALLKEYS;
}


LRESULT CTable::OnButton1(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CClientDC dc(m_hWnd);
	int old_j = -1;
	m_alfa += 5; 
	DrawCue(dc);
	for (int i = 0; i < m_cBalls; i++) {
		if (m_balls[i].m_cue.modulo() != 0) {
			for (int j = 0; j < m_cBalls; j++) {
				if (i == j)
					continue;
				if (m_balls[i].will_hit(m_balls[j])) {
					//g_ghost.draw_ghost_vel(dc);
					if (old_j >= 0)
						m_balls[old_j].m_bDrawGhostVel = false;
					m_balls[j].m_bDrawGhostVel = true;
					old_j = j;
					break;
				}
			}
			break;
		}
	}
	return 0;
}

LRESULT CTable::OnButton2(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CClientDC dc(m_hWnd);
	m_alfa -= 5;
	DrawCue(dc);
	return 0;
}

LRESULT CTable::OnButton3(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CWindow edit = ::GetDlgItem(GetParent(), IDC_EDIT1);
	CString s;

	edit.GetWindowText(s);
	for (int i = 0; i < m_cBalls; i++) {
		if (m_balls[i].m_cue.modulo() != 0) {
			m_balls[i].m_v = m_balls[i].m_cue * _wtoi(LPCTSTR(s));	
			m_balls[i].m_cue *= 0;
			m_balls[i].m_drawcue = false;
			InvalidateRect(NULL);
			m_bInit = false;
			break;
		}
	}
	return 0;
}

LRESULT CTable::OnStartSinuca(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SetGameType(GAME_SINUCA_BRASILEIRA);
	return 0;
}

LRESULT CTable::OnStartRandom(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	SetGameType(GAME_RANDOM);
	return 0;
}



void CTable::DrawCue(HDC hdc)
{
	CDCHandle dc(hdc);

	for (int i = 0; i < m_cBalls; i++) {
		if (m_balls[i].m_gone)
			continue;
		if (m_balls[i].m_drawcue) {
			m_balls[i].draw_cue(dc, m_alfa);
			for (int j = 0; j < m_cBalls; j++) {
				if (i == j || m_balls[j].m_gone)
					continue;
				if (IsInAim(&m_balls[i], &m_balls[j])) {
					// draws the versors of the velocities after a possible collision TODO
					// if there is no ball[j] in aim at a shorter distance
					PostMessage(WM_COMMAND, IDC_BUTTON3, IDC_BUTTON3);
					break;
				}
			}
			break;
		}
	}
}

void CTable::InitBalls(const CRect &r)
{
	NO5TL::CColor color;
	
	srand((unsigned int)GetTickCount64());
	
	for(int i=0;i<m_cBalls;i++){
		m_balls[i].create(i,10,color.Random(),m_hWnd);
		m_balls[i].SetBox(r);
		m_balls[i].m_p.x = (ball_type)20 + ( i + 1 ) * 30;
		m_balls[i].m_p.y = (ball_type)20 + ( i + 1 ) * 15;
		m_balls[i].m_v.x = ((ball_type)80 + (rand() % 30 )) * ( i % 2 ? 1 : -1 );
		m_balls[i].m_v.y = ((ball_type)80 + (rand() % 30 )) * ( i % 2 ? 1 : -1 );
		m_balls[i].m_a.y = (ball_type)0;
	}
	KillTimer(1);
	SetTimer(1,m_dt);
}



void CTable::InitBallsSinuca(const CRect& r)
{
	NO5TL::CColor colors[] = { NO5TL::Colors::WHITE,NO5TL::Colors::RED,NO5TL::Colors::YELLOW,
		NO5TL::Colors::GREEN,NO5TL::Colors::MARRON,NO5TL::Colors::BLUE,NO5TL::Colors::PINK,
		NO5TL::Colors::BLACK };
	const CPoint ptMiddle = CPoint(2 * r.Width() / 3, r.Height() / 2);
	const CPoint ptWhite = CPoint(r.Width() / 4, r.Height() / 2);

	ATLASSERT(m_cBalls == 8);
	for (int i = 0; i < 8; i++) {
		m_balls[i].create(BALL_WHITE + i, 10, colors[i], m_hWnd);
		m_balls[i].SetBox(r);
		m_balls[i].m_gone = false;
		
		if (i == 0) {
			m_balls[i].m_p.x = ptWhite.x;
			m_balls[i].m_p.y = ptWhite.y;
		}
		else if (i == 7) {
			m_balls[i].m_p.x = ptMiddle.x;
			m_balls[i].m_p.y = ptMiddle.y;
		}
		else if (i == 6) {
			m_balls[i].m_p.x = ptMiddle.x + m_balls[i].m_r * 2;
			m_balls[i].m_p.y = ptMiddle.y + m_balls[i].m_r;
		}
		else if (i == 5) {
			m_balls[i].m_p.x = ptMiddle.x + m_balls[i].m_r * 2;
			m_balls[i].m_p.y = ptMiddle.y - m_balls[i].m_r;
		}
		else if (i == 4) {
			m_balls[i].m_p.x = ptMiddle.x + m_balls[i].m_r * 4;
			m_balls[i].m_p.y = ptMiddle.y + m_balls[i].m_r;
		}
		else if (i == 3) {
			m_balls[i].m_p.x = ptMiddle.x + m_balls[i].m_r * 4;
			m_balls[i].m_p.y = ptMiddle.y - m_balls[i].m_r;
		}
		else if (i == 2) {
			m_balls[i].m_p.x = ptMiddle.x + m_balls[i].m_r * 4;
			m_balls[i].m_p.y = ptMiddle.y + m_balls[i].m_r * 3;
		}
		else if (i == 1) {
			m_balls[i].m_p.x = ptMiddle.x + m_balls[i].m_r * 4;
			m_balls[i].m_p.y = ptMiddle.y - m_balls[i].m_r * 3;
		}
	}
	KillTimer(1);
	SetTimer(1, m_dt);
}

void CTable::DrawHoles(HDC hdc)
{
	CDCHandle dc(hdc);
	int i;

	for(i=0;i<m_cHoles;i++)
		m_holes[i].draw(dc);

}

void CTable::InitHoles(const CRect &r)
{
	CHole::m_r = 25;
	int i,j,hole;
	LONG cx = r.Width() / 2;
	LONG cy = r.Height();

	for(hole=0,i=0;i<2;i++){
		for(j=0;j<3;j++,hole++){
			m_holes[hole].m_c.x = (ball_type)(cx * j);
			m_holes[hole].m_c.y = (ball_type)(cy * i);
			m_holes[hole].m_hole = hole;
		}
	}
}

bool CTable::BallsStopped() const
{
	bool res = true;
	for (int i = 0; i < m_cBalls; i++) {
		if (!m_balls[i].m_gone) {
			if (m_balls[i].m_v.modulo() != 0)
				res = false;
		}
	}
	return res;
}

bool CTable::IsInAim(CBall* b1, CBall* b2)
{
	/*CBall ghost = *b1;
	bool shock = false;

	ghost.m_v = b1->m_cue * 100;
	for (int i = 0; !ghost.Bounce() && shock == false; i++) {
		ghost.Move(1000);
		shock = ghost.is_shock(*b2);
	}
	return shock;*/
	// pra cada bola, ver se adiantando a b1 é um hit TODO
	return false;
}

void CTable::EnsureNotOverllaping()
{
	for (int i = 0; i < m_cBalls; i++) {
		for (int j = 0; j < m_cBalls; j++) {
			if (m_balls[i].m_gone || m_balls[j].m_gone || i == j)
				continue;
			if (m_balls[i].is_shock(m_balls[j])) {
				//m_balls[i].m_v *= 1.01;
				m_balls[i].unoverllap(m_balls[j]);
			}
		}
	}
}

void CHole::draw(HDC hdc)
{
	CRect r;
	CDCHandle dc(hdc);
	POINT pt;

	r.left = (LONG)m_c.x - m_r;
	r.top = (LONG)m_c.y - m_r;
	r.right = (LONG)m_c.x + m_r;
	r.bottom = (LONG)m_c.y + m_r;
	pt.x = (LONG) m_c.x;
	pt.y = (LONG) m_c.y;
	dc.Arc(&r,pt,pt);
}

bool CHole::is_ball_inside(const CBall &b)
{
	Vector r = b.m_p - m_c;
	LONG d = (LONG)r.modulo();
	return d < m_r - b.m_r;
}



