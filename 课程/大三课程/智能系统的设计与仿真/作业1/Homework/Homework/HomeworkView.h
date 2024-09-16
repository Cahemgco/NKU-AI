
// HomeworkView.h: CHomeworkView 类的接口
//

#pragma once


class CHomeworkView : public CView
{
protected: // 仅从序列化创建
	CHomeworkView() noexcept;
	DECLARE_DYNCREATE(CHomeworkView)

// 特性
public:
	CHomeworkDoc* GetDocument() const;

// 操作
public:

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
	virtual ~CHomeworkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	static const int DATA_NUM = 500;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
private:
	int cx, cy, dx, dy;
	double m_array[DATA_NUM];


};

#ifndef _DEBUG  // HomeworkView.cpp 中的调试版本
inline CHomeworkDoc* CHomeworkView::GetDocument() const
   { return reinterpret_cast<CHomeworkDoc*>(m_pDocument); }
#endif

