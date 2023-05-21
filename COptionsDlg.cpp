#include "stdafx.h"
#include "resource.h"
#include "Ball.h"
#include "COptionsDlg.h"

LRESULT COptionsDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    //m_list.ModifyStyle(0, LBS_OWNERDRAWVARIABLE,  LBS_OWNERDRAWVARIABLE);
	m_list.SubclassWindow(GetDlgItem(IDC_LIST1));
    m_list.SetExtendedListStyle(PLS_EX_CATEGORIZED);
    HPROPERTY hBehaviour = m_list.AddItem(PropCreateCategory(_T("Behaviour"),1234));
    m_hWallFriction = m_list.AddItem(PropCreateSimple(_T("wall_friction"), m_wall_friction,1));
    m_hFriction = m_list.AddItem(PropCreateSimple(_T("friction"), m_friction,2));
    HPROPERTY hApperance = m_list.AddItem(PropCreateCategory(_T("Apperance"),1235));
    CString s;
    int idx = 0;
    s.Format(_T("%d"),(int) m_r);
    LPCTSTR brs[] = { _T("5"),_T("10"),_T("15"),_T("20") ,NULL}; // ball radius
    LPCTSTR hrs[] = { _T("15"),_T("25"),_T("30"),_T("40"),NULL };
    for (idx = 0; idx < sizeof(brs) / sizeof(brs[0]) - 1; idx++) {
        if (!s.Compare(brs[idx]))
            break;
    }
    m_hr = m_list.AddItem(PropCreateList(_T("Ball radius"), brs, idx, 3));
    s.Format(_T("%d"),(int) m_HoleRadius);
    for (idx = 0; idx < sizeof(hrs) / sizeof(hrs[0]) - 1; idx++) {
        if (!s.Compare(hrs[idx]))
            break;
    }
    m_hHoleRadius = m_list.AddItem(PropCreateList(_T("Hole radius"), hrs, idx, 4));

    CenterWindow(GetParent());
    bHandled = FALSE;
    
    return 0;
}

LRESULT COptionsDlg::OnOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CComVariant v;
    TCHAR buf[10] = { 0 };
    m_hFriction->GetValue(&v);
    m_friction = v.lVal;
    m_hWallFriction->GetValue(&v);
    m_wall_friction = v.lVal;
    if (m_friction < 0 || m_friction > 100 || m_wall_friction < 0 || m_wall_friction > 100) {
        MessageBox(_T("% values must be between 0 and 100"));
        return 0;
    }
    m_hr->GetDisplayValue(buf, 10);
    m_r = _ttol(buf);
    m_hHoleRadius->GetDisplayValue(buf, 10);
    m_HoleRadius = _ttol(buf);
    EndDialog(IDOK);
    return 0;
}

LRESULT COptionsDlg::OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(IDOK);
    return 0;
}