#pragma once
#include"global.h"
#include"CGameControl.h"
// CGameDlg �Ի���

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(CWnd* pParent = nullptr);   // ��׼���캯��
	virtual ~CGameDlg();


	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CDC m_dcMem;			    //�ڴ�DC
	CDC m_dcElement;			//Ԫ���ڴ�DC
	CDC m_dcMask;			    //�����ڴ�DC
	CDC m_dcBG;				    //����DC


	CPoint m_ptGameTop;		//��Ϸ����ʼ��
	CSize m_sizeElem;		//Ԫ�ص�ͼƬ��С
	CRect m_rtGameRect;     //��Ϸ�����С

	bool m_bFirstPoint;		//�ж�ѡ�е�ͼƬ�Ƿ��һ��ѡ�е�
	Vertex m_ptSelFirst;	//��һ��ѡ�еĵ�
	Vertex m_ptSelSec;		//�ڶ���ѡ�еĵ�
	CGameControl m_gameControl;

	void InitElement(void);//��ʼ��Ԫ����DC

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();//���ڳ�ʼ��
	afx_msg void OnPaint();//WM_PAINT��Ϣ����
	void UpdateWindow();//���½���
	void InitBackground();	//��ʼ������
	void UpdateMap();//������Ϸ��ͼ
	afx_msg void OnBnClickedBtnStart();//��ʼ��ť
	void DrawTipFrame(int nRow, int nCol);//���ƾ�����ʾ��
	bool IsLink();//��ͨ�ж�
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);//�жϵ��λ���Ƿ�����Ϸ��ͼ��
	void DrawTipLine(Vertex avPath[], int nVexnum);
	afx_msg void OnBnClickedReset();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnClickedButtonClue();
	afx_msg void OnClickedButtonH();
};