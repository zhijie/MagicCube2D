// MagicCube2DDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "RestoreStepsDlg.h"

///////////////////////////////////////////////////
//rotate direction
enum Axis{X,Y,Z};
//description of chunk position
enum ChunkIndex{NEGATIVE,MIDDLE,POSITIVE};
enum RotateDirection{CLOCKWISE,COUNTERCLOCKWISE,NOROTATE};
//description of the 6 aspects and the situation that no aspect
enum Aspect{FRONT,BACK,LEFT,RIGHT,UP,DOWN,NOASPECT};
//gray ,color of sheltered aspects which can be seen when rotation
enum Color{BLUE,GREEN,RED,ORANGE,WHITE,YELLOW,GRAY};

class Facelet{
public:
	Aspect asp;
	int index;
	Color color;
	Facelet():asp(NOASPECT),index(-1),color(GRAY){}
};
class Edge
{
public:
	Facelet faceletMain;
	Facelet faceletSide;
};
class Corner
{
public:
	Facelet faceletMain;
	Facelet facelet1;
	Facelet facelet2;
};

// CMagicCube2DDlg dialog
class CMagicCube2DDlg : public CDialog
{
// Construction
public:
	CMagicCube2DDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MAGICCUBE2D_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	static const int meshLen=30;
	static const int meshFullLen=90;
	CPoint org;
	CPoint orgMem;

	CDC* pDCmem;//memory dc
	CBitmap bmpMem;
	static const int bmpWidth=meshLen*4*3;
	static const int bmpHeight=meshLen*3*3;

	CBrush brushs[7];//WHITE,YELLOW,BLUE,GREEN,RED,ORANGE
	//indexes for small aspects in a big aspect is in the order shown in 3d version
	CPoint vertexes[6][9][4];//FRONT,BACK,LEFT,RIGHT,UP,DOWN
	CPoint aspectVertexes[6][4];
	
	Color colorSelected;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void UpdateView(void);
	Aspect GetAspect(CPoint point);
	int GetSmallAspectIndex(Aspect asp,CPoint point);
	bool IsInThisRect(CPoint* pts,CPoint point);
	afx_msg void OnBnClickedButton_F();
	afx_msg void OnBnClickedButton_B();
	afx_msg void OnBnClickedButton_U();
	afx_msg void OnBnClickedButton_D();
	afx_msg void OnBnClickedButton_R();
	afx_msg void OnBnClickedButton_L();
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonEmpty();
	afx_msg void OnBnClickedButtonSolution();
	afx_msg void OnBnClickedButtonRandom();

	bool IsValidLayout(void);
	
	void GetFacelet4(Facelet* facelet,Color color);
	Facelet GetNeighborEdgeFacelet(Facelet facelet);
	void GetNeighborCornerFacelets(Facelet* neighbor2,Facelet facelet);
	Edge GetEdge(Color colorMain,Color colorSide);
	bool IsCrossed(void);
	CString Cross(void);
	
	bool IsFirstLayerCornerRestored(void);	
	CString RestoreFirstLayerCorner(void);
	bool IsSecondLayerEdgeRestored(void);
	CString RestoreSecondLayerEdge(void);
	bool IsF2L(void);
	CString F2L(void);

	bool IsTopCrossed(void);
	CString BuildTopCross(void);
	
	//display restoration steps found
	CRestoreStepsDlg stepsDlg;

	//two ways to continue,one is standard cfop,the other is not famous,but 
	//has fewer formulas
	
	//1:fewer formulas method
	bool IsTopCrossMatched(void);
	CString MatchTopCross(void);
	bool IsTopCornerBack(void);
	bool IsTopCornerBack(Facelet f);
	int HashCorner(Color c1,Color c2,Color c3);
	CString PutBackTopCorner(void);
	bool IsTopCornerRestored(void);
	CString RestoreTopCorner(void);
	CString MethodFewerFormula();

	//2:cfop
	void HashOLL(char*);
	bool IsTopFaceRestored(void);
	void OLL(void);
	void HashPLL(char*);//not finished
	void PLL(void);
	void CFOP(void);

	void Executor(const char* cmd);
	CString UndoCmdString(const char* cmd);
	int CountSteps(const char* cmd);

	afx_msg void OnSave();
	afx_msg void OnOpen();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCross();
	afx_msg void OnL2Edge();
	afx_msg void OnTopcross();
	afx_msg void OnOll();
	afx_msg void OnPll();
	afx_msg void OnL1Corner();
	CEdit cmdstringEditCtl;
	afx_msg void OnExcute();
	afx_msg void OnMatchtopcross();
	afx_msg void OnPutbackTopcorner();
	afx_msg void OnRestoreTopcorner();
};
