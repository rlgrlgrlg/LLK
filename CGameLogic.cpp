#include "pch.h"
#include "CGameLogic.h"
#include<iostream>
#include<fstream>
#include "CGameDlg.h"
#include<math.h>
CGameLogic::CGameLogic()
{
	//for (int i = 0; i < 4; i++)
	//{
	//	m_avPath[i].col = 0;
	//	m_avPath[i].row = 0;
	//	m_avPath[i].info = 0;
	//}

	m_nVexNum = 0;     //数组中实际元素的个数
	m_nCorner = 0;
}


CGameLogic::~CGameLogic()
{
}


// 初始化游戏地图
void CGameLogic::ReleaseMap(int** pGameMap)
{
	for (int i = 0; i < CGameControl::s_nRows; i++)
	{
		delete[]pGameMap[i];
	}
	delete[]pGameMap;
}
void CGameLogic::InitMap(CGraph &g)
{
	//游戏地图初始化

	int nRows = CGameControl::s_nRows;
	int nCols = CGameControl::s_nCols;
	int nPicNum = CGameControl::s_nPicNum;

	int** pGameMap = new int* [nRows];
	if (pGameMap == NULL)
	{

	}

	else
	{
		for (int i = 0; i < nRows; i++)
		{
			pGameMap[i] = new int[nCols];
			if (pGameMap == NULL)
			{

			}
			memset(pGameMap[i], NULL, sizeof(int) * nCols);
		}
	}
	if ((nRows * nCols) % (nPicNum * 2) != 0)
	{
		ReleaseMap(pGameMap);
	}
	int nRepeatNum = nRows * nCols / nPicNum;
	int nCount = 0;
	for (int i = 0; i < nPicNum; i++)
	{
		for (int j = 0; j < nRepeatNum; j++)
		{
			pGameMap[nCount / nCols][nCount % nCols] = i;
			nCount++;
		}
	}
	srand((int)time(NULL));
	int nVertexNum = nRows * nCols;
	for (int i = 0; i < nVertexNum; i++)
	{
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;
		int nTmp = pGameMap[nIndex1 / nCols][nIndex1 % nCols];
		pGameMap[nIndex1 / nCols][nIndex1 % nCols] = pGameMap[nIndex2 / nCols][nIndex2 % nCols];
		pGameMap[nIndex2 / nCols][nIndex2 % nCols] = nTmp;
	}
	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			g.AddVertex(pGameMap[i][j]);
			UpdateArc(g, i, j);
		}
	}
}

//连通判断
bool CGameLogic::IsLink(CGraph& g, Vertex v1, Vertex v2) {

	//获取索引
	int nIndex1 = v1.row * NCOL + v1.col;
	int nIndex2 = v2.row * NCOL + v2.col;
	
	//判断两点间是否有边
//	if (g.GetArc(nIndex1, nIndex2)==true)
	if (g.GetVertex(nIndex1) == BLANK || g.GetVertex(nIndex2) == BLANK)
	{
		return false;
	}
	PushVertex(nIndex1);
	if(SearchPath1(g,nIndex1,nIndex2)==true)
	{
//		PushVertex(nIndex2);
		return true;
	}
	PopVertex();
	return false;
}

