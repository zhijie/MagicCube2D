/* 
 * @ Author: onezeros@yahoo.cn
 * @ blog  : blog.csdn.net/onezeros
 * @ Date  : 10/16/2010
 * @ Note  : You can use this program for any purpose if you keep this note. 
 *			 I'll appreciate if you can email bugs you find to me.
 *			 Have A Nice Day!
 */

// MagicCube2DDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MagicCube2D.h"
#include "MagicCube2DDlg.h"

#include <vector>
#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	
	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMagicCube2DDlg dialog

///////////////////////////////////////////////////
//global functions
///////////////////////////////////////////////////
//store colors of every little aspect of magic cube ,in xyz order
//initialized with up white,down yellow,front blue,back green,left red,right orange
Color aspectColors[6][9]={
	//front
	{BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE},
	//back
	{GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN},
	//left
	{RED,RED,RED,RED,RED,RED,RED,RED,RED},
	//right
	{ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE},
	//up
	{WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE},
	//down
	{YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW}	
};
Color aspectColor_org[6][9]={
	//front
	{BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE,BLUE},
	//back
	{GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN,GREEN},
	//left
	{RED,RED,RED,RED,RED,RED,RED,RED,RED},
	//right
	{ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE,ORANGE},
	//up
	{WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE,WHITE},
	//down
	{YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW}	
};
///////////////////////////////////////////////////
//rotate color table
///////////////////////////////////////////////////
//update color table when rotate a chunk
//	rotation clockwise once will be of same effect with rotation counterclockwise triple
//but this will be inefficient.so I will write code for the two situation seperately
//convention:look from positive to negative along axis to decide clockwise or not

void rotateFullAspectClockwise(Aspect asp,int* index)
{	
	Color t[2];
	for (int i=0;i<2;i++){
		t[i]=aspectColors[asp][index[i+6]];
	}
	for (int i=7;i>1;i--){
		aspectColors[asp][index[i]]=aspectColors[asp][index[i-2]];
	}
	for (int i=0;i<2;i++){
		aspectColors[asp][index[i]]=t[i];
	}	
}
void rotateFullAspectAnticlockwise(Aspect asp,int* index)
{
	Color t[2];
	for (int i=0;i<2;i++){
		t[i]=aspectColors[asp][index[i]];
	}
	for (int i=0;i<6;i++){
		aspectColors[asp][index[i]]=aspectColors[asp][index[i+2]];
	}
	for (int i=0;i<2;i++){
		aspectColors[asp][index[i+6]]=t[i];
	}
}
void rotateXLeftClockwise()
{
	int index[9]={6,7,8,5,2,1,0,3};
	rotateFullAspectClockwise(LEFT,index);

	//front ,down,back,up
	for (int i=0;i<=6;i+=3){
		Color t=aspectColors[FRONT][i];
		aspectColors[FRONT][i]=aspectColors[DOWN][i];
		aspectColors[DOWN][i]=aspectColors[BACK][6-i];
		aspectColors[BACK][6-i]=aspectColors[UP][6-i];
		aspectColors[UP][6-i]=t;
	}
}
void rotateXLeftAnticlockwise()
{
	int index[9]={6,7,8,5,2,1,0,3};
	rotateFullAspectAnticlockwise(LEFT,index);

	//front ,up,back,down	
	for (int i=0;i<=6;i+=3){
		Color t=aspectColors[FRONT][i];
		aspectColors[FRONT][i]=aspectColors[UP][6-i];
		aspectColors[UP][6-i]=aspectColors[BACK][6-i];	
		aspectColors[BACK][6-i]=aspectColors[DOWN][i];	
		aspectColors[DOWN][i]=t;
	}
}
void rotateXRightClockwise()
{
	int index[9]={6,7,8,5,2,1,0,3};
	rotateFullAspectClockwise(RIGHT,index);

	//front ,down,back,up
	for (int i=0;i<=6;i+=3){
		Color t=aspectColors[FRONT][i+2];
		aspectColors[FRONT][i+2]=aspectColors[DOWN][i+2];
		aspectColors[DOWN][i+2]=aspectColors[BACK][6-i+2];
		aspectColors[BACK][6-i+2]=aspectColors[UP][6-i+2];
		aspectColors[UP][6-i+2]=t;
	}
}
void rotateXRightAnticlockwise()
{
	int index[9]={6,7,8,5,2,1,0,3};
	rotateFullAspectAnticlockwise(RIGHT,index);

	//front ,up,back,down
	for (int i=0;i<=6;i+=3){
		Color t=aspectColors[FRONT][i+2];
		aspectColors[FRONT][i+2]=aspectColors[UP][6-i+2];
		aspectColors[UP][6-i+2]=aspectColors[BACK][6-i+2];	
		aspectColors[BACK][6-i+2]=aspectColors[DOWN][i+2];	
		aspectColors[DOWN][i+2]=t;
	}	
}
void rotateXMiddleClockwise()
{
	//front ,down,back,up
	for (int i=0;i<=6;i+=3){
		Color t=aspectColors[FRONT][i+1];
		aspectColors[FRONT][i+1]=aspectColors[DOWN][i+1];
		aspectColors[DOWN][i+1]=aspectColors[BACK][6-i+1];
		aspectColors[BACK][6-i+1]=aspectColors[UP][6-i+1];
		aspectColors[UP][6-i+1]=t;
	}
}
void rotateXMiddleAnticlockwise()
{
	//front ,up,back,down
	for (int i=0;i<=6;i+=3){
		Color t=aspectColors[FRONT][i+1];
		aspectColors[FRONT][i+1]=aspectColors[UP][6-i+1];
		aspectColors[UP][6-i+1]=aspectColors[BACK][6-i+1];	
		aspectColors[BACK][6-i+1]=aspectColors[DOWN][i+1];	
		aspectColors[DOWN][i+1]=t;
	}
}
void rotateYDownClockwise()
{
	int index[9]={6,3,0,1,2,5,8,7};
	rotateFullAspectClockwise(DOWN,index);

	//front,right,back,left
	for (int i=0;i<3;i++){
		Color t=aspectColors[FRONT][i];
		aspectColors[FRONT][i]=aspectColors[RIGHT][3*(2-i)];
		aspectColors[RIGHT][3*(2-i)]=aspectColors[BACK][2-i];
		aspectColors[BACK][2-i]=aspectColors[LEFT][i*3];
		aspectColors[LEFT][i*3]=t;
	}
}
void rotateYDownAnticlockwise()
{
	int index[9]={6,3,0,1,2,5,8,7};
	rotateFullAspectAnticlockwise(DOWN,index);

	//front,left,back,right
	for (int i=0;i<3;i++){
		Color t=aspectColors[FRONT][i];
		aspectColors[FRONT][i]=aspectColors[LEFT][3*i];
		aspectColors[LEFT][3*i]=aspectColors[BACK][2-i];
		aspectColors[BACK][2-i]=aspectColors[RIGHT][(2-i)*3];
		aspectColors[RIGHT][(2-i)*3]=t;
	}
}
void rotateYUpClockwise()
{
	int index[9]={6,3,0,1,2,5,8,7};
	rotateFullAspectClockwise(UP,index);
	//front,right,back,left
	for (int i=0;i<3;i++){
		Color t=aspectColors[FRONT][6+i];
		aspectColors[FRONT][6+i]=aspectColors[RIGHT][2+3*(2-i)];
		aspectColors[RIGHT][2+3*(2-i)]=aspectColors[BACK][6+2-i];
		aspectColors[BACK][6+2-i]=aspectColors[LEFT][2+i*3];
		aspectColors[LEFT][2+i*3]=t;
	}
}
void rotateYUpAnticlockwise()
{
	int index[9]={6,3,0,1,2,5,8,7};
	rotateFullAspectAnticlockwise(UP,index);
	//front,left,back,right
	for (int i=0;i<3;i++){
		Color t=aspectColors[FRONT][6+i];
		aspectColors[FRONT][6+i]=aspectColors[LEFT][2+3*i];
		aspectColors[LEFT][2+3*i]=aspectColors[BACK][6+2-i];
		aspectColors[BACK][6+2-i]=aspectColors[RIGHT][2+(2-i)*3];
		aspectColors[RIGHT][2+(2-i)*3]=t;
	}
}
void rotateYMiddleClockwise()
{
	//front,right,back,left
	for (int i=0;i<3;i++){
		Color t=aspectColors[FRONT][3+i];
		aspectColors[FRONT][3+i]=aspectColors[RIGHT][1+3*(2-i)];
		aspectColors[RIGHT][1+3*(2-i)]=aspectColors[BACK][3+2-i];
		aspectColors[BACK][3+2-i]=aspectColors[LEFT][1+i*3];
		aspectColors[LEFT][1+i*3]=t;
	}
}
void rotateYMiddleAnticlockwise()
{
	//front,left,back,right
	for (int i=0;i<3;i++){
		Color t=aspectColors[FRONT][3+i];
		aspectColors[FRONT][3+i]=aspectColors[LEFT][1+3*i];
		aspectColors[LEFT][1+3*i]=aspectColors[BACK][3+2-i];
		aspectColors[BACK][3+2-i]=aspectColors[RIGHT][1+(2-i)*3];
		aspectColors[RIGHT][1+(2-i)*3]=t;
	}
}
void rotateZFrontClockwise()
{
	int index[9]={0,3,6,7,8,5,2,1};
	rotateFullAspectClockwise(FRONT,index);

	Color t=aspectColors[UP][6];
	aspectColors[UP][6]=aspectColors[UP][8];
	aspectColors[UP][8]=t;
	t=aspectColors[DOWN][6];
	aspectColors[DOWN][6]=aspectColors[DOWN][8];
	aspectColors[DOWN][8]=t;

	Color tmp[3];
	int size=3*sizeof(Color);
	memcpy(tmp,&aspectColors[LEFT][6],size);
	memcpy(&aspectColors[LEFT][6],&aspectColors[DOWN][6],size);
	memcpy(&aspectColors[DOWN][6],&aspectColors[RIGHT][6],size);
	memcpy(&aspectColors[RIGHT][6],&aspectColors[UP][6],size);
	memcpy(&aspectColors[UP][6],tmp,size);
}
void rotateZFrontAnticlockwise()
{
	int index[9]={0,3,6,7,8,5,2,1};
	rotateFullAspectAnticlockwise(FRONT,index);

	Color t=aspectColors[LEFT][6];
	aspectColors[LEFT][6]=aspectColors[LEFT][8];
	aspectColors[LEFT][8]=t;
	t=aspectColors[RIGHT][6];
	aspectColors[RIGHT][6]=aspectColors[RIGHT][8];
	aspectColors[RIGHT][8]=t;

	Color tmp[3];
	int size=3*sizeof(Color);
	memcpy(tmp,&aspectColors[LEFT][6],size);
	memcpy(&aspectColors[LEFT][6],&aspectColors[UP][6],size);
	memcpy(&aspectColors[UP][6],&aspectColors[RIGHT][6],size);
	memcpy(&aspectColors[RIGHT][6],&aspectColors[DOWN][6],size);
	memcpy(&aspectColors[DOWN][6],tmp,size);
}
void rotateZBackClockwise()
{
	int index[9]={0,3,6,7,8,5,2,1};
	rotateFullAspectClockwise(BACK,index);

	Color t=aspectColors[UP][0];
	aspectColors[UP][0]=aspectColors[UP][2];
	aspectColors[UP][2]=t;
	t=aspectColors[DOWN][0];
	aspectColors[DOWN][0]=aspectColors[DOWN][2];
	aspectColors[DOWN][2]=t;

	Color tmp[3];
	int size=3*sizeof(Color);
	memcpy(tmp,&aspectColors[LEFT][0],size);
	memcpy(&aspectColors[LEFT][0],&aspectColors[DOWN][0],size);
	memcpy(&aspectColors[DOWN][0],&aspectColors[RIGHT][0],size);
	memcpy(&aspectColors[RIGHT][0],&aspectColors[UP][0],size);
	memcpy(&aspectColors[UP][0],tmp,size);	
}
void rotateZBackAnticlockwise()
{
	int index[9]={0,3,6,7,8,5,2,1};
	rotateFullAspectAnticlockwise(BACK,index);

	Color t=aspectColors[LEFT][0];
	aspectColors[LEFT][0]=aspectColors[LEFT][2];
	aspectColors[LEFT][2]=t;
	t=aspectColors[RIGHT][0];
	aspectColors[RIGHT][0]=aspectColors[RIGHT][2];
	aspectColors[RIGHT][2]=t;

	Color tmp[3];
	int size=3*sizeof(Color);
	memcpy(tmp,&aspectColors[LEFT][0],size);
	memcpy(&aspectColors[LEFT][0],&aspectColors[UP][0],size);
	memcpy(&aspectColors[UP][0],&aspectColors[RIGHT][0],size);
	memcpy(&aspectColors[RIGHT][0],&aspectColors[DOWN][0],size);
	memcpy(&aspectColors[DOWN][0],tmp,size);
}
void rotateZMiddleClockwise()
{
	Color t=aspectColors[UP][3];
	aspectColors[UP][3]=aspectColors[UP][5];
	aspectColors[UP][5]=t;
	t=aspectColors[DOWN][3];
	aspectColors[DOWN][3]=aspectColors[DOWN][5];
	aspectColors[DOWN][5]=t;

	Color tmp[3];
	int size=3*sizeof(Color);
	memcpy(tmp,&aspectColors[LEFT][3],size);
	memcpy(&aspectColors[LEFT][3],&aspectColors[DOWN][3],size);
	memcpy(&aspectColors[DOWN][3],&aspectColors[RIGHT][3],size);
	memcpy(&aspectColors[RIGHT][3],&aspectColors[UP][3],size);
	memcpy(&aspectColors[UP][3],tmp,size);
}
void rotateZMiddleAnticlockwise()
{
	Color t=aspectColors[LEFT][3];
	aspectColors[LEFT][3]=aspectColors[LEFT][5];
	aspectColors[LEFT][5]=t;
	t=aspectColors[RIGHT][3];
	aspectColors[RIGHT][3]=aspectColors[RIGHT][5];
	aspectColors[RIGHT][5]=t;

	Color tmp[3];
	int size=3*sizeof(Color);
	memcpy(tmp,&aspectColors[LEFT][3],size);
	memcpy(&aspectColors[LEFT][3],&aspectColors[UP][3],size);
	memcpy(&aspectColors[UP][3],&aspectColors[RIGHT][3],size);
	memcpy(&aspectColors[RIGHT][3],&aspectColors[DOWN][3],size);
	memcpy(&aspectColors[DOWN][3],tmp,size);
}
//rotate color table functions
typedef void (*PRotateColorTable)();
PRotateColorTable pRotateColorTable[3][3][2]={
	//x from negative to positive,i.e. from left to right
	{{rotateXLeftClockwise,rotateXLeftAnticlockwise},
	{rotateXMiddleClockwise,rotateXMiddleAnticlockwise},
	{rotateXRightClockwise,rotateXRightAnticlockwise}},
	//y
	{{rotateYDownClockwise,rotateYDownAnticlockwise},
	{rotateYMiddleClockwise,rotateYMiddleAnticlockwise},
	{rotateYUpClockwise,rotateYUpAnticlockwise}},
	//z
	{{rotateZBackClockwise,rotateZBackAnticlockwise},
	{rotateZMiddleClockwise,rotateZMiddleAnticlockwise},
	{rotateZFrontClockwise,rotateZFrontAnticlockwise}},
};

