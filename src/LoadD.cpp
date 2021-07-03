

#include "pch.h"
#include "LoadD.h"
#include "afxdialogex.h"
#include <vector>
#include "Lesson.h"
#include "PackerDlg.h"
#include <clocale>
#include "SuccD.h"

using namespace std;


BOOL isThreadAlive=false;

IMPLEMENT_DYNAMIC(LoadD, CDialogEx)

LoadD::LoadD(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LOADING, pParent)
{
	task = 0;
	dlgss = (CPackerDlg*)pParent;
}

LoadD::~LoadD()
{
}

void LoadD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROG, progbar);
	DDX_Control(pDX, IDC_PT, stat);
}


BEGIN_MESSAGE_MAP(LoadD, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


#define ST(x) stat->SetWindowTextW(_T(x))
#define STS(x) stat->SetWindowTextW(x)

BOOL inline hasFile(CString path) {
	return PathFileExists(path);
}


void GetFiles(CString csDirPath, vector<CString>& vctPath)
{
	csDirPath += "\\*";
	HANDLE file;
	WIN32_FIND_DATA fileData;
	file = FindFirstFile(csDirPath.GetBuffer(), &fileData);
	if (file != INVALID_HANDLE_VALUE)
	{
		vctPath.push_back(fileData.cFileName);
		while (FindNextFile(file, &fileData))
		{
			vctPath.push_back(fileData.cFileName);
		}
	}
}

BOOL GetLines(CString path, vector<CString>& vec) {
	FILE* f;
	USES_CONVERSION;
	int o=fopen_s(&f, W2A(path), "r,ccs=UNICODE");
	CStdioFile file(f);
	CString strValue = _T("");
	while (file.ReadString(strValue))
	{
		vec.push_back(strValue);
	}
	file.Close();
	return true;
}

BOOL PutLines(CString path, vector<CString> vec) {
	FILE* f;
	USES_CONVERSION;
	int o = fopen_s(&f, W2A(path), "w+,ccs=UNICODE");
	CStdioFile file(f);
	int s = vec.size();
	for (int i = 0; i < s; i++) {
		file.WriteString(vec.at(i));
	}
	file.Close();
	return true;
}


CString SelectFolderPath()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));

	sInfo.pidlRoot = ILCreateFromPath(_T("\\"));
	sInfo.lpszTitle = _T("请选择保存路径");
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

UINT workThread(LPVOID para) {
	isThreadAlive = true;
	LoadD* ld = (LoadD*)para;

	vector<Lesson*>* allLessonsp =ld->dlgss->allLessons;

	CProgressCtrl* prog = (CProgressCtrl*)ld->GetDlgItem(IDC_PROG);
	CStatic* stat = (CStatic*)ld->GetDlgItem(IDC_PT);
	CString path = ld->path;
	ST("搜索中");

	if (!hasFile(path+_T("\\App.config")))
	{
		ld->MessageBox(_T("目录错误，请检查目录或手动定位至\"AppData\\Roaming\\Seewo\\EasiNote5\\Data\""));
		isThreadAlive = false;
		return -1;
	}
	ST("搜索用户中");
	vector<CString> users;
	vector<CString> ust;
	vector<CString> phones;
	//vector<Lesson> classes;
	GetFiles(path, ust);
	int ustsize = ust.size();
	int found = 0;
	for (int i = 0; i < ustsize; i++)
	{
		CString tc = ust.at(i);
		char fc = tc.GetAt(0);
		if (fc >= '0' && fc <= '9' && tc.GetLength()==11)
		{
			users.push_back(path+_T("\\") + tc+_T("\\Courseware\\"));
			phones.push_back(tc);
			CString numt;
			numt.Format(_T("%d"), ++found);
			CString s = _T("搜索到") + numt + _T("个用户");
			STS(s);
		}
	}
	int userss = users.size();
	if (userss==0)
	{
		ld->MessageBox(_T("没有发现留下来的课件呢"));
		isThreadAlive = false;
		return - 1;
	}

	ST("搜索课件中");
	
	found = 0;
	for (int i = 0; i < userss; i++) {
		vector<CString> fs;
		CString tPhone = phones.at(i);
		CString tUser = users.at(i);
		GetFiles(users.at(i), fs);
		int sfs = fs.size();
		for (int j = 0; j < sfs; j++) {
			CString tp = tUser+fs.at(j) + _T("\\")+fs.at(j)+_T(".json");
			if (hasFile(tp))
			{
				Lesson* l=new Lesson();
				l->path = new CString(tp);
				l->phone = new CString(tPhone);
				//ld->MessageBox(_T("Found") + tp);
				allLessonsp->push_back(l);

				CString numt;
				numt.Format(_T("%d"), ++found);
				STS(_T("搜索到") + numt + _T("个课件"));
				//delete& numt;
			}
		}
	}

	ST("解析课件中");

	int alls = allLessonsp->size();
	prog->SetRange(0,alls);
	prog->SetStep(1);

	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "chs");

	for (int i = 0; i < alls; i++) {
		Lesson* l = allLessonsp->at(i);
		STS(_T("正在解析 ")+*l->phone+_T("的课件"));
		vector<CString> lines;
		GetLines(*l->path, lines);
		int sl = lines.size();
		vector<Lesson::CSS*>* cssv = new vector<Lesson::CSS*>();
		CString fName;
		bool inName = false;
		for (int j = 0; j < sl; j++)
		{
			CString tl = lines.at(j);
			if (tl.Find(_T("\"Name\""))!=-1)
			{
				tl.Replace(_T(" "), _T(""));
				l->name = new CString(tl.Mid(8, tl.GetLength() - 10));
			}
			else if (tl.Find(_T("\"FileName\"")) != -1) {
				tl.Replace(_T(" "), _T(""));
				fName = tl.Mid(12, tl.GetLength() - 14);
				inName = true;
			}
			else if (tl.Find(_T("\"RelativePath\"")) != -1 && inName) {
				tl.Replace(_T(" "), _T(""));
				Lesson::CSS* css;
				css = new Lesson::CSS();
				css->rela = new CString(tl.Mid(16, tl.GetLength() - 17));
				css->path = new CString(fName);
				cssv->push_back(css);
				inName = false;
			}
			else if (tl.Find(_T("\"Author\"")) != -1) {
				tl.Replace(_T(" "), _T(""));
				l->user= new CString(tl.Mid(10, tl.GetLength() - 12));
			}
		}
		l->files = cssv;
		prog->OffsetPos(1);
	}

	setlocale(LC_CTYPE, old_locale);
	free(old_locale);

	//TODO Code here
	isThreadAlive = false;
	return 0;
}

