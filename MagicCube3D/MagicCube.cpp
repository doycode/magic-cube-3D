#include "StdAfx.h"
#include "MagicCube.h"

/***********************************************************************************
C++�Ѿ���һЩ��д�õ�ͷ�ļ��������׼������ȵȣ������Ǵ����VC++��Include�ļ����
������ʹ��#include <....h>����ʱ���������͵�����ļ�����ȥ�Ҷ�Ӧ���ļ�����Ȼ������
��д��ȥ����һ�������Լ���д��ͷ�ļ��������Ǹ�Include�ļ�����ͻ�����ˡ����԰���
C++�ṩ��ͷ�ļ�ʱ��Ӧ��ʹ�ü����š��෴�أ�#include "....h"�����������ڵ�ǰ�ļ�����
��Ŀ¼�����Ƿ��з��ϵ��ļ������û���ٵ�Include�ļ�����ȥ�Ҷ�Ӧ���ļ�����ˣ�������
���ļ���C++�ṩ�Ļ����Լ���д�ģ�ʹ��#include "....h"����һ������ȷ�ġ�
************************************************************************************/
#include <fstream>

using namespace std;

const GLubyte colorTable[7][3] = {
	{0, 0, 255},//blue
	{0, 255, 0},//green
	{255, 0, 0},//red
	{255, 111, 0},//orange
	{255, 255, 255},//white
	{255, 255, 0},//yellow
	{0, 0, 0}//black
};

//�����и�ֵ����
Color aspectColors[6][9];

//initial
Color aspectColor_org[6][9] = {
	{ RED, RED, RED, RED, RED, RED, RED, RED, RED },//front
	{ ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE, ORANGE },//back
	{ YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW, YELLOW},//left
	{ WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE, WHITE},//right
	{ GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN, GREEN },//up
	{ BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE, BLUE }//down
};

//16*6 - 24(����24�����ظ�����һ��) - 8*2(�˸�������������) = 56
//�����������56��(ȥ���ظ���)
//û�е�����������21  22  25  26  37  38  41  42
const int aspectDotIndex[6][16] = {
	{ 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 },//front
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 },//back
	{ 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60 },//left
	{ 3, 7, 11, 15, 19, 23, 27, 31, 35, 39, 43, 47, 51, 55, 59, 63 },//right
	{ 12, 13, 14, 15, 28, 29, 30, 31, 44, 45, 46, 47, 60, 61, 62, 63 },//up
	{ 0, 1, 2, 3, 16, 17, 18, 19, 32, 33, 34, 35, 48, 49, 50, 51 }//down
};

//Z��ˮƽ���F��
const GLfloat points[64][3] = {
	{-1.5,-1.5,-1.5},{-0.5,-1.5,-1.5},{0.5,-1.5,-1.5},{1.5,-1.5,-1.5},//P0--P3
	{-1.5,-0.5,-1.5},{-0.5,-0.5,-1.5},{0.5,-0.5,-1.5},{1.5,-0.5,-1.5},//P4--P7
	{-1.5,0.5,-1.5},{-0.5,0.5,-1.5},{0.5,0.5,-1.5},{1.5,0.5,-1.5},//P8--P11
	{-1.5,1.5,-1.5},{-0.5,1.5,-1.5},{0.5,1.5,-1.5},{1.5,1.5,-1.5},//P12--P15
	{-1.5,-1.5,-0.5},{-0.5,-1.5,-0.5},{0.5,-1.5,-0.5},{1.5,-1.5,-0.5},//P16--P19
	{-1.5,-0.5,-0.5},{},{},{1.5,-0.5,-0.5},//P20--P23
	{-1.5,0.5,-0.5},{},{},{1.5,0.5,-0.5},//P24--P27
	{-1.5,1.5,-0.5},{-0.5,1.5,-0.5},{0.5,1.5,-0.5},{1.5,1.5,-0.5},//P28--P31
	{-1.5,-1.5,0.5},{-0.5,-1.5,0.5},{0.5,-1.5,0.5},{1.5,-1.5,0.5},//P32--P35
	{-1.5,-0.5,0.5},{},{},{1.5,-0.5,0.5},//P36--P39
	{-1.5,0.5,0.5},{},{},{1.5,0.5,0.5},//P40--P43
	{-1.5,1.5,0.5},{-0.5,1.5,0.5},{0.5,1.5,0.5},{1.5,1.5,0.5},//P44-P47
	{-1.5,-1.5,1.5},{-0.5,-1.5,1.5},{0.5,-1.5,1.5},{1.5,-1.5,1.5},//P48--P51
	{-1.5,-0.5,1.5},{-0.5,-0.5,1.5},{0.5,-0.5,1.5},{1.5,-0.5,1.5},//P52--P55
	{-1.5,0.5,1.5},{-0.5,0.5,1.5},{0.5,0.5,1.5},{1.5,0.5,1.5},//P56-P59
	{-1.5,1.5,1.5},{-0.5,1.5,1.5},{0.5,1.5,1.5},{1.5,1.5,1.5}//P60--P63
};

int indexArray[4] = {0};

//draw aspects
//index of points table, including 4 elements
inline void vertexs(int* index)
{
	glVertex3fv(points[index[0]]);//Specifies a vertex.
	glVertex3fv(points[index[1]]);
	glVertex3fv(points[index[2]]);
	glVertex3fv(points[index[3]]);
}

//draw small aspect of big aspect asp, indexed vertex in aspectDotIndex by index
inline void drawAspect(Aspect asp, int index)
{
	int colorIndex = (index >> 2) * 3 + index % 4;

	glColor3ubv(colorTable[aspectColors[asp][colorIndex]]);//Sets the current color from an already existing array of color values.

	int indexArray[4] = {aspectDotIndex[asp][index], aspectDotIndex[asp][index + 4],
		aspectDotIndex[asp][index + 5], aspectDotIndex[asp][index + 1]};

	glBegin(GL_QUADS);
	vertexs(indexArray);
	glEnd();

	glColor3ubv(colorTable[BLACK]);
	glLineWidth(11.f);
	glBegin(GL_LINE_LOOP);
	vertexs(indexArray);
	glEnd();
}

//draw full aspect
inline void drawFullAspect(Aspect asp)
{
	for(int line = 0; line < 3; line++)
	{
		for(int i = 0; i < 3; i++)//3 aspects of every line
		{
			int ind = line * 4 + i;
			drawAspect(asp, ind);
		}
	}
}


void drawXRight(void)
{
	drawFullAspect(RIGHT);//right part

	for(int i = 0; i < 3; i++)
	{
		drawAspect(UP, 2 + (i << 2));//up part
		drawAspect(DOWN, 2 + (i << 2));
		drawAspect(FRONT, 2 + (i << 2));
		drawAspect(BACK, 2 + (i << 2));
	}

	glColor3ubv(colorTable[BLACK]);//shield part

	//right
	indexArray[0] = 2;
	indexArray[1] = 14;
	indexArray[2] = 62;
	indexArray[3] = 50;

	glBegin(GL_QUADS);

	vertexs(indexArray);

	glEnd();
}

void drawXMiddle(void)
{
	for (int i = 0; i < 3; i++)
	{
		drawAspect(UP, 1 + (i << 2));
		drawAspect(DOWN, 1 + (i << 2));
		drawAspect(FRONT, 1 + (i << 2));
		drawAspect(BACK, 1 + (i << 2));
	}

	glColor3ubv(colorTable[BLACK]);

	//right
	indexArray[0] = 2;
	indexArray[1] = 14;
	indexArray[2] = 62;
	indexArray[3] = 50;	

	glBegin(GL_QUADS);	

	vertexs(indexArray);

	glEnd();

	//left
	indexArray[0] = 1;
	indexArray[1] = 13;
	indexArray[2] = 61;
	indexArray[3] = 49;	

	glBegin(GL_QUADS);	

	vertexs(indexArray);

	glEnd();
}

void drawXLeft(void)
{
	drawFullAspect(LEFT);

	for (int i = 0; i < 3; i++)
	{

		drawAspect(UP, i << 2);
		drawAspect(DOWN, i << 2);
		drawAspect(FRONT, i << 2);
		drawAspect(BACK, i << 2);
	}

	glColor3ubv(colorTable[BLACK]);

	//left
	indexArray[0] = 1;
	indexArray[1] = 13;
	indexArray[2] = 61;
	indexArray[3] = 49;	

	glBegin(GL_QUADS);	

	vertexs(indexArray);

	glEnd();
}

void drawYDown(void)
{
	drawFullAspect(DOWN);

	for (int i = 0; i < 3; i++)
	{
		drawAspect(LEFT, i << 2);
		drawAspect(RIGHT, i << 2);
		drawAspect(FRONT, i);
		drawAspect(BACK, i);
	}

	glColor3ubv(colorTable[BLACK]);

	//down
	indexArray[0] = 52;
	indexArray[1] = 55;
	indexArray[2] = 7;
	indexArray[3] = 4;	

	glBegin(GL_QUADS);

	vertexs(indexArray);

	glEnd();
}

void drawYMiddle(void)
{
	for (int i = 0; i < 3; i++)
	{
		drawAspect(LEFT, 1 + (i << 2));
		drawAspect(RIGHT,1 + (i << 2));
		drawAspect(FRONT, 4 + i);
		drawAspect(BACK, 4 + i);
	}

	glColor3ubv(colorTable[BLACK]);

	//up
	indexArray[0] = 56;
	indexArray[1] = 59;
	indexArray[2] = 11;
	indexArray[3] = 8;	
	glBegin(GL_QUADS);

	vertexs(indexArray);

	glEnd();

	//down
	indexArray[0] = 52;
	indexArray[1] = 55;
	indexArray[2] = 7;
	indexArray[3] = 4;

	glBegin(GL_QUADS);
	vertexs(indexArray);

	glEnd();
}

void drawYUp(void)
{
	drawFullAspect(UP);

	for (int i = 0; i < 3; i++)
	{
		drawAspect(LEFT, 2 + (i << 2));
		drawAspect(RIGHT, 2 + (i << 2));
		drawAspect(FRONT, 8 + i);
		drawAspect(BACK, 8 + i);
	}

	glColor3ubv(colorTable[BLACK]);

	//up
	indexArray[0] = 56;
	indexArray[1] = 59;
	indexArray[2] = 11;
	indexArray[3] = 8;	

	glBegin(GL_QUADS);

	vertexs(indexArray);

	glEnd();
}

void drawZFront()
{
	drawFullAspect(FRONT);

	for (int i = 8; i < 11; i++)
	{		
		drawAspect(LEFT, i);
		drawAspect(RIGHT, i);
		drawAspect(UP, i);
		drawAspect(DOWN, i);
	}

	glColor3ubv(colorTable[BLACK]);

	//front
	indexArray[0] = 32;
	indexArray[1] = 35;
	indexArray[2] = 47;
	indexArray[3] = 44;	

	glBegin(GL_QUADS);

	vertexs(indexArray);

	glEnd();
}

