// debugDlg.cpp: 实现文件
//

#include "pch.h"
#include "resource.h"
#include "afxdialogex.h"
#include "debugDlg.h"


// debugDlg 对话框

IMPLEMENT_DYNAMIC(debugDlg, CDialogEx)

debugDlg::debugDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, debugLog(_T(""))
{
	debugLog.Empty();
}

debugDlg::~debugDlg()
{
	dlog.Empty();
	debugLog.Empty();
}

void debugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, debugLog);
}

void debugDlg::UpdateLog(const char * log)
{
	CString str(log);
	CTime t;
	t = t.GetCurrentTime();
	
	str = t.Format("[%H:%M:%S] ") + str;
	str += "\r\n";
	debugLog = str + debugLog;
	
	UpdateData(0);
}

void debugDlg::UpdateLog(CString log)
{
	CTime t;
	t = t.GetCurrentTime();

	log = t.Format("[%H:%M:%S] ") + log;
	log += "\r\n";
	debugLog = log + debugLog;
	UpdateData(0);
}

void debugDlg::UpdateLog(std::string log)
{
	CTime t;
	t = t.GetCurrentTime();
	CString cstr(log.c_str());
	cstr = t.Format("[%H:%M:%S] ") + cstr;
	cstr += "\r\n";
	debugLog = cstr + debugLog;
	UpdateData(0);
}

void debugDlg::UpdateLog(void)
{
	CTime t;
	t = t.GetCurrentTime();

	dlog = t.Format("[%H:%M:%S] ") + dlog;
	dlog += "\r\n";
	debugLog = dlog + debugLog;
	dlog.Empty();

	UpdateData(0);
}


BEGIN_MESSAGE_MAP(debugDlg, CDialogEx)
END_MESSAGE_MAP()


// debugDlg 消息处理程序