BOOL DeleteDirectory(CString directory_path){
	CFileFind filefind;
	CString path = directory_path;
	if (path.Right(1) != "\\")//目录的最右边需要“\”字符
		path += "\\";
	path += "*.*";
	BOOL res = filefind.FindFile(path);
	while (res)
	{
		res = filefind.FindNextFile();
		//是文件时直接删除
		if (!filefind.IsDots() && !filefind.IsDirectory())
			DeleteFile(filefind.GetFilePath());//删除文件
		else if (filefind.IsDots())
			continue;
		else if (filefind.IsDirectory())//为目录
		{
			path = filefind.GetFilePath();
			//是目录时继续递归，删除该目录下的文件
			DeleteDirectory(path);
			//目录为空后删除目录
			RemoveDirectory((LPCTSTR)path);
		}
	}
	return TRUE;
}

UINT saveWork(LPVOID para) {
	isThreadAlive = true;
	LoadD* ld = (LoadD*)para;

	vector<Lesson*>* allLessonsp = ld->dlgss->allLessons;
	if (!allLessonsp) {
		isThreadAlive = false;
		return 0;
	}

	CProgressCtrl* prog = (CProgressCtrl*)ld->GetDlgItem(IDC_PROG);
	CStatic* stat = (CStatic*)ld->GetDlgItem(IDC_PT);
	CString path = ld->path;
	ST("处理中");

	vector<Lesson*> ls;
	int as = allLessonsp->size();
	for (int i = 0; i < as; i++)
	{
		Lesson* l =allLessonsp->at(i);
		if (*l->checked==TRUE)
		{
			ls.push_back(l);
		}
	}
	int lss = ls.size();
	if (lss == 0) {
		isThreadAlive = false;
		return 0;
	}
	
	CString oPath = SelectFolderPath();
	if (oPath.Compare(_T("")) == 0) {
		isThreadAlive = false;
		return 0;
	}
	prog->SetRange(0, lss);
	prog->SetStep(1);
	//prog->OffsetPos(1);

	for (int i = 0; i < lss; i++) {
		CString hint;
		hint.Format(_T("正在处理 %d/%d"), i + 1, lss);
		STS(hint);
		Lesson* l = ls.at(i);
		CString orgPath = *l->path + _T("\\..\\");
		CString workPath = *l->path + _T("\\..\\Work\\");
		CString slPath = workPath + _T("\\Slides\\");
		CString resPath = workPath + _T("\\Resources\\");
		DeleteDirectory(workPath);
		CreateDirectory(workPath, 0);
		CreateDirectory(slPath, 0);
		CreateDirectory(resPath, 0);

		vector<Lesson::CSS*>* csss = l->files;
		vector<CString> types;
		vector<CString> overrided;

		int cssss = csss->size();
		for (int j = 0; j < cssss; j++)
		{
			Lesson::CSS* tcs = csss->at(j);
			CString rela = *tcs->rela;
			bool hasSub = rela.Find(_T("\\\\")) != -1;
			if (hasSub)
			{
				CString slN;
				if (rela.Find(_T("Slides")) != -1) {
					slN = CString(rela);
					slN = slN.Mid(8, slN.GetLength() - 8);
					CopyFile(orgPath + *tcs->path, slPath + slN, true);
				}
				else if (rela.Find(_T("Resources")) != -1)
				{
					slN = CString(rela);
					slN = slN.Mid(11, slN.GetLength() - 11);
					CopyFile(orgPath + *tcs->path, resPath + slN, true);
				}
				Lesson::CSS* tcs = csss->at(j);
				CString kz = *tcs->path;
				int pp = kz.Find(_T("."));
				if (pp == -1)
				{
					overrided.push_back(_T("<Override PartName=\"/Resources/\""
						+ slN + _T(" ContentType=\"\" />")));
				}
				else
				{
					kz = kz.Mid(pp + 1, kz.GetLength() - pp - 1);
					CString adds = _T("<Default Extension=\"") + kz
						+ _T("\" ContentType=\"\" />");
					int typess = types.size();
					bool has = false;
					for (int k = 0; k < typess; k++) {
						if (adds.Compare(types.at(k)) == 0) {
							has = true;
							break;
						}
					}
					if (!has)
					{
						types.push_back(adds);
					}
				}
			}
			else {
				CopyFile(orgPath + *tcs->path, workPath + rela, true);
			}
		}

		vector<CString> lines;
		lines.push_back(_T("\xEF\xBB\xBF"));
		lines.push_back(_T(
			"<?xml version=\"1.0\" encoding=\"utf-8\"?><Types xmlns=\"http://schemas.openxmlformats.org/package/2006/content-types\">"));
		lines.insert(lines.end(), types.begin(), types.end());
		lines.insert(lines.end(), overrided.begin(), overrided.end());
		lines.push_back(_T("</Types>"));

		PutLines(workPath + _T("[Content_Types].xml"), lines);

		int nRetCode = 0;

		CString hint1;
		hint1.Format(_T("正在压制 %d/%d"), i + 1, lss);
		STS(hint1);

		HRSRC hRes = FindResource(theApp.m_hInstance, MAKEINTRESOURCE(IDR_EXE1), _T("EXE"));
		//获取资源长度  
		DWORD len = SizeofResource(theApp.m_hInstance, hRes);
		HGLOBAL hg = LoadResource(theApp.m_hInstance, hRes);
		LPVOID lp = (LPSTR)LockResource(hg);
		CFile file;
		file.Open(workPath + _T("7z.exe"), CFile::modeCreate | CFile::modeWrite);
		char* cp = (char*)lp;
		for (int i = 0; i < len; i++)
		{
			file.Write(cp++, 1);
		}
		CString filePath = file.GetFilePath();
		file.Close();
		SHELLEXECUTEINFO ShExecInfo = { 0 };
		ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo.hwnd = NULL;
		ShExecInfo.lpVerb = NULL;
		CString wp1 = workPath + _T("7z.exe");
		ShExecInfo.lpFile = wp1;
		CString para1 = CString(_T("a -mx0 -r ") + workPath + _T("\\o.zip ") + workPath + _T("\\* "));
		ShExecInfo.lpParameters = para1;
		ShExecInfo.lpDirectory = NULL;
		ShExecInfo.nShow = SW_HIDE;
		ShExecInfo.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo);
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

		SHELLEXECUTEINFO ShExecInfo1 = { 0 };
		ShExecInfo1.cbSize = sizeof(SHELLEXECUTEINFO);
		ShExecInfo1.fMask = SEE_MASK_NOCLOSEPROCESS;
		ShExecInfo1.hwnd = NULL;
		ShExecInfo1.lpVerb = NULL;
		ShExecInfo1.lpFile = wp1;
		CString para2 = CString(_T("d ") + workPath + _T("\\o.zip 7z.exe"));
		ShExecInfo1.lpParameters = para2;
		ShExecInfo1.lpDirectory = NULL;
		ShExecInfo1.nShow = SW_HIDE;
		ShExecInfo1.hInstApp = NULL;
		ShellExecuteEx(&ShExecInfo1);
		WaitForSingleObject(ShExecInfo1.hProcess, INFINITE);
		FreeResource(hg);

		CString hint2;
		hint2.Format(_T("正在保存 %d/%d"), i + 1, lss);
		STS(hint2);

		MoveFile(workPath + _T("o.zip"), oPath + _T("\\") + *l->name + _T(".enbx"));

		CString hint3;
		hint3.Format(_T("正在清理 %d/%d"), i + 1, lss);
		STS(hint3);

		DeleteDirectory(workPath);

		prog->OffsetPos(1);
	}

	ST("完成！");
	ld->SetWindowTextW(_T("完成"));

	SuccD sd;
	sd.path = oPath;
	sd.DoModal();

	isThreadAlive = false;
	return 0;
}


BOOL LoadD::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}


void LoadD::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
	case 1: {
		if (!isThreadAlive)
		{
			EndDialog(0);
		}
		break;
	}
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


void LoadD::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (task == 0) {
		HANDLE h = AfxBeginThread(workThread, this, THREAD_PRIORITY_IDLE);
	}
	else if (task == 1)
	{
		HANDLE h = AfxBeginThread(saveWork, this, THREAD_PRIORITY_IDLE);
	}
	SetTimer(1, 1, NULL);
}
