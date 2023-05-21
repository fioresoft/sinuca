// Ball.cpp: implementation of the Ball class.
//
//////////////////////////////////////////////////////////////////////

#include <cmath>
#include "stdafx.h"
#include "resource.h"
#include "Ball.h"
#define CUE_LEN 500

CBall g_ghost;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBall::CBall()
{
	m_gone = false;
	m_id = -1;
	m_drawcue = false; 
	m_ghost = false;
	m_hWnd = NULL;
	m_bDrawGhostVel = false;
}

CBall::~CBall()
{

}

CBall& CBall::operator=(const CBall& rhs)
{
	m_p = rhs.m_p;
	m_v = rhs.m_v;
	m_a = rhs.m_a;
	m_cue = rhs.m_cue;
	m_r = rhs.m_r;
	m_ghost = rhs.m_ghost;
	m_bDrawGhostVel = rhs.m_bDrawGhostVel;
			
	return *this;
}

bool CBall::Bounce(void)
{
	bool bounce = false;

	if(m_p.x - m_r < m_box.left){
		m_p.x = m_box.left + m_r;
		m_v.x = - m_v.x;
		bounce = true;
	}
	if(m_p.x + m_r > m_box.right){
		m_p.x = m_box.right - m_r;
		m_v.x = - m_v.x;
		bounce = true;
	}
	if(m_p.y - m_r < m_box.top){
		m_p.y = m_box.top + m_r;
		m_v.y = -m_v.y;
		bounce = true;
	}
	if(m_p.y + m_r > m_box.bottom){
		m_p.y = m_box.bottom - m_r;
		m_v.y = - m_v.y;
		bounce = true;
	}
	if(bounce){
		m_v *= ( (100 - m_wall_friction) / 100.f );
	}
	return bounce;
}

void CBall::Move(UINT dt)
{
	m_v.x = (ball_type)(m_v.x + m_a.x * (dt / 1000.0));
	m_v.y = (ball_type)(m_v.y + m_a.y * (dt / 1000.0));
	m_v  *= (( 100 - (m_friction * dt / 1000.f)) / 100.0f);
	m_p.x = (ball_type)(m_p.x + m_v.x * (dt / 1000.0));
	m_p.y = (ball_type)(m_p.y + m_v.y * (dt / 1000.0));
	Bounce();

	if(fabs(m_v.x) < m_r * 2)
		m_v.x = 0;
	if(fabs(m_v.y) < m_r * 2)
		m_v.y = 0;
}

void CBall::SetBox(const CRect &r)
{
	m_box = r;
	Bounce();
}

void CBall::create(int id,COLORREF clr,HWND hWnd)
{
	CClientDC dc(hWnd);
	CDC dcmem;
	HBRUSH hbr;
	CBrush br;
	HPEN hpen;
	HBITMAP hBmp1,hBmp2;

	m_hWnd = hWnd;
	m_id = id;

	if(!m_bmp.IsNull())
		m_bmp.DeleteObject();
	if (!br.IsNull())
		br.DeleteObject();

	dcmem.CreateCompatibleDC(dc);
	ATLASSERT(!dcmem.IsNull());
	m_bmp.CreateCompatibleBitmap(dc,2 * m_r,2 * m_r);
	hBmp1 = dcmem.SelectBitmap(m_bmp);
	// draws a black rectangle with black borders
	hpen = dcmem.SelectPen((HPEN)GetStockObject(BLACK_PEN));
	hbr = dcmem.SelectBrush((HBRUSH)GetStockObject(BLACK_BRUSH));
	dcmem.Rectangle(0,0,2* m_r,2* m_r);
	dcmem.SelectBrush(hbr);
	dcmem.SelectPen(hpen);
	// draws a circle with black pen and clr brush
	br.CreateSolidBrush(clr);
	hbr = dcmem.SelectBrush(br);
	hpen = dcmem.SelectPen((HPEN)GetStockObject(BLACK_PEN));
	dcmem.Ellipse(0,0,2 * m_r - 1,2 * m_r - 1);
	dcmem.SelectBrush(hbr);
	dcmem.SelectPen(hpen);

	// create mask bitmap
	if (!m_bmpMask.IsNull())
		m_bmpMask.DeleteObject();
	m_bmpMask.CreateCompatibleBitmap(dc,2 * m_r,2 * m_r);
	hBmp2 = dcmem.SelectBitmap(m_bmpMask);
	hpen = dcmem.SelectPen((HPEN)GetStockObject(WHITE_PEN));
	hbr = dcmem.SelectBrush((HBRUSH)GetStockObject(WHITE_BRUSH));
	dcmem.Rectangle(0,0, 2 * m_r, 2 * m_r);
	dcmem.SelectPen((HPEN)GetStockObject(BLACK_PEN));
	dcmem.SelectBrush((HBRUSH)GetStockObject(BLACK_BRUSH));
	dcmem.Ellipse(0,0,2 * m_r - 1, 2 * m_r - 1);
	dcmem.SelectBrush(hbr);
	dcmem.SelectPen(hpen);
	dcmem.SelectBitmap(hBmp1);
}

