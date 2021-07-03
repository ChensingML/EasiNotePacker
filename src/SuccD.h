#pragma once


// SuccD 对话框

class SuccD : public CDialogEx
{
	DECLARE_DYNAMIC(SuccD)

public:
	CString path;
	SuccD(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~SuccD();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SUCC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOf();
	afx_msg void OnBnClickedCl();
};
