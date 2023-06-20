#pragma once
#include"CGraph.h"
#include"global.h"
class CGameLogic
{
protected:
	Vertex m_avPath[MAX_VERTEX_NUM];    //使用栈来保存连通路径中的关键点：起始点v0、拐点v1和终点v2
	int m_anPath[MAX_VERTEX_NUM];//用来保存连通路径的顶点索引
	int m_nCorner;//拐点数
	void AddVertex(Vertex v);        //向数组中添加一个元素
	int m_nVexNum;         //表示顶点数
	void DeleteVertex();             //向数组中删除一个元素

public:
	CGameLogic();
	~CGameLogic();
	void UpdateArc(CGraph& g, int nRow, int nCol);
	void InitMap(CGraph& g);
	void ReleaseMap(int** m_anMap);
	bool IsLink(CGraph& g, Vertex v1, Vertex v2);//连通判断
/*	bool LinkInRow(CGraph& g, Vertex v1, Vertex v2);//行号相同时，判断横向是否连通
	bool LinkInCol(CGraph& g, Vertex v1, Vertex v2);//列号相同时，判断纵向是否连通
	bool LineX(CGraph& g, int nRow, int nCol1, int nCol2);//判断（nRow1，nCol2）到（nRow2，nCol2）能否连通
	bool LineY(CGraph& g, int nRow1, int nRow2, int nCol);
	bool OneCornerLink(CGraph& g, Vertex v1, Vertex v2);
	bool TwoCornerLink(CGraph& g, Vertex v1, Vertex v2);
*/
	void Clear(CGraph& g, Vertex v1, Vertex v2);
	int GetVexPath(Vertex *avPath);
	void PushVertex(int v);
	void PopVertex(void);
	bool IsBlank(CGraph& g);
	bool SearchPath1(CGraph& g, int nV0, int nV1);//采用深度优先搜索，判断两个点是否连通
	bool IsExist(int nVi);//判断索引为nVi的点是否在m_anPath数组中，避免重复访问
	bool IsCorner(void);//判断m_anPath中，连续三个点是否构成一个顶点
	bool SearchValidPath(CGraph& g);
	void Reset(CGraph& g);
};