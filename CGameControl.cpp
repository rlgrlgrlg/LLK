#include "pch.h"
#include "CGameControl.h"
#include"CGameLogic.h"

bool CGameControl::Link(Vertex avPath[MAX_VERTEX_NUM], int& nVexnum)
{

	//判断是否为同一个图片
	if (m_ptSelFirst.row == m_ptSelSec.row && m_ptSelFirst.col == m_ptSelSec.col)
	{
		return false;
	}

	//判断图片是否相同
	if (GetElement(m_ptSelSec.row,m_ptSelSec.col) !=GetElement(m_ptSelFirst.row,m_ptSelFirst.col) )
	{
		return false;
	}

	//连通判断
	if (m_GameLogic.IsLink(m_anMap, m_ptSelFirst, m_ptSelSec) == true)
	{
		//消子
		m_GameLogic.Clear(m_anMap, m_ptSelFirst, m_ptSelSec);

		//返回路径顶点
//		avPath[0] = m_ptSelFirst;
//		avPath[1] = m_ptSelSec;
		nVexnum = m_GameLogic.GetVexPath(avPath);

		return true;
	}

	return false;
}

//设置第一个点函数
void CGameControl::SetFirstPoint(int nRow, int nCol)
{
	m_ptSelFirst.row = nRow;
	m_ptSelFirst.col = nCol;

}

//设置第二个点函数
void CGameControl::SetSecPoint(int nRow, int nCol)
{
	m_ptSelSec.row = nRow;
	m_ptSelSec.col = nCol;

}

//开始游戏函数
void CGameControl::StartGame(void)
{
	m_anMap.InitGraph();
	m_GameLogic.InitMap(m_anMap);

}

//获得某行某列的图片编号函数
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

