#pragma once

#include "PackerDlg.h"

// LoadD 对话框

class LoadD : public CDialogEx
{
	DECLARE_DYNAMIC(LoadD)

public:
	CPackerDlg* dlgss;
	CProgressCtrl progbar;
	CStatic stat;
	CString path;
	int task;						//0-Search; 1-Save
	LoadD(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LoadD();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOADING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
