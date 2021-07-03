// SuccD.cpp: 实现文件
//

#include "pch.h"
#include "EasiNotePacker.h"
#include "SuccD.h"
#include "afxdialogex.h"


// SuccD 对话框

IMPLEMENT_DYNAMIC(SuccD, CDialogEx)

SuccD::SuccD(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SUCC, pParent)
{

}

SuccD::~SuccD()
{
}

void SuccD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SuccD, CDialogEx)
	ON_BN_CLICKED(IDC_OF, &SuccD::OnBnClickedOf)
	ON_BN_CLICKED(IDC_CL, &SuccD::OnBnClickedCl)
END_MESSAGE_MAP()


// SuccD 消息处理程序


void SuccD::OnBnClickedOf()
{
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
	EndDialog(0);
}


void SuccD::OnBnClickedCl()
{
	EndDialog(0);
}