void drawZMiddle()
{	
	for (int i = 4; i < 7; i++)
	{
		drawAspect(LEFT, i);
		drawAspect(RIGHT, i);
		drawAspect(UP, i);
		drawAspect(DOWN, i);
	}

	glColor3ubv(colorTable[BLACK]);

	//front
	indexArray[0] = 32;
	indexArray[1] = 35;
	indexArray[2] = 47;
	indexArray[3] = 44;	

	glBegin(GL_QUADS);	

	vertexs(indexArray);

	glEnd();

	//back
	indexArray[0] = 16;
	indexArray[1] = 19;
	indexArray[2] = 31;
	indexArray[3] = 28;	

	glBegin(GL_QUADS);

	vertexs(indexArray);

	glEnd();
}

void drawZBack()
{
	drawFullAspect(BACK);	

	for (int i = 0; i < 3; i++)
	{
		drawAspect(LEFT, i);
		drawAspect(RIGHT, i);
		drawAspect(UP, i);
		drawAspect(DOWN, i);
	}

	glColor3ubv(colorTable[BLACK]);

	//back
	indexArray[0] = 16;
	indexArray[1] = 19;
	indexArray[2] = 31;
	indexArray[3] = 28;	

	glBegin(GL_QUADS);

	vertexs(indexArray);

	glEnd();
}

/******************************************************************************************
����ϵ��Z�����F�棬X�����R�棬Y�����U��

����ÿ����9С�����������Դ��������������������򿴣�

U��D�棺
0  1  2
3  4  5
6  7  8

F��B�棺
6  7  8
3  4  5
0  1  2

L��R��
8  5  2
7  4  1
6  3  0
*******************************************************************************************/
void rotateFullAspectClockwise(Aspect asp, int* index)
{
	Color t[2];
	t[0] = aspectColors[asp][index[6]];
	t[1] = aspectColors[asp][index[7]];

	for(int i = 7; i > 1; i--)
	{
		aspectColors[asp][index[i]] = aspectColors[asp][index[i - 2]];
	}

	aspectColors[asp][index[0]] = t[0];
	aspectColors[asp][index[1]] = t[1];
}

void rotateFullAspectAnticlockwise(Aspect asp, int* index)
{
	Color t[2];
	t[0] = aspectColors[asp][index[0]];
	t[1] = aspectColors[asp][index[1]];

	for(int i = 0; i < 6; i++)
	{
		aspectColors[asp][index[i]] = aspectColors[asp][index[i + 2]];
	}

	aspectColors[asp][index[6]] = t[0];
	aspectColors[asp][index[7]] = t[1];
}

void rotateXLeftClockwise()
{
	int index[8] = {6, 7, 8, 5, 2, 1, 0, 3};//����תʱ������Ϊ4�����Ŀ�λ�ò���

	rotateFullAspectClockwise(LEFT, index);

	//front, down, back, up
	for (int i = 0; i < 7; i += 3)
	{
		Color t = aspectColors[FRONT][i];
		aspectColors[FRONT][i] = aspectColors[DOWN][i];
		aspectColors[DOWN][i] = aspectColors[BACK][6 - i];
		aspectColors[BACK][6 - i] = aspectColors[UP][6 - i];
		aspectColors[UP][6 - i] = t;
	}
}

void rotateXLeftAnticlockwise()
{
	int index[8] = {6, 7, 8, 5, 2, 1, 0, 3};

	rotateFullAspectAnticlockwise(LEFT, index);

	//front, up, back, down	
	for (int i = 0; i < 7; i += 3)
	{
		Color t = aspectColors[FRONT][i];
		aspectColors[FRONT][i] = aspectColors[UP][6 - i];
		aspectColors[UP][6 - i] = aspectColors[BACK][6 - i];	
		aspectColors[BACK][6 - i] = aspectColors[DOWN][i];	
		aspectColors[DOWN][i] = t;
	}
}

void rotateXRightClockwise()
{
	int index[8] = {6, 7, 8, 5, 2, 1, 0, 3};

	rotateFullAspectClockwise(RIGHT, index);

	//front, down, back, up
	for (int i = 0; i < 7; i += 3)
	{
		Color t = aspectColors[FRONT][i + 2];
		aspectColors[FRONT][i + 2] = aspectColors[DOWN][i + 2];
		aspectColors[DOWN][i + 2] = aspectColors[BACK][8 - i];// 8 - i = 6 - i + 2
		aspectColors[BACK][8 - i] = aspectColors[UP][8 - i];
		aspectColors[UP][8 - i] = t;
	}
}

void rotateXRightAnticlockwise()
{
	int index[8] = {6, 7, 8, 5, 2, 1, 0, 3};

	rotateFullAspectAnticlockwise(RIGHT, index);

	//front, up, back, down
	for (int i = 0; i < 7; i += 3)
	{
		Color t = aspectColors[FRONT][i + 2];
		aspectColors[FRONT][i + 2] = aspectColors[UP][8 - i];
		aspectColors[UP][8 - i] = aspectColors[BACK][8 - i];	
		aspectColors[BACK][8 - i] = aspectColors[DOWN][i + 2];	
		aspectColors[DOWN][i + 2] = t;
	}	
}

void rotateXMiddleClockwise()
{
	//front, down, back, up
	for (int i = 0; i < 7; i += 3)
	{
		Color t = aspectColors[FRONT][i + 1];
		aspectColors[FRONT][i + 1] = aspectColors[DOWN][i + 1];
		aspectColors[DOWN][i + 1] = aspectColors[BACK][7 - i];//7 - i = 6 - i + 1
		aspectColors[BACK][7 - i] = aspectColors[UP][7 - i];
		aspectColors[UP][7 - i] = t;
	}
}

void rotateXMiddleAnticlockwise()
{
	//front, up, back, down
	for (int i = 0; i < 7; i += 3)
	{
		Color t = aspectColors[FRONT][i + 1];
		aspectColors[FRONT][i + 1] = aspectColors[UP][7 - i];
		aspectColors[UP][7 - i] = aspectColors[BACK][7 - i];	
		aspectColors[BACK][7 - i] = aspectColors[DOWN][i + 1];	
		aspectColors[DOWN][i + 1] = t;
	}
}

void rotateYDownClockwise()
{
	int index[8] = {6, 3, 0, 1, 2, 5, 8, 7};

	rotateFullAspectClockwise(DOWN, index);

	//front,right,back,left
	for (int i = 0; i < 3; i++)
	{
		Color t = aspectColors[FRONT][i];
		aspectColors[FRONT][i] = aspectColors[RIGHT][3 * (2 - i)];
		aspectColors[RIGHT][3 * (2 - i)] = aspectColors[BACK][2 - i];
		aspectColors[BACK][2 - i] = aspectColors[LEFT][i * 3];
		aspectColors[LEFT][i * 3] = t;
	}
}

void rotateYDownAnticlockwise()
{
	int index[8] = {6, 3, 0, 1, 2, 5, 8, 7};

	rotateFullAspectAnticlockwise(DOWN, index);

	//front,left,back,right
	for (int i = 0; i < 3; i++)
	{
		Color t = aspectColors[FRONT][i];
		aspectColors[FRONT][i] = aspectColors[LEFT][3 * i];
		aspectColors[LEFT][3 * i] = aspectColors[BACK][2 - i];
		aspectColors[BACK][2 - i] = aspectColors[RIGHT][(2 - i) * 3];
		aspectColors[RIGHT][(2 - i) * 3] = t;
	}
}

void rotateYUpClockwise()
{
	int index[8] = {6, 3, 0, 1, 2, 5, 8, 7};

	rotateFullAspectClockwise(UP,index);

	//front,right,back,left
	for (int i = 0; i < 3; i++)
	{
		Color t = aspectColors[FRONT][6 + i];
		aspectColors[FRONT][6 + i] = aspectColors[RIGHT][8 - 3 * i];
		aspectColors[RIGHT][8 - 3 * i] = aspectColors[BACK][8 - i];
		aspectColors[BACK][8 - i] = aspectColors[LEFT][2 + 3 * i];
		aspectColors[LEFT][2 + 3 * i] = t;
	}
}

void rotateYUpAnticlockwise()
{
	int index[8] = {6, 3, 0, 1, 2, 5, 8, 7};

	rotateFullAspectAnticlockwise(UP, index);

	//front,left,back,right
	for (int i = 0; i < 3; i++)
	{
		Color t = aspectColors[FRONT][6 + i];
		aspectColors[FRONT][6 + i] = aspectColors[LEFT][2 + 3 * i];
		aspectColors[LEFT][2 + 3 * i] = aspectColors[BACK][8 - i];
		aspectColors[BACK][8 - i] = aspectColors[RIGHT][8 - 3 * i];
		aspectColors[RIGHT][8 - 3 * i] = t;
	}
}

void rotateYMiddleClockwise()
{
	//front,right,back,left
	for (int i = 0; i < 3; i++)
	{
		Color t = aspectColors[FRONT][3 + i];
		aspectColors[FRONT][3 + i] = aspectColors[RIGHT][7 - 3 * i];
		aspectColors[RIGHT][7 - 3 * i] = aspectColors[BACK][5 - i];
		aspectColors[BACK][5 - i] = aspectColors[LEFT][1 + 3 * i];
		aspectColors[LEFT][1 + 3 * i] = t;
	}
}

void rotateYMiddleAnticlockwise()
{
	//front,left,back,right
	for (int i = 0; i < 3; i++)
	{
		Color t = aspectColors[FRONT][3 + i];
		aspectColors[FRONT][3 + i] = aspectColors[LEFT][1 + 3 * i];
		aspectColors[LEFT][1 + 3 * i] = aspectColors[BACK][5 - i];
		aspectColors[BACK][5 - i] = aspectColors[RIGHT][7 - 3 * i];
		aspectColors[RIGHT][7 - 3 * i] = t;
	}
}

void rotateZFrontClockwise()
{
	int index[8] = {0, 3, 6, 7, 8, 5, 2, 1};

	rotateFullAspectClockwise(FRONT, index);

	for(int i = 6; i < 9; i++)
	{
		Color t = aspectColors[UP][i];
		aspectColors[UP][i] = aspectColors[LEFT][i];
		aspectColors[LEFT][i] = aspectColors[DOWN][14 - i];
		aspectColors[DOWN][14 - i] = aspectColors[RIGHT][14 - i];
		aspectColors[RIGHT][14 - i] = t;
	}
}

void rotateZFrontAnticlockwise()
{
	int index[8] = {0, 3, 6, 7, 8, 5, 2, 1};

	rotateFullAspectAnticlockwise(FRONT, index);

	for(int i = 6; i < 9; i++)
	{
		Color t = aspectColors[UP][i];
		aspectColors[UP][i] = aspectColors[RIGHT][14 - i];
		aspectColors[RIGHT][14 - i] = aspectColors[DOWN][14 - i];
		aspectColors[DOWN][14 - i] = aspectColors[LEFT][i];
		aspectColors[LEFT][i] = t;
	}
}

void rotateZBackClockwise()
{
	int index[8] = {0, 3, 6, 7, 8, 5, 2, 1};

	rotateFullAspectClockwise(BACK, index);

	for(int i = 0; i < 3; i++)
	{
		Color t = aspectColors[UP][i];
		aspectColors[UP][i] = aspectColors[LEFT][i];
		aspectColors[LEFT][i] = aspectColors[DOWN][2 - i];
		aspectColors[DOWN][2 - i] = aspectColors[RIGHT][2 - i];
		aspectColors[RIGHT][2 - i] = t;
	}
}

