// CGameDlg.cpp: ʵ���ļ�
//

#include "pch.h"
#include "LLK.h"
#include "CGameDlg.h"
#include "afxdialogex.h"
#include "LLKDlg.h"
#include"resource.h"


// CGameDlg �Ի���

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME_DIALOG , pParent)
{
	// ��ʼ����ʼ������
	m_ptGameTop.x = 50;
	m_ptGameTop.y = 50;

	//��ʼ��ͼƬԪ�ش�С
	m_sizeElem.cx = 40;
	m_sizeElem.cy = 40;

	//��ʼ����Ϸ��������
	m_rtGameRect.left = m_ptGameTop.x;
	m_rtGameRect.top = m_ptGameTop.y;//��Ϸ����ĳ�ʼ��
	m_rtGameRect.right = m_rtGameRect.left + m_sizeElem.cx * CGameControl::s_nCols;
	m_rtGameRect.bottom = m_rtGameRect.top + m_sizeElem.cy * CGameControl::s_nRows;

	//��ʼ��ͼ��ѡ��״̬
	m_bFirstPoint = true;
}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_START, &CGameDlg::OnBnClickedBtnStart)
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CGameDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_BUTTON_CLUE, &CGameDlg::OnClickedButtonClue)
	ON_BN_CLICKED(IDC_BUTTON_H, &CGameDlg::OnClickedButtonH)
END_MESSAGE_MAP()


// CGameDlg ��Ϣ�������

//��ʼ�����ڱ����ʹ�С
void CGameDlg::InitBackground()
{

	//����λͼ
	HANDLE Backbmp = ::LoadImage(NULL, _T("theme\\picture\\fruit_bg.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	//��õ�ǰ�Ի������Ƶ����
	CClientDC dc(this);

	//��������Ƶ���ݼ��ݵ��ڴ�DC
	m_dcBG.CreateCompatibleDC(&dc);

	//��λͼ��Դѡ��DC
	m_dcBG.SelectObject(Backbmp);

	//��ʼ���ڴ�DC
	m_dcMem.CreateCompatibleDC(&dc);
	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc, 800, 600);
	m_dcMem.SelectObject(&bmpMem);

	//���Ʊ���ͼƬ
	m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);

	//���´���
	UpdateWindow();
}

void CGameDlg::InitElement()
{
	//��õ�ǰ�Ի������Ƶ�ڴ�
	CClientDC dc(this);

	HANDLE hBmp = ::LoadImageW(NULL, _T("theme\\picture\\fruit_element.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	m_dcElement.CreateCompatibleDC(&dc);
	m_dcElement.SelectObject(hBmp);

	HANDLE hMask = ::LoadImageW(NULL, _T("theme\\picture\\fruit_mask.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	m_dcMask.CreateCompatibleDC(&dc);
	m_dcMask.SelectObject(hMask);

}

BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->SetWindowTextW(_T("������������������ģʽ"));

	InitBackground();

	InitElement();

	return TRUE;  // return TRUE unless you set the focus to a control
}


void CGameDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// ���Ʊ���ͼƬ	
	dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY);

	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}

void CGameDlg::UpdateWindow()
{

	// TODO: �ڴ˴����ʵ�ִ���.

	//�������ڴ�С
	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);		//��ô��ڴ�С
	this->GetWindowRect(rtClient);	//��ÿͻ�����С

	//����������߿�Ĵ�С
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Width() - rtClient.Width();

	//���ô��ڴ�С
	MoveWindow(0, 0, 800 + nSpanWidth, 600 + nSpanHeight);

	//���öԻ�����ʾ����windows����������
	CenterWindow();
}




//���µ�ͼ
void CGameDlg::UpdateMap()
{

	//����ͼƬ�Ķ���������ͼƬ��С
	int nTop = m_ptGameTop.y;
	int nLeft = m_ptGameTop.x;
	int nElemW = m_sizeElem.cx;
	int nElemH = m_sizeElem.cy;

	m_dcMem.BitBlt(m_rtGameRect.left, m_rtGameRect.top, m_rtGameRect.Width(), m_rtGameRect.Height(), &m_dcBG, m_rtGameRect.left, m_rtGameRect.top, SRCCOPY);

	for (int i = 0; i < CGameControl::s_nRows; i++) {
		for (int j = 0; j < CGameControl::s_nCols; j++) {

			int nInfo = m_gameControl.GetElement(i, j);       //��ǰԪ��ͼƬ����ֵ
			if (nInfo == BLANK)  continue;

			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcMask, 0, m_gameControl.GetElement(i,j) * nElemH, SRCPAINT);

			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH, &m_dcElement, 0, m_gameControl.GetElement(i,j) * nElemH, SRCAND);

		}
	}

}


//��ʼ��Ϸ
void CGameDlg::OnBnClickedBtnStart()
{


	m_gameControl.StartGame();
	UpdateMap();

	Invalidate(FALSE);
}



