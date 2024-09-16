
// MYTESTView.h: CMYTESTView 类的接口
//

#pragma once



class CMYTESTView : public CView
{
protected: // 仅从序列化创建
	CMYTESTView() noexcept;
	DECLARE_DYNCREATE(CMYTESTView)

// 特性
public:
	CMYTESTDoc* GetDocument() const;

// 操作
public:
	int m_Shape;
	int m_LineType;
	bool m_isDraw;
	COLORREF color;
	CRect rect;

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
	virtual ~CMYTESTView();
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
	afx_msg void OnDlg();
};

#ifndef _DEBUG  // MYTESTView.cpp 中的调试版本
inline CMYTESTDoc* CMYTESTView::GetDocument() const
   { return reinterpret_cast<CMYTESTDoc*>(m_pDocument); }
#endif

