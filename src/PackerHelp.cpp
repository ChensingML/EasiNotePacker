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
	a("Q：没有课件？？");
	a("A：可能这台电脑上本来就没有课件，也可能是老师清除了缓存，更可能是电脑上安装了例如Deep Freeze（冰点）的还原软件，这类软件会在电脑重启后还原电脑，使用EasiNotePacker前最好卸载或禁用这类软件");
	a("");
	a("Q：为什么一直提示搜索不到（目录错误）");
	a("A：第一，软件的搜索目录是ADMIN1~，所以只能搜索第一个管理员账户的课件，多账户请手动定位；第二，不让软件访问硬盘可能导致这个错误；第三，没装希沃绝对会导致这个错误(doge)");
	a("");
	a("Q：怎样手动定位");
	a("A：右键\"搜索\"按钮，然后手动选择当前账户文件夹下的\"AppData\\Roaming\\Seewo\\EasiNote5\\Data\"目录");
	a("");
	a("Q：这什么破软件，我要拷打作者！");
	a("A：QQ1052470899 随时奉陪~");
	a("");
	a("Q：是男人就开源");
	a("A：https://github.com/ChensingML/EasiNotePacker (doge)");
	a("");
	a("");
	a("EasiNotePacker v1.0.0.1      2021/7/16 22:47");

	return TRUE;
}
