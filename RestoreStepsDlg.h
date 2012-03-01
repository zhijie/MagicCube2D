#pragma once
#include "afxwin.h"


// CRestoreStepsDlg 对话框

class CRestoreStepsDlg : public CDialog
{
	DECLARE_DYNAMIC(CRestoreStepsDlg)

public:
	CRestoreStepsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRestoreStepsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit editDisplay;
};
