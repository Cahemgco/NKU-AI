#pragma once
#include "afxdialogex.h"


// CMyDlg 对话框

class CMyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMyDlg)

public:
	CMyDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMyDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_x1;
	int m_y1;
	int m_x2;
	int m_y2;
	int m_Shape;
	int m_LineType;
	afx_msg void OnBnClickedButtonSetcolor();
	COLORREF color;
	int m_C;
	int m_S;
	//afx_msg void OnEnChangeCalc();
	//afx_msg void OnEnChangeCals();
	afx_msg void OnBnClickedCal();
	afx_msg void OnBnClickedReset();
	int m_LineLength;
	afx_msg void OnBnClickedRatioRect();
	afx_msg void OnBnClickedRadioRound();
	afx_msg void OnBnClickedRadioLine();
	CStatic Static_C;
	CStatic Static_S;
	CStatic Static_X1;
	CStatic Static_Y1;
	CStatic Static_X2;
	CStatic Static_Y2;
};
