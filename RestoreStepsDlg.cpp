// RestoreStepsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicCube2D.h"
#include "RestoreStepsDlg.h"


// CRestoreStepsDlg 对话框

IMPLEMENT_DYNAMIC(CRestoreStepsDlg, CDialog)

CRestoreStepsDlg::CRestoreStepsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRestoreStepsDlg::IDD, pParent)
{

}

CRestoreStepsDlg::~CRestoreStepsDlg()
{
}

void CRestoreStepsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, editDisplay);
}


BEGIN_MESSAGE_MAP(CRestoreStepsDlg, CDialog)
END_MESSAGE_MAP()


// CRestoreStepsDlg 消息处理程序
