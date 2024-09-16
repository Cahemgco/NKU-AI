
// Homework2View.h: CHomework2View 类的接口
//

#pragma once
#include<vector>

class CHomework2View : public CView
{
private:
	std::vector<CPoint> m_point;
	int index;
	COLORREF m_nColors[4]; // 颜色值
	int color;	// 颜色索引

protected: // 仅从序列化创建
	CHomework2View() noexcept;
	DECLARE_DYNCREATE(CHomework2View)

// 特性
public:
	CHomework2Doc* GetDocument() const;

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
	virtual ~CHomework2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateBlack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateRed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBlue(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateStartflash(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateStopflash(CCmdUI* pCmdUI);
	afx_msg void OnStartflash();
	afx_msg void OnStopflash();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // Homework2View.cpp 中的调试版本
inline CHomework2Doc* CHomework2View::GetDocument() const
   { return reinterpret_cast<CHomework2Doc*>(m_pDocument); }
#endif

