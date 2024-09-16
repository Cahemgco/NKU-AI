#pragma once
#include "afxdialogex.h"


// CMyDlg 对话框

class CMyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlg)

public:
	CMyDlg(CWnd* pParent = nullptr);   // 标准构造函数
	CMyDlg(double A, double h0, double A0, double Ku, double Max, double T, double TimeLength, double TimeStep, double Kp, double Ki, double Kd, int InputType, CWnd* pParent = nullptr); // 带参数的构造函数
	virtual ~CMyDlg();
	virtual BOOL OnInitDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_A;
	double m_h0;
	double m_A0;
	double m_Ku;
	double m_Max;
	double m_T;
	double m_TimeLength;
	double m_TimeStep;
	double m_Kp;
	double m_Ki;
	double m_Kd;
	int m_InputType;
	afx_msg void OnBnClickedJieyue();
	afx_msg void OnBnClickedSin();
};