CMagicCube2DDlg::CMagicCube2DDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicCube2DDlg::IDD, pParent),org(meshLen*3+5,meshLen*2*3-10),colorSelected(BLUE),orgMem(15,15)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	pDCmem=new CDC;
	stepsDlg.Create(IDD_DIALOG_RESULT,this);

	brushs[0].CreateSolidBrush(RGB(0,0,255));
	brushs[1].CreateSolidBrush(RGB(0,255,0));
	brushs[2].CreateSolidBrush(RGB(255,0,0));
	brushs[3].CreateSolidBrush(RGB(255,102,0));
	brushs[4].CreateSolidBrush(RGB(255,255,255));
	brushs[5].CreateSolidBrush(RGB(255,255,0));
	brushs[6].CreateSolidBrush(RGB(0x99,0x99,0x99));
	float len=meshLen*0.707106f;
	//vertexes for every aspect
	//front
	vertexes[FRONT][0][0]=org;
	vertexes[FRONT][0][1].x=org.x;
	vertexes[FRONT][0][1].y=org.y-meshLen;
	vertexes[FRONT][0][2].x=org.x+meshLen;
	vertexes[FRONT][0][2].y=org.y-meshLen;
	vertexes[FRONT][0][3].x=org.x+meshLen;
	vertexes[FRONT][0][3].y=org.y;
	for (int i=1;i<=2;i++){		
		for (int ind=0;ind<4;ind++){
			vertexes[FRONT][i][ind].x=vertexes[FRONT][i-1][ind].x+meshLen;
			vertexes[FRONT][i][ind].y=vertexes[FRONT][0][ind].y;
		}
	}
	for (int i=3;i<=8;i++){
		for (int ind=0;ind<4;ind++){
			vertexes[FRONT][i][ind].x=vertexes[FRONT][i-3][ind].x;
			vertexes[FRONT][i][ind].y=vertexes[FRONT][i-3][ind].y-meshLen;
		}
	}
	//back
	CPoint v(int(org.x+meshLen*3+int(len*3)),int(org.y-int(len*3)));
	vertexes[BACK][2][0]=v;
	vertexes[BACK][2][1].x=v.x;
	vertexes[BACK][2][1].y=v.y-meshLen;
	vertexes[BACK][2][2].x=v.x+meshLen;
	vertexes[BACK][2][2].y=v.y-meshLen;
	vertexes[BACK][2][3].x=v.x+meshLen;
	vertexes[BACK][2][3].y=v.y;
	for (int i=1;i>=0;i--){		
		for (int ind=0;ind<4;ind++){
			vertexes[BACK][i][ind].x=vertexes[BACK][i+1][ind].x+meshLen;
			vertexes[BACK][i][ind].y=vertexes[BACK][2][ind].y;
		}
	}
	for (int i=3;i<=8;i++){
		for (int ind=0;ind<4;ind++){
			vertexes[BACK][i][ind].x=vertexes[BACK][i-3][ind].x;
			vertexes[BACK][i][ind].y=vertexes[BACK][i-3][ind].y-meshLen;
		}
	}
	
	//left
	for (int ind=0;ind<4;ind++){
		vertexes[LEFT][0][ind].x=vertexes[FRONT][0][ind].x-meshLen*3;
		vertexes[LEFT][0][ind].y=vertexes[FRONT][0][ind].y;
	}
	for (int i=1;i<=2;i++){		
		for (int ind=0;ind<4;ind++){
			vertexes[LEFT][i][ind].y=vertexes[LEFT][i-1][ind].y-meshLen;
			vertexes[LEFT][i][ind].x=vertexes[LEFT][0][ind].x;
		}
	}
	for (int i=3;i<=8;i++){
		for (int ind=0;ind<4;ind++){
			vertexes[LEFT][i][ind].x=vertexes[LEFT][i-3][ind].x+meshLen;
			vertexes[LEFT][i][ind].y=vertexes[LEFT][i-3][ind].y;
		}
	}
	//right
	vertexes[RIGHT][0][3]=v;
	vertexes[RIGHT][0][0].x=v.x-int(len);
	vertexes[RIGHT][0][0].y=v.y+int(len);
	vertexes[RIGHT][0][1].x=vertexes[RIGHT][0][0].x;
	vertexes[RIGHT][0][1].y=vertexes[RIGHT][0][0].y-meshLen;
	vertexes[RIGHT][0][2].x=vertexes[RIGHT][0][3].x;
	vertexes[RIGHT][0][2].y=vertexes[RIGHT][0][3].y-meshLen;
	
	for (int i=1;i<=2;i++){		
		for (int ind=0;ind<4;ind++){
			vertexes[RIGHT][i][ind].y=vertexes[RIGHT][i-1][ind].y-meshLen;
			vertexes[RIGHT][i][ind].x=vertexes[RIGHT][0][ind].x;
		}
	}
	for (int i=3;i<=8;i++){
		for (int ind=0;ind<4;ind++){
			vertexes[RIGHT][i][ind].x=vertexes[RIGHT][i-3][ind].x-int(len);
			vertexes[RIGHT][i][ind].y=vertexes[RIGHT][i-3][ind].y+int(len);
		}
	}
	//up
	v.x=org.x+int(len*3);
	v.y=org.y-int(len*3)-meshLen*3;
	vertexes[UP][0][0].x=v.x-int(len);
	vertexes[UP][0][0].y=v.y+int(len);
	vertexes[UP][0][1]=v;
	vertexes[UP][0][2].x=v.x+meshLen;
	vertexes[UP][0][2].y=v.y;
	vertexes[UP][0][3].x=vertexes[UP][0][0].x+meshLen;
	vertexes[UP][0][3].y=vertexes[UP][0][0].y;
	for (int i=1;i<=2;i++){		
		for (int ind=0;ind<4;ind++){
			vertexes[UP][i][ind].x=vertexes[UP][i-1][ind].x+meshLen;
			vertexes[UP][i][ind].y=vertexes[UP][0][ind].y;
		}
	}
	for (int i=3;i<=8;i++){
		for (int ind=0;ind<4;ind++){
			vertexes[UP][i][ind].x=vertexes[UP][i-3][ind].x-int(len);
			vertexes[UP][i][ind].y=vertexes[UP][i-3][ind].y+int(len);
		}
	}
	//down
	for (int i=0;i<9;i++){
		for (int ind=0;ind<4;ind++){
			vertexes[DOWN][i][ind].x=vertexes[FRONT][i][ind].x;
			vertexes[DOWN][i][ind].y=vertexes[FRONT][i][ind].y+meshLen*3;
		}
	}
	
	aspectVertexes[0][0]=vertexes[0][0][0];
	aspectVertexes[0][1]=vertexes[0][6][1];
	aspectVertexes[0][2]=vertexes[0][8][2];
	aspectVertexes[0][3]=vertexes[0][2][3];
	aspectVertexes[1][0]=vertexes[1][2][0];
	aspectVertexes[1][1]=vertexes[1][8][1];
	aspectVertexes[1][2]=vertexes[1][6][2];
	aspectVertexes[1][3]=vertexes[1][0][3];
	aspectVertexes[2][0]=vertexes[2][0][0];
	aspectVertexes[2][1]=vertexes[2][2][1];
	aspectVertexes[2][2]=vertexes[2][8][2];
	aspectVertexes[2][3]=vertexes[2][6][3];
	aspectVertexes[3][0]=vertexes[3][6][0];
	aspectVertexes[3][1]=vertexes[3][8][1];
	aspectVertexes[3][2]=vertexes[3][2][2];
	aspectVertexes[3][3]=vertexes[3][0][3];
	aspectVertexes[4][0]=vertexes[4][6][0];
	aspectVertexes[4][1]=vertexes[4][0][1];
	aspectVertexes[4][2]=vertexes[4][2][2];
	aspectVertexes[4][3]=vertexes[4][8][3];
	aspectVertexes[5][0]=vertexes[5][0][0];
	aspectVertexes[5][1]=vertexes[5][6][1];
	aspectVertexes[5][2]=vertexes[5][8][2];
	aspectVertexes[5][3]=vertexes[5][2][3];
	srand(unsigned(time(NULL)));
}

void CMagicCube2DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, cmdstringEditCtl);
}

BEGIN_MESSAGE_MAP(CMagicCube2DDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_F, &CMagicCube2DDlg::OnBnClickedButton_F)
	ON_BN_CLICKED(IDC_BUTTON_B, &CMagicCube2DDlg::OnBnClickedButton_B)
	ON_BN_CLICKED(IDC_BUTTON_U, &CMagicCube2DDlg::OnBnClickedButton_U)
	ON_BN_CLICKED(IDC_BUTTON_D, &CMagicCube2DDlg::OnBnClickedButton_D)
	ON_BN_CLICKED(IDC_BUTTON_R, &CMagicCube2DDlg::OnBnClickedButton_R)
	ON_BN_CLICKED(IDC_BUTTON_L, &CMagicCube2DDlg::OnBnClickedButton_L)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CMagicCube2DDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_EMPTY, &CMagicCube2DDlg::OnBnClickedButtonEmpty)
	ON_BN_CLICKED(IDC_BUTTON_SOLUTION, &CMagicCube2DDlg::OnBnClickedButtonSolution)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM, &CMagicCube2DDlg::OnBnClickedButtonRandom)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMagicCube2DDlg::OnSave)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CMagicCube2DDlg::OnOpen)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_CROSS, &CMagicCube2DDlg::OnCross)
	ON_BN_CLICKED(IDC_BUTTON_EDGE_F2, &CMagicCube2DDlg::OnL2Edge)
	ON_BN_CLICKED(IDC_BUTTON_TOPCROSS, &CMagicCube2DDlg::OnTopcross)
	ON_BN_CLICKED(IDC_BUTTON_OLL, &CMagicCube2DDlg::OnOll)
	ON_BN_CLICKED(IDC_BUTTON_PLL, &CMagicCube2DDlg::OnPll)
	ON_BN_CLICKED(IDC_BUTTON_CORNER_F1, &CMagicCube2DDlg::OnL1Corner)
	ON_BN_CLICKED(IDC_BUTTON_EXCUTE, &CMagicCube2DDlg::OnExcute)
	ON_BN_CLICKED(IDC_BUTTON_MATCHTOPCROSS, &CMagicCube2DDlg::OnMatchtopcross)
	ON_BN_CLICKED(IDC_BUTTON_PUTBACKTopCorner, &CMagicCube2DDlg::OnPutbackTopcorner)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE_TOPCIRNER, &CMagicCube2DDlg::OnRestoreTopcorner)
