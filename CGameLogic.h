#pragma once
#include"CGraph.h"
#include"global.h"
class CGameLogic
{
protected:
	Vertex m_avPath[MAX_VERTEX_NUM];    //ʹ��ջ��������ͨ·���еĹؼ��㣺��ʼ��v0���յ�v1���յ�v2
	int m_anPath[MAX_VERTEX_NUM];//����������ͨ·���Ķ�������
	int m_nCorner;//�յ���
	void AddVertex(Vertex v);        //�����������һ��Ԫ��
	int m_nVexNum;         //��ʾ������
	void DeleteVertex();             //��������ɾ��һ��Ԫ��

public:
	CGameLogic();
	~CGameLogic();
	void UpdateArc(CGraph& g, int nRow, int nCol);
	void InitMap(CGraph& g);
	void ReleaseMap(int** m_anMap);
	bool IsLink(CGraph& g, Vertex v1, Vertex v2);//��ͨ�ж�
/*	bool LinkInRow(CGraph& g, Vertex v1, Vertex v2);//�к���ͬʱ���жϺ����Ƿ���ͨ
	bool LinkInCol(CGraph& g, Vertex v1, Vertex v2);//�к���ͬʱ���ж������Ƿ���ͨ
	bool LineX(CGraph& g, int nRow, int nCol1, int nCol2);//�жϣ�nRow1��nCol2������nRow2��nCol2���ܷ���ͨ
	bool LineY(CGraph& g, int nRow1, int nRow2, int nCol);
	bool OneCornerLink(CGraph& g, Vertex v1, Vertex v2);
	bool TwoCornerLink(CGraph& g, Vertex v1, Vertex v2);
*/
	void Clear(CGraph& g, Vertex v1, Vertex v2);
	int GetVexPath(Vertex *avPath);
	void PushVertex(int v);
	void PopVertex(void);
	bool IsBlank(CGraph& g);
	bool SearchPath1(CGraph& g, int nV0, int nV1);//������������������ж��������Ƿ���ͨ
	bool IsExist(int nVi);//�ж�����ΪnVi�ĵ��Ƿ���m_anPath�����У������ظ�����
	bool IsCorner(void);//�ж�m_anPath�У������������Ƿ񹹳�һ������
	bool SearchValidPath(CGraph& g);
	void Reset(CGraph& g);
};