void rotateZBackAnticlockwise()
{
	int index[8] = {0, 3, 6, 7, 8, 5, 2, 1};

	rotateFullAspectAnticlockwise(BACK, index);

	for(int i = 0; i < 3; i++)
	{
		Color t = aspectColors[UP][i];
		aspectColors[UP][i] = aspectColors[RIGHT][2 - i];
		aspectColors[RIGHT][2 - i] = aspectColors[DOWN][2 - i];
		aspectColors[DOWN][2 - i] = aspectColors[LEFT][i];
		aspectColors[LEFT][i] = t;
	}
}

void rotateZMiddleClockwise()
{
	for(int i = 0; i < 3; i++)
	{
		Color t = aspectColors[UP][i + 3];
		aspectColors[UP][i + 3] = aspectColors[LEFT][i + 3];
		aspectColors[LEFT][i + 3] = aspectColors[DOWN][5 - i];
		aspectColors[DOWN][5 - i] = aspectColors[RIGHT][5 - i];
		aspectColors[RIGHT][5 - i] = t;
	}
}

void rotateZMiddleAnticlockwise()
{
	for(int i = 0; i < 3; i++)
	{
		Color t = aspectColors[UP][i + 3];
		aspectColors[UP][i + 3] = aspectColors[RIGHT][5 - i];
		aspectColors[RIGHT][5 - i] = aspectColors[DOWN][5 - i];
		aspectColors[DOWN][5 - i] = aspectColors[LEFT][i + 3];
		aspectColors[LEFT][i + 3] = t;
	}
}