//行号相同时，判断横向是否连通
/*bool CGameLogic::LinkInRow(CGraph& g, Vertex v1, Vertex v2) {
	int nCol1 = v1.col;
	int nCol2 = v2.col;
	int nRow = v1.row;
	if (nCol1 > nCol2) {
		int nTemp = nCol1;
		nCol1 = nCol2;
		nCol2 = nTemp;
	}

	//直通
	for (int i = nCol1 + 1; i <= nCol2; i++)
	{
		if (i == nCol2)return true;
		if (g[nRow][i] != BLANK)break;
	}//判断这条直线上是否都为空白区域，如果全为空白区域,则横向连通
	return false;
}

//列号相同时，判断纵向是否连通
bool CGameLogic::LinkInCol(CGraph& g, Vertex v1, Vertex v2) {
	int nRow1 = v1.row;
	int nRow2 = v2.row;
	int nCol = v1.col;
	//保证nRow1的值大于nRow2
	if (nRow1 > nRow2)
	{
		int nTemp = nRow1;
		nRow1 = nRow2;
		nRow2 = nTemp;
	}
	for (int i = nRow1 + 1; i <= nRow2; i++)
	{
		if (i == nRow2)return true;
		if (g[i][nCol] != BLANK)break;
	}//判断这条直线上是否都为空白区域，如果全为空白区域,则纵向连通
	return false;
}

//判断（nRow1，nCol1）到（nRow1，nCol2）能否连通
bool CGameLogic::LineX(CGraph& g, int nRow, int nCol1, int nCol2)
{
	if (nCol1 > nCol2)
	{
		//交换数据
		int nTemp = nCol1;
		nCol1 = nCol2;
		nCol2 = nTemp;
	}

	for (int i = nCol1; i <= nCol2; i++)
	{
		if (i == nCol2 && g[nRow][i] == BLANK)return true;
		if (g[nRow][i] != BLANK)break;
	}
	return false;
}

//判断（nRow1，nCol2）到（nRow2，nCol2）能否连通
bool CGameLogic::LineY(CGraph &g, int nRow1, int nRow2, int nCol)
{
	if (nRow1 > nRow2)
	{
		//交换数据
		int nTemp = nRow1;
		nRow1 = nRow2;
		nRow2 = nTemp;
	}

	for (int i = nRow1; i <= nRow2; i++)
	{
		if (i == nRow2 && g[i][nCol] == BLANK)return true;
		if (g[i][nCol] != BLANK)break;
	}
	return false;
}

// 判断（nRow1，nCol1）到（nRow2，nCol2）能否连通(两条直线消子)
bool CGameLogic::OneCornerLink(CGraph& g, Vertex v1, Vertex v2)
{
	//直角能够消子，那么顶点一定在与两个点的行和列相交的点，只有这两个点为空，才有可能实现两条直线消子
	int nRow1 = v1.row;
	int nCol1 = v1.col;
	int nRow2 = v2.row;
	int nCol2 = v2.col;
	if (nRow1 > nRow2)
	{
		int nTemp = nRow1;
		nRow1 = nRow2;
		nRow2 = nTemp;

		nTemp = nCol1;
		nCol1 = nCol2;
		nCol2 = nTemp;
	}

	if (nCol1 > nCol2)
	{
		//判断（nRow1 + 1, nCol1）到（nRow2，nCol2 + 1）能否连通
		if (LineY(g, nRow1 + 1, nRow2, nCol1) && LineX(g, nRow2, nCol1, nCol2 + 1))
		{
			Vertex v = { nRow2,nCol1,BLANK };
			AddVertex(v);
			return true;
		}
		//判断（nRow2 - 1, nCol2）到（nRow1，nCol1 - 1）能否连通
		if (LineY(g, nRow2 - 1, nRow1, nCol2) && LineX(g, nRow1, nCol2, nCol1 - 1))
		{
			Vertex v = { nRow1,nCol2,BLANK };
			AddVertex(v);
			return true;
		}
	}
	else
	{
		//判断（nRow1 + 1, nCol1）到（nRow2，nCol2 - 1）能否连通
		if (LineY(g, nRow1 + 1, nRow2, nCol1) && LineX(g, nRow2, nCol1, nCol2 - 1))
		{
			Vertex v = { nRow2,nCol1,BLANK };
			AddVertex(v);
			return true;
		}
		//判断（nRow2 - 1, nCol2）到（nRow1，nCol1 + 1）能否连通
		if (LineY(g, nRow2 - 1, nRow1, nCol2) && LineX(g, nRow1, nCol2, nCol1 + 1))
		{
			Vertex v = { nRow1,nCol2,BLANK };
			AddVertex(v);
			return true;
		}
	}

	return false;
}

//向数组中添加一个元素
void CGameLogic::AddVertex(Vertex v)
{
	m_avPath[m_nVexNum] = v;
	m_nVexNum++;
}

//判断（nRow1，nCol1）到（nRow2，nCol2）能否连通(三条直线消子)
bool CGameLogic::TwoCornerLink(CGraph &g, Vertex v1, Vertex v2)
{
	int nRow1 = v1.row;
	int nCol1 = v1.col;
	int nRow2 = v2.row;
	int nCol2 = v2.col;
	bool IsTemp = false;

	//确保nRow1<nRow2
	if (nRow1 > nRow2)
	{
		//数据交换
		int nTemp = nRow1;
		nRow1 = nRow2;
		nRow2 = nTemp;

		nTemp = nCol1;
		nCol1 = nCol2;
		nCol2 = nTemp;

		IsTemp = true;        //表明两个点交换
	}
	if (nCol1 > nCol2)
	{
		//找到一条与Y轴平行的连通直线段
		for (int nCol = 0; nCol < 4; nCol++)
		{
			if (g[nRow1][nCol] == BLANK && g[nRow2][nCol] == BLANK)
			{
				if (LineY(g, nRow1, nRow2, nCol)) //判断该条Y轴直线是否连通
				{
					//连通的直线在选中的两个点的左边
					if (nCol2 > nCol && LineX(g, nRow1, nCol - 1, nCol) && LineX(g, nRow2, nCol, nCol2 - 1))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow1;
						Vx1.col = nCol;
						Vx2.row = nRow2;
						Vx2.col = nCol;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
					//连通的直线在选中的两个点的中间
					if (nCol1 > nCol && nCol > nCol2 && LineX(g, nRow1, nCol, nCol1 - 1) && LineX(g, nRow2, nCol2 + 1, nCol))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow1;
						Vx1.col = nCol;
						Vx2.row = nRow2;
						Vx2.col = nCol;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
					//连通的直线在选中的两个点的右边
					if (nCol > nCol1 && LineX(g, nRow1, nCol1 + 1, nCol) && LineX(g, nRow2, nCol, nCol2 + 1))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow1;
						Vx1.col = nCol;
						Vx2.row = nRow2;
						Vx2.col = nCol;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
				}
			}
		}

		//找到一条与X轴平行的连通直线段
		for (int nRow = 0; nRow < 4; nRow++)
		{
			if (g[nRow][nCol1] == BLANK && g[nRow][nCol2] == BLANK)
			{
				if (LineX(g, nRow, nCol1, nCol2)) //判断该条Y轴直线是否连通
				{
					//连通的直线在选中的两个点的上面
					if (nRow1 > nRow && LineY(g, nRow1 - 1, nRow, nCol1) && LineY(g, nRow, nRow2 - 1, nCol2))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow;
						Vx1.col = nCol1;
						Vx2.row = nRow;
						Vx2.col = nCol2;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
					//连通的直线在选中的两个点的中间
					if (nRow2 > nRow && nRow > nRow1 && LineY(g, nRow1 + 1, nRow, nCol1) && LineY(g, nRow, nRow2 - 1, nCol2))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow;
						Vx1.col = nCol1;
						Vx2.row = nRow;
						Vx2.col = nCol2;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
					//连通的直线在选中的两个点的下面
					if (nRow > nRow2 && LineY(g, nRow1 + 1, nRow, nCol1) && LineY(g, nRow, nRow2 + 1, nCol2))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow;
						Vx1.col = nCol1;
						Vx2.row = nRow;
						Vx2.col = nCol2;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
				}
			}
		}
	}
	else
	{
		//找到一条与Y轴平行的连通直线段
		for (int nCol = 0; nCol < 4; nCol++)
		{
			if (g[nRow1][nCol] == BLANK && g[nRow2][nCol] == BLANK)
			{
				if (LineY(g, nRow1, nRow2, nCol)) //判断该条Y轴直线是否连通
				{
					//连通的直线在选中的两个左边
					if (nCol1 > nCol && LineX(g, nRow1, nCol1 - 1, nCol) && LineX(g, nRow2, nCol, nCol2 - 1))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow1;
						Vx1.col = nCol;
						Vx2.row = nRow2;
						Vx2.col = nCol;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
					//连通的直线在选中的两个点之间
					if (nCol2 > nCol && nCol > nCol1 && LineX(g, nRow1, nCol1 + 1, nCol) && LineX(g, nRow2, nCol, nCol2 - 1))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow1;
						Vx1.col = nCol;
						Vx2.row = nRow2;
						Vx2.col = nCol;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
					//连通的直线在选中的两个点右边
					if (nCol > nCol2 && LineX(g, nRow1, nCol1 + 1, nCol) && LineX(g, nRow2, nCol, nCol2 + 1))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow1;
						Vx1.col = nCol;
						Vx2.row = nRow2;
						Vx2.col = nCol;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
				}
			}
		}

		//找到一条与X轴平行的连通直线段
		for (int nRow = 0; nRow < 4; nRow++)
		{
			if (g[nRow][nCol1] == BLANK && g[nRow][nCol2] == BLANK)
			{
				if (LineX(g, nRow, nCol1, nCol2)) //判断该条Y轴直线是否连通
				{
					//连通直线在两个点上面
					if (nRow1 > nRow && LineY(g, nRow1 - 1, nRow, nCol1) && LineY(g, nRow, nRow2 - 1, nCol2))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow;
						Vx1.col = nCol1;
						Vx2.row = nRow;
						Vx2.col = nCol2;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
					//连通直线在两个点之间
					if (nRow2 > nRow && nRow > nRow1 && LineY(g, nRow1 + 1, nRow, nCol1) && LineY(g, nRow, nRow2 - 1, nCol2))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow;
						Vx1.col = nCol1;
						Vx2.row = nRow;
						Vx2.col = nCol2;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
					//连通直线在两个点下面
					if (nRow > nRow2 && LineY(g, nRow1 + 1, nRow, nCol1) && LineY(g, nRow, nRow2 + 1, nCol2))
					{
						//保存节点
						Vertex Vx1;
						Vertex Vx2;
						Vx1.row = nRow;
						Vx1.col = nCol1;
						Vx2.row = nRow;
						Vx2.col = nCol2;

						if (IsTemp == false)
						{
							AddVertex(Vx1);
							AddVertex(Vx2);
							return true;
						}
						else
						{
							AddVertex(Vx2);
							AddVertex(Vx1);
							return true;
						}
					}
				}
			}
		}
	}
	//找到一条与X轴平行的连通直线段
	return false;
}

//向数组中删除一个元素
void CGameLogic::DeleteVertex()
{
	m_nVexNum--;
}
*/
//消子
void CGameLogic::Clear(CGraph& g, Vertex v1, Vertex v2)
{
	//获得顶点索引
	int nIndex1 = v1.row * NCOL + v1.col;
	int nIndex2 = v2.row * NCOL + v2.col;
	//更新顶点信息
	g.UpdateVertex(nIndex1, BLANK);
	g.UpdateVertex(nIndex2, BLANK);
	//更新边
	UpdateArc(g, v1.row, v1.col);
	UpdateArc(g, v2.row, v2.col);
}

