#pragma once
#include"global.h"
#include"CGameLogic.h"
#include"CGraph.h"
class CGameControl
{
private:
	CGraph m_anMap;
	CGameLogic m_GameLogic;
	Vertex m_ptSelFirst;	//第一次选中的点
	Vertex m_ptSelSec;		//第二次选中的点

public:
	const static int s_nRows=NROW;
	const static int s_nCols=NCOL;
	const static int s_nPicNum=8;
	void StartGame(void);								//开始游戏函数
	int GetElement(int nRow, int nCol);					//获得某行某列的图片编号函数
	bool Link(Vertex avPath[MAX_VERTEX_NUM], int& nVexnum); //先判断所选择的图片是否是同一张图片，然后判断是否是同一种图片
	void SetFirstPoint(int nRow, int nCol);
	void SetSecPoint(int nRow, int nCol);
	bool Help(Vertex avPath[], int& nVexnum);
	void Reset();
	bool IsWin();
};
