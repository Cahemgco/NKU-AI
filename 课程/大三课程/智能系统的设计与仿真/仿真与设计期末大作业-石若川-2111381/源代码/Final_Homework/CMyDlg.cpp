// CMyDlg.cpp: 实现文件
//

#include "pch.h"
#include "Final_Homework.h"
#include "afxdialogex.h"
#include "CMyDlg.h"


// CMyDlg 对话框

IMPLEMENT_DYNAMIC(CMyDlg, CDialogEx)

CMyDlg::CMyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_A(10.0)
	, m_h0(0.0)
	, m_A0(0.5)
	, m_Ku(1.0)
	, m_Max(5.0)
	, m_T(5.0)
	, m_TimeLength(20.0)
	, m_TimeStep(0.1)
	, m_Kp(4.0)
	, m_Ki(0.1)
	, m_Kd(0)
	, m_InputType(0)
{

}

CMyDlg::CMyDlg(double A, double h0, double A0, double Ku, double Max, double T, double TimeLength, double TimeStep, double Kp, double Ki, double Kd, int InputType, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_A(A)
	, m_h0(h0)
	, m_A0(A0)
	, m_Ku(Ku)
	, m_Max(Max)
	, m_T(T)
	, m_TimeLength(TimeLength)
	, m_TimeStep(TimeStep)
	, m_Kp(Kp)
	, m_Ki(Ki)
	, m_Kd(Kd)
	, m_InputType(InputType)
{

}

CMyDlg::~CMyDlg()
{
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_A, m_A);
	DDX_Text(pDX, IDC_EDIT_h0, m_h0);
	DDX_Text(pDX, IDC_EDIT_A0, m_A0);
	DDX_Text(pDX, IDC_EDIT_Ku, m_Ku);
	DDX_Text(pDX, IDC_EDIT_Max, m_Max);
	DDX_Text(pDX, IDC_EDIT_T, m_T);
	DDX_Text(pDX, IDC_EDIT_TimeLength, m_TimeLength);
	DDX_Text(pDX, IDC_EDIT_TimeStep, m_TimeStep);
	DDX_Text(pDX, IDC_EDIT_Kp, m_Kp);
	DDX_Text(pDX, IDC_EDIT_Ki, m_Ki);
	DDX_Text(pDX, IDC_EDIT_Kd, m_Kd);
	DDX_Radio(pDX, IDC_Jieyue, m_InputType);
}


BEGIN_MESSAGE_MAP(CMyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Jieyue, &CMyDlg::OnBnClickedJieyue)
	ON_BN_CLICKED(IDC_Sin, &CMyDlg::OnBnClickedSin)
END_MESSAGE_MAP()


// CMyDlg 消息处理程序
BOOL CMyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 在这里初始化控件的值
	UpdateData(FALSE);

	return TRUE; // return TRUE unless you set the focus to a control
}


void CMyDlg::OnBnClickedJieyue()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_h0 = 0.0;
	UpdateData(false);
}

void CMyDlg::OnBnClickedSin()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	m_h0 = 5.0;
	UpdateData(false);
}