//获取连接路径
int CGameLogic::GetVexPath(Vertex avPath[MAX_VERTEX_NUM])
{
	for (int i = 0; i < m_nVexNum; i++)
	{
		avPath[i].row=m_anPath[i]/NCOL;
		avPath[i].col = m_anPath[i] - avPath[i].row * NCOL;
	}
	int t = m_nVexNum;
	m_nVexNum = 0;
	m_nCorner = 0;
	return t;//数组中实际元素的个数
}

bool CGameLogic::IsBlank(CGraph& g) {
	int Index = 1;
	for (int i = 0; i < CGameControl::s_nRows; i++)
	{
		for (int j = 0; j < CGameControl::s_nCols; j++) {
			if (g.GetVertex(i*NCOL+j) != BLANK) {
				Index = -1;
				return false;
			}
		}
	}
	if (Index == 1)
		return true;
}

//更新边
void CGameLogic::UpdateArc(CGraph& g, int nRow, int nCol)
{
	int nIndex = nRow * NCOL + nCol;
	//左边相邻
	if (nCol > 0)
	{
		int nIndexl = nIndex - 1;
		int nInfo1 = g.GetVertex(nIndexl);
		int nInfo2 = g.GetVertex(nIndex);
		if (nInfo1==nInfo2||nInfo1==BLANK||nInfo2==BLANK)
		{
			g.AddArc(nIndexl, nIndex);
		}
	}
	//右边
	if (nCol < NCOL-1)
	{
		int nIndexr = nIndex + 1;
		int nInfo1 = g.GetVertex(nIndexr);
		int nInfo2 = g.GetVertex(nIndex);
		if (nInfo1==nInfo2||nInfo1==BLANK||nInfo2==BLANK)
		{
			g.AddArc(nIndexr, nIndex);
		}
	}
	//上面
	if (nRow > 0)
	{
		int nIndexu = nIndex - NCOL;
		int nInfo1 = g.GetVertex(nIndexu);
		int nInfo2 = g.GetVertex(nIndex);
		if (nInfo1==nInfo2||nInfo1==BLANK||nInfo2==BLANK)
		{
			g.AddArc(nIndexu, nIndex);
		}
	}
	//下面
	if (nRow < NROW-1)
	{
		int nIndexd = nIndex + NCOL;
		int nInfo1 = g.GetVertex(nIndexd);
		int nInfo2 = g.GetVertex(nIndex);
		if (nInfo1==nInfo2||nInfo1==BLANK||nInfo2==BLANK)
		{
			g.AddArc(nIndexd, nIndex);
		}
	}
}
void CGameLogic::PushVertex(int v)
{
	m_anPath[m_nVexNum++] = v;
	if (IsCorner())
	{
		m_nCorner++;
	}
}

