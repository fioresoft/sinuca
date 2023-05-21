#pragma once
#include "viksoe/PropertyList.h"
class COptionsDlg :
    public CDialogImpl<COptionsDlg>
{
public:
    LONG m_wall_friction;
    LONG m_friction;
    ball_type m_r;
    ball_type m_HoleRadius;
    HPROPERTY m_hr;
    HPROPERTY m_hHoleRadius;
    HPROPERTY m_hWallFriction;
    HPROPERTY m_hFriction;
    CPropertyListCtrl m_list;
public:
    enum { IDD = IDD_OPTIONS};
public:
    BEGIN_MSG_MAP(COptionsDlg)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        COMMAND_ID_HANDLER(IDOK,OnOk)
        COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
        REFLECT_NOTIFICATIONS()
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
    LRESULT OnOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
    LRESULT OnCancel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