//���ƾ�����ʾ��
void CGameDlg::DrawTipFrame(int nRow, int nCol) {

	CClientDC dc(this);
	CBrush brush(RGB(233, 43, 43));//���ο����ɫ
	CRect rtTipFrame;

	//���ݾ��ε����Ͻ���������Ͻ�������л���
	rtTipFrame.left = m_ptGameTop.x + nCol * m_sizeElem.cx;
	rtTipFrame.top = m_ptGameTop.y + nRow * m_sizeElem.cy;
	rtTipFrame.right = rtTipFrame.left + m_sizeElem.cx;
	rtTipFrame.bottom = rtTipFrame.top + m_sizeElem.cy;
	dc.FrameRect(rtTipFrame, &brush);
}

//�жϵ��λ���Ƿ�����Ϸ��ͼ��
void CGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	//�ж������������
	if (point.y<m_rtGameRect.top || point.y>m_rtGameRect.bottom || point.x < m_rtGameRect.left || point.x > m_rtGameRect.right) {
		return CDialogEx::OnLButtonUp(nFlags, point);
	}
	int nRow = (point.y - m_ptGameTop.y) / m_sizeElem.cy;
	int nCol = (point.x - m_ptGameTop.x) / m_sizeElem.cx;

	if (nRow>m_gameControl.s_nRows||nCol>m_gameControl.s_nCols) {
		return CDialogEx::OnLButtonUp(nFlags, point);
	}
	// ��һ����
	if (m_bFirstPoint) {
		//������ʾ��
		DrawTipFrame(nRow, nCol);
		m_gameControl.SetFirstPoint(nRow, nCol);

	}
	//�ڶ�����
	else {
		//������ʾ��
		DrawTipFrame(nRow, nCol);
		m_gameControl.SetSecPoint(nRow, nCol);

		Vertex avPath[MAX_VERTEX_NUM];     //���·��
		int nVexnum = 0;      //�������

		//�ж��Ƿ�����ͬͼƬ����������
		if (m_gameControl.Link(avPath, nVexnum)) {

			DrawTipLine(avPath, nVexnum);
			//���µ�ͼ
			UpdateMap();
		}
		Sleep(200);    //�ӳ�
		InvalidateRect(m_rtGameRect, FALSE); //�ֲ����θ���

		if (m_gameControl.IsWin()) {
			MessageBox(_T("\t��Ϸʤ��"), _T("����������������ģʽ"));

		}
	}

	m_bFirstPoint = !m_bFirstPoint; //����ֵ
	CDialogEx::OnLButtonUp(nFlags, point);
}

//��ͨ�ж�
bool CGameDlg::IsLink()
{
	if(m_gameControl.GetElement(m_ptSelFirst.row,m_ptSelFirst.col)==m_gameControl.GetElement(m_ptSelSec.row,m_ptSelSec.col))
		return true;
	return false;
}
//������ʾ��
void CGameDlg::DrawTipLine(Vertex avPath[MAX_VERTEX_NUM], int nVexnum)
{
	CClientDC dc(this);

	//���û���
	CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));

	//������ѡ��DC
	CPen* pOldPen = dc.SelectObject(&penLine);

	dc.MoveTo(m_ptGameTop.x + avPath[0].col * m_sizeElem.cx + m_sizeElem.cx / 2,
		m_ptGameTop.y + avPath[0].row * m_sizeElem.cy + m_sizeElem.cy / 2);
	//����������
	for (int i = 0; i < nVexnum - 1; i++)
	{

		dc.LineTo(m_ptGameTop.x + avPath[i + 1].col * m_sizeElem.cx + m_sizeElem.cx / 2,
			m_ptGameTop.y + avPath[i + 1].row * m_sizeElem.cy + m_sizeElem.cy / 2);
	}

	dc.SelectObject(pOldPen);
}

void CGameDlg::OnBnClickedReset()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_gameControl.Reset();

	//���½���
	UpdateMap();
	//�ֲ����θ���
	InvalidateRect(m_rtGameRect, FALSE);

}

void CGameDlg::OnBnClickedButton7()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CGameDlg::OnClickedButtonClue()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Vertex avPath[MAX_VERTEX_NUM];
	int nVexnum=0;
	if (m_gameControl.Help(avPath,nVexnum))
	{
		DrawTipFrame(avPath[0].row, avPath[0].col);
		DrawTipFrame(avPath[nVexnum-1].row, avPath[nVexnum-1].col);
		Sleep(200);    //�ӳ�
		InvalidateRect(m_rtGameRect, FALSE); //�ֲ����θ���
	}

}


void CGameDlg::OnClickedButtonH()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	MessageBox(_T("���������ͬ��ͼƬ���������ӣ������ߵĹս���2����һ�£�����ȥ��ȫ����������Ϸʤ��"), _T("����������������"));
}
