#pragma once
#include"global.h"
#include"CGameLogic.h"
#include"CGraph.h"
class CGameControl
{
private:
	CGraph m_anMap;
	CGameLogic m_GameLogic;
	Vertex m_ptSelFirst;	//��һ��ѡ�еĵ�
	Vertex m_ptSelSec;		//�ڶ���ѡ�еĵ�

public:
	const static int s_nRows=NROW;
	const static int s_nCols=NCOL;
	const static int s_nPicNum=8;
	void StartGame(void);								//��ʼ��Ϸ����
	int GetElement(int nRow, int nCol);					//���ĳ��ĳ�е�ͼƬ��ź���
	bool Link(Vertex avPath[MAX_VERTEX_NUM], int& nVexnum); //���ж���ѡ���ͼƬ�Ƿ���ͬһ��ͼƬ��Ȼ���ж��Ƿ���ͬһ��ͼƬ
	void SetFirstPoint(int nRow, int nCol);
	void SetSecPoint(int nRow, int nCol);
	bool Help(Vertex avPath[], int& nVexnum);
	void Reset();
	bool IsWin();
};
