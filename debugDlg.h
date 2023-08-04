#pragma once
#include "afxdialogex.h"


// debugDlg 对话框

class debugDlg : public CDialogEx
{
	DECLARE_DYNAMIC(debugDlg)
public:
	debugDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~debugDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString debugLog;
	CString dlog;

	void UpdateLog(const char * log);
	void UpdateLog(CString log);
	void UpdateLog(std::string log);
	void UpdateLog(void);
};
