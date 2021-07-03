#pragma once


// PackerHelp 对话框

class PackerHelp : public CDialog
{
	DECLARE_DYNAMIC(PackerHelp)

public:
	PackerHelp(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PackerHelp();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HELP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	virtual BOOL OnInitDialog();
	void a(char *str);
};
