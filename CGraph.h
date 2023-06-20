#ifndef CGRAPH_H
#define CGRAPH_H
class CGraph
{
public:
//	CGraph();
//	~CGraph();



public:

#define MAX_VERTEX_NUM 64					//顶点个数（8行8列）
#define NCOL 8
#define NROW 8
	typedef int Vertices[MAX_VERTEX_NUM];	//顶点数据类型
	typedef bool AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];//边数据类型  矩阵

	void AddVertex(int info);//将顶点info添加到顶点数组中
	int GetVertex(int nIndex);//获取顶点数组中索引为nIndex的顶点
	int GetVexnum();//获取顶点数组中的顶点数
	void AddArc(int nV1Index, int nV2Index);//添加关系矩阵中，nV1Index与 nV2Index
	bool GetArc(int nV1Index, int nV2Index);//获取两个顶点的弧信息
	void UpdateVertex(int nIndex, int info);//将图顶点数组中索引号为nIndex的顶点的值更新为info
	void ClearGraph();//将图中顶点和边还原为初始状态
	void Change(int n1,int n2);//交换两个顶点的值
	  
	AdjMatrix m_AdjMatrix;	//边数组  
	Vertices m_Vertices;//顶点数组
	int m_nVexnum;		//顶点数
	int m_nArcnum;		//边数量
	void InitGraph();		//初始化图结构

};

#endif