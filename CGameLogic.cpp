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

	m_nVexNum = 0;     //������ʵ��Ԫ�صĸ���
	m_nCorner = 0;
}


CGameLogic::~CGameLogic()
{
}


// ��ʼ����Ϸ��ͼ
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
	//��Ϸ��ͼ��ʼ��

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

//��ͨ�ж�
bool CGameLogic::IsLink(CGraph& g, Vertex v1, Vertex v2) {

	//��ȡ����
	int nIndex1 = v1.row * NCOL + v1.col;
	int nIndex2 = v2.row * NCOL + v2.col;
	
	//�ж�������Ƿ��б�
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

//�к���ͬʱ���жϺ����Ƿ���ͨ
/*bool CGameLogic::LinkInRow(CGraph& g, Vertex v1, Vertex v2) {
	int nCol1 = v1.col;
	int nCol2 = v2.col;
	int nRow = v1.row;
	if (nCol1 > nCol2) {
		int nTemp = nCol1;
		nCol1 = nCol2;
		nCol2 = nTemp;
	}

	//ֱͨ
	for (int i = nCol1 + 1; i <= nCol2; i++)
	{
		if (i == nCol2)return true;
		if (g[nRow][i] != BLANK)break;
	}//�ж�����ֱ�����Ƿ�Ϊ�հ��������ȫΪ�հ�����,�������ͨ
	return false;
}

//�к���ͬʱ���ж������Ƿ���ͨ
bool CGameLogic::LinkInCol(CGraph& g, Vertex v1, Vertex v2) {
	int nRow1 = v1.row;
	int nRow2 = v2.row;
	int nCol = v1.col;
	//��֤nRow1��ֵ����nRow2
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
	}//�ж�����ֱ�����Ƿ�Ϊ�հ��������ȫΪ�հ�����,��������ͨ
	return false;
}

//�жϣ�nRow1��nCol1������nRow1��nCol2���ܷ���ͨ
bool CGameLogic::LineX(CGraph& g, int nRow, int nCol1, int nCol2)
{
	if (nCol1 > nCol2)
	{
		//��������
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

//�жϣ�nRow1��nCol2������nRow2��nCol2���ܷ���ͨ
bool CGameLogic::LineY(CGraph &g, int nRow1, int nRow2, int nCol)
{
	if (nRow1 > nRow2)
	{
		//��������
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

// �жϣ�nRow1��nCol1������nRow2��nCol2���ܷ���ͨ(����ֱ������)
bool CGameLogic::OneCornerLink(CGraph& g, Vertex v1, Vertex v2)
{
	//ֱ���ܹ����ӣ���ô����һ��������������к����ཻ�ĵ㣬ֻ����������Ϊ�գ����п���ʵ������ֱ������
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
		//�жϣ�nRow1 + 1, nCol1������nRow2��nCol2 + 1���ܷ���ͨ
		if (LineY(g, nRow1 + 1, nRow2, nCol1) && LineX(g, nRow2, nCol1, nCol2 + 1))
		{
			Vertex v = { nRow2,nCol1,BLANK };
			AddVertex(v);
			return true;
		}
		//�жϣ�nRow2 - 1, nCol2������nRow1��nCol1 - 1���ܷ���ͨ
		if (LineY(g, nRow2 - 1, nRow1, nCol2) && LineX(g, nRow1, nCol2, nCol1 - 1))
		{
			Vertex v = { nRow1,nCol2,BLANK };
			AddVertex(v);
			return true;
		}
	}
	else
	{
		//�жϣ�nRow1 + 1, nCol1������nRow2��nCol2 - 1���ܷ���ͨ
		if (LineY(g, nRow1 + 1, nRow2, nCol1) && LineX(g, nRow2, nCol1, nCol2 - 1))
		{
			Vertex v = { nRow2,nCol1,BLANK };
			AddVertex(v);
			return true;
		}
		//�жϣ�nRow2 - 1, nCol2������nRow1��nCol1 + 1���ܷ���ͨ
		if (LineY(g, nRow2 - 1, nRow1, nCol2) && LineX(g, nRow1, nCol2, nCol1 + 1))
		{
			Vertex v = { nRow1,nCol2,BLANK };
			AddVertex(v);
			return true;
		}
	}

	return false;
}

//�����������һ��Ԫ��
void CGameLogic::AddVertex(Vertex v)
{
	m_avPath[m_nVexNum] = v;
	m_nVexNum++;
}

//�жϣ�nRow1��nCol1������nRow2��nCol2���ܷ���ͨ(����ֱ������)
bool CGameLogic::TwoCornerLink(CGraph &g, Vertex v1, Vertex v2)
{
	int nRow1 = v1.row;
	int nCol1 = v1.col;
	int nRow2 = v2.row;
	int nCol2 = v2.col;
	bool IsTemp = false;

	//ȷ��nRow1<nRow2
	if (nRow1 > nRow2)
	{
		//���ݽ���
		int nTemp = nRow1;
		nRow1 = nRow2;
		nRow2 = nTemp;

		nTemp = nCol1;
		nCol1 = nCol2;
		nCol2 = nTemp;

		IsTemp = true;        //���������㽻��
	}
	if (nCol1 > nCol2)
	{
		//�ҵ�һ����Y��ƽ�е���ֱͨ�߶�
		for (int nCol = 0; nCol < 4; nCol++)
		{
			if (g[nRow1][nCol] == BLANK && g[nRow2][nCol] == BLANK)
			{
				if (LineY(g, nRow1, nRow2, nCol)) //�жϸ���Y��ֱ���Ƿ���ͨ
				{
					//��ͨ��ֱ����ѡ�е�����������
					if (nCol2 > nCol && LineX(g, nRow1, nCol - 1, nCol) && LineX(g, nRow2, nCol, nCol2 - 1))
					{
						//����ڵ�
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
					//��ͨ��ֱ����ѡ�е���������м�
					if (nCol1 > nCol && nCol > nCol2 && LineX(g, nRow1, nCol, nCol1 - 1) && LineX(g, nRow2, nCol2 + 1, nCol))
					{
						//����ڵ�
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
					//��ͨ��ֱ����ѡ�е���������ұ�
					if (nCol > nCol1 && LineX(g, nRow1, nCol1 + 1, nCol) && LineX(g, nRow2, nCol, nCol2 + 1))
					{
						//����ڵ�
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

		//�ҵ�һ����X��ƽ�е���ֱͨ�߶�
		for (int nRow = 0; nRow < 4; nRow++)
		{
			if (g[nRow][nCol1] == BLANK && g[nRow][nCol2] == BLANK)
			{
				if (LineX(g, nRow, nCol1, nCol2)) //�жϸ���Y��ֱ���Ƿ���ͨ
				{
					//��ͨ��ֱ����ѡ�е������������
					if (nRow1 > nRow && LineY(g, nRow1 - 1, nRow, nCol1) && LineY(g, nRow, nRow2 - 1, nCol2))
					{
						//����ڵ�
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
					//��ͨ��ֱ����ѡ�е���������м�
					if (nRow2 > nRow && nRow > nRow1 && LineY(g, nRow1 + 1, nRow, nCol1) && LineY(g, nRow, nRow2 - 1, nCol2))
					{
						//����ڵ�
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
					//��ͨ��ֱ����ѡ�е������������
					if (nRow > nRow2 && LineY(g, nRow1 + 1, nRow, nCol1) && LineY(g, nRow, nRow2 + 1, nCol2))
					{
						//����ڵ�
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
		//�ҵ�һ����Y��ƽ�е���ֱͨ�߶�
		for (int nCol = 0; nCol < 4; nCol++)
		{
			if (g[nRow1][nCol] == BLANK && g[nRow2][nCol] == BLANK)
			{
				if (LineY(g, nRow1, nRow2, nCol)) //�жϸ���Y��ֱ���Ƿ���ͨ
				{
					//��ͨ��ֱ����ѡ�е��������
					if (nCol1 > nCol && LineX(g, nRow1, nCol1 - 1, nCol) && LineX(g, nRow2, nCol, nCol2 - 1))
					{
						//����ڵ�
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
					//��ͨ��ֱ����ѡ�е�������֮��
					if (nCol2 > nCol && nCol > nCol1 && LineX(g, nRow1, nCol1 + 1, nCol) && LineX(g, nRow2, nCol, nCol2 - 1))
					{
						//����ڵ�
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
					//��ͨ��ֱ����ѡ�е��������ұ�
					if (nCol > nCol2 && LineX(g, nRow1, nCol1 + 1, nCol) && LineX(g, nRow2, nCol, nCol2 + 1))
					{
						//����ڵ�
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

		//�ҵ�һ����X��ƽ�е���ֱͨ�߶�
		for (int nRow = 0; nRow < 4; nRow++)
		{
			if (g[nRow][nCol1] == BLANK && g[nRow][nCol2] == BLANK)
			{
				if (LineX(g, nRow, nCol1, nCol2)) //�жϸ���Y��ֱ���Ƿ���ͨ
				{
					//��ֱͨ��������������
					if (nRow1 > nRow && LineY(g, nRow1 - 1, nRow, nCol1) && LineY(g, nRow, nRow2 - 1, nCol2))
					{
						//����ڵ�
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
					//��ֱͨ����������֮��
					if (nRow2 > nRow && nRow > nRow1 && LineY(g, nRow1 + 1, nRow, nCol1) && LineY(g, nRow, nRow2 - 1, nCol2))
					{
						//����ڵ�
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
					//��ֱͨ��������������
					if (nRow > nRow2 && LineY(g, nRow1 + 1, nRow, nCol1) && LineY(g, nRow, nRow2 + 1, nCol2))
					{
						//����ڵ�
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
	//�ҵ�һ����X��ƽ�е���ֱͨ�߶�
	return false;
}

//��������ɾ��һ��Ԫ��
void CGameLogic::DeleteVertex()
{
	m_nVexNum--;
}
*/
//����
void CGameLogic::Clear(CGraph& g, Vertex v1, Vertex v2)
{
	//��ö�������
	int nIndex1 = v1.row * NCOL + v1.col;
	int nIndex2 = v2.row * NCOL + v2.col;
	//���¶�����Ϣ
	g.UpdateVertex(nIndex1, BLANK);
	g.UpdateVertex(nIndex2, BLANK);
	//���±�
	UpdateArc(g, v1.row, v1.col);
	UpdateArc(g, v2.row, v2.col);
}

//��ȡ����·��
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
	return t;//������ʵ��Ԫ�صĸ���
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

//���±�
void CGameLogic::UpdateArc(CGraph& g, int nRow, int nCol)
{
	int nIndex = nRow * NCOL + nCol;
	//�������
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
	//�ұ�
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
	//����
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
	//����
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
	//�õ�������
	int nVexnum = g.GetVexnum();
	//�����������
	for (int i = 0; i < nVexnum; i++)
	{
		if (g.GetArc(nV0, i) && !IsExist(i))
		{
			//ѹ�뵱ǰ����
			PushVertex(i);
			if (m_nCorner > 2)
			{
				PopVertex();
				continue;
			}
			if (i != nV1)
			{
				//�м�ڵ㲻Ϊ�գ�·����ͨ
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

