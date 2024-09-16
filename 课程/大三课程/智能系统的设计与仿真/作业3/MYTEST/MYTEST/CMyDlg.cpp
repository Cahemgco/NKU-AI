// CMyDlg.cpp: 实现文件
//

#include "pch.h"
#include "MYTEST.h"
#include "afxdialogex.h"
#include "CMyDlg.h"


// CMyDlg 对话框

IMPLEMENT_DYNAMIC(CMyDlg, CDialogEx)

CMyDlg::CMyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_x1(0)
	, m_y1(0)
	, m_x2(0)
	, m_y2(0)
	, m_Shape(0)
	, m_LineType(0)
	, m_C(0)
	, m_S(0)
	, m_LineLength(0)
{
	color = RGB(255, 0, 0);
}

CMyDlg::~CMyDlg()
{
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_X1, m_x1);
	DDX_Text(pDX, IDC_EDIT_Y1, m_y1);
	DDX_Text(pDX, IDC_EDIT_X2, m_x2);
	DDX_Text(pDX, IDC_EDIT_Y2, m_y2);
	DDX_Radio(pDX, IDC_RATIO_RECT, m_Shape);
	DDX_Radio(pDX, IDC_RADIO_SOLID, m_LineType);
	DDX_Text(pDX, IDC_EDIT2, m_C);
	DDX_Text(pDX, IDC_EDIT3, m_S);
	DDX_Control(pDX, IDC_STATIC_C, Static_C);
	DDX_Control(pDX, IDC_STATIC_S, Static_S);
	DDX_Control(pDX, IDC_STATIC_X1, Static_X1);
	DDX_Control(pDX, IDC_STATIC_Y1, Static_Y1);
	DDX_Control(pDX, IDC_STATIC_X2, Static_X2);
	DDX_Control(pDX, IDC_STATIC_Y2, Static_Y2);
}


BEGIN_MESSAGE_MAP(CMyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SETCOLOR, &CMyDlg::OnBnClickedButtonSetcolor)
	ON_BN_CLICKED(ID_CAL, &CMyDlg::OnBnClickedCal)
	ON_BN_CLICKED(ID_Reset, &CMyDlg::OnBnClickedReset)
	ON_BN_CLICKED(IDC_RATIO_RECT, &CMyDlg::OnBnClickedRatioRect)
	ON_BN_CLICKED(IDC_RADIO_ROUND, &CMyDlg::OnBnClickedRadioRound)
	ON_BN_CLICKED(IDC_RADIO_LINE, &CMyDlg::OnBnClickedRadioLine)
END_MESSAGE_MAP()


// CMyDlg 消息处理程序
void CMyDlg::OnBnClickedButtonSetcolor()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog colorDlg(color);	// 构造对话框，传入初始颜色值
	if (IDOK == colorDlg.DoModal())	// 显示对话框并判断是否点击“确定
	{
		color = colorDlg.GetColor(); // 获取颜色对话框中选择的颜色值
	}
}


void CMyDlg::OnBnClickedCal()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	switch (m_Shape) {
	case 0:
		m_C = 2 * abs(m_x2 - m_x1 + m_y2 - m_y1);
		m_S = abs(m_x2 - m_x1) * abs(m_y2 - m_y1);

		break;
	case 1:
		m_C = int(2 * 3.14 * m_x2);
		m_S = int(3.14 * m_x2 * m_x2);
		break;
	case 2:
		m_LineLength = int(sqrt(pow(m_x2 - m_x1, 2) + pow(m_y2 - m_y1, 2)));
		m_C = m_LineLength;
		break;
	}
	UpdateData(false);
}


void CMyDlg::OnBnClickedReset()
{
	// TODO: 在此添加控件通知处理程序代码
	m_C = 0;
	m_S = 0;
	m_LineLength = 0;
	UpdateData(false);
}


void CMyDlg::OnBnClickedRatioRect()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_CALC)->ShowWindow(SW_NORMAL);
	GetDlgItem(IDC_CALS)->ShowWindow(SW_NORMAL);
	GetDlgItem(IDC_EDIT_Y2)->ShowWindow(SW_NORMAL);
	Static_C.SetWindowTextW(L"周长");
	Static_S.SetWindowTextW(L"面积");
	Static_X1.SetWindowTextW(L"X1");
	Static_X2.SetWindowTextW(L"X2");
	Static_Y1.SetWindowTextW(L"Y1");
	Static_Y2.SetWindowTextW(L"Y2");
}


void CMyDlg::OnBnClickedRadioRound()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_CALC)->ShowWindow(SW_NORMAL);
	GetDlgItem(IDC_CALS)->ShowWindow(SW_NORMAL);
	GetDlgItem(IDC_EDIT_Y2)->ShowWindow(SW_HIDE);
	Static_C.SetWindowTextW(L"周长");
	Static_S.SetWindowTextW(L"面积");
	Static_X1.SetWindowTextW(L"x");
	Static_X2.SetWindowTextW(L"r");
	Static_Y1.SetWindowTextW(L"y");
	Static_Y2.SetWindowTextW(L"");
}


void CMyDlg::OnBnClickedRadioLine()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_CALC)->ShowWindow(SW_NORMAL);
	GetDlgItem(IDC_CALS)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_EDIT_Y2)->ShowWindow(SW_NORMAL);
	Static_C.SetWindowTextW(L"线长");
	Static_S.SetWindowTextW(L"");
	Static_X1.SetWindowTextW(L"X1");
	Static_X2.SetWindowTextW(L"X2");
	Static_Y1.SetWindowTextW(L"Y1");
	Static_Y2.SetWindowTextW(L"Y2");
}
