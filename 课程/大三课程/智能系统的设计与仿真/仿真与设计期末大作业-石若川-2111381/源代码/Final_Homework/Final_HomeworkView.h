
// Final_HomeworkView.h: CFinalHomeworkView 类的接口
//

#pragma once
#include<vector>

class CFinalHomeworkView : public CView
{
protected: // 仅从序列化创建
	CFinalHomeworkView() noexcept;
	DECLARE_DYNCREATE(CFinalHomeworkView)

// 特性
public:
	CFinalHomeworkDoc* GetDocument() const;

// 操作
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
	int m_interval;
	int m_num;
	bool m_Draw;
	bool m_isPause;
	std::vector<double> m_timePoints;
	std::vector<double> m_InputValue;

	std::vector<double> m_hValues; // 液位
	std::vector<double> m_uValues; // 控制量
	std::vector<double> m_QValues; // 输入流量
	std::vector<double> m_errorValues; // 误差

	void RungeKutta4();

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CFinalHomeworkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOptDlg();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnStart();
	afx_msg void OnPause();
	afx_msg void OnStop();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
};

#ifndef _DEBUG  // Final_HomeworkView.cpp 中的调试版本
inline CFinalHomeworkDoc* CFinalHomeworkView::GetDocument() const
   { return reinterpret_cast<CFinalHomeworkDoc*>(m_pDocument); }
#endif