//rotate color table functions
typedef void (*PRotateColorTable)();
PRotateColorTable pRotateColorTable[3][3][2]={//from negative to positive,i.e. from left to right
	//x
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

#undef L//ȡ��L�ĺ궨��
#undef S

PRotateColorTable F = rotateZFrontClockwise;
PRotateColorTable Fr = rotateZFrontAnticlockwise;

PRotateColorTable B = rotateZBackAnticlockwise;
PRotateColorTable Br = rotateZBackClockwise;

PRotateColorTable R = rotateXRightClockwise;
PRotateColorTable Rr = rotateXRightAnticlockwise;

PRotateColorTable L = rotateXLeftAnticlockwise;
PRotateColorTable Lr = rotateXLeftClockwise;

PRotateColorTable U = rotateYUpClockwise;
PRotateColorTable Ur = rotateYUpAnticlockwise;

PRotateColorTable D = rotateYDownAnticlockwise;
PRotateColorTable Dr = rotateYDownClockwise;

//rotate middle layer along y axis clockwisely
PRotateColorTable E = rotateYMiddleClockwise;
PRotateColorTable Er = rotateYMiddleAnticlockwise;
void E2()//ִ��������ת������ת180��
{
	E();
	E();
}

//rotate middle layer along x axis clockwisely
PRotateColorTable M = rotateXMiddleClockwise;
PRotateColorTable Mr = rotateXMiddleAnticlockwise;
void M2()
{
	M();
	M();
}

//rotate middle layer along z axis clockwisely
PRotateColorTable S = rotateZMiddleClockwise;
PRotateColorTable Sr = rotateZMiddleAnticlockwise;
void S2()
{
	S();
	S();
}

void F2()
{
	F();
	F();
}

void B2()
{
	B();
	B();
}

void R2()
{
	R();
	R();
}

void L2()
{
	L();
	L();
}

void U2()
{
	U();
	U();
}

void D2()
{
	D();
	D();
}

//rotate whole cube along x axis clockwisely
void x()
{
	rotateXLeftClockwise();
	rotateXMiddleClockwise();
	rotateXRightClockwise();
}

void xr()
{
	rotateXLeftAnticlockwise();
	rotateXMiddleAnticlockwise();
	rotateXRightAnticlockwise();
}

void x2()
{
	x();
	x();
}

//rotate whole cube along y axis clockwisely
void y()
{
	rotateYDownClockwise();
	rotateYMiddleClockwise();
	rotateYUpClockwise();
}

void yr()
{
	rotateYDownAnticlockwise();
	rotateYMiddleAnticlockwise();
	rotateYUpAnticlockwise();
}

void y2()
{
	y();
	y();
}

//rotate whole cube along z axis clockwisely
void z()
{
	rotateZBackClockwise();
	rotateZMiddleClockwise();
	rotateZFrontClockwise();
}

void zr()
{
	rotateZBackAnticlockwise();
	rotateZMiddleAnticlockwise();
	rotateZFrontAnticlockwise();
}

void z2()
{
	z();
	z();
}

//rotate top two layer clockwisely
void u()
{
	rotateYUpClockwise();
	rotateYMiddleClockwise();
}

void ur()
{
	rotateYUpAnticlockwise();
	rotateYMiddleAnticlockwise();
}

void u2()
{
	u();u();
}

//rotate right two layer clockwisely
void r()
{
	rotateXRightClockwise();
	rotateXMiddleClockwise();
}

void rr()
{
	rotateXRightAnticlockwise();
	rotateXMiddleAnticlockwise();
}

void r2()
{
	r();
	r();
}

//rotate front two layer clockwisely
void f()
{
	rotateZFrontClockwise();
	rotateZMiddleClockwise();
}

void fr()
{
	rotateZFrontAnticlockwise();
	rotateZMiddleAnticlockwise();
}

void f2()
{
	f();
	f();
}

//rotate down two layer anticlockwisely
void d()
{
	rotateYMiddleAnticlockwise();
	rotateYDownAnticlockwise();
}

void dr()
{
	rotateYDownClockwise();
	rotateYMiddleClockwise();
}

void d2()
{
	d();
	d();
}

//rotate left two layer anticlockwisely
void l()
{
	rotateXMiddleAnticlockwise();
	rotateXLeftAnticlockwise();
}

void lr()
{
	rotateXMiddleClockwise();
	rotateXLeftClockwise();
}

void l2()
{
	l();
	l();
}

//rotate back two layer anticlockwisely
void b()
{
	rotateZMiddleAnticlockwise();
	rotateZBackAnticlockwise();
}

void br()
{
	rotateZMiddleClockwise();
	rotateZBackClockwise();
}

void b2()
{
	b();
	b();
}

//internal operations ,only command string manipulation added.
//another way to do this is set parameter for these functions.
//e.g. F(bool b){rotate and if b ,then string manipulation }
//for such short functions ,I prefer write another set of functions.

#define COMMANDLEN 500
//store operation string
static char cmdString[COMMANDLEN];
static int cmdStrPtr = 0;

void _F()
{
	F();
	cmdString[cmdStrPtr++] = 'F';
}

void _Fr()//F'��ʾ��ʱ����ת
{
	Fr();
	cmdString[cmdStrPtr++] = 'F';
	cmdString[cmdStrPtr++] = '\'';
}

void _F2()
{
	F();
	F();
	cmdString[cmdStrPtr++] = 'F';
	cmdString[cmdStrPtr++] = '2';
}

void _B()
{
	B();
	cmdString[cmdStrPtr++] = 'B';
}

void _Br()
{
	Br();
	cmdString[cmdStrPtr++] = 'B';
	cmdString[cmdStrPtr++] = '\'';
}

void _B2()
{
	B();
	B();
	cmdString[cmdStrPtr++] = 'B';
	cmdString[cmdStrPtr++] = '2';
}

void _R()
{
	R();
	cmdString[cmdStrPtr++] = 'R';
}

void _Rr()
{
	Rr();
	cmdString[cmdStrPtr++] = 'R';
	cmdString[cmdStrPtr++] = '\'';
}

void _R2()
{
	R();
	R();
	cmdString[cmdStrPtr++] = 'R';
	cmdString[cmdStrPtr++] = '2';
}

void _L()
{
	L();
	cmdString[cmdStrPtr++] = 'L';
}

void _Lr()
{
	Lr();
	cmdString[cmdStrPtr++] = 'L';
	cmdString[cmdStrPtr++] = '\'';
}

void _L2()
{
	L();
	L();
	cmdString[cmdStrPtr++] = 'L';
	cmdString[cmdStrPtr++] = '2';
}

void _U()
{
	U();
	cmdString[cmdStrPtr++] = 'U';
}

void _Ur()
{
	Ur();
	cmdString[cmdStrPtr++] = 'U';
	cmdString[cmdStrPtr++] = '\'';
}

void _U2()
{
	U();
	U();
	cmdString[cmdStrPtr++] = 'U';
	cmdString[cmdStrPtr++] = '2';
}

void _D()
{
	D();
	cmdString[cmdStrPtr++] = 'D';
}

void _Dr()
{
	Dr();
	cmdString[cmdStrPtr++] = 'D';
	cmdString[cmdStrPtr++] = '\'';
}

void _D2()
{
	D();
	D();
	cmdString[cmdStrPtr++] = 'D';
	cmdString[cmdStrPtr++] = '2';
}

void _E()
{
	E();
	cmdString[cmdStrPtr++] = 'E';
}

void _Er()
{
	Er();
	cmdString[cmdStrPtr++] = 'E';
	cmdString[cmdStrPtr++] = '\'';
}

void _E2()
{
	E();
	E();
	cmdString[cmdStrPtr++] = 'E';
	cmdString[cmdStrPtr++] = '2';
}

void _M()
{
	M();
	cmdString[cmdStrPtr++] = 'M';
}

void _Mr()
{
	Mr();
	cmdString[cmdStrPtr++] = 'M';
	cmdString[cmdStrPtr++] = '\'';
}

#undef _M2//_M2���궨�壬�˴���Ҫundef������
void _M2()
{
	M();
	M();
	cmdString[cmdStrPtr++] = 'M';
	cmdString[cmdStrPtr++] = '2';
}

void _S()
{
	S();
	cmdString[cmdStrPtr++] = 'S';
}

void _Sr()
{
	Sr();
	cmdString[cmdStrPtr++] = 'S';
	cmdString[cmdStrPtr++] = '\'';
}

void _S2()
{
	S();
	S();
	cmdString[cmdStrPtr++] = 'S';
	cmdString[cmdStrPtr++] = '2';
}

void _x()
{
	x();
	cmdString[cmdStrPtr++] = 'x';
}

void _xr()
{
	xr();
	cmdString[cmdStrPtr++] = 'x';
	cmdString[cmdStrPtr++] = '\'';
}

void _x2()
{
	x();
	x();
	cmdString[cmdStrPtr++] = 'x';
	cmdString[cmdStrPtr++] = '2';
}

void _y()
{
	y();
	cmdString[cmdStrPtr++] = 'y';
}

void _yr()
{
	yr();
	cmdString[cmdStrPtr++] = 'y';
	cmdString[cmdStrPtr++] = '\'';
}

void _y2()
{
	y();
	y();
	cmdString[cmdStrPtr++] = 'y';
	cmdString[cmdStrPtr++] = '2';
}

void _z()
{
	z();
	cmdString[cmdStrPtr++] = 'z';
}

void _zr()
{
	zr();
	cmdString[cmdStrPtr++] = 'z';
	cmdString[cmdStrPtr++] = '\'';
}

void _z2()
{
	z();
	z();
	cmdString[cmdStrPtr++] = 'z';
	cmdString[cmdStrPtr++] = '2';
}

void _u()
{
	u();
	cmdString[cmdStrPtr++] = 'u';
}

void _ur()
{
	ur();
	cmdString[cmdStrPtr++] = 'u';
	cmdString[cmdStrPtr++] = '\'';
}

void _u2()
{
	u();
	u();
	cmdString[cmdStrPtr++] = 'u';
	cmdString[cmdStrPtr++] = '2';
}

void _r()
{
	r();
	cmdString[cmdStrPtr++] = 'r';
}

void _rr()
{
	rr();
	cmdString[cmdStrPtr++] = 'r';
	cmdString[cmdStrPtr++] = '\'';
}

void _r2()
{
	r();
	r();
	cmdString[cmdStrPtr++] = 'r';
	cmdString[cmdStrPtr++] = '2';
}

void _f()
{
	f();
	cmdString[cmdStrPtr++] = 'f';
}

void _fr()
{
	fr();
	cmdString[cmdStrPtr++] = 'f';
	cmdString[cmdStrPtr++] = '\'';
}

void _f2()
{
	f();
	f();
	cmdString[cmdStrPtr++] = 'f';
	cmdString[cmdStrPtr++] = '2';
}

void _d()
{
	d();
	cmdString[cmdStrPtr++] = 'd';
}

void _dr()
{
	dr();
	cmdString[cmdStrPtr++] = 'd';
	cmdString[cmdStrPtr++] = '\'';
}

void _d2()
{
	d();
	d();
	cmdString[cmdStrPtr++] = 'd';
	cmdString[cmdStrPtr++] = '2';
}

void _l()
{
	l();
	cmdString[cmdStrPtr++] = 'l';
}

void _lr()
{
	lr();
	cmdString[cmdStrPtr++] = 'l';
	cmdString[cmdStrPtr++] = '\'';
}

void _l2()
{
	l();
	l();
	cmdString[cmdStrPtr++] = 'l';
	cmdString[cmdStrPtr++] = '2';
}

void _b()
{
	b();
	cmdString[cmdStrPtr++] = 'b';
}

void _br()
{
	br();
	cmdString[cmdStrPtr++] = 'b';
	cmdString[cmdStrPtr++] = '\'';
}

void _b2()
{
	b();
	b();
	cmdString[cmdStrPtr++] = 'b';
	cmdString[cmdStrPtr++] = '2';
}

bool IsCrossed()
{	
	for (int i = 1; i < 8; i += 2)
	{
		if (aspectColors[DOWN][4] != aspectColors[DOWN][i])
		{
			return false;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if(i < 2)
		{
			if (aspectColors[i][4] != aspectColors[i][1])
			{
				return false;
			}
		}
		else
		{
			if (aspectColors[i][4] != aspectColors[i][3])
			{
				return false;
			}
		}

	}
	return true;
}

CString Cross()
{
	//two method to make cross: rule database or search rules are used here
	memset(cmdString, 0, COMMANDLEN);
	cmdStrPtr = 0;

	int testCount = 0;

	while (!IsCrossed())
	{
		if (testCount > 3)
		{
			MessageBox(NULL, _T("bug emerged in function: Cross()"), _T("error"), MB_OK);

			char * asp[6] = {"FRONT��:", "BACK��:", "LEFT��:", "RIGHT��:", "UP��:", "DOWN��:"};
			char * col[6] = {"BLUE", "GREEN", "RED", "ORANGE", "WHITE", "YELLOW"};

			FILE * fp = fopen("bug/Cross.txt", "a+");
			if(fp != NULL)
			{
				for(int i = 0; i < 6; i++)
				{
					fprintf(fp, "%s\n", asp[i]);
					for(int j = 0; j < 9; j++)
					{
						fprintf(fp, "%11s", col[aspectColors[i][j]]);
						if(8 == j)
						{
							fprintf(fp, "\n");
						}
					}
				}
				fprintf(fp, "\n\n");
				fclose(fp);
			}
			exit(-1);
		}

		int numDone = 0;//��¼���㻨�����(����ʮ�ּܳ������ĵ�������)
		while(numDone < 4)
		{
			if(aspectColors[UP][3] == aspectColors[DOWN][4] &&
				aspectColors[UP][7] == aspectColors[DOWN][4] &&
				aspectColors[UP][5] == aspectColors[DOWN][4] &&
				aspectColors[UP][1] == aspectColors[DOWN][4])
			{
				numDone = 4;
				continue;
			}

			if(0 == numDone)
			{
				//���Կ��Ǻ����һ���жϣ�������ÿ�ζ�ҪnumDone++
				//UP��
				if(aspectColors[UP][3] == aspectColors[DOWN][4])
				{
					numDone++;
					_y();//��Y��˳ʱ����ת90
				}
				else if(aspectColors[UP][7] == aspectColors[DOWN][4])//����UP���ʡ������ٲ���
				{
					_y();
				}
				else if(aspectColors[UP][5] == aspectColors[DOWN][4])
				{
					_y2();
				}
				else if(aspectColors[UP][1] == aspectColors[DOWN][4])
				{
					_yr();
				}
				//DOWN������ת�м�㣡
				else if(aspectColors[DOWN][3] == aspectColors[DOWN][4])
				{
					_L2();
				}
				else if(aspectColors[DOWN][7] == aspectColors[DOWN][4])
				{
					//_Dr();
					//_L2();
					_F2();
					_y();
				}
				else if(aspectColors[DOWN][5] == aspectColors[DOWN][4])
				{
					//_R2();
					//_U2();
					_R2();
					_y2();
				}
				else if(aspectColors[DOWN][1] == aspectColors[DOWN][4])
				{
					_B2();
					_yr();
				}
				//FRONT
				else if(aspectColors[FRONT][3] == aspectColors[DOWN][4])
				{
					_Lr();
				}
				else if(aspectColors[FRONT][1] == aspectColors[DOWN][4])
				{
					_F();
					_Lr();
				}
				else if(aspectColors[FRONT][5] == aspectColors[DOWN][4])
				{
					_R();
					_y2();
				}
				else if(aspectColors[FRONT][7] == aspectColors[DOWN][4])
				{
					_Fr();
					_Lr();
				}
				//BACK
				else if(aspectColors[BACK][3] == aspectColors[DOWN][4])
				{
					_L();
				}
				else if(aspectColors[BACK][1] == aspectColors[DOWN][4])
				{
					_Br();
					_L();
				}
				else if(aspectColors[BACK][5] == aspectColors[DOWN][4])
				{
					_Rr();
					_y2();

				}
				else if(aspectColors[BACK][7] == aspectColors[DOWN][4])
				{
					_B();
					_L();
				}
				//LEFT
				else if(aspectColors[LEFT][3] == aspectColors[DOWN][4])
				{
					_Lr();
					_F();
					_y();
				}
				else if(aspectColors[LEFT][1] == aspectColors[DOWN][4])
				{
					_Br();
					_yr();
				}
				else if(aspectColors[LEFT][5] == aspectColors[DOWN][4])
				{
					_L();
					_F();
					_y();
				}
				else if(aspectColors[LEFT][7] == aspectColors[DOWN][4])
				{
					_F();
					_y();
				}
				else if(aspectColors[RIGHT][3] == aspectColors[DOWN][4])
				{
					_R();
					_Fr();
					_y();
				}
				else if(aspectColors[RIGHT][1] == aspectColors[DOWN][4])
				{
					_B();
					_yr();
				}
				else if(aspectColors[RIGHT][5] == aspectColors[DOWN][4])
				{
					_Rr();
					_Fr();
					_y();
				}
				else if(aspectColors[RIGHT][7] == aspectColors[DOWN][4])
				{
					_Fr();
					_y();
				}
			}
			else if(1 == numDone)//����ת�м��
			{
				if(aspectColors[UP][3] == aspectColors[DOWN][4])
				{
					numDone++;
					_y();//��Y��˳ʱ����ת90
				}
				else if(aspectColors[UP][7] == aspectColors[DOWN][4])
				{
					_F();
					_E();//����
					_Lr();
				}
				else if(aspectColors[UP][5] == aspectColors[DOWN][4])
				{
					_yr();
				}
				//DOWN������ת�м�㣡
				else if(aspectColors[DOWN][3] == aspectColors[DOWN][4])
				{
					_L2();
				}
				else if(aspectColors[DOWN][7] == aspectColors[DOWN][4])
				{
					_Dr();
					_L2();
				}
				else if(aspectColors[DOWN][5] == aspectColors[DOWN][4])
				{
					//_D2();
					//_L2();
					_R2();
					_yr();
				}
				else if(aspectColors[DOWN][1] == aspectColors[DOWN][4])
				{
					_D();
					_L2();
				}
				//FRONT
				else if(aspectColors[FRONT][3] == aspectColors[DOWN][4])
				{
					_Lr();
				}
				else if(aspectColors[FRONT][1] == aspectColors[DOWN][4])
				{
					_F();
					_Lr();
				}
				else if(aspectColors[FRONT][5] == aspectColors[DOWN][4])
				{
					_R();
					_yr();
				}
				else if(aspectColors[FRONT][7] == aspectColors[DOWN][4])
				{
					_Fr();
					_Lr();
				}
				//BACK
				else if(aspectColors[BACK][3] == aspectColors[DOWN][4])
				{
					_L();
				}
				else if(aspectColors[BACK][1] == aspectColors[DOWN][4])
				{
					_D2();
					//_S();//����ת�������м��ı�
					_F();
					_Lr();
				}
				else if(aspectColors[BACK][5] == aspectColors[DOWN][4])
				{
					_Rr();
					_yr();
				}
				//LEFT
				else if(aspectColors[LEFT][3] == aspectColors[DOWN][4])
				{
					_D();
					_F();
					_Lr();
				}
				else if(aspectColors[LEFT][1] == aspectColors[DOWN][4])
				{
					_Er();
					_Lr();
				}
				else if(aspectColors[LEFT][5] == aspectColors[DOWN][4])
				{
					_L();
					_E();
					_L();
				}
				else if(aspectColors[LEFT][7] == aspectColors[DOWN][4])
				{
					_E();
					_L();
				}
				//RIGHT
				else if(aspectColors[RIGHT][3] == aspectColors[DOWN][4])
				{
					_Dr();
					_F();
					_Lr();
				}
				else if(aspectColors[RIGHT][1] == aspectColors[DOWN][4])
				{
					_Er();
					_L();
				}
				else if(aspectColors[RIGHT][5] == aspectColors[DOWN][4])
				{
					_Rr();
					_E();
					_Lr();
				}
				else if(aspectColors[RIGHT][7] == aspectColors[DOWN][4])
				{
					_E();
					_Lr();
				}
			}
			else if(2 == numDone)
			{
				if(aspectColors[UP][3] == aspectColors[DOWN][4])
				{
					numDone++;
					_y();//��Y��˳ʱ����ת90
				}
				else if(aspectColors[UP][7] == aspectColors[DOWN][4])
				{
					_yr();
				}
				//DOWN������ת�м�㣡
				else if(aspectColors[DOWN][3] == aspectColors[DOWN][4])
				{
					_L2();
				}
				else if(aspectColors[DOWN][7] == aspectColors[DOWN][4])
				{
					_F2();
					_yr();
				}
				else if(aspectColors[DOWN][5] == aspectColors[DOWN][4])
				{
					_D2();
					_L2();
				}
				else if(aspectColors[DOWN][1] == aspectColors[DOWN][4])
				{
					_D();
					_L2();
				}
				//FRONT
				else if(aspectColors[FRONT][3] == aspectColors[DOWN][4])
				{
					_Lr();
				}
				else if(aspectColors[FRONT][1] == aspectColors[DOWN][4])
				{
					_F();
					_Lr();
				}
				else if(aspectColors[FRONT][5] == aspectColors[DOWN][4])
				{
					_F2();
					_Lr();
				}
				else if(aspectColors[FRONT][7] == aspectColors[DOWN][4])
				{
					_Fr();
					_Lr();
				}
				//BACK
				else if(aspectColors[BACK][3] == aspectColors[DOWN][4])
				{
					_L();
				}
				else if(aspectColors[BACK][1] == aspectColors[DOWN][4])
				{
					_D2();
					_F();
					_Lr();
				}
				else if(aspectColors[BACK][5] == aspectColors[DOWN][4])
				{
					//_E();
					//_Fr();
					//_yr();
					_E2();
					_Lr();
				}
				//LEFT
				else if(aspectColors[LEFT][3] == aspectColors[DOWN][4])
				{
					_D();
					_F();
					_Lr();
				}
				else if(aspectColors[LEFT][1] == aspectColors[DOWN][4])
				{
					_Er();
					_Lr();
				}
				else if(aspectColors[LEFT][5] == aspectColors[DOWN][4])
				{
					_L();
					_E();
					_L();
				}
				else if(aspectColors[LEFT][7] == aspectColors[DOWN][4])
				{
					_E();
					_L();
				}
				//RIGHT
				else if(aspectColors[RIGHT][3] == aspectColors[DOWN][4])
				{
					_Dr();
					_F();
					_Lr();
				}
				else if(aspectColors[RIGHT][1] == aspectColors[DOWN][4])
				{
					_Er();
					_L();
				}
				else if(aspectColors[RIGHT][7] == aspectColors[DOWN][4])
				{
					_E();
					_Lr();
				}
			}
			else if(3 == numDone)
			{
				if(aspectColors[UP][3] == aspectColors[DOWN][4])
				{
					numDone++;
					_y();//��Y��˳ʱ����ת90
				}
				//DOWN������ת�м�㣡
				else if(aspectColors[DOWN][3] == aspectColors[DOWN][4])
				{
					_L2();
				}
				else if(aspectColors[DOWN][7] == aspectColors[DOWN][4])
				{
					_Dr();
					_L2();
				}
				else if(aspectColors[DOWN][5] == aspectColors[DOWN][4])
				{
					_D2();
					_L2();
				}
				else if(aspectColors[DOWN][1] == aspectColors[DOWN][4])
				{
					_D();
					_L2();
				}
				//FRONT
				else if(aspectColors[FRONT][3] == aspectColors[DOWN][4])
				{
					_Lr();
				}
				else if(aspectColors[FRONT][1] == aspectColors[DOWN][4])
				{
					_Dr();
					_Lr();
					_E();
					_L();
				}
				else if(aspectColors[FRONT][5] == aspectColors[DOWN][4])
				{
					_E2();
					_L();
				}
				//BACK
				else if(aspectColors[BACK][3] == aspectColors[DOWN][4])
				{
					_L();
				}
				else if(aspectColors[BACK][1] == aspectColors[DOWN][4])
				{
					_D();
					_Lr();
					_E();
					_L();
				}
				else if(aspectColors[BACK][5] == aspectColors[DOWN][4])
				{
					_E2();
					_Lr();
				}
				//LEFT
				else if(aspectColors[LEFT][3] == aspectColors[DOWN][4])
				{
					_Lr();
					_E();
					_L();
				}
				else if(aspectColors[LEFT][1] == aspectColors[DOWN][4])
				{
					_Er();
					_Lr();
				}
				else if(aspectColors[LEFT][5] == aspectColors[DOWN][4])
				{
					_L();
					_E();
					_L();
				}
				else if(aspectColors[LEFT][7] == aspectColors[DOWN][4])
				{
					_E();
					_L();
				}
				//RIGHT
				else if(aspectColors[RIGHT][3] == aspectColors[DOWN][4])
				{
					_D2();
					_Lr();
					_E();
					_L();
				}
				else if(aspectColors[RIGHT][1] == aspectColors[DOWN][4])
				{
					_Er();
					_L();
				}
				else if(aspectColors[RIGHT][7] == aspectColors[DOWN][4])
				{
					_E();
					_Lr();
				}
			}

			if(aspectColors[UP][3] == aspectColors[DOWN][4])
			{
				numDone++;
				_y();//��Y��˳ʱ����ת90
			}
		}

		for(int i = 0; i < 4; i++)
		{
			//while(aspectColors[FRONT][7] != aspectColors[FRONT][4])
			//{
			//	_U();
			//}
			if(aspectColors[BACK][7] == aspectColors[FRONT][4] && aspectColors[UP][1] == aspectColors[DOWN][4])
			{
				_U2();
			}
			else if(aspectColors[LEFT][5] == aspectColors[FRONT][4] && aspectColors[UP][3] == aspectColors[DOWN][4])
			{
				_Ur();
			}
			else if(aspectColors[RIGHT][5] == aspectColors[FRONT][4] && aspectColors[UP][5] == aspectColors[DOWN][4])
			{
				_U();
			}

			//if(aspectColors[FRONT][7] == aspectColors[FRONT][4] && aspectColors[UP][7] == aspectColors[DOWN][4])
			//{
				_F2();
				_y();
			//}
		}
	}

	_z2();//��ʮ��ת������

	return CString(cmdString);
}

bool IsFirstLayerCornerRestored()
{
	if (aspectColors[FRONT][6] == aspectColors[FRONT][7] && aspectColors[FRONT][8] == aspectColors[FRONT][7] &&
		aspectColors[BACK][6] == aspectColors[BACK][7] && aspectColors[BACK][8] == aspectColors[BACK][7] &&
		aspectColors[LEFT][2] == aspectColors[LEFT][5] && aspectColors[LEFT][8] == aspectColors[LEFT][5] &&
		aspectColors[RIGHT][2] == aspectColors[RIGHT][5] && aspectColors[RIGHT][8] == aspectColors[RIGHT][5])
	{
		for (int i = 1; i < 9; i += 2)
		{
			if (aspectColors[UP][i] != aspectColors[UP][4])
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

int HashCorner(Color c1, Color c2, Color c3)
{
	return (c1 + 1) * (c2 + 1) * (c3 + 1);
}

CString RestoreFirstLayerCorner()
{
	memset(cmdString, 0, COMMANDLEN);
	cmdStrPtr = 0;

	int testCount = 0;

	while (!IsFirstLayerCornerRestored())
	{
		testCount++;
		if (testCount > 8)
		{
			MessageBox(NULL, _T("bug emerged in function: RestoreFirstLayerCorner()"), _T("error"), MB_OK);

			char * asp[6] = {"FRONT��:", "BACK��:", "LEFT��:", "RIGHT��:", "UP��:", "DOWN��:"};
			char * col[6] = {"BLUE", "GREEN", "RED", "ORANGE", "WHITE", "YELLOW"};

			FILE * fp = fopen("bug/RestoreFirstLayerCorner.txt", "a+");
			if(fp != NULL)
			{
				for(int i = 0; i < 6; i++)
				{
					fprintf(fp, "%s\n", asp[i]);
					for(int j = 0; j < 9; j++)
					{
						fprintf(fp, "%11s", col[aspectColors[i][j]]);
						if(8 == j)
						{
							fprintf(fp, "\n");
						}
					}
				}
				fprintf(fp, "\n\n");
				fclose(fp);
			}
			exit(-1);
		}

		int cornerHashDst[8];//hash value of this corner should be

		cornerHashDst[0] = HashCorner(aspectColors[UP][4], aspectColors[BACK][4], aspectColors[LEFT][4]);
		cornerHashDst[1] = HashCorner(aspectColors[UP][4], aspectColors[LEFT][4], aspectColors[FRONT][4]);
		cornerHashDst[2] = HashCorner(aspectColors[UP][4], aspectColors[FRONT][4], aspectColors[RIGHT][4]);
		cornerHashDst[3] = HashCorner(aspectColors[UP][4], aspectColors[RIGHT][4], aspectColors[BACK][4]);

		cornerHashDst[4] = HashCorner(aspectColors[DOWN][4], aspectColors[BACK][4], aspectColors[LEFT][4]);
		cornerHashDst[5] = HashCorner(aspectColors[DOWN][4], aspectColors[LEFT][4], aspectColors[FRONT][4]);
		cornerHashDst[6] = HashCorner(aspectColors[DOWN][4], aspectColors[FRONT][4], aspectColors[RIGHT][4]);
		cornerHashDst[7] = HashCorner(aspectColors[DOWN][4], aspectColors[RIGHT][4], aspectColors[BACK][4]);

		int cornerHashSrc[8];//hash value for current corners
		//start from leftBack corner,counterclockwise

		cornerHashSrc[0] = HashCorner(aspectColors[UP][0], aspectColors[BACK][6], aspectColors[LEFT][2]);
		cornerHashSrc[1] = HashCorner(aspectColors[UP][6], aspectColors[LEFT][8], aspectColors[FRONT][6]);
		cornerHashSrc[2] = HashCorner(aspectColors[UP][8], aspectColors[FRONT][8], aspectColors[RIGHT][8]);
		cornerHashSrc[3] = HashCorner(aspectColors[UP][2], aspectColors[RIGHT][2], aspectColors[BACK][8]);

		cornerHashSrc[4] = HashCorner(aspectColors[DOWN][0], aspectColors[BACK][0], aspectColors[LEFT][0]);
		cornerHashSrc[5] = HashCorner(aspectColors[DOWN][6], aspectColors[LEFT][6], aspectColors[FRONT][0]);
		cornerHashSrc[6] = HashCorner(aspectColors[DOWN][8], aspectColors[FRONT][2], aspectColors[RIGHT][6]);
		cornerHashSrc[7] = HashCorner(aspectColors[DOWN][2], aspectColors[RIGHT][0], aspectColors[BACK][2]);

		//�Ȱ����п�������Ƶ�Ŀ��λ�·�
		//����ǰ�ǿ�ʼ��ԭ
		if(cornerHashDst[2] == cornerHashSrc[0])//�����ǰ�Ͻ��ڴ�ʱ������Ͻ�
		{
			if(aspectColors[LEFT][2] == aspectColors[UP][4])//������������ǲ��Ǻ���һ���������UP4ɫ�ڵ���������
			{
				_Lr();
				_D2();
				_L();
			}
			else
			{
				_B();
				_D2();
				_Br();
			}
		}
		else if(cornerHashDst[2] == cornerHashSrc[1])//�����ǰ�Ͻ��ڴ�ʱ����ǰ�Ͻ�
		{
			if(aspectColors[FRONT][6] == aspectColors[UP][4])
			{
				_Fr();
				_D2();
				_F();
				_Dr();
			}
			else
			{
				_L();
				_D();
				_Lr();
			}	
		}
		else if(cornerHashDst[2] == cornerHashSrc[2])
		{
			if(aspectColors[UP][8] != aspectColors[UP][4])//�ǲ���ֻҪ8��4����ɫһ����һ���ź��ˣ�
			{
				if(aspectColors[RIGHT][8] == aspectColors[UP][4])
				{
					_F();
					_Dr();
					_Fr();
				}
				else
				{
					_Rr();
					_D();
					_R();
				}				
			}
		}
		else if(cornerHashDst[2] == cornerHashSrc[3])
		{
			if(aspectColors[RIGHT][2] == aspectColors[UP][4])
			{
				_R();
				_D2();
				_Rr();
				_D();
			}
			else
			{
				_Br();
				_Dr();
				_B();
			}
		}
		else if(cornerHashDst[2] == cornerHashSrc[4])
		{
			_D2();
		}
		else if(cornerHashDst[2] == cornerHashSrc[5])
		{
			_D();
		}
		else if(cornerHashDst[2] == cornerHashSrc[7])
		{
			_Dr();
		}

		//��ʱ��ǰ�Ͻ�����Ŀ��λ�·���������ǰ�½�
		//���������UP4ɫ��ǰ�桢���桢����
		if(aspectColors[FRONT][2] == aspectColors[UP][4])
		{
			_F();
			_D();
			_Fr();
		}
		else if(aspectColors[RIGHT][6] == aspectColors[UP][4])
		{
			_Rr();
			_Dr();
			_R();
		}
		else if(aspectColors[DOWN][8] == aspectColors[UP][4])
		{
			_Rr();
			_D2();
			_R();
			_D();
		}

		if(aspectColors[UP][8] == aspectColors[UP][4] &&//�������������ƺ�û��Ҫ
			aspectColors[FRONT][8] == aspectColors[FRONT][4] && aspectColors[RIGHT][8] == aspectColors[RIGHT][4])
		{
			_y();
		}
	}

	_z2();//��Z�ᷭ����

	return CString(cmdString);
}

//whether edges on second layer are restored
bool IsSecondLayerEdgeRestored()
{
	if (aspectColors[FRONT][4] == aspectColors[FRONT][3] && aspectColors[FRONT][4] == aspectColors[FRONT][5]
	&& aspectColors[BACK][4] == aspectColors[BACK][3] && aspectColors[BACK][4] == aspectColors[BACK][5]
	&& aspectColors[LEFT][4] == aspectColors[LEFT][1] && aspectColors[LEFT][4] == aspectColors[LEFT][7]
	&& aspectColors[RIGHT][4] == aspectColors[RIGHT][1] && aspectColors[RIGHT][4] == aspectColors[RIGHT][7])
	{
		return true;
	}
	return false;
}

CString RestoreSecondLayerEdge()
{
	memset(cmdString, 0, COMMANDLEN);
	cmdStrPtr = 0;

	int testCount = 0;
	while(!IsSecondLayerEdgeRestored())
	{
		testCount++;
		if (testCount > 8)
		{
			MessageBox(NULL, _T("bug emerged in function: RestoreSecondLayerEdge()"), _T("error"), MB_OK);

			char * asp[6] = {"FRONT��:", "BACK��:", "LEFT��:", "RIGHT��:", "UP��:", "DOWN��:"};
			char * col[6] = {"BLUE", "GREEN", "RED", "ORANGE", "WHITE", "YELLOW"};

			//FILE * fp = fopen("E:\develop\VS2010_Projects\Rubik_Cube\MagicCube3D\bug\RestoreSecondLayerEdge.txt",
			//	"a+");
			FILE * fp = fopen("bug/RestoreSecondLayerEdge.txt", "a+");
			if(fp != NULL)
			{
				for(int i = 0; i < 6; i++)
				{
					fprintf(fp, "%s\n", asp[i]);
					for(int j = 0; j < 9; j++)
					{
						//fprintf(fp, "%s\t", col[aspectColors[i][j]]);
						fprintf(fp, "%11s", col[aspectColors[i][j]]);
						if(8 == j)
						{
							fprintf(fp, "\n");
						}
					}
				}
				fprintf(fp, "\n\n");
				fclose(fp);
			}
			exit(-1);
		}

		if(aspectColors[FRONT][5] != aspectColors[UP][4] &&
			aspectColors[RIGHT][7] != aspectColors[UP][4])
		{
			if(aspectColors[FRONT][5] != aspectColors[FRONT][4] ||
				aspectColors[RIGHT][7] != aspectColors[RIGHT][4])//�᲻�����F5��ɫ�Զ�R7�Բ��ϵ������
			{
				while(!(aspectColors[UP][3] == aspectColors[UP][4] || aspectColors[LEFT][5] == aspectColors[UP][4]))
				{
					_U();
				}
				//�Ȱ��޹صĶ�����Ƶ�Ŀ��λ�ã���Ŀ����Ƶ�����
				_R(); _Ur(); _Rr(); _Ur(); _Fr(); _U(); _F();//Ŀ�����UP1�����ǹ̶�����
			}
		}
		else if(aspectColors[FRONT][5] == aspectColors[UP][4] ||
			aspectColors[RIGHT][7] == aspectColors[UP][4])
		{//Ҳ�������������F5�麬��UP4ɫ������Ŀ���������������λ��
			if((aspectColors[FRONT][4] + 1) * (aspectColors[RIGHT][4] + 1) ==
				(aspectColors[LEFT][1] + 1) * (aspectColors[BACK][3] + 1) ||
				(aspectColors[FRONT][4] + 1) * (aspectColors[RIGHT][4] + 1) ==
				(aspectColors[RIGHT][1] + 1) * (aspectColors[BACK][5] + 1) ||
				(aspectColors[FRONT][4] + 1) * (aspectColors[RIGHT][4] + 1) ==
				(aspectColors[FRONT][3] + 1) * (aspectColors[LEFT][7] + 1))
			{
				_y();
				continue;
			}
		}

		if(aspectColors[BACK][7] == aspectColors[FRONT][4] && aspectColors[UP][1] == aspectColors[RIGHT][4])
		{
			_Ur(); _R(); _Ur(); _Rr(); _Ur(); _Fr(); _U(); _F();
		}
		else if(aspectColors[FRONT][7] == aspectColors[FRONT][4] && aspectColors[UP][7] == aspectColors[RIGHT][4])
		{
			_U(); _R(); _Ur(); _Rr(); _Ur(); _Fr(); _U(); _F();
		}
		else if(aspectColors[RIGHT][5] == aspectColors[FRONT][4] && aspectColors[UP][5] == aspectColors[RIGHT][4])
		{
			_U2(); _R(); _Ur(); _Rr(); _Ur(); _Fr(); _U(); _F();
		}
		else if(aspectColors[LEFT][5] == aspectColors[FRONT][4] && aspectColors[UP][3] == aspectColors[RIGHT][4])
		{
			_R(); _Ur(); _Rr(); _Ur(); _Fr(); _U(); _F();
		}
		else if(aspectColors[RIGHT][5] == aspectColors[RIGHT][4] && aspectColors[UP][5] == aspectColors[FRONT][4])
		{
			_Ur(); _Fr(); _U(); _F(); _U(); _R(); _Ur(); _Rr();
		}
		else if(aspectColors[BACK][7] == aspectColors[RIGHT][4] && aspectColors[UP][1] == aspectColors[FRONT][4])
		{
			_Fr(); _U(); _F(); _U(); _R(); _Ur(); _Rr();
		}
		else if(aspectColors[LEFT][5] == aspectColors[RIGHT][4] && aspectColors[UP][3] == aspectColors[FRONT][4])
		{
			_U(); _Fr(); _U(); _F(); _U(); _R(); _Ur(); _Rr();
		}
		else if(aspectColors[FRONT][7] == aspectColors[RIGHT][4] && aspectColors[UP][7] == aspectColors[FRONT][4])
		{
			_U2(); _Fr(); _U(); _F(); _U(); _R(); _Ur(); _Rr();
		}

		if(aspectColors[FRONT][5] == aspectColors[FRONT][4] && aspectColors[RIGHT][4] == aspectColors[RIGHT][7])
		{
			_y();
		}
	}
	return CString(cmdString);
}

CString F2L()//�����źõ�ǰ������Ϣ
{
	CString ret;
	ret = "#Cross:\r\n\t#";
	ret += Cross();
	
	ret += "#\r\nRestore Corners of First Layer:\r\n\t#";
	ret += RestoreFirstLayerCorner();

	ret += "#\r\nRestore Edges of Second Layer:\r\n\t#";
	ret += RestoreSecondLayerEdge();

	return ret;
}

bool IsTopCrossed()
{
	for (int i = 1; i < 9; i += 2)
	{
		if (aspectColors[UP][i] != aspectColors[UP][4])
		{
			return false;
		}
	}
	return true;
}

CString BuildTopCross()
{
	memset(cmdString, 0, COMMANDLEN);
	cmdStrPtr = 0;

	int testCount = 0;
	while(!IsTopCrossed())
	{
		testCount++;
		if (testCount > 5)
		{
			MessageBox(NULL, _T("bug emerged in function: BuildTopCross()"), _T("error"), MB_OK);

			char * asp[6] = {"FRONT��:", "BACK��:", "LEFT��:", "RIGHT��:", "UP��:", "DOWN��:"};
			char * col[6] = {"BLUE", "GREEN", "RED", "ORANGE", "WHITE", "YELLOW"};

			FILE * fp = fopen("bug/BuildTopCross.txt", "a+");
			if(fp != NULL)
			{
				for(int i = 0; i < 6; i++)
				{
					fprintf(fp, "%s\n", asp[i]);
					for(int j = 0; j < 9; j++)
					{
						fprintf(fp, "%11s", col[aspectColors[i][j]]);
						if(8 == j)
						{
							fprintf(fp, "\n");
						}
					}
				}
				fprintf(fp, "\n\n");
				fclose(fp);
			}
			exit(-1);
		}

		int num = 0;//��¼���㣨�������ģ�������ͬɫ�ĸ���
		for(int i = 1; i < 8; i += 2)
		{
			if(aspectColors[UP][4] == aspectColors[UP][i])
			{
				num++;
			}
		}

		if(0 == num)
		{
			_F(); _R(); _U(); _Rr(); _Ur(); _Fr();
		}
		else if(2 == num)
		{
			if(aspectColors[UP][4] == aspectColors[UP][1] && aspectColors[UP][4] == aspectColors[UP][7])
			{
				_U(); _F(); _R(); _U(); _Rr(); _Ur(); _Fr();
			}
			else if(aspectColors[UP][4] == aspectColors[UP][3] && aspectColors[UP][4] == aspectColors[UP][5])
			{
				_F(); _R(); _U(); _Rr(); _Ur(); _Fr();
			}
			else if(aspectColors[UP][4] == aspectColors[UP][3] && aspectColors[UP][4] == aspectColors[UP][1])
			{
				_U2(); _f(); _R(); _U(); _Rr(); _Ur(); _fr();
			}
			else if(aspectColors[UP][4] == aspectColors[UP][3] && aspectColors[UP][4] == aspectColors[UP][7])
			{
				_Ur(); _f(); _R(); _U(); _Rr(); _Ur(); _fr();
			}
			else if(aspectColors[UP][4] == aspectColors[UP][5] && aspectColors[UP][4] == aspectColors[UP][7])
			{
				_f(); _R(); _U(); _Rr(); _Ur(); _fr();
			}
			else if(aspectColors[UP][4] == aspectColors[UP][5] && aspectColors[UP][4] == aspectColors[UP][1])
			{
				_U(); _f(); _R(); _U(); _Rr(); _Ur(); _fr();
			}
		}
	}
	return CString(cmdString);
}

void Executor(const char* cmd)
{
	while(*cmd)
	{
		switch(*cmd)
		{
		case ' '://�Ȱѿո��˵�							
			break;

		case '#':				
			do
			{
				cmd++;
			}
			while(*cmd != '#' && *cmd);
			break;

		case 'F':
			if ('\'' == *(cmd + 1))
			{
				Fr();	
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				F2();
				cmd++;
			}
			else
			{
				F();
			}
			break;

		case 'B':
			if ('\'' == *(cmd + 1))
			{
				Br();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				B2();
				cmd++;
			}
			else
			{
				B();
			}
			break;

		case 'L':
			if ('\'' == *(cmd + 1))
			{	
				Lr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				L2();
				cmd++;
			}
			else
			{
				L();
			}
			break;

		case 'R':
			if ('\'' == *(cmd + 1))
			{
				Rr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				R2();
				cmd++;
			}
			else
			{
				R();
			}
			break;

		case 'U':
			if ('\'' == *(cmd + 1))
			{
				Ur();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				U2();
				cmd++;
			}
			else
			{
				U();
			}
			break;

		case 'D':
			if ('\'' == *(cmd + 1))
			{
				Dr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				D2();
				cmd++;
			}
			else
			{
				D();
			}
			break;

		case 'x':
			if ('\'' == *(cmd + 1))
			{
				xr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				x2();
				cmd++;
			}
			else
			{
				x();
			}
			break;

		case 'y':
			if ('\'' == *(cmd + 1))
			{
				yr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				y2();
				cmd++;
			}
			else
			{
				y();
			}
			break;

		case 'z':
			if ('\'' == *(cmd + 1))
			{
				zr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				z2();
				cmd++;
			}
			else
			{
				z();
			}
			break;

		case 'f':
			if ('\'' == *(cmd + 1))
			{
				fr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				f2();
				cmd++;
			}
			else
			{
				f();
			}
			break;

		case 'b':
			if ('\'' == *(cmd + 1))
			{
				br();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				b2();
				cmd++;
			}
			else
			{
				b();
			}
			break;

		case 'l':
			if ('\'' == *(cmd + 1))
			{
				lr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				l2();
				cmd++;
			}
			else
			{
				l();
			}
			break;

		case 'r':
			if ('\'' == *(cmd + 1))
			{
				rr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				r2();
				cmd++;
			}
			else
			{
				r();
			}
			break;

		case 'u':
			if ('\'' == *(cmd + 1))
			{
				ur();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				u2();
				cmd++;
			}
			else
			{
				u();
			}
			break;

		case 'd':
			if ('\'' == *(cmd + 1))
			{
				dr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				d2();
				cmd++;
			}
			else
			{
				d();
			}
			break;

		case 'E':
			if ('\'' == *(cmd + 1))
			{
				Er();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				E2();
				cmd++;
			}
			else
			{
				E();
			}
			break;

		case 'M':
			if ('\'' == *(cmd + 1))
			{
				Mr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				M2();
				cmd++;
			}
			else
			{
				M();
			}
			break;

		case 'S':
			if ('\'' == *(cmd + 1))
			{
				Sr();
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				S2();
				cmd++;
			}
			else
			{
				S();
			}
			break;

		default:
			break;
		}
		cmd++;
	}
}

bool IsTopSurfaceComplete()
{
	for(int i = 0; i < 9; i++)
	{
		if(aspectColors[UP][i] != aspectColors[UP][4])
		{
			return false;
		}
	}
	return true;
}

CString RestoreTopSurface()
{
	ASSERT(IsTopCrossed());
	
	memset(cmdString, 0, COMMANDLEN);
	cmdStrPtr = 0;

	int testCount = 0;

	while(!IsTopSurfaceComplete())
	{
		testCount++;
		if(testCount > 8)
		{
			MessageBox(NULL, _T("bug emerged in function: RestoreTopSurface()"), _T("error"), MB_OK);

			char * asp[6] = {"FRONT��:", "BACK��:", "LEFT��:", "RIGHT��:", "UP��:", "DOWN��:"};
			char * col[6] = {"BLUE", "GREEN", "RED", "ORANGE", "WHITE", "YELLOW"};

			FILE * fp = fopen("bug/RestoreTopSurface.txt", "a+");
			if(fp != NULL)
			{
				for(int i = 0; i < 6; i++)
				{
					fprintf(fp, "%s\n", asp[i]);
					for(int j = 0; j < 9; j++)
					{
						fprintf(fp, "%11s", col[aspectColors[i][j]]);
						if(8 == j)
						{
							fprintf(fp, "\n");
						}
					}
				}
				fprintf(fp, "\n\n");
				fclose(fp);
			}
			exit(-1);
		}

		int num = 0;//��¼���������Ĳ�ͬɫ�ĸ���
		for(int i = 0; i < 9; i += 2)
		{
			if(aspectColors[UP][4] != aspectColors[UP][i])
			{
				num++;
			}
		}

		//λ��һ��Ҫ�Ŷ�
		if(3 == num)
		{
			while(aspectColors[UP][4] != aspectColors[UP][0])
			{
				_y();
			}

			if(aspectColors[FRONT][6] == aspectColors[UP][4])
			{
				_Rr(); _Ur(); _R(); _Ur(); _Rr(); _U2(); _R();
			}
			else if(aspectColors[FRONT][8] == aspectColors[UP][4])
			{
				_F(); _U(); _Fr(); _U(); _F(); _U2(); _Fr();
			}
		}
		else if(2 == num)
		{
			while(!(aspectColors[UP][4] != aspectColors[UP][0] && aspectColors[BACK][6] == aspectColors[UP][4]))
			{
				_y();
			}

			_Rr(); _Ur(); _R(); _Ur(); _Rr(); _U2(); _R();
		}
		else if(4 == num)
		{
			while(aspectColors[LEFT][2] != aspectColors[UP][4])
			{
				_y();
			}

			_Rr(); _Ur(); _R(); _Ur(); _Rr(); _U2(); _R();
		}
	}	
	return CString(cmdString);
}

bool IsTopCornerBack()
{
	int cornerHashDst[4];//hash value of this corner should be
	int cornerHashSrc[4];//hash value for current corners
	//start from leftFront corner, counterclockwise
	cornerHashDst[0] = HashCorner(aspectColors[UP][4], aspectColors[LEFT][4], aspectColors[FRONT][4]);
	cornerHashDst[1] = HashCorner(aspectColors[UP][4], aspectColors[FRONT][4], aspectColors[RIGHT][4]);
	cornerHashDst[2] = HashCorner(aspectColors[UP][4], aspectColors[RIGHT][4], aspectColors[BACK][4]);
	cornerHashDst[3] = HashCorner(aspectColors[UP][4], aspectColors[BACK][4], aspectColors[LEFT][4]);

	cornerHashSrc[0] = HashCorner(aspectColors[UP][6], aspectColors[LEFT][8], aspectColors[FRONT][6]);
	cornerHashSrc[1] = HashCorner(aspectColors[UP][8], aspectColors[FRONT][8], aspectColors[RIGHT][8]);
	cornerHashSrc[2] = HashCorner(aspectColors[UP][2], aspectColors[RIGHT][2], aspectColors[BACK][8]);
	cornerHashSrc[3] = HashCorner(aspectColors[UP][0], aspectColors[BACK][6], aspectColors[LEFT][2]);

	for (int i = 0; i < 4; i++)
	{
		if (cornerHashSrc[i] != cornerHashDst[i])
		{
			return false;
		}
	}
	return true;
}

CString PutBackTopCorner()
{
	memset(cmdString, 0, COMMANDLEN);
	cmdStrPtr = 0;

	int testCount = 0;		
	while(!IsTopCornerBack())
	{
		testCount++;
		if (testCount > 3)
		{
			MessageBox(NULL, _T("bug emerged in function: PutBackTopCorner()"), _T("error"), MB_OK);

			char * asp[6] = {"FRONT��:", "BACK��:", "LEFT��:", "RIGHT��:", "UP��:", "DOWN��:"};
			char * col[6] = {"BLUE", "GREEN", "RED", "ORANGE", "WHITE", "YELLOW"};

			FILE * fp = fopen("bug/PutBackTopCorner.txt", "a+");
			if(fp != NULL)
			{
				for(int i = 0; i < 6; i++)
				{
					fprintf(fp, "%s\n", asp[i]);
					for(int j = 0; j < 9; j++)
					{
						fprintf(fp, "%11s", col[aspectColors[i][j]]);
						if(8 == j)
						{
							fprintf(fp, "\n");
						}
					}
				}
				fprintf(fp, "\n\n");
				fclose(fp);
			}
			exit(-1);
		}
		
		int num = 0;//����ͬɫ�ߵĸ���
		if(aspectColors[FRONT][6] == aspectColors[FRONT][8])
		{
			num++;
			_yr();
		}
		if(aspectColors[RIGHT][2] == aspectColors[RIGHT][8])
		{
			num++;
		}
		if(aspectColors[BACK][6] == aspectColors[BACK][8])
		{
			num++;
			_y();
		}
		if(aspectColors[LEFT][2] == aspectColors[LEFT][8])
		{
			num++;
			_y2();
		}

		if(1 == num || 0 == num)
		{
			_R2(); _F2(); _Rr(); _Br(); _R(); _F2(); _Rr(); _B(); _Rr();
		}
		else if(4 == num)
		{
			if(aspectColors[FRONT][6] == aspectColors[RIGHT][4])
			{
				_Ur();
			}
			else if(aspectColors[FRONT][6] == aspectColors[BACK][4])
			{
				_U2();
			}
			else if(aspectColors[FRONT][6] == aspectColors[LEFT][4])
			{
				_U();
			}
		}
	}
	return CString(cmdString);
}

bool IsTopEdgeComplete()
{
	if(aspectColors[FRONT][7] == aspectColors[FRONT][4] && aspectColors[BACK][7] == aspectColors[BACK][4] &&
		aspectColors[LEFT][5] == aspectColors[LEFT][4] && aspectColors[RIGHT][5] == aspectColors[RIGHT][4] &&
		aspectColors[UP][1] == aspectColors[UP][4] && aspectColors[UP][3] == aspectColors[UP][4] &&
		aspectColors[UP][5] == aspectColors[UP][4] && aspectColors[UP][7] == aspectColors[UP][4])
	{
		return true;
	}
	return false;
}

CString RestoreTopEdge()
{
	memset(cmdString, 0, COMMANDLEN);
	cmdStrPtr = 0;

	ASSERT(IsTopCornerBack());

	int testCount = 0;
	while(!IsTopEdgeComplete())
	{
		testCount++;
		if(testCount > 4)
		{
			MessageBox(NULL, _T("bug emerged in function: RestoreTopEdge()"), _T("error"), MB_OK);

			char * asp[6] = {"FRONT��:", "BACK��:", "LEFT��:", "RIGHT��:", "UP��:", "DOWN��:"};
			char * col[6] = {"BLUE", "GREEN", "RED", "ORANGE", "WHITE", "YELLOW"};

			FILE * fp = fopen("bug/RestoreTopEdge.txt", "a+");
			if(fp != NULL)
			{
				for(int i = 0; i < 6; i++)
				{
					fprintf(fp, "%s\n", asp[i]);
					for(int j = 0; j < 9; j++)
					{
						fprintf(fp, "%11s", col[aspectColors[i][j]]);
						if(8 == j)
						{
							fprintf(fp, "\n");
						}
					}
				}
				fprintf(fp, "\n\n");
				fclose(fp);
			}
			exit(-1);
		}
		
		bool flg = false;
		if(aspectColors[FRONT][7] == aspectColors[FRONT][6] && aspectColors[FRONT][7] == aspectColors[FRONT][8])
		{
			flg = true;
			_y2();
		}
		else if(aspectColors[BACK][7] == aspectColors[BACK][6] && aspectColors[BACK][7] == aspectColors[BACK][8])
		{
			flg = true;
		}
		else if(aspectColors[LEFT][5] == aspectColors[LEFT][2] && aspectColors[LEFT][5] == aspectColors[LEFT][8])
		{
			flg = true;
			_y();
		}
		else if(aspectColors[RIGHT][5] == aspectColors[RIGHT][2] && aspectColors[RIGHT][5] == aspectColors[RIGHT][8])
		{
			flg = true;
			_yr();
		}

		if(flg)
		{
			if(aspectColors[FRONT][4] == aspectColors[LEFT][5] && aspectColors[LEFT][4] == aspectColors[RIGHT][5]
			&& aspectColors[RIGHT][4] == aspectColors[FRONT][7])//������������Ϳ�����
			{
				_R(); _Ur(); _R(); _U(); _R(); _U(); _R(); _Ur(); _Rr(); _Ur(); _R2();
			}
			else
			{
				_R2(); _U(); _R(); _U(); _Rr(); _Ur(); _Rr(); _Ur(); _Rr(); _U(); _Rr();
			}
		}
		else
		{
			if(aspectColors[FRONT][4] == aspectColors[BACK][7] && aspectColors[FRONT][7] == aspectColors[BACK][4]
			&& aspectColors[LEFT][4] == aspectColors[RIGHT][5] && aspectColors[LEFT][5] == aspectColors[RIGHT][4])
			{
				_M2(); _U(); _M2(); _U2(); _M2(); _U(); _M2();
			}
			else if(aspectColors[FRONT][7] == aspectColors[LEFT][4] && aspectColors[FRONT][4] == aspectColors[LEFT][5])
			{
				_y(); _M2(); _U(); _M2(); _U(); _M(); _U2(); _M2(); _U2(); _M(); _U2();
			}
			else
			{
				_M2(); _U(); _M2(); _U(); _M(); _U2(); _M2(); _U2(); _M(); _U2();
			}
		}
	}
	return CString(cmdString);
}

//get command string which can restore the state before 
//execution of the parameter string
CString UndoCmdString(const char* cmd)
{
	CString undoCmd;
	while(*cmd)
	{
		switch(*cmd)
		{
		case ' ':
			break;

		case '#':				
			do
			{
				cmd++;
			}
			while(*cmd!='#'&&*cmd);
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
			if ('\'' == *(cmd + 1))
			{
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
				undoCmd.Insert(1,'2');
				cmd++;
			}
			else
			{
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

//���㲽������XYZ��������ת����
int CountSteps(const char* cmd)
{
	int steps=0;

	while(*cmd)
	{
		switch(*cmd)
		{
		case ' ':
			break;

		case '#':				
			do
			{
				cmd++;
			}
			while(*cmd != '#' && *cmd);
			break;

		case 'F':
		case 'B':
		case 'L':
		case 'R':
		case 'U':
		case 'D':
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
			if ('\'' == *(cmd + 1))
			{
				cmd++;
			}
			else if ('2' == *(cmd + 1))
			{
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

CString MethodFewerFormula()
{
	CString cmd;
	cmd = F2L();
	cmd += "#\r\nBuil Cross On Top Aspect:\r\n\t#";
	cmd += BuildTopCross();
	cmd += "#\r\nRestore Top Surface:\r\n\t#";
	cmd += RestoreTopSurface();
	cmd += "#\r\nPut Back Top Corners:\r\n\t#";
	cmd += PutBackTopCorner();
	cmd += "#\r\nRestore Top Edges:\r\n\t#";
	cmd += RestoreTopEdge();

	int len = CountSteps(cmd.GetBuffer());
	CString str;
	str.Format("#\r\n\r\nStep Number:\r\n\t%d#", len);
	cmd += str;
	//the view is not updated,and we should undo all the operations 
	//during the steps above
	CString undo = UndoCmdString(cmd.GetBuffer(cmd.GetLength()));
	Executor(undo.GetBuffer(undo.GetLength()));

	return cmd;
}

CMagicCube::CMagicCube(void)
{	
}

CMagicCube::~CMagicCube(void)
{
}

void CMagicCube::initialize()
{
	memcpy(aspectColors, aspectColor_org, sizeof(aspectColor_org));
	rotateInfo = RotateInfo();
}

//rotate color table according to current rotateInfo
void CMagicCube::rotateColor()
{
	switch (rotateInfo.type)
	{
	case 'F':
		if (rotateInfo.reversed)
		{
			Fr();
		}
		else 
		{
			F();
		}
		break;

	case 'B':
		if (rotateInfo.reversed)
		{
			Br();
		}
		else 
		{
			B();
		}
		break;

	case 'L':
		if (rotateInfo.reversed)
		{
			Lr();
		}
		else 
		{
			L();
		}
		break;

	case 'R':
		if (rotateInfo.reversed)
		{
			Rr();
		}
		else 
		{
			R();
		}
		break;

	case 'U':
		if (rotateInfo.reversed)
		{
			Ur();
		}
		else 
		{
			U();
		}
		break;

	case 'D':
		if (rotateInfo.reversed)
		{
			Dr();
		}
		else 
		{
			D();
		}
		break;

	case 'x':
		if (rotateInfo.reversed)
		{
			xr();
		}
		else 
		{
			x();
		}
		break;

	case 'y':
		if (rotateInfo.reversed)
		{
			yr();
		}
		else 
		{
			y();
		}
		break;

	case 'z':
		if (rotateInfo.reversed)
		{
			zr();
		}
		else 
		{
			z();
		}
		break;

	case 'u':
		if (rotateInfo.reversed)
		{
			ur();
		}
		else 
		{
			u();
		}
		break;

	case 'r':
		if (rotateInfo.reversed)
		{
			rr();
		}
		else 
		{
			r();
		}
		break;

	case 'f':
		if (rotateInfo.reversed)
		{
			fr();
		}
		else 
		{
			f();
		}
		break;

	case 'd':
		if (rotateInfo.reversed)
		{
			dr();
		}
		else 
		{
			d();
		}
		break;

	case 'l':
		if (rotateInfo.reversed)
		{
			lr();
		}
		else 
		{
			l();
		}
		break;

	case 'b':
		if (rotateInfo.reversed)
		{
			br();
		}
		else 
		{
			b();
		}
		break;

	case 'E':
		if (rotateInfo.reversed)
		{
			Er();
		}
		else 
		{
			E();
		}
		break;

	case 'M':
		if (rotateInfo.reversed)
		{
			Mr();
		}
		else 
		{
			M();
		}
		break;

	case 'S':
		if (rotateInfo.reversed)
		{
			Sr();
		}
		else 
		{
			S();
		}
		break;

	default:
		break;
	}
}

void CMagicCube::display()
{	
	static float rotateVect[3][2][3] = {
		{{1, 0, 0}, {-1, 0, 0}},//��X����ת������ʾ��ʱ��
		{{0, 1, 0}, {0, -1, 0}},//��Y����ת
		{{0, 0, 1}, {0, 0, -1}}//��Z����ת
	};
	Axis axis;
	int direct;
	switch (rotateInfo.type)
	{
	case 'F':
		axis = Z;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		drawZBack();
		drawZMiddle();
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);
		drawZFront();
		glPopMatrix();
		break;

	case 'B':
		axis = Z;
		if (rotateInfo.reversed)
		{
			direct = 1;
		}
		else 
		{
			direct = 0;
		}					
		drawZMiddle();
		drawZFront();
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);
		drawZBack();
		glPopMatrix();
		break;

	case 'L':
		axis = X;
		if (rotateInfo.reversed)
		{
			direct = 1;
		}
		else 
		{
			direct = 0;
		}
		drawXMiddle();
		drawXRight();
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);
		drawXLeft();
		glPopMatrix();
		break;

	case 'R':
		axis = X;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		drawXMiddle();
		drawXLeft();
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);
		drawXRight();
		glPopMatrix();
		break;

	case 'U':
		axis = Y;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		drawYMiddle();
		drawYDown();
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);
		drawYUp();
		glPopMatrix();
		break;

	case 'D':
		axis = Y;
		if (rotateInfo.reversed)
		{
			direct = 1;
		}
		else 
		{
			direct = 0;
		}
		drawYMiddle();
		drawYUp();
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);
		drawYDown();
		glPopMatrix();
		break;

	case 'x':
		axis = X;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);
		drawXLeft();
		drawXMiddle();
		drawXRight();
		glPopMatrix();
		break;

	case 'y':
		axis = Y;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);
		drawYDown();
		drawYMiddle();
		drawYUp();
		glPopMatrix();
		break;

	case 'z':
		axis = Z;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);
		drawZBack();
		drawZMiddle();
		drawZFront();
		glPopMatrix();
		break;

	case 'u':
		axis = Y;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);			
		drawYMiddle();
		drawYUp();
		glPopMatrix();
		drawYDown();
		break;

	case 'r':
		axis=X;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);			
		drawXMiddle();
		drawXRight();
		glPopMatrix();
		drawXLeft();
		break;

	case 'f':
		axis = Z;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);			
		drawZMiddle();
		drawZFront();
		glPopMatrix();
		drawZBack();
		break;

	case 'd':
		axis = Y;
		if (rotateInfo.reversed)
		{
			direct = 1;
		}
		else 
		{
			direct = 0;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);			
		drawYMiddle();
		drawYDown();
		glPopMatrix();
		drawYUp();
		break;

	case 'l':
		axis = X;
		if (rotateInfo.reversed)
		{
			direct = 1;
		}
		else 
		{
			direct = 0;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);			
		drawXMiddle();
		drawXLeft();
		glPopMatrix();
		drawXRight();
		break;

	case 'b':
		axis = Z;
		if (rotateInfo.reversed)
		{
			direct = 1;
		}
		else 
		{
			direct = 0;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);			
		drawZMiddle();
		drawZBack();
		glPopMatrix();
		drawZFront();
		break;

	case 'E':
		axis = Y;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);			
		drawYMiddle();			
		glPopMatrix();
		drawYDown();
		drawYUp();
		break;

	case 'M':
		axis = X;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);			
		drawXMiddle();
		glPopMatrix();
		drawXRight();
		drawXLeft();
		break;

	case 'S':
		axis = Z;
		if (rotateInfo.reversed)
		{
			direct = 0;
		}
		else 
		{
			direct = 1;
		}
		glPushMatrix();
		glRotatef(rotateInfo.angle, rotateVect[axis][direct][0],
			rotateVect[axis][direct][1], rotateVect[axis][direct][2]);			
		drawZMiddle();
		glPopMatrix();
		drawZFront();
		drawZBack();
		break;

	default:
		drawZMiddle();
		drawZFront();
		drawZBack();
		break;
	}
}

CString CMagicCube::restore()
{
	return MethodFewerFormula();
}