void CGameLogic::PopVertex()
{
	if (IsCorner())
	{
		m_nCorner--;
	}
	m_nVexNum--;
}
bool CGameLogic::SearchPath1(CGraph& g, int nV0, int nV1)
{
	//得到顶点数
	int nVexnum = g.GetVexnum();
	//深度优先搜索
	for (int i = 0; i < nVexnum; i++)
	{
		if (g.GetArc(nV0, i) && !IsExist(i))
		{
			//压入当前顶点
			PushVertex(i);
			if (m_nCorner > 2)
			{
				PopVertex();
				continue;
			}
			if (i != nV1)
			{
				//中间节点不为空，路径不通
				if (g.GetVertex(i) != BLANK)
				{
					PopVertex();
					continue;
				}
				if (SearchPath1(g, i, nV1))
				{
					return true;
				}
				else
				{
					PopVertex();
				}
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}

bool CGameLogic::IsExist(int v)
{
	for (int i = 0; i < m_nVexNum; i++)
	{
		if (m_anPath[i] == v)
		{
			return true;
		}
	}
	return false;
}

bool CGameLogic::IsCorner(void)
{
	if (m_nVexNum >= 3)
	{
		if (m_anPath[m_nVexNum - 1] + m_anPath[m_nVexNum - 3] == 2 * m_anPath[m_nVexNum - 2])
			return false;
		else
			return true;
	}
	return false;
}
bool CGameLogic::SearchValidPath(CGraph& g)
{
	int Info=-1;
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		if (g.GetVertex(i) != BLANK)
		{
			Info = g.GetVertex(i);
			for (int j = i + 1; j < MAX_VERTEX_NUM; j++)
			{
				if (Info == g.GetVertex(j))
				{
					
					PushVertex(i);
					if(SearchPath1(g,i,j)==true)
					{
						return true;
					}
					PopVertex();
				}
			}
		}
	}
	return false;
}
void CGameLogic::Reset(CGraph& g)
{
	int x1, x2,t;
	srand((int)time(NULL));
	for (int i = 0; i < MAX_VERTEX_NUM; i++)
	{
		x1 = rand() % MAX_VERTEX_NUM;
		x2 = rand() % MAX_VERTEX_NUM;
		t = g.GetVertex(x1);
		g.UpdateVertex(x1, g.GetVertex(x2));
		g.UpdateVertex(x2, t);
		UpdateArc(g, x1 / NCOL, x1 - x1 / NCOL * NCOL);
		UpdateArc(g, x2 / NCOL, x2 - x2 / NCOL * NCOL);
	}
}

