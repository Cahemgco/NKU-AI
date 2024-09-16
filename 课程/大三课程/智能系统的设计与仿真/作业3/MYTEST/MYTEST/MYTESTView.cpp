
// MYTESTView.cpp: CMYTESTView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MYTEST.h"
#endif

#include "MYTESTDoc.h"
#include "MYTESTView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CMyDlg.h"


// CMYTESTView

IMPLEMENT_DYNCREATE(CMYTESTView, CView)

BEGIN_MESSAGE_MAP(CMYTESTView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DLG, &CMYTESTView::OnDlg)
END_MESSAGE_MAP()

// CMYTESTView 构造/析构

CMYTESTView::CMYTESTView() noexcept
{
	// TODO: 在此处添加构造代码
	m_Shape = 0;
	m_LineType = 0;
	m_isDraw = false;
	color = RGB(255, 0, 0);
}

CMYTESTView::~CMYTESTView()
{
}

BOOL CMYTESTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMYTESTView 绘图

void CMYTESTView::OnDraw(CDC* pDC)
{
	CMYTESTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	CPen newPen; CPen * pOldPen;
	switch(m_LineType) {
		case 0: newPen.CreatePen(PS_SOLID, 1, color); break;
		case 1: newPen.CreatePen(PS_DASH, 1, color); break;
		case 2: newPen.CreatePen(PS_DASHDOT, 1, color); break;
	}
	pOldPen = pDC->SelectObject(&newPen);
	CString str;
	switch(m_Shape) {
		case 0: 
			pDC->Rectangle(rect); 
			pDC->SetTextColor(RGB(0, 0, 0));
			if (m_isDraw)
			{
				str.Format(_T("（形状：矩形, R：%d,G：%d,B：%d）"), GetRValue(color), GetGValue(color), GetBValue(color));
				pDC->TextOut(rect.left, rect.bottom, str);
			}
			break;
		case 1: 
			pDC->Ellipse(rect);
			pDC->SetTextColor(RGB(0, 0, 0));
			if (m_isDraw)
			{
				str.Format(_T("（形状：圆形, R：%d,G：%d,B：%d）"), GetRValue(color), GetGValue(color), GetBValue(color));
				pDC->TextOut(rect.left, rect.bottom, str);
			}
			break;
		case 2: 
			pDC->MoveTo(CPoint(rect.left, rect.top));
			pDC->LineTo(CPoint(rect.right, rect.bottom));
			if (m_isDraw)
			{
				str.Format(_T("（形状：直线, R：%d,G：%d,B：%d）"), GetRValue(color), GetGValue(color), GetBValue(color));
				pDC->TextOut(rect.left, rect.bottom, str);
			}
			break;
	}
	pDC->SelectObject(pOldPen);
}


// CMYTESTView 打印

BOOL CMYTESTView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMYTESTView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMYTESTView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMYTESTView 诊断

#ifdef _DEBUG
void CMYTESTView::AssertValid() const
{
	CView::AssertValid();
}

void CMYTESTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMYTESTDoc* CMYTESTView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMYTESTDoc)));
	return (CMYTESTDoc*)m_pDocument;
}
#endif //_DEBUG


// CMYTESTView 消息处理程序


void CMYTESTView::OnDlg()
{
	// TODO: 在此添加命令处理程序代码
	CMyDlg Dlg;
	//Dlg.DoModal();

	if (Dlg.DoModal() == IDOK) {
		m_isDraw = true;
		m_Shape = Dlg.m_Shape; // 绘制形状
		m_LineType = Dlg.m_LineType; // 线的类型

		switch (m_Shape) {
		case 0:
			rect.left = Dlg.m_x1;
			rect.right = Dlg.m_x2;
			rect.top = Dlg.m_y1;
			rect.bottom = Dlg.m_y2;
			break;
		case 1:
			rect.left = Dlg.m_x1 - Dlg.m_x2; // 圆心-半径
			rect.right = Dlg.m_x1 + Dlg.m_x2; // 圆心+半径
			rect.top = Dlg.m_y1 - Dlg.m_x2;
			rect.bottom = Dlg.m_y1 + Dlg.m_x2;
			break;
		case 2:
			rect.left = Dlg.m_x1;
			rect.right = Dlg.m_x2;
			rect.top = Dlg.m_y1;
			rect.bottom = Dlg.m_y2;
			break;
		}

		color = Dlg.color;
		Invalidate();
	}
}
