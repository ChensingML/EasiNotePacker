// PackerHelp.cpp: 实现文件
//

#include "pch.h"
#include "EasiNotePacker.h"
#include "PackerHelp.h"
#include "afxdialogex.h"


// PackerHelp 对话框

#define A(x) 

CEdit c;

IMPLEMENT_DYNAMIC(PackerHelp, CDialog)

PackerHelp::PackerHelp(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_HELP, pParent)
{

}

PackerHelp::~PackerHelp()
{
}

void PackerHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HD, c);
}


BEGIN_MESSAGE_MAP(PackerHelp, CDialog)
END_MESSAGE_MAP()


// PackerHelp 消息处理程序


void PackerHelp::OnOK()
{
}

void PackerHelp::a(char* str) {
	CString strc;
	c.GetWindowTextW(strc);
	c.SetWindowTextW(strc + "\r\n" + str);
}


BOOL PackerHelp::OnInitDialog()
{
	CDialog::OnInitDialog();

	//c.SetWindowTextW(_T("s\r\nl"));
	a("一个不太正经的帮助");
	a("Q：这玩意怎么用");
	a("A：搜索、选课件、获取、选文件夹，然后你就能看到你选的一堆enbx的导出文件了~");
	a("");
	a("Q：所以这东西有什么实际作用");
	a("A：As we know，希沃白班5在打开课件的时候本地都会留一个缓存，嗯，你懂得，不用多说了~");
	a("");
	a("Q：这什么破软件，我要拷打作者！");
	a("A：QQ1052470899 随时奉陪~");
	a("");
	a("Q：是男人就开源");
	a("A：https://github.com/ChensingML/EasiNotePacker (doge)");

	return TRUE;
}