END_MESSAGE_MAP()


// CMagicCube2DDlg message handlers

BOOL CMagicCube2DDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CClientDC dc(this);
	bmpMem.CreateCompatibleBitmap(&dc,bmpWidth,bmpHeight);
	pDCmem->CreateCompatibleDC(&dc);
	pDCmem->SelectObject(&bmpMem);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMagicCube2DDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMagicCube2DDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{	
		UpdateView();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMagicCube2DDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//check whether a point is in the quadrangle
bool CMagicCube2DDlg::IsInThisRect(CPoint* pts,CPoint point)
{
	if (pts[0].x==pts[1].x){
		if (pts[0].y==pts[3].y){//rectangle
			if (point.x>pts[0].x&&point.x<pts[3].x&&
				point.y<pts[0].y&&point.y>pts[1].y){
					return true;
			}
		}else{//irregular quadrangle:3
			int vdist=point.x-pts[0].x;
			if (point.x>pts[0].x&&point.x<pts[3].x&&
				point.y<pts[0].y-vdist&&point.y>pts[1].y-vdist){
					return true;
			}
		}		
	}else{//irregular quadrangle:4
		int vdist=pts[0].y-point.y;
		if (point.y<pts[0].y&&point.y>pts[1].y&&
			point.x>pts[0].x+vdist&&point.x<pts[3].x+vdist){
				return true;
		}
	}
	

	return false;
}
//get which aspect the point is in
Aspect CMagicCube2DDlg::GetAspect(CPoint point)
{	
	for (int i=0;i<6;i++){
		if (IsInThisRect(aspectVertexes[i],point)){
			return Aspect(i);
		}
	}
	return NOASPECT;
}
int CMagicCube2DDlg::GetSmallAspectIndex(Aspect asp,CPoint point)
{
	for (int i=0;i<9;i++){
		if (IsInThisRect(vertexes[asp][i],point)){
			return i;
		}
	}
	return -1;
}
void CMagicCube2DDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	point.x-=orgMem.x;
	point.y-=orgMem.y;
	Aspect asp=GetAspect(point);
	if (asp!=NOASPECT){
		int index=GetSmallAspectIndex(asp,point);
		if (index>=0){
			if (index==4){
				colorSelected=aspectColors[asp][index];
			}else{
				aspectColors[asp][index]=colorSelected;
			}
			UpdateView();
		}				
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CMagicCube2DDlg::UpdateView(void)
{
	CPen pen(PS_SOLID,1,BLACK_PEN);
	pDCmem->SelectObject(&pen);
	for (int i=0;i<6;i++){
		for (int asp=0;asp<9;asp++){
			pDCmem->SelectObject(&brushs[aspectColors[i][asp]]);
			pDCmem->Polygon(vertexes[i][asp],4);
		}
	}
	//show selected color
	CRect rc(org.x+(meshFullLen<<1),org.y,org.x+(meshFullLen<<1)+meshLen,org.y+meshLen);
	pDCmem->FillRect(&rc,&brushs[colorSelected]);
	//text
	CFont font;
	VERIFY(font.CreateFont(
		15,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial"));                 // lpszFacename
	pDCmem->SelectObject(&font);
	pDCmem->SetTextColor(RGB(255,255,255));
	pDCmem->SetBkColor(RGB(0,0,0));
	char* text="Selected Color";
	pDCmem->TextOut(rc.left-20,rc.bottom+3,text,strlen(text));

	CClientDC dc(this);
	dc.BitBlt(orgMem.x,orgMem.y,bmpWidth,bmpHeight,pDCmem,0,0,SRCCOPY);
}

void CMagicCube2DDlg::OnBnClickedButton_F()
{
	//pRotateColorTable[Z][POSITIVE][CLOCKWISE]();
	rotateZFrontClockwise();
	UpdateView();
}

void CMagicCube2DDlg::OnBnClickedButton_B()
{
	//pRotateColorTable[Z][NEGATIVE][COUNTERCLOCKWISE]();
	rotateZBackAnticlockwise();
	UpdateView();
}

void CMagicCube2DDlg::OnBnClickedButton_U()
{
	//pRotateColorTable[Y][POSITIVE][CLOCKWISE]();
	rotateYUpClockwise();
	UpdateView();
}

void CMagicCube2DDlg::OnBnClickedButton_D()
{
	//pRotateColorTable[Y][NEGATIVE][COUNTERCLOCKWISE]();
	rotateYDownAnticlockwise();
	UpdateView();
}

void CMagicCube2DDlg::OnBnClickedButton_R()
{
	//pRotateColorTable[X][POSITIVE][CLOCKWISE]();
	rotateXRightClockwise();
	UpdateView();
}

void CMagicCube2DDlg::OnBnClickedButton_L()
{
	//pRotateColorTable[X][NEGATIVE][COUNTERCLOCKWISE]();
	rotateXLeftAnticlockwise();
	UpdateView();
}

void CMagicCube2DDlg::OnBnClickedButtonReset()
{	
	memcpy(aspectColors,aspectColor_org,sizeof(aspectColor_org));
	colorSelected=BLUE;
	UpdateView();
}

void CMagicCube2DDlg::OnBnClickedButtonEmpty()
{
	for (int i=0;i<6;i++){
		for (int j=0;j<9;j++){
			if (j==4){
				aspectColors[i][j]=aspectColor_org[i][j];
			}else{
				aspectColors[i][j]=GRAY;
			}
		}
	}
	UpdateView();
}

void CMagicCube2DDlg::OnBnClickedButtonRandom()
{
	//reset first
	memcpy(aspectColors,aspectColor_org,sizeof(aspectColor_org));
	/*generate a random layout,with random rotation ,it will be better to 
	use cube scrambler */

	int scrambleNumber=30;
	while (scrambleNumber-->0){
		pRotateColorTable[rand()%3][rand()%2*2][rand()%2]();
	}
	UpdateView();
}
///////////////////////////////////////////////////
//auto solve
///////////////////////////////////////////////////

///////////////////////////////////////////////////
//operations
#undef L
#undef S

PRotateColorTable F=rotateZFrontClockwise;

PRotateColorTable Fr=rotateZFrontAnticlockwise;
PRotateColorTable B=rotateZBackAnticlockwise;
PRotateColorTable Br=rotateZBackClockwise;

PRotateColorTable R=rotateXRightClockwise;
PRotateColorTable Rr=rotateXRightAnticlockwise;
PRotateColorTable L=rotateXLeftAnticlockwise;
PRotateColorTable Lr=rotateXLeftClockwise;

PRotateColorTable U=rotateYUpClockwise;
PRotateColorTable Ur=rotateYUpAnticlockwise;
PRotateColorTable D=rotateYDownAnticlockwise;
PRotateColorTable Dr=rotateYDownClockwise;

//rotate middle layer along y axis clockwisely
PRotateColorTable E=rotateYMiddleClockwise;
PRotateColorTable Er=rotateYMiddleAnticlockwise;
void E2(){
	E();E();
}
//rotate middle layer along x axis clockwisely
PRotateColorTable M=rotateXMiddleClockwise;
PRotateColorTable Mr=rotateXMiddleAnticlockwise;
void M2(){
	E();E();
}
//rotate middle layer along z axis clockwisely
PRotateColorTable S=rotateZMiddleClockwise;
PRotateColorTable Sr=rotateZMiddleClockwise;
void S2(){
	S();S();
}

void F2(){
	F();F();
}
void B2(){
	B();B();
}
void R2(){
	R();R();
}
void L2(){
	L();L();
}
void U2(){
	U();U();
}
void D2(){
	D();D();
}
//rotate whole cube along x axis clockwisely
void x(){
	rotateXLeftClockwise();
	rotateXMiddleClockwise();
	rotateXRightClockwise();
}
void xr(){
	rotateXLeftAnticlockwise();
	rotateXMiddleAnticlockwise();
	rotateXRightAnticlockwise();
}
void x2(){
	x();x();
}
//rotate whole cube along y axis clockwisely
void y(){
	rotateYDownClockwise();
	rotateYMiddleClockwise();
	rotateYUpClockwise();
}
void yr(){
	rotateYDownAnticlockwise();
	rotateYMiddleAnticlockwise();
	rotateYUpAnticlockwise();
}
void y2(){
	y();y();
}
//rotate whole cube along z axis clockwisely
void z(){
	rotateZBackClockwise();
	rotateZMiddleClockwise();
	rotateZFrontClockwise();
}
void zr(){
	rotateZBackAnticlockwise();
	rotateZMiddleAnticlockwise();
	rotateZFrontAnticlockwise();
}
void z2(){
	z();z();
}
//rotate top two layer clockwisely
void u(){
	rotateYUpClockwise();
	rotateYMiddleClockwise();
}
void ur(){
	rotateYUpAnticlockwise();
	rotateYMiddleAnticlockwise();
}
void u2(){
	u();u();
}
//rotate right two layer clockwisely
void r(){
	rotateXRightClockwise();
	rotateXMiddleClockwise();
}
void rr(){
	rotateXRightAnticlockwise();
	rotateXMiddleAnticlockwise();
}
void r2(){
	r();r();
}
//rotate front two layer clockwisely
void f(){
	rotateZFrontClockwise();
	rotateZMiddleClockwise();
}
void fr(){
	rotateZFrontAnticlockwise();
	rotateZMiddleAnticlockwise();
}
void f2(){
	f();f();
}
//rotate down two layer anticlockwisely
void d(){
	rotateYMiddleAnticlockwise();
	rotateYDownAnticlockwise();
}
void dr(){
	rotateYDownClockwise();
	rotateYMiddleClockwise();
}
void d2(){
	d();d();
}
//rotate left two layer anticlockwisely
void l(){
	rotateXMiddleAnticlockwise();
	rotateXLeftAnticlockwise();
}
void lr(){
	rotateXMiddleClockwise();
	rotateXLeftClockwise();
}
void l2(){
	l();l();
}
//rotate back two layer anticlockwisely
void b(){
	rotateZMiddleAnticlockwise();
	rotateZBackAnticlockwise();
}
void br(){
	rotateZMiddleClockwise();
	rotateZBackClockwise();
}
void b2(){
	b();b();
}

///////////////////////////////////////////////////
//internal operations ,only command string manipulation added.
//another way to do this is set parameter for these functions.
//e.g. F(bool b){rotate and if b ,then string manipulation }
//for such short functions ,I prefer write another set of functions.
///////////////////////////////////////////////////
#define COMMANDLEN 500
//store operation string
static char cmdString[COMMANDLEN];
static int cmdStrPtr;

void _F(){
	F();
	cmdString[cmdStrPtr++]='F';
}
void _Fr(){
	Fr();
	cmdString[cmdStrPtr++]='F';
	cmdString[cmdStrPtr++]='\'';
}
void _F2(){
	F();F();
	cmdString[cmdStrPtr++]='F';
	cmdString[cmdStrPtr++]='2';
}
void _B(){
	B();
	cmdString[cmdStrPtr++]='B';
}
void _Br(){
	Br();
	cmdString[cmdStrPtr++]='B';
	cmdString[cmdStrPtr++]='\'';
}
void _B2(){
	B();B();
	cmdString[cmdStrPtr++]='B';
	cmdString[cmdStrPtr++]='2';
}
void _R(){
	R();
	cmdString[cmdStrPtr++]='R';
}
void _Rr(){
	Rr();
	cmdString[cmdStrPtr++]='R';
	cmdString[cmdStrPtr++]='\'';
}
void _R2(){
	R();R();
	cmdString[cmdStrPtr++]='R';
	cmdString[cmdStrPtr++]='2';
}
void _L(){
	L();
	cmdString[cmdStrPtr++]='L';
}
void _Lr(){
	Lr();
	cmdString[cmdStrPtr++]='L';
	cmdString[cmdStrPtr++]='\'';
}
void _L2(){
	L();L();
	cmdString[cmdStrPtr++]='L';
	cmdString[cmdStrPtr++]='2';
}
void _U(){
	U();
	cmdString[cmdStrPtr++]='U';
}
void _Ur(){
	Ur();
	cmdString[cmdStrPtr++]='U';
	cmdString[cmdStrPtr++]='\'';
}
void _U2(){
	U();U();
	cmdString[cmdStrPtr++]='U';
	cmdString[cmdStrPtr++]='2';
}
void _D(){
	D();
	cmdString[cmdStrPtr++]='D';
}
void _Dr(){
	Dr();
	cmdString[cmdStrPtr++]='D';
	cmdString[cmdStrPtr++]='\'';
}
void _D2(){
	D();D();
	cmdString[cmdStrPtr++]='D';
	cmdString[cmdStrPtr++]='2';
}
void _E(){
	E();
	cmdString[cmdStrPtr++]='E';
}
void _Er(){
	Er();
	cmdString[cmdStrPtr++]='E';
	cmdString[cmdStrPtr++]='\'';
}
void _E2(){
	E();E();
	cmdString[cmdStrPtr++]='E';
	cmdString[cmdStrPtr++]='2';
}
void _M(){
	M();
	cmdString[cmdStrPtr++]='M';
}
void _Mr(){
	Mr();
	cmdString[cmdStrPtr++]='M';
	cmdString[cmdStrPtr++]='\'';
}
//interesting ,some functions' name are macros defined by vc
#undef _M2
void _M2(){
	M();M();
	cmdString[cmdStrPtr++]='M';
	cmdString[cmdStrPtr++]='2';
}
void _S(){
	S();
	cmdString[cmdStrPtr++]='S';
}
void _Sr(){
	Sr();
	cmdString[cmdStrPtr++]='S';
	cmdString[cmdStrPtr++]='\'';
}
void _S2(){
	S();S();
	cmdString[cmdStrPtr++]='S';
	cmdString[cmdStrPtr++]='2';
}
void _x(){
	x();
	cmdString[cmdStrPtr++]='x';
}
void _xr(){
	xr();
	cmdString[cmdStrPtr++]='x';
	cmdString[cmdStrPtr++]='\'';
}
void _x2(){
	x();x();
	cmdString[cmdStrPtr++]='x';
	cmdString[cmdStrPtr++]='2';
}
void _y(){
	y();
	cmdString[cmdStrPtr++]='y';
}
void _yr(){
	yr();
	cmdString[cmdStrPtr++]='y';
	cmdString[cmdStrPtr++]='\'';
}
void _y2(){
	y();y();
	cmdString[cmdStrPtr++]='y';
	cmdString[cmdStrPtr++]='2';
}
void _z(){
	z();
	cmdString[cmdStrPtr++]='z';
}
void _zr(){
	zr();
	cmdString[cmdStrPtr++]='z';
	cmdString[cmdStrPtr++]='\'';
}
void _z2(){
	z();z();
	cmdString[cmdStrPtr++]='z';
	cmdString[cmdStrPtr++]='2';
}
void _u(){
	u();
	cmdString[cmdStrPtr++]='u';
}
void _ur(){
	ur();
	cmdString[cmdStrPtr++]='u';
	cmdString[cmdStrPtr++]='\'';
}
void _u2(){
	u();u();
	cmdString[cmdStrPtr++]='u';
	cmdString[cmdStrPtr++]='2';
}
void _r(){
	r();
	cmdString[cmdStrPtr++]='r';
}
void _rr(){
	rr();
	cmdString[cmdStrPtr++]='r';
	cmdString[cmdStrPtr++]='\'';
}
void _r2(){
	r();r();
	cmdString[cmdStrPtr++]='r';
	cmdString[cmdStrPtr++]='2';
}
void _f(){
	f();
	cmdString[cmdStrPtr++]='f';
}
void _fr(){
	fr();
	cmdString[cmdStrPtr++]='f';
	cmdString[cmdStrPtr++]='\'';
}
void _f2(){
	f();f();
	cmdString[cmdStrPtr++]='f';
	cmdString[cmdStrPtr++]='2';
}
void _d(){
	d();
	cmdString[cmdStrPtr++]='d';
}
void _dr(){
	dr();
	cmdString[cmdStrPtr++]='d';
	cmdString[cmdStrPtr++]='\'';
}
void _d2(){
	d();d();
	cmdString[cmdStrPtr++]='d';
	cmdString[cmdStrPtr++]='2';
}
void _l(){
	l();
	cmdString[cmdStrPtr++]='l';
}
void _lr(){
	lr();
	cmdString[cmdStrPtr++]='l';
	cmdString[cmdStrPtr++]='\'';
}
void _l2(){
	l();l();
	cmdString[cmdStrPtr++]='l';
	cmdString[cmdStrPtr++]='2';
}
void _b(){
	b();
	cmdString[cmdStrPtr++]='b';
}
void _br(){
	br();
	cmdString[cmdStrPtr++]='b';
	cmdString[cmdStrPtr++]='\'';
}
void _b2(){
	b();b();
	cmdString[cmdStrPtr++]='b';
	cmdString[cmdStrPtr++]='2';
}
///////////////////////////////////////////////////



//check whether the user edited cube is a valid cube
bool CMagicCube2DDlg::IsValidLayout(void)
{
	return false;
}
//find the four facelets on edge for a color
void CMagicCube2DDlg::GetFacelet4(Facelet* facelet,Color color)
{
	int pointer=0;
	int index[4]={1,3,5,7};//facelets numbered 1,3,5,7 is on edges
	for (int i=0;i<6;i++){		
		for (int j=0;j<4;j++){
			if (aspectColors[i][index[j]]==color){			
				facelet[pointer].asp=(Aspect)i;
				facelet[pointer].index=index[j];
				facelet[pointer].color=aspectColors[facelet[pointer].asp][facelet[pointer].index];
				pointer++;
				if (pointer>=4){//all have been found
					return;
				}
			}
		}
	}
	ASSERT(pointer==4);
}
//find the other facelet on the same edge
Facelet CMagicCube2DDlg::GetNeighborEdgeFacelet(Facelet facelet)
{
	Facelet faceletNeighbor;
	switch(facelet.asp){
		case FRONT:
			faceletNeighbor.index=7;
			switch (facelet.index){				
				case 1:
					faceletNeighbor.asp=DOWN;
					break;
				case 3:
					faceletNeighbor.asp=LEFT;
					break;
				case 5:
					faceletNeighbor.asp=RIGHT;
					break;
				case 7:
					faceletNeighbor.asp=UP;
					break;
				default:
					break;
			}
			break;
		case BACK:
			faceletNeighbor.index=1;
			switch (facelet.index){				
				case 1:
					faceletNeighbor.asp=DOWN;
					break;
				case 3:
					faceletNeighbor.asp=LEFT;
					break;
				case 5:
					faceletNeighbor.asp=RIGHT;
					break;
				case 7:
					faceletNeighbor.asp=UP;
					break;
				default:
					break;
			}
			break;
		case RIGHT:
			faceletNeighbor.index=5;
			switch (facelet.index){				
				case 1:					
					faceletNeighbor.asp=BACK;
					break;
				case 3:
					faceletNeighbor.asp=DOWN;
					break;
				case 5:
					faceletNeighbor.asp=UP;
					break;
				case 7:
					faceletNeighbor.asp=FRONT;
					break;
				default:
					break;
			}
			break;
		case LEFT:
			faceletNeighbor.index=3;
			switch (facelet.index){				
				case 1:
					faceletNeighbor.asp=BACK;
					break;
				case 3:
					faceletNeighbor.asp=DOWN;
					break;
				case 5:
					faceletNeighbor.asp=UP;
					break;
				case 7:
					faceletNeighbor.asp=FRONT;
					break;
				default:
					break;
			}
			break;
		case UP:
			switch (facelet.index){
				case 1:
					faceletNeighbor.asp=BACK;
					faceletNeighbor.index=7;
					break;
				case 3:
					faceletNeighbor.asp=LEFT;
					faceletNeighbor.index=5;
					break;
				case 5:
					faceletNeighbor.asp=RIGHT;
					faceletNeighbor.index=5;
					break;
				case 7:
					faceletNeighbor.asp=FRONT;
					faceletNeighbor.index=7;
					break;
				default:
					break;
			}
			break;
		case DOWN:
			switch (facelet.index){
				case 1:
					faceletNeighbor.asp=BACK;
					faceletNeighbor.index=1;
					break;
				case 3:
					faceletNeighbor.asp=LEFT;
					faceletNeighbor.index=3;
					break;
				case 5:
					faceletNeighbor.asp=RIGHT;
					faceletNeighbor.index=3;
					break;
				case 7:
					faceletNeighbor.asp=FRONT;
					faceletNeighbor.index=1;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	faceletNeighbor.color=aspectColors[faceletNeighbor.asp][faceletNeighbor.index];
	return faceletNeighbor;
}
//find the other two facelets on the same corner
void CMagicCube2DDlg::GetNeighborCornerFacelets(Facelet* neighbor2,Facelet facelet)
{
	if (facelet.asp==FRONT){
		switch(facelet.index){
			case 0:
				neighbor2[0].asp=LEFT;
				neighbor2[0].index=6;
				neighbor2[1].asp=DOWN;
				neighbor2[1].index=6;
				break;
			case 2:
				neighbor2[0].asp=DOWN;
				neighbor2[0].index=8;
				neighbor2[1].asp=RIGHT;
				neighbor2[1].index=6;
				break;
			case 6:
				neighbor2[0].asp=LEFT;
				neighbor2[0].index=8;
				neighbor2[1].asp=UP;
				neighbor2[1].index=6;
				break;
			case 8:
				neighbor2[0].asp=UP;
				neighbor2[0].index=8;
				neighbor2[1].asp=RIGHT;
				neighbor2[1].index=8;
				break;
			default:
				break;
		}
	}else if(facelet.asp==BACK){
		switch(facelet.index){
			case 0:
				neighbor2[0].asp=LEFT;
				neighbor2[0].index=0;
				neighbor2[1].asp=DOWN;
				neighbor2[1].index=0;
				break;
			case 2:
				neighbor2[0].asp=DOWN;
				neighbor2[0].index=2;
				neighbor2[1].asp=RIGHT;
				neighbor2[1].index=0;
				break;
			case 6:
				neighbor2[0].asp=UP;
				neighbor2[0].index=0;
				neighbor2[1].asp=LEFT;
				neighbor2[1].index=2;
				break;
			case 8:
				neighbor2[0].asp=UP;
				neighbor2[0].index=2;
				neighbor2[1].asp=RIGHT;
				neighbor2[1].index=2;
				break;
			default:
				break;
		}
	}
	
	neighbor2[0].color=aspectColors[neighbor2[0].asp][neighbor2[0].index];
	neighbor2[1].color=aspectColors[neighbor2[1].asp][neighbor2[1].index];
}
//find the edge contains the two given color ,in which first is color 
//main face ,the second is color of side face
Edge CMagicCube2DDlg::GetEdge(Color colorMain,Color colorSide)
{
	Edge edge;
	Facelet facelets[4];
	GetFacelet4(facelets,colorMain);
	Facelet faceletOther;
	for (int i=0;i<4;i++){
		faceletOther=GetNeighborEdgeFacelet(facelets[i]);
		if (faceletOther.color==colorSide){
			edge.faceletMain=facelets[i];
			edge.faceletSide=faceletOther;
			return edge;
		}
	}
	return edge;
}
//rotate cube to cross state,and the cross is built on down face
CString CMagicCube2DDlg::Cross(void)
{
	//two method to make cross:rule database or search
	//rules are used here
	
	/* for an edge which contained in the cross,there are 24 situations.
	 *but I categorize them into 3 kinds in a clear way which came up my
	 *mind in a sudden.Set front face is the face on which the cross will
	 *be built.colorMain on LEFT/RIGHT/BACK contains 12 situations,while 
	 *colorSide on LEFT/RIGHT/BACK contains 12 situations too.
	 *4 situations was repetive.there 4 situations are on the front face.
	 */
	memset(cmdString,0,COMMANDLEN);
	cmdStrPtr=0;

	int testCount=0;
	while (!IsCrossed()){		
		testCount++;
		if (testCount>3){
			CString filename="bug/Cross.ml";
			CFile fileout(filename.GetBuffer(),CFile::modeCreate|CFile::modeWrite);
			fileout.Write(aspectColors,sizeof(aspectColors));
			fileout.Close();
			MessageBox("bug emerged in function:Cross,saved as Cross.ml");
			break;
		}

		//record number of faces done 
		int numDone=0;

		//check whether there is a facelet is of same color with center on FRONT face
		/*Facelet facelet;
		facelet.asp=FRONT;
		for (int i=1;i<9;i+=2){
			if (aspectColors[FRONT][i]==aspectColors[FRONT][4]){
				facelet.index=i;
				facelet.color=aspectColors[FRONT][4];
				break;
			}
		}*/
		//it will work still,if this 'if' sentence does not exist.
		//this is a special situation detection
		//if (facelet.index>=0){//found
		//	//turn the facelet to left
		//	if (facelet.index==1){
		//		_F();
		//	}else if (facelet.index==7){
		//		_Fr();
		//	}else if (facelet.index==5){
		//		_F2();
		//	}
		//	//get neighbor's color
		//	Facelet f=GetNeighborEdgeFacelet(facelet);
		//	//turn corresponding face to position
		//	if(aspectColors[3][4]==f.color){
		//		_b2();
		//	}else if (aspectColors[4][4]==f.color){
		//		_b();
		//	}else if (aspectColors[5][4]==f.color){
		//		_br();
		//	}
		//	numDone=1;
		//	_zr();
		//}
		while (numDone<4){
			//find edges
			Edge edge=GetEdge(aspectColors[FRONT][4],aspectColors[UP][4]);
			if (edge.faceletMain.asp==LEFT){//main facelet 
				//move main color to position 5 first
				if (edge.faceletMain.index==7){
					_Lr();_Ur();
				}else if (edge.faceletMain.index==1){
					/* numDone estimation is to check whether there is a need to 
					*turn side rotated back .this can reduce the steps needed to
					*restore the cube.but is hindered optimization of program*/
					_L();_Ur();
					if (numDone!=0){
						_Lr();
					}
				}else if (edge.faceletMain.index==3){										
					_L2();_Ur();
					if (numDone!=0){
						_L2();
					}
				}else if (edge.faceletMain.index==5){//5
					_Ur();
				}
				//then
				//_Ur();
			}else if (edge.faceletMain.asp==RIGHT){
				//move main color to position 5 first
				if (edge.faceletMain.index==7){
					_R();_U();
				}else if (edge.faceletMain.index==1){
					_Rr();_U();
					if (numDone==3){
						_R();
					}
				}else if (edge.faceletMain.index==3){
					_R2();_U();
					if (numDone==3){
						_R2();
					}
				}else if (edge.faceletMain.index==5){//5
					_U();
				}
				//then
				//_U();
			}else if (edge.faceletMain.asp==BACK&&edge.faceletMain.index==1){
				//move main color to position 7 first			
				_B2();_U2();
			}else if (edge.faceletMain.asp==BACK&&edge.faceletMain.index==7){
					_U2();
			
				/*else if (edge.faceletMain.index==3){
					_Br();
				}else if (edge.faceletMain.index==5){
					_B();
				}*/
				//then
				//_U2();
			}else if (edge.faceletSide.asp==LEFT){//side facelet
				//move side color to position 1 first
				if (edge.faceletSide.index==3){
					_L();_Br();
					if (numDone!=0){
						_Lr();
					}	
					_U2();
				}else if (edge.faceletSide.index==5){
					_Lr();_Br();
					if (numDone!=0){
						_L();
					}	
					_U2();
				}else if (edge.faceletSide.index==7){
					if (numDone==0){
						_F();
					}else{
						_L2();_Br();
						if (numDone!=0){
							_L2();
						}
						_U2();
					}				
				}else if (edge.faceletSide.index==1){
					_Br();_U2();
				}
				//then
				//_U2();
			}else if (edge.faceletSide.asp==RIGHT){
				//move side color to position 1 first
				if (edge.faceletSide.index==3){
					_Rr();_B();
					if (numDone==3){
						_R();
					}
					_U2();
				}else if (edge.faceletSide.index==5){
					_R();_B();
					if (numDone==3){
						_Rr();
					}
					_U2();
				}else if (edge.faceletSide.index==7){
					if (numDone==0){
						_Fr();
					}else{
						_R2();_B();
						if (numDone==3){
							_R2();
						}
						_U2();
					}				
				}else if (edge.faceletSide.index==1){
					_Br();_U2();
				}
				//then
				//_U2();
			}else if (edge.faceletSide.asp==BACK&&edge.faceletSide.index==7){
				//for the symmetry of code,I did not merge the code below
		
				_B();_L();_Ur();
				if (numDone!=0){
					_Lr();
				}
			}/*else if (edge.faceletSide.index==3){
					_L();_Ur();
					if (numDone!=0){
						_Lr();
					}
				}*/else if (edge.faceletSide.asp==BACK&&edge.faceletSide.index==1){
					_B();_Rr();_U();
					if (numDone==3){
						_R();
					}
				/*else if (edge.faceletSide.index==5){
					_Rr();_U();
					if (numDone==3){
						_R();
					}
				}*/
			}else if (edge.faceletMain.asp==FRONT){
				if (edge.faceletMain.index==1){//special situations on FRONT face
					if (numDone==0){
						_F2();
					}else{
						_D2();_B2();_U2();					
					}				
				}//else{
				//	//in correct position
				//}
			}else if (edge.faceletSide.asp==FRONT){
				if (edge.faceletSide.index==1){
					_D();_Rr();_B();_U2();
					if (numDone==3){
						_R();
					}
				}else{//7
					_U();_Lr();_Br();_U2();
					if (numDone!=0){
						_L();
					}
				}
			}	
			
			_zr();		
			numDone++;
		}
	}
	
	return CString(cmdString);

}
//whether the cross has been built
bool CMagicCube2DDlg::IsCrossed()
{	
	for (int i=1;i<8;i+=2){
		if (aspectColors[FRONT][4]!=aspectColors[FRONT][i]){
			return false;
		}
	}
	for (int i=2;i<=5;i++){
		if (aspectColors[i][4]!=aspectColors[i][7]){
			return false;
		}
	}
	return true;

}
//whether corner of first layer is ready
bool CMagicCube2DDlg::IsFirstLayerCornerRestored()
{
	if (aspectColors[FRONT][0]==aspectColors[FRONT][1]&&
		aspectColors[FRONT][2]==aspectColors[FRONT][1]&&
		aspectColors[BACK][0]==aspectColors[BACK][1]&&
		aspectColors[BACK][2]==aspectColors[BACK][1]&&
		aspectColors[LEFT][0]==aspectColors[LEFT][3]&&
		aspectColors[LEFT][6]==aspectColors[LEFT][3]&&
		aspectColors[RIGHT][0]==aspectColors[RIGHT][3]&&
		aspectColors[RIGHT][6]==aspectColors[RIGHT][3]){
			bool allSame=true;
			for (int i=1;i<9;i+=2){
				if (aspectColors[DOWN][i]!=aspectColors[DOWN][4]){
					allSame=false;
					break;
				}
			}
			if (allSame){
				return true;
			}
	}
	return false;
}
//restore corners of first layer
CString CMagicCube2DDlg::RestoreFirstLayerCorner()
{
	memset(cmdString,0,COMMANDLEN);
	cmdStrPtr=0;

	int testCount=0;
	while (!IsFirstLayerCornerRestored()){
		testCount++;
		if (testCount>20){
			MessageBox("bug emerged in function:RestoreFirstLayerCorner");
			CString filename="bug/RestoreFirstLayerCorner.ml";
			CFile fileout(filename.GetBuffer(),CFile::modeCreate|CFile::modeWrite);
			fileout.Write(aspectColors,sizeof(aspectColors));
			fileout.Close();
			exit(-1);
		}
		bool searchUp=true;
		//search corners on top which should be on bottom
		for (int i=0;i<4;i++){
			bool found=false;
			//find and adjust the correct corner block
			if (aspectColors[FRONT][6]==aspectColors[DOWN][4]){//left
				if (aspectColors[UP][6]==aspectColors[FRONT][4]){// UP FRONT
					if (aspectColors[LEFT][8]==aspectColors[LEFT][4]){//LEFT LEFT
						_yr();
					}else{//LEFT RIGHT
						_Ur();
					}
				}else if (aspectColors[UP][6]==aspectColors[LEFT][4]){//UP LEFT
					if (aspectColors[LEFT][8]==aspectColors[FRONT][4]){//LEFT FRONT
						_yr();
					}else{//LEFT BACK
						_U();_y2();
					}
				}else if (aspectColors[LEFT][8]==aspectColors[FRONT][4]){//LEFT FRONT
					_Ur();
				}else if (aspectColors[LEFT][8]==aspectColors[LEFT][4]){//LEFT LEFT
					_U();_y2();
				}else {//left front corner should be on right back
					_U2();_y();
				}
				found=true;
			}else if (aspectColors[FRONT][8]==aspectColors[DOWN][4]){//right
				if (aspectColors[UP][8]==aspectColors[FRONT][4]){// UP FRONT
					if (aspectColors[RIGHT][8]==aspectColors[RIGHT][4]){//RIGHT RIGHT
						//on correct position
					}else{//RIGHT LEFT
						//_U();_yr();
						_d();
					}
				}else if (aspectColors[UP][8]==aspectColors[RIGHT][4]){//UP RIGHT
					if (aspectColors[RIGHT][8]==aspectColors[FRONT][4]){//RIGHT FRONT
						//on correct position
					}else{//RIGHT BACK
						//_Ur();_y();
						_dr();
					}
				}else if (aspectColors[RIGHT][8]==aspectColors[FRONT][4]){//RIGHT FRONT
					_d();
				}else if (aspectColors[RIGHT][8]==aspectColors[RIGHT][4]){//RIGHT RIGHT
					_dr();
				}else{//right front corner should be on left back
					_U2();_y2();
				}
				found=true;
			}
			
			if (found){//if found ,use formula to restore the corner
				//the corner is on FRT corner	
				if (aspectColors[FRONT][8]==aspectColors[DOWN][4]){
					_U();_R();_Ur();_Rr();
				}else{
					_R();_U();_Rr();
				}
				searchUp=false;
				break;/* for */
			}else{//if not found,search next side
				_y();
			}
		}
		//search top if target not found on sides
		bool searchDown=false;
		if (!searchUp){
			continue;/* while */
		}else{
			bool found=true;
			//search top and adjust 
			if (aspectColors[UP][0]==aspectColors[DOWN][4]){
				_y2();
			}else if (aspectColors[UP][2]==aspectColors[DOWN][4]){
				_y();
			}else if (aspectColors[UP][6]==aspectColors[DOWN][4]){
				_yr();
			}else if (aspectColors[UP][8]==aspectColors[DOWN][4]){
				//correct
			}else {
				searchDown=true;
				found=false;
			}
			if (found){
				//adjustment
				if (aspectColors[FRONT][8]==aspectColors[FRONT][4]){
					_d();
				}else if (aspectColors[RIGHT][8]==aspectColors[RIGHT][4]){
					_dr();
				}else {
					_d2();
				}
				//formula
				_R();_U2();_Rr();_Ur();
				//then to another situation
				//_R();_U();_Rr();
			}
		}
		if (searchDown){
			//search bottom and adjust
			for (int i=0;i<4;i++){
				bool found=true;
				if (aspectColors[FRONT][0]==aspectColors[DOWN][4]){
					_yr();
				}else if (aspectColors[FRONT][2]==aspectColors[DOWN][4]){
					//correct
				}else{
					if (aspectColors[DOWN][6]==aspectColors[DOWN][4]&&
						aspectColors[FRONT][0]!=aspectColors[FRONT][4]){
							_R();_U();_Rr();							
					}
					_y();
					found=false;
				}
				if (found){
					//restore
					if (aspectColors[RIGHT][6]==aspectColors[DOWN][4]){
						_R();_U();_Rr();_U();
						//then
						//_R();_U();_Rr();
					}else if (aspectColors[FRONT][2]==aspectColors[DOWN][4]){
						_R();_Ur();_Rr();
						//then
						//_U();_R();_Ur();_Rr();
					}
					break;
				}
			}
		}
	}
	return CString(cmdString);
}
//whether edges on second layer are restored
bool CMagicCube2DDlg::IsSecondLayerEdgeRestored(void)
{
	if (aspectColors[FRONT][4]==aspectColors[FRONT][3]&&
		aspectColors[FRONT][4]==aspectColors[FRONT][5]&&
		aspectColors[BACK][4]==aspectColors[BACK][3]&&
		aspectColors[BACK][4]==aspectColors[BACK][5]&&
		aspectColors[LEFT][4]==aspectColors[LEFT][1]&&
		aspectColors[LEFT][4]==aspectColors[LEFT][7]&&
		aspectColors[RIGHT][4]==aspectColors[RIGHT][1]&&
		aspectColors[RIGHT][4]==aspectColors[RIGHT][7]){
			return true;
	}
	return false;
}

//restore edges of second layer
CString CMagicCube2DDlg::RestoreSecondLayerEdge()
{
	ASSERT(IsFirstLayerCornerRestored());

	memset(cmdString,0,COMMANDLEN);
	cmdStrPtr=0;

	int testCount=0;
	while(!IsSecondLayerEdgeRestored()){
		testCount++;
		if (testCount>10){
			MessageBox("bug emerged in function:RestoreSecondLayerEdge");
			CString filename="bug/RestoreSecondLayerEdge.ml";
			CFile fileout(filename.GetBuffer(),CFile::modeCreate|CFile::modeWrite);
			fileout.Write(aspectColors,sizeof(aspectColors));
			fileout.Close();
			exit(-1);
		}
		bool found=false;
		for (int i=0;i<4;i++){
			if (aspectColors[FRONT][7]!=aspectColors[UP][4]&&
				aspectColors[UP][7]!=aspectColors[UP][4]){//found			
					if (aspectColors[FRONT][7]!=aspectColors[FRONT][4]){//front not matched
						//rotate to match
						if (aspectColors[FRONT][7]==aspectColors[LEFT][4]){
							_d();
						}else if (aspectColors[FRONT][7]==aspectColors[RIGHT][4]){
							_dr();
						}else if (aspectColors[FRONT][7]==aspectColors[BACK][4]){
							_d2();
						}
					}
					_yr();//adjustment
					//formula					
					if (aspectColors[UP][5]==aspectColors[FRONT][4]){//RIGHT FRONT
						_Rr();_Ur();_Rr();_Ur();_Rr();_U();_R();_U();_R();
					}else {//RIGHT BACK
						_R();_U();_R();_U();_R();_Ur();_Rr();_Ur();_Rr();
					}

					found=true;
					break;
			}
			_y();
		}
		if (!found){//search middle layer
			for (int i=0;i<4;i++){
				if (aspectColors[FRONT][5]!=aspectColors[FRONT][4]){
					_Rr();_Ur();_Rr();_Ur();_Rr();_U();_R();_U();_R();
					break;
				}
				_y();
			}
		}
	}
	return CString(cmdString);
}
//whether F2L is done
bool CMagicCube2DDlg::IsF2L()
{
	if (IsFirstLayerCornerRestored()&&
		IsSecondLayerEdgeRestored()){
			return true;
	}
	return false;
}
//restore First 2 Layer
CString CMagicCube2DDlg::F2L(void)
{
	CString ret;
	ret="#Cross:\r\n\t#";
	//make cross
	ret+=Cross();
	xr();
	ret+="#\r\nRotate to bottom:\r\n\t#";
	ret+="x'";
	//restore corners of first layer
	ret+="#\r\nRestore Corners of First Layer:\r\n\t#";
	ret+=RestoreFirstLayerCorner();
	//restore edges of second layer
	ret+="#\r\nRestore Edges of Second Layer:\r\n\t#";
	ret+=RestoreSecondLayerEdge();

	return ret;
}
///////////////////////////////////////////////////
//method 1:ferwer formula
//reference:Meissen 
///////////////////////////////////////////////////
//whether top cross is built
bool CMagicCube2DDlg::IsTopCrossed()
{
	for (int i=1;i<9;i+=2){
		if (aspectColors[UP][i]!=aspectColors[UP][4]){
			return false;
		}
	}
	return true;
}
//built cross on top
CString CMagicCube2DDlg::BuildTopCross()
{
	ASSERT(IsF2L());
	
	memset(cmdString,0,COMMANDLEN);
	cmdStrPtr=0;

	int testCount=0;
	while(!IsTopCrossed()){
		testCount++;
		if (testCount>5){
			CString filename="bug/BuildTopCross.ml";
			CFile fileout(filename.GetBuffer(),CFile::modeCreate|CFile::modeWrite);
			fileout.Write(aspectColors,sizeof(aspectColors));
			fileout.Close();
			MessageBox("bug emerged in function:BuildTopCross");
			exit(-1);
		}

		if (aspectColors[UP][3]==aspectColors[UP][4]&&
			aspectColors[UP][5]==aspectColors[UP][4]){//situation 1a
				if (aspectColors[FRONT][7]!=aspectColors[UP][4]){
					//adjustment
					_y2();
				}
				_F();_R();_U();_Rr();_Ur();_Fr();
		}else if (aspectColors[UP][1]==aspectColors[UP][4]&&//situation 1b
			aspectColors[UP][7]==aspectColors[UP][4]){
				_y();
				if (aspectColors[FRONT][7]!=aspectColors[UP][4]){
					//adjustment
					_y2();
				}
				_F();_R();_U();_Rr();_Ur();_Fr();
		}else if (aspectColors[UP][3]==aspectColors[UP][4]){//situation 2a
			bool isTwo=false;
			if(aspectColors[UP][7]==aspectColors[UP][4]){
				_y();
				isTwo=true;
			}else if (aspectColors[UP][1]==aspectColors[UP][4]){
				isTwo=true;
			}
			if (isTwo){
				_F();_U();_R();_Ur();_Rr();_Fr();
			}
		}else if (aspectColors[UP][5]==aspectColors[UP][4]){//situation 2b
			bool isTwo=false;
			if(aspectColors[UP][7]==aspectColors[UP][4]){
				_y2();
				isTwo=true;
			}else if (aspectColors[UP][1]==aspectColors[UP][4]){
				_yr();
				isTwo=true;
			}
			if (isTwo){
				_F();_U();_R();_Ur();_Rr();_Fr();
			}
		}else{//situation 3
			for (int i=0;i<4;i++){
				if (aspectColors[FRONT][7]==aspectColors[UP][4]&&
					aspectColors[RIGHT][5]==aspectColors[UP][4]){
						_F();_U();_R();_Ur();_Rr();_Fr();
						//then
						//_F();_R();_U();_Rr();_Ur();_Fr();
						break;
				}
				_y();
			}			
		}
	}
	return CString(cmdString);
}

//
bool CMagicCube2DDlg::IsTopCrossMatched(void)
{
	if (aspectColors[FRONT][7]!=aspectColors[FRONT][4]){
		return false;
	}
	if (aspectColors[BACK][7]!=aspectColors[BACK][4]){
		return false;
	}
	if (aspectColors[LEFT][5]!=aspectColors[LEFT][4]){
		return false;
	}
	if (aspectColors[RIGHT][5]!=aspectColors[RIGHT][4]){
		return false;
	}
	return true;
}
CString CMagicCube2DDlg::MatchTopCross(void)
{
	ASSERT(IsTopCrossed());
	
	memset(cmdString,0,COMMANDLEN);
	cmdStrPtr=0;

	int testCount=0;
	while (!IsTopCrossMatched()){
		testCount++;
		if (testCount>1){
			MessageBox("bug emerged in function:MatchTopCross,saved in MatchTopCross.ml");
			CString filename="bug/MatchTopCross.ml";
			CFile fileout(filename.GetBuffer(filename.GetLength()),CFile::modeCreate|CFile::modeWrite);
			fileout.Write(aspectColors,sizeof(aspectColors));
			fileout.Close();
			exit(-1);
		}
		//match edge first,then use formula
		for (int jj=0;jj<4;jj++){
			//match
			for (int i=0;i<4;i++){//match Back
				if (aspectColors[BACK][4]!=aspectColors[BACK][7]){
					_U();
				}else{
					break;
				}
			}
			if (IsTopCrossMatched()){
				return CString(cmdString);
			}
			//formula
			if (aspectColors[LEFT][4]==aspectColors[LEFT][5]){//LEFT match
				char* formula="(RU2R')(U'RU'R')U'";
				Executor(formula);
				strcpy(cmdString+cmdStrPtr,formula);
				cmdStrPtr+=strlen(formula);
				return CString(cmdString);
			}else if (aspectColors[FRONT][4]==aspectColors[FRONT][7]){//RIGHT match
				char* formula="(RU2R')(U'RU'R')y(RU2R')(U'RU'R')U'";
				Executor(formula);
				strcpy(cmdString+cmdStrPtr,formula);
				cmdStrPtr+=strlen(formula);
				return CString(cmdString);
			}
			_yr();
		}
		MessageBox("special situation not recorded found,saved in file :MatchTopCross-special.ml");
		CString filename="MatchTopCross-special.ml";
		CFile fileout(filename.GetBuffer(filename.GetLength()),CFile::modeCreate|CFile::modeWrite);
		fileout.Write(aspectColors,sizeof(aspectColors));
		fileout.Close();
	}	
	return CString(cmdString);
}
bool CMagicCube2DDlg::IsTopCornerBack(void)
{
	int cornerHashDst[4];//hash value of this corner should be
	int cornerHashSrc[4];//hash value for current corners
	//start from leftFront corner,counterclockwise
	cornerHashDst[0]=HashCorner(aspectColors[UP][4],aspectColors[LEFT][4],aspectColors[FRONT][4]);
	cornerHashDst[1]=HashCorner(aspectColors[UP][4],aspectColors[FRONT][4],aspectColors[RIGHT][4]);
	cornerHashDst[2]=HashCorner(aspectColors[UP][4],aspectColors[RIGHT][4],aspectColors[BACK][4]);
	cornerHashDst[3]=HashCorner(aspectColors[UP][4],aspectColors[BACK][4],aspectColors[LEFT][4]);

	cornerHashSrc[0]=HashCorner(aspectColors[UP][6],aspectColors[LEFT][8],aspectColors[FRONT][6]);
	cornerHashSrc[1]=HashCorner(aspectColors[UP][8],aspectColors[FRONT][8],aspectColors[RIGHT][8]);
	cornerHashSrc[2]=HashCorner(aspectColors[UP][2],aspectColors[RIGHT][2],aspectColors[BACK][8]);
	cornerHashSrc[3]=HashCorner(aspectColors[UP][0],aspectColors[BACK][6],aspectColors[LEFT][2]);
	for (int i=0;i<4;i++){
		if (cornerHashSrc[i]!=cornerHashDst[i]){
			return false;
		}
	}
	return true;
}
//calculate a hash value for a corner to differ from other corners
//according to its colors.there are various methods
int CMagicCube2DDlg::HashCorner(Color c1,Color c2,Color c3)
{
	return (c1+1)*(c2+1)*(c3+1);
}
CString CMagicCube2DDlg::PutBackTopCorner(void)
{
	ASSERT(IsTopCrossMatched());
	
	memset(cmdString,0,COMMANDLEN);
	cmdStrPtr=0;

	int testCount=0;		
	while(!IsTopCornerBack()){
		testCount++;
		if (testCount>3){
			CString filename="bug/PutBackTopCorner.ml";
			CFile fileout(filename.GetBuffer(),CFile::modeCreate|CFile::modeWrite);
			fileout.Write(aspectColors,sizeof(aspectColors));
			fileout.Close();
			MessageBox("bug emerged in function:PutBackTopCorner");
			exit(-1);
		}

		int cornerHashDst[4];//hash value of this corner should be
		int cornerHashSrc[4];//hash value for current corners
		//start from leftFront corner,counterclockwise
		cornerHashDst[0]=HashCorner(aspectColors[UP][4],aspectColors[LEFT][4],aspectColors[FRONT][4]);
		cornerHashDst[1]=HashCorner(aspectColors[UP][4],aspectColors[FRONT][4],aspectColors[RIGHT][4]);
		cornerHashDst[2]=HashCorner(aspectColors[UP][4],aspectColors[RIGHT][4],aspectColors[BACK][4]);
		cornerHashDst[3]=HashCorner(aspectColors[UP][4],aspectColors[BACK][4],aspectColors[LEFT][4]);

		cornerHashSrc[0]=HashCorner(aspectColors[UP][6],aspectColors[LEFT][8],aspectColors[FRONT][6]);
		cornerHashSrc[1]=HashCorner(aspectColors[UP][8],aspectColors[FRONT][8],aspectColors[RIGHT][8]);
		cornerHashSrc[2]=HashCorner(aspectColors[UP][2],aspectColors[RIGHT][2],aspectColors[BACK][8]);
		cornerHashSrc[3]=HashCorner(aspectColors[UP][0],aspectColors[BACK][6],aspectColors[LEFT][2]);

		//analysis
		//find backed corner first
		bool found=false;
		for (int i=0;i<4;i++){
			if (cornerHashDst[i]==cornerHashSrc[i]){
				found=true;
				//analyze relative positions of the other three corners
				RotateDirection direction;
				if (cornerHashDst[(i+1)%4]==cornerHashSrc[(i+2)%4]){//clockwise
					direction=CLOCKWISE;					
				}else if (cornerHashDst[(i+2)%4]==cornerHashSrc[(i+1)%4]){//clockwise
					direction=COUNTERCLOCKWISE;
				}else {
					MessageBox("something wrong with PutBackTopCorner");
					exit(-1);
				}
				if (direction==COUNTERCLOCKWISE){
					//adjust direction of the cube
					if (i==3){
						_yr();
					}else if (i==2){
						_y2();
					}else if (i==1){
						_y();
					}
					char* formula="RU'L'UR'U'LU";
					Executor(formula);
					strcpy(cmdString+cmdStrPtr,formula);
					cmdStrPtr+=strlen(formula);
				}else {
					if (i==3){
						_y2();
					}else if (i==2){
						_y();
					}else if (i==1){
						_yr();
					}
					char* formula="L'URU'LUR'U'";
					Executor(formula);
					strcpy(cmdString+cmdStrPtr,formula);
					cmdStrPtr+=strlen(formula);
				}
			}
		}
		if (!found){//none of the four corners is in right position
			char* formula="RU'L'UR'U'LUy";
			Executor(formula);
			strcpy(cmdString+cmdStrPtr,formula);
			cmdStrPtr+=strlen(formula);
		}
	}
	return CString(cmdString);
}
bool CMagicCube2DDlg::IsTopCornerRestored(void)
{
	if (aspectColors[UP][0]!=aspectColors[UP][4]||
		aspectColors[UP][2]!=aspectColors[UP][4]||
		aspectColors[UP][6]!=aspectColors[UP][4]||
		aspectColors[UP][8]!=aspectColors[UP][4]||
		aspectColors[LEFT][2]!=aspectColors[LEFT][4]||
		aspectColors[LEFT][8]!=aspectColors[LEFT][4]||
		aspectColors[RIGHT][2]!=aspectColors[RIGHT][4]||
		aspectColors[RIGHT][8]!=aspectColors[RIGHT][4]||
		aspectColors[FRONT][6]!=aspectColors[FRONT][4]||
		aspectColors[FRONT][8]!=aspectColors[FRONT][4]||
		aspectColors[BACK][6]!=aspectColors[BACK][4]||
		aspectColors[BACK][8]!=aspectColors[BACK][4]){
			return false;
	}

	return true;
}
CString CMagicCube2DDlg::RestoreTopCorner(void)
{
	memset(cmdString,0,COMMANDLEN);
	cmdStrPtr=0;
	//TOP FRONT:RF'R'F twice
	//TOP RIGHT:F'RFR' twice
	//ASSERT(IsTopCornerBack());
	if (!IsTopCornerRestored()){
		//find a start corner
		for (int i=0;i<4;i++){
			if (aspectColors[UP][8]!=aspectColors[UP][4]){
					break;
			}
			_y();
		}
		int testCount=0;
		while(!IsTopCornerRestored()){
			testCount++;
			if (testCount>12){
				MessageBox("bug merged in function:RestoreTopCorner");
				CString filename="bug/RestoreTopCorner.ml";
				CFile fileout(filename.GetBuffer(),CFile::modeCreate|CFile::modeWrite);
				fileout.Write(aspectColors,sizeof(aspectColors));
				fileout.Close();
				exit(-1);
			}
			if (aspectColors[UP][8]!=aspectColors[UP][4]){
				if (aspectColors[FRONT][8]==aspectColors[UP][4]){
					char* formula="RF'R'FRF'R'F";
					Executor(formula);
					strcpy(cmdString+cmdStrPtr,formula);
					cmdStrPtr+=strlen(formula);
				}else {
					char* formula="F'RFR'F'RFR'";
					Executor(formula);
					strcpy(cmdString+cmdStrPtr,formula);
					cmdStrPtr+=strlen(formula);
				}
			}else{
				_U();
			}
		}
		
		//last step to restore
		if (aspectColors[FRONT][7]!=aspectColors[FRONT][4]){
			if (aspectColors[BACK][7]!=aspectColors[FRONT][4]){
				_U2();
			}else if (aspectColors[LEFT][5]!=aspectColors[FRONT][4]){
				_Ur();
			}else if (aspectColors[RIGHT][5]!=aspectColors[FRONT][4]){
				_U();
			}

		}else{
			//bingo
		}
		
	}
	return CString(cmdString);
}

CString CMagicCube2DDlg::MethodFewerFormula()
{
	CString cmd;
	cmd=F2L();
	cmd+="#\r\nBuil Cross On Top Aspect:\r\n\t#";
	cmd+=BuildTopCross();
	cmd+="#\r\nMatch Top Cross:\r\n\t#";
	cmd+=MatchTopCross();
	cmd+="#\r\nPut Back Top Corners:\r\n\t#";
	cmd+=PutBackTopCorner();
	cmd+="#\r\nRestore Top Corners:\r\n\t#";
	cmd+=RestoreTopCorner();

	int len=CountSteps(cmd.GetBuffer());
	CString str;
	str.Format("#\r\n\r\nStep Number:\r\n\t%d#",len);
	cmd+=str;
	//the view is not updated,and we should undo all the operations 
	//during the steps above
	CString undo=UndoCmdString(cmd.GetBuffer(cmd.GetLength()));
	Executor(undo.GetBuffer(undo.GetLength()));

	return cmd;
}
///////////////////////////////////////////////////
//Method 2:cfop,not finished
///////////////////////////////////////////////////
//hash function for oll
void CMagicCube2DDlg::HashOLL(char* hashstr)
{
	//start from left side,then front,right,back
	//length of the hash string is 12,for 12 facelets around top face
	memset(hashstr,'0',12);
	if (aspectColors[LEFT][2]==aspectColors[UP][4]){
		hashstr[0]='1';
	}
	if (aspectColors[LEFT][5]==aspectColors[UP][4]){
		hashstr[1]='1';
	}
	if (aspectColors[LEFT][8]==aspectColors[UP][4]){
		hashstr[2]='1';
	}
	if (aspectColors[FRONT][6]==aspectColors[UP][4]){
		hashstr[3]='1';
	}
	if (aspectColors[FRONT][7]==aspectColors[UP][4]){
		hashstr[4]='1';
	}
	if (aspectColors[FRONT][8]==aspectColors[UP][4]){
		hashstr[5]='1';
	}
	if (aspectColors[RIGHT][8]==aspectColors[UP][4]){
		hashstr[6]='1';
	}
	if (aspectColors[RIGHT][5]==aspectColors[UP][4]){
		hashstr[7]='1';
	}
	if (aspectColors[RIGHT][2]==aspectColors[UP][4]){
		hashstr[8]='1';
	}
	if (aspectColors[BACK][8]==aspectColors[UP][4]){
		hashstr[9]='1';
	}
	if (aspectColors[BACK][7]==aspectColors[UP][4]){
		hashstr[10]='1';
	}
	if (aspectColors[BACK][6]==aspectColors[UP][4]){
		hashstr[11]='1';
	}

}
//whether top face is restored
bool CMagicCube2DDlg::IsTopFaceRestored(void)
{
	for (int i=0;i<9;i++){
		if (aspectColors[UP][i]!=aspectColors[UP][4]){
			return false;
		}
	}
	return true;	
}
//Orientation the Last Layer
void CMagicCube2DDlg::OLL(void)
{
	//BuildTopCross();
	//I choose standard OLL method to restore top face,there are 
	//too many situations.A possible way is to hash them.This will
	//highly reduce program complexity .

	//length of the string is 12,for the sack of rotation,4 string 
	//is used,in yr rotation order,i.e. counterclockwise.
	char hashstr[4][13]={0};
	HashOLL(hashstr[0]);
	for (int i=1;i<4;i++){
		int t=i*3;
		memcpy(hashstr[i],hashstr[0]+12-t,t);
		memcpy(hashstr[i]+t,hashstr[0],12-t);
	}
	//hash of every situations
	//if BuildTopCross function is used first,only first 7 ollHash 
	//strings are used.the second part are corresponding operations
	static const char* ollHash[57][2]={
		/* 01 */{"100100100000","(RU'2)R'U'(RU'R')"},
		/* 02 */{"000001001001","RUR'URU2R'"},//U'R'U2RUR'UR
		/* 03 */{"101000101000","(RUR'U)(RU'R'U)(RU2R')"},// (RU'2)(R'U'RUR'U')(RU'R')
		/* 04 */{"101001000100","RU'2(R'2U')(R2U')(R'2U'2R"},
		/* 05 */{"000100000001","(rUR'U')(r'FRF')"},
		/* 06 */{"000101000000","(RUR'URU'2R')U(RU'2R'U'RU'R'"},
		/* 07 */{"100001000000","F'(rUR'U')(r'FR)"},
		/* 08 */{"111010111010","(RU'2)(R'2FRF')U2(R'FRF')"},
		/* 09 */{"111011010110","(FRUR'U'F')(fRUR'U'f')"},
		/* 10 */{"011010011011","(f R U R' U' f')U' (F R U R' U' F')"},
		/* 11 */{"110110110010","(fRUR'U'f')y(FRUR'U'F')"},
		/* 12 */{"011010010110","(RUR'U)(R'FRF')U2(R'FRF')"},
		/* 13 */{"010010010111","F(RUR'U)y'(R'U2)(R'FRF')"},
		/* 14 */{"011010110010","r'(RURUR'U')r(R'2FRF')"},
		/* 15 */{"010010010010","r'(RURUR'U')r2(R'2UR U')"},
		/* 16 */{"101011000110","f (R U R' U')2 f'"},
		/* 17 */{"010100111001","(R'U'RU'R'U)y'(R' URB)"},
		/* 18 */{"101010101010","(r U r')(U R U' R' ) 2 (r U' r')"},
		/* 19 */{"111000111000","(RU'U')(R'2U')(R U'R' U2)(F R F')"},
		/* 20 */{"000011001011","(rU'r'U')(rU r')y' (R' U R)"},
		/* 21 */{"100110000110","(R'FRUR' F'R) y'(R U' R')"},
		/* 22 */{"001010001011","(r' U' r)(R' U' R U)(r' U r)"},
		/* 23 */{"100110100010","(rUr')(RUR'U')(rU' r')"},
		/* 24 */{"000110000011","(RUR'U')(R'F R F')"},
		/* 25 */{"101010000010","F(R U R' U') F'"},
		/* 26 */{"000010000010","(RUR'U' r)(R'U R U') r'"},
		/* 27 */{"000010010000","(rUR'U')(r' RU)(R U' R')"},
		/* 28 */{"000010100011","(r U' r' U' r) y (R U R' f')"},
		/* 29 */{"001010000110","(R'F R U R' U') (F' U R)"},
		/* 30 */{"001010100010","(R'U' R U) y (r U R' U') r' R"},
		/* 31 */{"010000111000","(R'U')(R' F R F') (U R)"},
		/* 32 */{"010110000001","(R'U' F)(U R U' R' F' R)"},
		/* 33 */{"010100000011","(R U B'U')(R' U R B R')"},
		/* 34 */{"000000111010","x'U'F'R'FR U"},
		/* 35 */{"111000000010","f(RUR'U')f'"},
		/* 36 */{"000110011000","F(RU'R'U')(R U R' F')"},
		/* 37 */{"010100001010","R U'2(R'2FR F')(R U'2 R')"},
		/* 38 */{"011000001011","(r'U2)(R UR' U) r"},
		/* 39 */{"110110100000","(rU2)(R'U'R U') r'"},
		/* 40 */{"000100110110","r'U'R U'R'U2 r"},
		/* 41 */{"000011011001","rUR'UR U'2r'"},
		/* 42 */{"010001001011","r'(R2U)(R'U R) U'2 (R' U R')"},
		/* 43 */{"010110100100","r(R'2U')(RU'R') U2 (R U' R) r'"},
		/* 44 */{"100110010100","(R'U'R) y'x'(R U' R' F)(R U R')"},
		/* 45 */{"001001010011","(RUR'U)(R'FR F')(R U'2 R')"},
		/* 46 */{"000111010101","(r U2)(R' U' R U R' U')(R U' r')"},
		/* 47 */{"101001010110","B'(R'U'RU)2B"},
		/* 48 */{"000101010111","r' U2(R U R' U')(R U R' U) r"},
		/* 49 */{"101011010100","F(RU R'U')2 F'"},
		/* 50 */{"000110111001","R'FR2 B' R'2 F' R2 B R'"},
		/* 51 */{"000100111011","RB'R'2 F R2 B R'2 F' R"},
		/* 52 */{"011000100010","(R2U R' B')(R U' R'2 U)(R B R')"},
		/* 53 */{"110010001000","(RUR' U')(R U' R' F') U' (F R U R')"},
		/* 54 */{"010010000101","(R' U R U'2 R' U') y (L' U L U F)"},
		/* 55 */{"010101000010","(RU' R' U2 R U) y (R U' R' U' F')"},
		/* 56 */{"000010110001","(RU R' U)(R U' R' U')(R' F R F')"},
		/* 57 */{"000100011010","(R' U' R U')(R' U R U)(R B' R' B)"}
	};
	for (int i=0;i<57;i++){
		for (int j=0;j<4;j++){
			if (strcmp(hashstr[j],ollHash[i][0])==0){
				if (j==1){
					yr();
				}else if (j==2){
					y2();
				}else if (j==3){
					y();
				}
				Executor(ollHash[i][1]);
				if (!IsTopFaceRestored()){
					char errorInfo[100];
					sprintf_s(errorInfo,100,"something wrong with ollHash:%s\nnumber%d",ollHash[i][1],i);
				}
				return;
			}
		}
	}
	MessageBox("special situation not recorded found,saved in file :OLLSpecialFound.ml");
	CString filename="OLLSpecialFound.ml";
	CFile fileout(filename.GetBuffer(),CFile::modeCreate|CFile::modeWrite);
	fileout.Write(aspectColors,sizeof(aspectColors));
	fileout.Close();
}
//hash function for PLL
void CMagicCube2DDlg::HashPLL(char* hashstr)
{
	//not finished
}
//Permutation the Last Layer
void CMagicCube2DDlg::PLL(void)
{
	ASSERT(IsTopFaceRestored());

	char hashstr[4][13]={0};
	HashPLL(hashstr[0]);
	for (int i=1;i<4;i++){
		int t=i*3;
		memcpy(hashstr[i],hashstr[0]+12-t,t);
		memcpy(hashstr[i]+t,hashstr[0],12-t);
	}
	//hash string here is different with oll hash,because one hash 
	//maybe correspond two situations
	static const char* pllHash[18][3]={//0,1,14 are special
		/* 0102 */{"101101101111","(R2 U)(R U R' U')(R' U')(R' U R')","(R U' R)(U R U R)(U' R' U' R2)"},
		/* 0304 */{"110010010011","(l U' R)D2(R' U R)D2 R2","x' R2 D2(R' U' R)D2(R' U R')"},
		/* 05 */{"101000000110","(R'2 u' R U' R)(U R' u)(R2 f R' f')",""},
		/* 06 */{"101000110000","(R U R')y'(R2' u' R U')(R' U R' u R2)",""},
		/* 07 */{"101011000000","(R2 u)(R' U R' U')(R u') (R2' F' U F)",""},
		/* 08 */{"101000011000","(R' d' F) (R2 u) (R' U) (R U' R u' R'2)",""},
		/* 09 */{"111100000011","(R U R' F')(R U R' U')(R' F R2 U' R' U')",""},
		/* 10 */{"110000001111","z (U' R D') (R2 U R' U' R2 U) z' (R U')",""},
		/* 11 */{"011101100010","(R' U2)(R U2')(R' F)(R U R' U')(R' F' R2 U')",""},
		/* 12 */{"110010001101","(R U'2 R' U2) (R B' R' U') (R U R B R'2 U) ",""},//  x'(R2 u' R' u)R2 x' y'(R' U R' U')R2
		/* 13 */{"101110000011","(R U R' U')(R' F)(R2 U' R' U')(R U R' F')",""},
		/* 14 */{"000110011000","F(R U' R' U')(R U R' F')(R U R' U')(R' F R F')",""},
		/* 15 */{"110001110001","z (U' R D') (R2 U R' U')z'( R U R')z (R2 U R')z' (R U')",""},
		/* 16 */{"011100011100","L U' R U'2 r' F r R' U' R U'2 r' F R' F",""},
		/* 1721 */{"101101101101","(U R' U')(R U' R)(U R U')(R' U R U)(R2 U')(R' U)","M'2 U' M'2 U2 M'2 U' M'2"},
		/* 18 */{"011110001100","(R' U R' U') y x2 (R' U R' U' R2) y x (R' U' R U R)",""},
		/* 19 */{"010010010010","L x' (R U R')(D R U')(r' R' F) r D' r' F' ",""},//(R' U R' U')B' D B' D' B2 R' (B' R B R)
		/* 20 */{"111100010001","U'(R' U R U') R'2 F' z R' F R D z' x (U R' U' R2)",""},
	};
}
//I use CFOP method to restore the cube
void CMagicCube2DDlg::CFOP(void)
{
	F2L();		
	OLL();		
	PLL();
}
void CMagicCube2DDlg::OnBnClickedButtonSolution()
{
	CString cmd=MethodFewerFormula();
	//Executor(cmd.GetBuffer());
	//UpdateView();

	//give the result instead	
	cmd.Replace('#',' ');
	stepsDlg.editDisplay.SetWindowText(cmd.GetBuffer(cmd.GetLength()));		
	stepsDlg.ShowWindow(SW_SHOW);
}


void CMagicCube2DDlg::OnSave()
{
	CFileDialog fileDlg(false,NULL,NULL,4|2,"MagicCube Layout Files(*.ml)|*.ml||");
	if (fileDlg.DoModal()==IDOK){
		CString filename=fileDlg.GetPathName();
		CString ext=fileDlg.GetFileExt();
		if (ext!="ml"){
			filename.Append(".ml");
		}
		CFile fileout(filename.GetBuffer(),CFile::modeCreate|CFile::modeWrite);
		fileout.Write(aspectColors,sizeof(aspectColors));
		fileout.Close();
	}	
}

void CMagicCube2DDlg::OnOpen()
{
	CFileDialog fileDlg(true,NULL,NULL,4|2,"MagicCube Layout Files(*.ml)|*.ml||");
	if (fileDlg.DoModal()==IDOK){
		CString filename=fileDlg.GetPathName();
		CFile filein(filename.GetBuffer(),CFile::modeRead);
		filein.Read(aspectColors,sizeof(aspectColors));
		filein.Close();
	}
}

void CMagicCube2DDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMagicCube2DDlg::OnCross()
{
	//make cross
	Cross();
	xr();

	UpdateView();
}
void CMagicCube2DDlg::OnL1Corner()
{
	RestoreFirstLayerCorner();
	UpdateView();
}
void CMagicCube2DDlg::OnL2Edge()
{
	RestoreSecondLayerEdge();
	UpdateView();
}

void CMagicCube2DDlg::OnTopcross()
{
	BuildTopCross();
	UpdateView();
}
void CMagicCube2DDlg::OnOll()
{
	OLL();
	UpdateView();
}

void CMagicCube2DDlg::OnPll()
{
	PLL();
	UpdateView();
}


void CMagicCube2DDlg::OnMatchtopcross()
{
	MatchTopCross();
	UpdateView();
}

void CMagicCube2DDlg::OnPutbackTopcorner()
{
	PutBackTopCorner();
	UpdateView();
}

void CMagicCube2DDlg::OnRestoreTopcorner()
{
	RestoreTopCorner();
	UpdateView();
}
void CMagicCube2DDlg::Executor(const char* cmd)
{
	//space,'(' and ')' can be filtered first,and "'2" can be replaced by 2
	
	while(*cmd){
		switch(*cmd){
			case ' ':				
			case '(':
			case ')':				
				break;
			case '#':				
				do{
					cmd++;
				}while(*cmd!='#'&&*cmd);
				break;
			case 'F':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						F2();
						cmd++;
					}else{
						Fr();
					}					
					cmd++;
				}else if (*(cmd+1)=='2'){
					F2();
					cmd++;
				}else{
					F();
				}
				break;
			case 'B':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						B2();
						cmd++;
					}else{
						Br();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					B2();
					cmd++;
				}else{
					B();
				}
				break;
			case 'L':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						L2();
						cmd++;
					}else{
						Lr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					L2();
					cmd++;
				}else{
					L();
				}
				break;
			case 'R':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						R2();
						cmd++;
					}else{
						Rr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					R2();
					cmd++;
				}else{
					R();
				}
				break;
			case 'U':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						U2();
						cmd++;
					}else{
						Ur();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					U2();
					cmd++;
				}else{
					U();
				}
				break;
			case 'D':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						D2();
						cmd++;
					}else{
						Dr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					D2();
					cmd++;
				}else{
					D();
				}
				break;
			case 'x':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						x2();
						cmd++;
					}else{
						xr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					x2();
					cmd++;
				}else{
					x();
				}
				break;
			case 'y':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						y2();
						cmd++;
					}else{
						yr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					y2();
					cmd++;
				}else{
					y();
				}
				break;
			case 'z':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						z2();
						cmd++;
					}else{
						zr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					z2();
					cmd++;
				}else{
					z();
				}
				break;
			case 'f':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						f2();
						cmd++;
					}else{
						fr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					f2();
					cmd++;
				}else{
					f();
				}
				break;
			case 'b':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						b2();
						cmd++;
					}else{
						br();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					b2();
					cmd++;
				}else{
					b();
				}
				break;
			case 'l':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						l2();
						cmd++;
					}else{
						lr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					l2();
					cmd++;
				}else{
					l();
				}
				break;
			case 'r':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						r2();
						cmd++;
					}else{
						rr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					r2();
					cmd++;
				}else{
					r();
				}
				break;
			case 'u':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						u2();
						cmd++;
					}else{
						ur();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					u2();
					cmd++;
				}else{
					u();
				}
				break;
			case 'd':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						d2();
						cmd++;
					}else{
						dr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					d2();
					cmd++;
				}else{
					d();
				}
				break;
			case 'E':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						E2();
						cmd++;
					}else{
						Er();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					E2();
					cmd++;
				}else{
					E();
				}
				break;
			case 'M':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						M2();
						cmd++;
					}else{
						Mr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					M2();
					cmd++;
				}else{
					M();
				}
				break;
			case 'S':
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2
						S2();
						cmd++;
					}else{
						Sr();
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					S2();
					cmd++;
				}else{
					S();
				}
				break;
			default:
				break;
		}
		cmd++;
	}
}
//get command string which can restore the state before 
//execution of the parameter string
CString CMagicCube2DDlg::UndoCmdString(const char* cmd)
{
	CString undoCmd;
	while(*cmd){
		switch(*cmd){
			case ' ':
			case '(':
			case ')':
				break;
			case '#':				
				do{
					cmd++;
				}while(*cmd!='#'&&*cmd);
				break;
			case 'F':
			case 'B':
			case 'L':
			case 'R':
			case 'U':
			case 'D':
			case 'x':
			case 'y':
			case 'z':
			case 'u':
			case 'r':
			case 'f':
			case 'd':
			case 'l':
			case 'b':
			case 'E':
			case 'M':
			case 'S':
				undoCmd.Insert(0,*cmd);
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2						
						undoCmd.Insert(1,'2');
						cmd++;
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					undoCmd.Insert(1,'2');
					cmd++;
				}else{
					undoCmd.Insert(1,'\'');
				}
				break;
			default:
				break;					
		}
		cmd++;
	}


	return undoCmd;
}
//calculate how many steps does the restoration takes
int CMagicCube2DDlg::CountSteps(const char* cmd)
{
	//method:
	int steps=0;
	while(*cmd){
		switch(*cmd){
			case ' ':
			case '(':
			case ')':
				break;
			case '#':				
				do{
					cmd++;
				}while(*cmd!='#'&&*cmd);
				break;
			case 'F':
			case 'B':
			case 'L':
			case 'R':
			case 'U':
			case 'D':
			/*case 'x'://do not counts
			case 'y':
			case 'z':*/
			case 'u':
			case 'r':
			case 'f':
			case 'd':
			case 'l':
			case 'b':
			case 'E':
			case 'M':
			case 'S':
				steps++;
				if (*(cmd+1)=='\''){
					if (*(cmd+2)=='2'){//E'2==E2						
						//steps++;
						cmd++;
					}
					cmd++;
				}else if (*(cmd+1)=='2'){
					//steps++;
					cmd++;
				}
				break;
			default:
				break;					
		}
		cmd++;
	}
	return steps;
}
void CMagicCube2DDlg::OnExcute()
{
	CString cmd;	
	cmdstringEditCtl.GetWindowText(cmd);
	if (cmd.GetLength()>100){
		MessageBox("command too long");
		cmdstringEditCtl.SetWindowText("");
	}else {
		Executor(cmd.GetBuffer());
		cmd.ReleaseBuffer();
		UpdateView();
	}	
}



