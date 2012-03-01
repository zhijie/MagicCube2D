// @Author:onezeros@yahoo.cn
// @Date:10/15/2010

#include <gl/glut.h>
#include <memory>
#include <cstdlib>
#include <ctime>
using namespace std;

//definition of zyx order:
//	from back to front ,from bottom to up,from left to right
//	x changes first ,y secondly,z last


//initial window size 
GLsizei winWidth=400,winHeight=400;

///////////////////////////////////////////////////
//rotate direction
enum Axis{X,Y,Z};
//description of chunk position
enum ChunkIndex{NEGATIVE,MIDDLE,POSITIVE};
enum RotateDirection{CLOCKWISE,COUNTERCLOCKWISE,NOROTATE};
//description of the 6 aspects and the situation that no aspect
enum Aspect{FRONT,BACK,LEFT,RIGHT,UP,DOWN,NOASPECT};

///////////////////////////////////////////////////
//global constants and variables
///////////////////////////////////////////////////
//gray ,color of sheltered aspects which can be seen when rotation
enum Color{WHITE,YELLOW,BLUE,GREEN,RED,ORANGE,GRAY};
const GLubyte colorTable[7][3]={
	{255,255,255},{255,255,0},{0,0,255},{0,255,0},{255,0,0},{255,102,0},{0x99,0x99,0x99}
};

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
//index of points of aspects
const int aspectDotIndex[6][16]={
	//front
	{48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63},
	//back
	{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
	//left
	{0,4,8,12,16,20,24,28,32,36,40,44,48,52,56,60},
	//right
	{3,7,11,15,19,23,27,31,35,39,43,47,51,55,59,63},
	//up
	{12,13,14,15,28,29,30,31,44,45,46,47,60,61,62,63},
	//down
	{0,1,2,3,16,17,18,19,32,33,34,35,48,49,50,51,}
};
//	coordinates of points: in zyx order
//length of little dice is 1
const GLfloat points[64][3]={
	{-1.5,-1.5,-1.5},{-0.5,-1.5,-1.5},{0.5,-1.5,-1.5},{1.5,-1.5,-1.5},//back
	{-1.5,-0.5,-1.5},{-0.5,-0.5,-1.5},{0.5,-0.5,-1.5},{1.5,-0.5,-1.5},
	{-1.5,0.5,-1.5},{-0.5,0.5,-1.5},{0.5,0.5,-1.5},{1.5,0.5,-1.5},
	{-1.5,1.5,-1.5},{-0.5,1.5,-1.5},{0.5,1.5,-1.5},{1.5,1.5,-1.5},
	{-1.5,-1.5,-0.5},{-0.5,-1.5,-0.5},{0.5,-1.5,-0.5},{1.5,-1.5,-0.5},//
	{-1.5,-0.5,-0.5},{-0.5,-0.5,-0.5},{0.5,-0.5,-0.5},{1.5,-0.5,-0.5},
	{-1.5,0.5,-0.5},{-0.5,0.5,-0.5},{0.5,0.5,-0.5},{1.5,0.5,-0.5},
	{-1.5,1.5,-0.5},{-0.5,1.5,-0.5},{0.5,1.5,-0.5},{1.5,1.5,-0.5},
	{-1.5,-1.5,0.5},{-0.5,-1.5,0.5},{0.5,-1.5,0.5},{1.5,-1.5,0.5},//
	{-1.5,-0.5,0.5},{-0.5,-0.5,0.5},{0.5,-0.5,0.5},{1.5,-0.5,0.5},
	{-1.5,0.5,0.5},{-0.5,0.5,0.5},{0.5,0.5,0.5},{1.5,0.5,0.5},
	{-1.5,1.5,0.5},{-0.5,1.5,0.5},{0.5,1.5,0.5},{1.5,1.5,0.5},
	{-1.5,-1.5,1.5},{-0.5,-1.5,1.5},{0.5,-1.5,1.5},{1.5,-1.5,1.5},//front
	{-1.5,-0.5,1.5},{-0.5,-0.5,1.5},{0.5,-0.5,1.5},{1.5,-0.5,1.5},
	{-1.5,0.5,1.5},{-0.5,0.5,1.5},{0.5,0.5,1.5},{1.5,0.5,1.5},
	{-1.5,1.5,1.5},{-0.5,1.5,1.5},{0.5,1.5,1.5},{1.5,1.5,1.5}
};

int indexArray[4]={0};

///////////////////////////////////////////////////
//control parameters
///////////////////////////////////////////////////
//is left mouse button down
bool isLButtonDown=false;
//block to control by keyboard
ChunkIndex chunkIndexByKeyboard=MIDDLE;
//lookAt parameter
class GLPoint2f 
{
public:
	float x;
	float y;
	GLPoint2f():x(0),y(0){}
	GLPoint2f(float x,float y){
		this->x=x;
		this->y=y;
	}
};
class GLPoint3f 
{
public:
	float x;
	float y;
	float z;
	GLPoint3f():x(0),y(0),z(0){}
	GLPoint3f(float x,float y,float z){
		this->x=x;
		this->y=y;
		this->z=z;
	}
};
GLPoint3f eyePos(5,5,5);
const float eyePosDistanceToCenter=75;
//selected situation
class TheSelected{
public:
	GLPoint2f selectedPoint;
	Aspect aspect;
	int aspectIndex;
	TheSelected():aspectIndex(0),aspect(NOASPECT){}
};
TheSelected theSelected;
//rotate information
class RotateInfo
{
public:
	Axis axis;
	ChunkIndex chunckIndex;
	RotateDirection direction;
	int angle;
	RotateInfo():axis(Z),chunckIndex(MIDDLE),direction(NOROTATE),angle(0){}
};
RotateInfo rotateInfo;
//rotate parameters
const GLint fullRatateTime=300;//milliseconds
const GLint degreePerRot=6;
const GLint perRotateTime=fullRatateTime/(90/degreePerRot);//time for every rotation
bool isRotating=false;


///////////////////////////////////////////////////

void init(void)
{
	glClearColor(0,0,0,1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}
void reshapeFunc(GLint newWidth,GLint newHeight)
{
	//set viewport and perspective 
	glViewport(0,0,newWidth,newHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,(GLfloat)newWidth/(GLfloat)newHeight,0.1,3000.0);
	glMatrixMode(GL_MODELVIEW);

	winWidth=newWidth;
	winHeight=newHeight;
}
///////////////////////////////////////////////////
//draw aspects
///////////////////////////////////////////////////
//index of points table,including 4 elements
inline void vertexs(int* index)
{
	glVertex3fv(points[index[0]]);
	glVertex3fv(points[index[1]]);
	glVertex3fv(points[index[2]]);
	glVertex3fv(points[index[3]]);
}
//draw small aspect of big aspect asp,indexed vertex in aspectDotIndex by index
inline void drawAspect(Aspect asp,int index)
{
	int colorIndex=(index>>2)*3+index%4;
	glColor3ubv(colorTable[aspectColors[asp][colorIndex]]);
	int indexArray[4]={
		aspectDotIndex[asp][index],aspectDotIndex[asp][index+4],
		aspectDotIndex[asp][index+5],aspectDotIndex[asp][index+1]};
		glBegin(GL_QUADS);
		vertexs(indexArray);
		glEnd();

		glColor3ubv(colorTable[GRAY]);
		glLineWidth(2.f);
		glBegin(GL_LINE_LOOP);
		vertexs(indexArray);
		glEnd();
}
//draw a full aspect
inline void drawFullAspect(Aspect asp)
{
	for (int l=0;l<3;l++){//line
		for (int i=0;i<3;i++){//3 aspects of every line			
			int ind=l*4+i;
			drawAspect(asp,ind);				
		}
	}
}



void drawXRight(void)
{
	//right part
	drawFullAspect(RIGHT);
	for (int i=0;i<3;i++){
		//up part
		drawAspect(UP,2+(i<<2));
		//down part
		drawAspect(DOWN,2+(i<<2));
		//front part
		drawAspect(FRONT,2+(i<<2));
		//back part
		drawAspect(BACK,2+(i<<2));
	}
	//shielded part
	glColor3ubv(colorTable[GRAY]);
	//right
	indexArray[0]=2;
	indexArray[1]=14;
	indexArray[2]=62;
	indexArray[3]=50;	
	glBegin(GL_QUADS);	
	vertexs(indexArray);
	glEnd();
}
void drawXMiddle(void)
{
	for (int i=0;i<3;i++){
		//up part
		drawAspect(UP,1+(i<<2));
		//down part
		drawAspect(DOWN,1+(i<<2));
		//front part
		drawAspect(FRONT,1+(i<<2));
		//back part
		drawAspect(BACK,1+(i<<2));
	}
	//shielded part
	glColor3ubv(colorTable[GRAY]);
	//right
	indexArray[0]=2;
	indexArray[1]=14;
	indexArray[2]=62;
	indexArray[3]=50;	
	glBegin(GL_QUADS);	
	vertexs(indexArray);
	glEnd();
	//left
	indexArray[0]=1;
	indexArray[1]=13;
	indexArray[2]=61;
	indexArray[3]=49;	
	glBegin(GL_QUADS);	
	vertexs(indexArray);
	glEnd();
}
void drawXLeft(void)
{
	//left part
	drawFullAspect(LEFT);
	for (int i=0;i<3;i++){
		//up part
		drawAspect(UP,i<<2);
		//down part
		drawAspect(DOWN,i<<2);
		//front part
		drawAspect(FRONT,i<<2);
		//back part
		drawAspect(BACK,i<<2);
	}
	//shielded part
	glColor3ubv(colorTable[GRAY]);
	//left
	indexArray[0]=1;
	indexArray[1]=13;
	indexArray[2]=61;
	indexArray[3]=49;	
	glBegin(GL_QUADS);	
	vertexs(indexArray);
	glEnd();
}
void drawYDown(void)
{
	//down part
	drawFullAspect(DOWN);
	for (int i=0;i<3;i++){
		//left part
		drawAspect(LEFT,i<<2);
		//right part
		drawAspect(RIGHT,i<<2);
		//front part
		drawAspect(FRONT,i);
		//back part
		drawAspect(BACK,i);
	}
	//shielded part
	glColor3ubv(colorTable[GRAY]);
	//down
	indexArray[0]=52;
	indexArray[1]=55;
	indexArray[2]=7;
	indexArray[3]=4;	
	glBegin(GL_QUADS);
	vertexs(indexArray);
	glEnd();
}
void drawYMiddle(void)
{///////////////////////////////////////////////////?????????????????????????????
	for (int i=0;i<3;i++){
		//left part
		drawAspect(LEFT,1+(i<<2));
		//right part
		drawAspect(RIGHT,1+(i<<2));
		//front part
		drawAspect(FRONT,4+i);
		//back part
		drawAspect(BACK,4+i);
	}
	//shielded part
	glColor3ubv(colorTable[GRAY]);
	//up
	indexArray[0]=56;
	indexArray[1]=59;
	indexArray[2]=11;
	indexArray[3]=8;	
	glBegin(GL_QUADS);
	vertexs(indexArray);
	glEnd();
	//down
	indexArray[0]=52;
	indexArray[1]=55;
	indexArray[2]=7;
	indexArray[3]=4;	
	glBegin(GL_QUADS);
	vertexs(indexArray);
	glEnd();
}
void drawYUp(void)
{
	//up part
	drawFullAspect(UP);
	for (int i=0;i<3;i++){
		//left part
		drawAspect(LEFT,2+(i<<2));
		//right part
		drawAspect(RIGHT,2+(i<<2));
		//front part
		drawAspect(FRONT,8+i);
		//back part
		drawAspect(BACK,8+i);
	}
	//shielded part
	glColor3ubv(colorTable[GRAY]);
	//up
	indexArray[0]=56;
	indexArray[1]=59;
	indexArray[2]=11;
	indexArray[3]=8;	
	glBegin(GL_QUADS);
	vertexs(indexArray);
	glEnd();
}

void drawZFront()
{
	//front part
	drawFullAspect(FRONT);
	for (int i=8;i<=10;i++){		
		//left part
		drawAspect(LEFT,i);
		//right part
		drawAspect(RIGHT,i);
		//up part
		drawAspect(UP,i);
		//down part
		drawAspect(DOWN,i);
	}
	//shielded part
	glColor3ubv(colorTable[GRAY]);
	//front
	indexArray[0]=32;
	indexArray[1]=35;
	indexArray[2]=47;
	indexArray[3]=44;	
	glBegin(GL_QUADS);
	vertexs(indexArray);
	glEnd();
}
void drawZMiddle()
{	
	for (int i=4;i<=6;i++){
		//left part
		drawAspect(LEFT,i);
		//right part
		drawAspect(RIGHT,i);
		//up part
		drawAspect(UP,i);
		//down part
		drawAspect(DOWN,i);
	}
	//shielded part
	glColor3ubv(colorTable[GRAY]);
	//front
	indexArray[0]=32;
	indexArray[1]=35;
	indexArray[2]=47;
	indexArray[3]=44;	
	glBegin(GL_QUADS);	
	vertexs(indexArray);
	glEnd();
	//back
	indexArray[0]=16;
	indexArray[1]=19;
	indexArray[2]=31;
	indexArray[3]=28;	
	glBegin(GL_QUADS);
	vertexs(indexArray);
	glEnd();
}
void drawZBack()
{
	//back part
	drawFullAspect(BACK);	
	for (int i=0;i<=2;i++){
		//left part
		drawAspect(LEFT,i);
		//right part
		drawAspect(RIGHT,i);
		//up part
		drawAspect(UP,i);
		//down part
		drawAspect(DOWN,i);
	}
	//shielded part
	glColor3ubv(colorTable[GRAY]);
	//back
	indexArray[0]=16;
	indexArray[1]=19;
	indexArray[2]=31;
	indexArray[3]=28;	
	glBegin(GL_QUADS);
	vertexs(indexArray);
	glEnd();
}



typedef void (*PDrawBlock) ();
PDrawBlock pDrawBlocks[3][3]={
	{drawXLeft,drawXMiddle,drawXRight},
	{drawYDown,drawYMiddle,drawYUp},
	{drawZBack,drawZMiddle,drawZFront}
};
float rotateVect[3][3]={
	{1,0,0},{0,1,0},{0,0,1}
};

//rotate
void displayMagicCube()
{

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);	

	glLoadIdentity();
	gluLookAt(eyePos.x,eyePos.y,eyePos.z,0,0,0,0,1,0);
	
	//draw stationary part
	/*for (int i=0;i<3&&i!=(int)rotateInfo.chunckIndex;i++){
	pDrawBlocks[rotateInfo.axis][i]();
	}*/
	if (isRotating){
		pDrawBlocks[rotateInfo.axis][(rotateInfo.chunckIndex+1)%3]();
		pDrawBlocks[rotateInfo.axis][(rotateInfo.chunckIndex+2)%3]();
		//draw rotating part,rotate 90 degree
		glPushMatrix();
		glRotatef(rotateInfo.angle,rotateVect[rotateInfo.axis][0],
			rotateVect[rotateInfo.axis][1],rotateVect[rotateInfo.axis][2]);
		pDrawBlocks[rotateInfo.axis][rotateInfo.chunckIndex]();
		glPopMatrix();
	}else{//for speed's sack.if there is no this 'if' sentence,it will be ok still except the efficiency
		drawZBack();
		drawZFront();
		drawZMiddle();
	}

	glFlush();
}
///////////////////////////////////////////////////
//rotate color table
///////////////////////////////////////////////////
//update color table when rotate a chunk
//	rotation clockwise once will be of same effect with rotation counterclockwise triple
//but this will be inefficient.so I will write code for the two situation seperately
//convention:look from positive to negative along axis to decide clockwise or not

inline void rotateFullAspectClockwise(Aspect asp)
{
	int index[9]={2,5,8,1,4,7,0,3,6};
	int i=index[0];
	Color c=aspectColors[asp][i];
	do {
		aspectColors[asp][i]=aspectColors[asp][index[i]];
		i=index[i];
	} while (i!=0);
	aspectColors[asp][0]=c;
}
inline void rotateFullAspectAnticlockwise(Aspect asp)
{
	int index[9]={6,3,0,7,4,1,8,5,2};
	int i=index[0];
	Color c=aspectColors[asp][i];
	do {
		aspectColors[asp][i]=aspectColors[asp][index[i]];
		i=index[i];
	} while (i!=0);
	aspectColors[asp][0]=c;
}
void rotateXLeftClockwise()
{
	rotateFullAspectClockwise(LEFT);

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
	rotateFullAspectAnticlockwise(LEFT);

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
	rotateFullAspectClockwise(RIGHT);

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
	rotateFullAspectAnticlockwise(RIGHT);

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
	rotateFullAspectClockwise(DOWN);

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
	rotateFullAspectAnticlockwise(DOWN);

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
	rotateFullAspectClockwise(UP);
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
	rotateFullAspectAnticlockwise(UP);
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
	rotateFullAspectClockwise(FRONT);

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
	rotateFullAspectAnticlockwise(FRONT);

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
	rotateFullAspectClockwise(BACK);

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
	rotateFullAspectAnticlockwise(BACK);

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

//return value:
//	if cube is not rotating ,then rotate and return true,else just return false
void rotate(Axis axis,ChunkIndex chunckIndex,RotateDirection direction)
{
	if (direction!=NOROTATE){
		isRotating=true;

		rotateInfo.axis=axis;
		rotateInfo.chunckIndex=chunckIndex;
		rotateInfo.direction=direction;	
		pRotateColorTable[rotateInfo.axis][rotateInfo.chunckIndex][rotateInfo.direction]();
	}

}
///////////////////////////////////////////////////
//timer function
///////////////////////////////////////////////////
void timerFunc(int value)
{
	if (isRotating){
		if (rotateInfo.direction==CLOCKWISE){
			rotateInfo.angle+=degreePerRot;
			if (rotateInfo.angle>=90){
				rotateInfo.direction=NOROTATE;
				rotateInfo.angle=0;
				pRotateColorTable[rotateInfo.axis][rotateInfo.chunckIndex][rotateInfo.direction]();
				isRotating=false;
			}
		}else if (rotateInfo.direction==COUNTERCLOCKWISE){
			rotateInfo.angle-=degreePerRot;
			if (rotateInfo.angle<=-90){
				rotateInfo.direction=NOROTATE;
				rotateInfo.angle=0;
				pRotateColorTable[rotateInfo.axis][rotateInfo.chunckIndex][rotateInfo.direction]();
				isRotating=false;
			}
		}

		glutPostRedisplay();
	}
}
///////////////////////////////////////////////////
//keyboard function
///////////////////////////////////////////////////
void keyboardFunc(GLubyte key,GLint xMouse,GLint yMouse)
{
	switch(key){
		case '1':
			chunkIndexByKeyboard=POSITIVE;
			break;
		case '2':
			chunkIndexByKeyboard=MIDDLE;
			break;
		case '3':
			chunkIndexByKeyboard=NEGATIVE;
			break;
		case 'x':
			rotate(X,chunkIndexByKeyboard,CLOCKWISE);
			break;
		case 'X':
			rotate(X,chunkIndexByKeyboard,COUNTERCLOCKWISE);
			break;
		case 'y':
			rotate(Y,chunkIndexByKeyboard,CLOCKWISE);
			break;
		case 'Y':
			rotate(Y,chunkIndexByKeyboard,COUNTERCLOCKWISE);
			break;
		case 'z':
			rotate(Z,chunkIndexByKeyboard,CLOCKWISE);
			break;
		case 'Z':
			rotate(Z,chunkIndexByKeyboard,COUNTERCLOCKWISE);
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}
///////////////////////////////////////////////////
//menu functions
///////////////////////////////////////////////////
//shuffle
void shuffle()
{
	//rotation times
	int shuffleNum=50;
	while (shuffleNum-->0){		
		rotate(Axis(rand()%3),ChunkIndex(rand()%3),RotateDirection(rand()%2));		
	}
}
//check whether the magic cube is restored
bool isRestored()
{	
	for (int i=0;i<6;i++){
		Color color=aspectColors[i][0];
		for (int j=1;j<9;j++){
			if (aspectColors[i][j]!=color){
				return false;
			}
		}
	}
	return true;
}
