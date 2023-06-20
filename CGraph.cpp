#include "pch.h"
#include"CGraph.h"
void CGraph::AddVertex(int info)
{
	if (m_nVexnum >= MAX_VERTEX_NUM)
		return;
	m_Vertices[m_nVexnum] = info;
	m_nVexnum++;
	return;
}
int CGraph::GetVertex(int nIndex)
{
	return m_Vertices[nIndex];
}
int CGraph::GetVexnum()
{
	return MAX_VERTEX_NUM;
}
void CGraph::AddArc(int nV1Index, int nV2Index)
{
	m_AdjMatrix[nV1Index][nV2Index]=true;
	m_AdjMatrix[nV2Index][nV1Index] = true;
}
bool CGraph::GetArc(int nV1Index, int nV2Index)
{
	return m_AdjMatrix[nV1Index][nV2Index];
}
void CGraph::UpdateVertex(int nIndex, int info)
{
	m_Vertices[nIndex] = info;
}
void CGraph::ClearGraph()
{

}
void CGraph::Change(int n1, int n2)
{

}
void CGraph::InitGraph()
{
	//初始化顶点
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		m_Vertices[i] = -2;
	}
	//初始化边
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		for (int j = 0; j < MAX_VERTEX_NUM; j++)
		{
			m_AdjMatrix[i][j] = false;
		}
	}
	m_nVexnum = 0;
}