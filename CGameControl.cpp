#include "pch.h"
#include "CGameControl.h"
#include"CGameLogic.h"

bool CGameControl::Link(Vertex avPath[MAX_VERTEX_NUM], int& nVexnum)
{

	//�ж��Ƿ�Ϊͬһ��ͼƬ
	if (m_ptSelFirst.row == m_ptSelSec.row && m_ptSelFirst.col == m_ptSelSec.col)
	{
		return false;
	}

	//�ж�ͼƬ�Ƿ���ͬ
	if (GetElement(m_ptSelSec.row,m_ptSelSec.col) !=GetElement(m_ptSelFirst.row,m_ptSelFirst.col) )
	{
		return false;
	}

	//��ͨ�ж�
	if (m_GameLogic.IsLink(m_anMap, m_ptSelFirst, m_ptSelSec) == true)
	{
		//����
		m_GameLogic.Clear(m_anMap, m_ptSelFirst, m_ptSelSec);

		//����·������
//		avPath[0] = m_ptSelFirst;
//		avPath[1] = m_ptSelSec;
		nVexnum = m_GameLogic.GetVexPath(avPath);

		return true;
	}

	return false;
}

//���õ�һ���㺯��
void CGameControl::SetFirstPoint(int nRow, int nCol)
{
	m_ptSelFirst.row = nRow;
	m_ptSelFirst.col = nCol;

}

//���õڶ����㺯��
void CGameControl::SetSecPoint(int nRow, int nCol)
{
	m_ptSelSec.row = nRow;
	m_ptSelSec.col = nCol;

}

//��ʼ��Ϸ����
void CGameControl::StartGame(void)
{
	m_anMap.InitGraph();
	m_GameLogic.InitMap(m_anMap);

}

//���ĳ��ĳ�е�ͼƬ��ź���
int CGameControl::GetElement(int nRow, int nCol)
{
	return m_anMap.GetVertex(nRow*NCOL+ nCol);
}

bool CGameControl::IsWin() {
	CGameLogic logic;
	if (logic.IsBlank(m_anMap) == true) {
		logic.InitMap(m_anMap);
		return true;
	}
	return false;
}
bool CGameControl::Help(Vertex avPath[MAX_VERTEX_NUM],int &nVexnum)
{
	if (m_GameLogic.IsBlank(m_anMap))
	{
		return false;
	}
	else if(m_GameLogic.SearchValidPath(m_anMap))
	{
		nVexnum=m_GameLogic.GetVexPath(avPath);
		return true;
	}
	return false;
}
void CGameControl::Reset()
{
	if (m_GameLogic.IsBlank(m_anMap))
	{
		return ;
	}
	else
	{
		m_GameLogic.Reset(m_anMap);
	}
}