void CBall::draw(HDC hdc)
{
	CDC dcmem;
	CDCHandle dc(hdc);
	
	if (m_gone)
		return;

	if (m_ghost) {
		return;
	}

	if(dc.IsNull())
		return;

	dcmem.CreateCompatibleDC(dc);
	if (dcmem.IsNull()) {
		ATLTRACE(_T("error %08x\n"), ::GetLastError());
		return;
	}
	dcmem.SelectBitmap(m_bmpMask);
	dc.BitBlt((LONG)(m_p.x - m_r),(LONG)(m_p.y - m_r),LONG(2 * m_r),LONG(2 * m_r),dcmem,0,0,SRCAND);
	dcmem.SelectBitmap(m_bmp);
	dc.BitBlt(LONG(m_p.x - m_r),LONG(m_p.y - m_r),LONG(2 * m_r),LONG(2 * m_r),dcmem,0,0,SRCPAINT);
}

void CBall::erase(HDC hdc)
{
	CRect r;
	HPEN hpen;
	HBRUSH hbr;
	CDCHandle dc = hdc;

	r.left = (LONG)(m_p.x - m_r);
	r.top = (LONG)(m_p.y - m_r);
	r.right = (LONG)(m_p.x + m_r);
	r.bottom = (LONG)(m_p.y + m_r);

	hbr = dc.SelectBrush((HBRUSH)GetStockObject(WHITE_BRUSH));
	hpen = dc.SelectPen((HPEN)GetStockObject(WHITE_PEN));
	dc.Rectangle(&r);
	dc.SelectBrush(hbr);
	dc.SelectPen(hpen);
}

void CBall::GetRect(CRect &r) const
{
	r.left = (LONG) (m_p.x - m_r);
	r.top = (LONG) (m_p.y - m_r);
	r.right = (LONG) (m_p.x + m_r);
	r.bottom = (LONG) (m_p.y + m_r);
}

bool CBall::is_shock(const CBall &b2) const
{
	double xx = pow( ((double)m_p.x - b2.m_p.x),2);
	double yy = pow( ((double)m_p.y - b2.m_p.y),2);
	double d = sqrt(xx + yy);

	bool shock;
	shock = d < m_r + b2.m_r;
	return shock;
}

void CBall::do_shock(CBall &b2)
{
	Vector r = b2.m_p - m_p;
	Vector v1r,v1n;
	Vector v2r,v2n;
	Vector tmp;

	v1r = m_v.proj(r);
	v1n = m_v - v1r;
	v2r = b2.m_v.proj(r);
	v2n = b2.m_v - v2r;
	tmp = v1r;
	v1r = v2r;
	v2r = tmp;

	if (!b2.m_ghost) {
		m_v = (v1r + v1n);// *m_ball_elasticity;
		b2.m_v = (v2r + v2n);// *m_ball_elasticity;

		//////
		if (r.modulo() < m_r + b2.m_r) {
			Move(350);
		}
		//////
	}
	else {
		m_v1_ghost = v1r + v1n;
		m_v2_ghost = v2r + v2n;
		m_v1_ghost.normalize();
		m_v2_ghost.normalize();
		m_v1_ghost *=  (m_r * 2);
		m_v2_ghost *=  (m_r * 2);
	}
}

