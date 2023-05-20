#include "stdafx.h"
#include "resource.h"
#include "COptionsDlg.h"

LRESULT COptionsDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    //m_list.ModifyStyle(0, LBS_OWNERDRAWVARIABLE,  LBS_OWNERDRAWVARIABLE);
	m_list.SubclassWindow(GetDlgItem(IDC_LIST1));
    m_list.SetExtendedListStyle(PLS_EX_CATEGORIZED);
    ///*HPROPERTY hApperance = m_list.AddItem(PropCreateCategory(_T("Apperance")));
    //m_list.AddItem(PropC*/reateSimple(_T("dummy"), true, 3));
    HPROPERTY hBehaviour = m_list.AddItem(PropCreateCategory(_T("Behaviour"),1234));
    m_hWallFriction = m_list.AddItem(PropCreateSimple(_T("wall_friction"), m_wall_friction,1));
    m_hFriction = m_list.AddItem(PropCreateSimple(_T("friction"), m_friction,2));
    
    CenterWindow(GetParent());
    bHandled = FALSE;
    
    return 0;
}

LRESULT COptionsDlg::OnOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CComVariant v;
    m_hFriction->GetValue(&v);
    m_friction = v.lVal;
    m_hWallFriction->GetValue(&v);
    m_wall_friction = v.lVal;
    if (m_friction < 0 || m_friction > 100 || m_wall_friction < 0 || m_wall_friction > 100) {
        MessageBox(_T("% values must be between 0 and 100"));
        return 0;
    }
    EndDialog(IDOK);
    return 0;
}

LRESULT COptionsDlg::OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(IDOK);
    return 0;
}