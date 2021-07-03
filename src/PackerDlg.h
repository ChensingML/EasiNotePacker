
// PackerDlg.h: 头文件
//

#pragma once

#include <vector>
#include "Lesson.h"

using namespace std;


// CPackerDlg 对话框
class CPackerDlg : public CDialogEx
{
// 构造
public:
	CPackerDlg(CWnd* pParent = nullptr);	// 标准构造函数
	CString SelectFolderPath();
	vector<Lesson*>* allLessons=NULL, * showedLessons=NULL;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PACKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void Search(CString s);
	afx_msg void OnSpb();
	afx_msg void OnMenuCos();
	afx_msg void OnMenuNormal();
	virtual void OnOK();
	afx_msg void OnHelpb();
//	afx_msg void OnChooseList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedGet();
	afx_msg void OnEnChangeFif();
//	afx_msg void OnUpdateFif();
	CEdit fif;
	void updateL();
	afx_msg void OnBnClickedSa();
	afx_msg void OnBnClickedNsa();
};
