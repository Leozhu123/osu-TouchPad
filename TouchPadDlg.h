#pragma once


// TouchPadDlg 对话框
#include "resource.h"
#include "stdfax.h"
#include "stdafx.h"
class TouchPadDlg : public CDialog
{
	DECLARE_DYNAMIC(TouchPadDlg)

public:
	TouchPadDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~TouchPadDlg();
	bool isRunning = false;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
