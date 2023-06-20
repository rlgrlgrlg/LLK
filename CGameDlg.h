#pragma once
#include"global.h"
#include"CGameControl.h"
// CGameDlg 对话框

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameDlg();


	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CDC m_dcMem;			    //内存DC
	CDC m_dcElement;			//元素内存DC
	CDC m_dcMask;			    //掩码内存DC
	CDC m_dcBG;				    //背景DC


	CPoint m_ptGameTop;		//游戏区起始点
	CSize m_sizeElem;		//元素的图片大小
	CRect m_rtGameRect;     //游戏区域大小

	bool m_bFirstPoint;		//判断选中的图片是否第一次选中的
	Vertex m_ptSelFirst;	//第一次选中的点
	Vertex m_ptSelSec;		//第二次选中的点
	CGameControl m_gameControl;

	void InitElement(void);//初始化元素与DC

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();//窗口初始化
	afx_msg void OnPaint();//WM_PAINT消息函数
	void UpdateWindow();//更新界面
	void InitBackground();	//初始化背景
	void UpdateMap();//更新游戏地图
	afx_msg void OnBnClickedBtnStart();//开始按钮
	void DrawTipFrame(int nRow, int nCol);//绘制矩形提示框
	bool IsLink();//连通判断
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);//判断点击位置是否在游戏地图中
	void DrawTipLine(Vertex avPath[], int nVexnum);
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnClickedButtonClue();
	afx_msg void OnClickedButtonH();
};