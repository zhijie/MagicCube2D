#pragma once
#include "afxwin.h"


// CRestoreStepsDlg �Ի���

class CRestoreStepsDlg : public CDialog
{
	DECLARE_DYNAMIC(CRestoreStepsDlg)

public:
	CRestoreStepsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRestoreStepsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit editDisplay;
};