void CBall::draw_arrow(HDC hdc,bool bErase)
{
	CDCHandle dc = hdc;
	int rop;

	dc.SaveDC();
	dc.SetViewportOrg((int)m_p.x,(int)m_p.y);
	dc.MoveTo(0,0);
	if(bErase)
		rop = dc.SetROP2(R2_NOP);
	dc.LineTo((int)m_v.x,(int)m_v.y);
	if(bErase)
		dc.SetROP2(rop);
	dc.RestoreDC(-1);
}

void CBall::draw_ghost_vel(HDC hdc)
{
	CDCHandle dc = hdc;
	CPoint pto;
	CPen pen = (HPEN)GetStockObject(BLACK_PEN);
	CPoint pt(g_ghost.m_p.x, g_ghost.m_p.y);
	
	//dc.SaveDC();
	dc.SelectPen(pen);
	dc.MoveTo(pt.x, pt.y);
	dc.LineTo(pt.x + g_ghost.m_v1_ghost.x,pt.y+ g_ghost.m_v1_ghost.y);
	dc.MoveTo(pt.x, pt.y);
	dc.LineTo(pt.x+ g_ghost.m_v2_ghost.x, pt.y+ g_ghost.m_v2_ghost.y);
	
	//dc.RestoreDC(-1);
}

void CBall::draw_cue(HDC hdc,int angle)
{
	CDCHandle dc(hdc);
	CPoint p1,p2;
	CPoint pt;
	float angle_rad = angle * PI / 180;
	static CPen pen;

	if (pen.IsNull()) {
		pen.CreatePen((int)PS_DASHDOT, (int)0, (COLORREF)0);
	}

	//dc.SaveDC();
	p1.x = m_p.x + m_r  * cos(angle_rad);
	p1.y = m_p.y - m_r * sin(angle_rad);
	p2.x = m_p.x + (m_r + CUE_LEN) * cos(angle_rad);
	p2.y = m_p.y - (m_r + CUE_LEN) * sin(angle_rad);
	
	HPEN hPen = dc.SelectPen(pen);
	dc.MoveTo(p1, &pt);
	dc.LineTo(p2);
	dc.MoveTo(pt, &pt);
	dc.SelectPen(hPen);

	// store the vector
	m_cue.from_points(p1.x, p1.y, p2.x, p2.y);
	m_cue.normalize();
	//dc.RestoreDC(-1);
}

bool CBall::is_point_over(ball_type x, ball_type y) const
{
	ball_type distance_from_centre = sqrtf(pow(x - m_p.x,2.0) + pow(y - m_p.y,2.0));
	return distance_from_centre <= m_r;
}

void CBall::unoverllap(CBall& other)
{
	/*do{
		m_p.x += EPSILON;
		m_p.y += EPSILON;
	} while (is_shock(other));*/
}

bool CBall::will_hit(CBall& other)
{

	bool res = false;
#if 0
	CWindow wnd = ::GetDlgItem(GetParent(m_hWnd), IDC_EDIT1);
	CString s;

	wnd.GetWindowText(s);

	g_ghost.m_ghost = true;
	g_ghost.create(- 1,10, 0, m_hWnd);
	g_ghost.m_p = m_p;
	g_ghost.m_v = m_cue * _wtoi(LPCTSTR(s));
	g_ghost.m_cue = m_cue;
	g_ghost.m_r = m_r;
	g_ghost.m_box = m_box;
	g_ghost.m_bDrawGhostVel = m_bDrawGhostVel;
	

	while (!g_ghost.Bounce() && !(g_ghost.m_v.modulo() == 0)) {
		g_ghost.Move(200);
		if (res = g_ghost.is_shock(other)) {
			other.do_shock(g_ghost);
			g_ghost.m_v1_ghost = other.m_v1_ghost;
			g_ghost.m_v2_ghost = other.m_v2_ghost;
			g_ghost.m_p.x = other.m_p.x;
			g_ghost.m_p.y = other.m_p.y;
			g_ghost.m_bDrawGhostVel = true;// other.m_bDrawGhostVel;
			break;
		}
	}
#endif
	
	return res;
}