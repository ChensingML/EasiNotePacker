
// PackerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "EasiNotePacker.h"
#include "PackerDlg.h"
#include "afxdialogex.h"
#include "PackerHelp.h"
#include "LoadD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSplitButton search;
CListCtrl listb;

CString CPackerDlg::SelectFolderPath()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));

	sInfo.pidlRoot = ILCreateFromPath(_T("\\"));
	sInfo.lpszTitle = _T("请选择搜索路径");
	sInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			strFolderPath = szFolderPath;
		}
		::CoTaskMemFree(lpidlBrowse);
	}

	return strFolderPath;
}
// CPackerDlg 对话框

CString GetLocalAppdataPath()
{
	wchar_t m_lpszDefaultDir[MAX_PATH] = { 0 };
	wchar_t szDocument[MAX_PATH] = { 0 };

	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (pidl && SHGetPathFromIDList(pidl, szDocument))
	{
		GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
	}
	return m_lpszDefaultDir;
}



CPackerDlg::CPackerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PACKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPB, search);
	DDX_Control(pDX, IDC_LIST, listb);
	DDX_Control(pDX, IDC_FIF, fif);
}

BEGIN_MESSAGE_MAP(CPackerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(IDC_SPB, &CPackerDlg::OnSpb)
	ON_COMMAND(ID_MENU_32772, &CPackerDlg::OnMenuCos)
	ON_COMMAND(ID_MENU_32771, &CPackerDlg::OnMenuNormal)
	ON_COMMAND(IDC_HELPB, &CPackerDlg::OnHelpb)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST, &CPackerDlg::OnItemchangedList)
	ON_BN_CLICKED(IDC_GET, &CPackerDlg::OnBnClickedGet)
	ON_EN_CHANGE(IDC_FIF, &CPackerDlg::OnEnChangeFif)
//	ON_EN_UPDATE(IDC_FIF, &CPackerDlg::OnUpdateFif)
ON_BN_CLICKED(IDC_SA, &CPackerDlg::OnBnClickedSa)
ON_BN_CLICKED(IDC_NSA, &CPackerDlg::OnBnClickedNsa)
END_MESSAGE_MAP()



// CPackerDlg 消息处理程序

BOOL CPackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	search.SetDropDownMenu(IDR_S_MENU, 0);
	//ListView_GetExtendedListViewStyle(listb.GetSafeHwnd(), listb.GetExStyle() | LVS_EX_CHECKBOXES);
	listb.SetExtendedStyle(LVS_EX_CHECKBOXES);
	listb.InsertColumn(0, _T("课件名"),LVCFMT_LEFT,500);
	listb.InsertColumn(1, _T("创建者"), LVCFMT_LEFT, 70);
	listb.InsertColumn(2, _T("手机号"), LVCFMT_LEFT, 90);

	return TRUE;
}

void CPackerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CPackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPackerDlg::Search(CString s)
{
	if (s.Compare(_T(""))==0) {
		return;
	}
	if (allLessons) {
		int as = allLessons->size();
		for (int i = 0; i < as; i++)
		{
			delete allLessons->at(i);
		}
		allLessons->clear();
		vector<Lesson*>().swap(*allLessons);
		delete allLessons;
	}
	allLessons = new vector<Lesson*>();
	LoadD d(this);
	d.task = 0;
	d.path = s;
	d.DoModal();
	showedLessons = new vector<Lesson*>(*allLessons);
	updateL();
	fif.SetWindowTextW(_T(""));
}

void CPackerDlg::OnSpb()
{
	OnMenuNormal();
}

void CPackerDlg::OnBnClickedGet()
{
	LoadD d(this);
	d.task = 1;
	d.DoModal();
}


void CPackerDlg::OnMenuCos()
{
	Search(SelectFolderPath());
}


void CPackerDlg::OnMenuNormal()
{
	Search(GetLocalAppdataPath() + _T("\\..\\Roaming\\Seewo\\EasiNote5\\Data"));
}



void CPackerDlg::OnOK()
{
}


void CPackerDlg::OnHelpb()
{
	PackerHelp help;
	help.DoModal();
}

bool isig = false;

void CPackerDlg::OnItemchangedList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */
		&& !isig
		)
	{
		delete showedLessons->at(pNMLV->iItem)->checked;
		showedLessons->at(pNMLV->iItem)->checked = new int(1);
	}
	else if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */
		&& !isig
		)
	{
		delete showedLessons->at(pNMLV->iItem)->checked;
		showedLessons->at(pNMLV->iItem)->checked = new int(0);
	}
	else
	{
	}

	*pResult = 0;
}

bool sysc = false;

void CPackerDlg::OnEnChangeFif()
{
	if (sysc)
		return;
	isig = true;
	CString fi;
	fif.GetWindowTextW(fi);
	if (!allLessons) {
		sysc = true;
		fif.SetWindowTextW(_T(""));
		sysc = false;
		isig = false;
		return;
	}
	if (fi.IsEmpty())
	{
		showedLessons = new vector<Lesson*>(*allLessons);
	}
	else {
		showedLessons->clear();
		int alls = allLessons->size();
		for (int i = 0; i < alls; i++) {
			Lesson* l = allLessons->at(i);
			if (l->name->Find(fi) != -1 ||
				l->phone->Find(fi) != -1 ||
				l->user->Find(fi) != -1)
			{
				showedLessons->push_back(l);
			}
		}
	}
	updateL();
	isig = false;
}


//void CPackerDlg::OnUpdateFif()
//{
//	MessageBox(_T(""));
//}


void CPackerDlg::updateL()
{
	int als = showedLessons->size();
	listb.DeleteAllItems();
	for (int i = als - 1, j = 0; i != -1; i--, j++) {
		Lesson* l = showedLessons->at(i);
		listb.InsertItem(0, *l->name);
		listb.SetItemText(0, 1, *l->user);
		listb.SetItemText(0, 2, *l->phone);
		listb.SetCheck(0, *l->checked);
	}
}


void CPackerDlg::OnBnClickedSa()
{
	if (!showedLessons)
		return;
	int s = showedLessons->size();
	for (int i = 0; i < s; i++) {
		delete showedLessons->at(i)->checked;
		showedLessons->at(i)->checked = new int(1);
	}
	updateL();
}


void CPackerDlg::OnBnClickedNsa()
{
	if (!showedLessons)
		return;
	int s = showedLessons->size();
	for (int i = 0; i < s; i++) {
		delete showedLessons->at(i)->checked;
		showedLessons->at(i)->checked = new int(0);
	}
	updateL();
}
