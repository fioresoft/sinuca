// Ball.h: interface for the Ball class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BALL_H__22AFA235_0E27_11DA_A17D_000103DD18CD__INCLUDED_)
#define AFX_BALL_H__22AFA235_0E27_11DA_A17D_000103DD18CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef float ball_type;
typedef NO5TL::CVector<ball_type,2> Vector;
#define PI	3.141593f
#define EPSILON 0.1

class CBall  
{
public:
	Vector m_p;
	Vector m_v;
	Vector m_a;
	Vector m_cue;
	ball_type m_r;
	bool m_gone;
	static int m_friction;	// %
	static int m_wall_friction;	// %
	int m_id;
	bool m_drawcue;
	bool m_ghost;
	Vector m_v1_ghost, m_v2_ghost;
	bool m_bDrawGhostVel;
private:
	CRect m_box;	// ( x1,y1,x2,y2)
	CBitmap m_bmp;
	CBitmap m_bmpMask;
	HWND m_hWnd;
public:
	CBall();
	virtual ~CBall();
	void SetBox(const CRect &r);
	bool Bounce(void);
	void Move(UINT dt);
	void create(int id,int r,COLORREF clr,HWND hWnd);
	void draw(HDC hdc);
	void erase(HDC hdc);
	void GetRect(CRect &r) const;
	bool is_shock(const CBall &b2) const;
	void do_shock(CBall &b2);
	void draw_arrow(HDC hdc,bool bErase);
	void draw_cue(HDC hdc, int angle);
	bool is_point_over(ball_type x, ball_type y) const;
	void unoverllap(CBall& other);
	CBall& operator=(const CBall& rhs);
	bool will_hit(CBall& rhs);
	void draw_ghost_vel(HDC hdc);
};

extern CBall g_ghost;

#endif // !defined(AFX_BALL_H__22AFA235_0E27_11DA_A17D_000103DD18CD__INCLUDED_)
