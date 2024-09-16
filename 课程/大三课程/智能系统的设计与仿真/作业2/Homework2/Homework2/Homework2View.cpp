
// Homework2View.cpp: CHomework2View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Homework2.h"
#endif

#include "Homework2Doc.h"
#include "Homework2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHomework2View

IMPLEMENT_DYNCREATE(CHomework2View, CView)

BEGIN_MESSAGE_MAP(CHomework2View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_Black, &CHomework2View::OnUpdateBlack)
	ON_UPDATE_COMMAND_UI(ID_Red, &CHomework2View::OnUpdateRed)
	ON_UPDATE_COMMAND_UI(ID_Green, &CHomework2View::OnUpdateGreen)
	ON_UPDATE_COMMAND_UI(ID_Blue, &CHomework2View::OnUpdateBlue)
//	ON_UPDATE_COMMAND_UI(ID_StartFlash, &CHomework2View::OnUpdateStartflash)
//	ON_UPDATE_COMMAND_UI(ID_StopFlash, &CHomework2View::OnUpdateStopflash)
	ON_COMMAND(ID_StartFlash, &CHomework2View::OnStartflash)
	ON_COMMAND(ID_StopFlash, &CHomework2View::OnStopflash)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CHomework2View 构造/析构

CHomework2View::CHomework2View() noexcept
{
	// TODO: 在此处添加构造代码
	index = 0;
	color = 0;
	m_nColors[0] = RGB(0, 0, 0); // 黑色
	m_nColors[1] = RGB(255, 0, 0); // 红色
	m_nColors[2] = RGB(0, 255, 0); // 绿色
	m_nColors[3] = RGB(0, 0, 255); // 蓝色
}

CHomework2View::~CHomework2View()
{
}

BOOL CHomework2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CHomework2View 绘图

void CHomework2View::OnDraw(CDC* pDC)
{
	CHomework2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码

	if (m_point.size() >= 5) {
		CPen pen(PS_SOLID, 2, m_nColors[pDoc->GetColor()]); // 实线，宽度为2，给定颜色
		CPen* pOldPen = pDC->SelectObject(&pen);

		// 循环绘制五个点与下一个点的连线
		for (int i = 0; i < 5; ++i) {
			pDC->MoveTo(m_point[0].x, m_point[0].y);
			pDC->SetTextColor(m_nColors[pDoc->GetColor()]);

			CString str;
			str.Format(_T("鼠标单击位置：(%d,%d)"), m_point[i].x, m_point[i].y);
			pDC->TextOut(m_point[i].x, m_point[i].y, str);
		}
		pDC->MoveTo(m_point[4].x, m_point[4].y);
		pDC->LineTo(m_point[1].x, m_point[1].y);

		pDC->MoveTo(m_point[1].x, m_point[1].y);
		pDC->LineTo(m_point[3].x, m_point[3].y);

		pDC->MoveTo(m_point[3].x, m_point[3].y);
		pDC->LineTo(m_point[0].x, m_point[0].y);

		pDC->MoveTo(m_point[0].x, m_point[0].y);
		pDC->LineTo(m_point[2].x, m_point[2].y);

		pDC->MoveTo(m_point[2].x, m_point[2].y);
		pDC->LineTo(m_point[4].x, m_point[4].y);

		// 恢复原来的画笔和画刷
		pDC->SelectObject(pOldPen);
		//pDC->SelectObject(pOldBrush)
	}
}


// CHomework2View 打印

BOOL CHomework2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHomework2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHomework2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CHomework2View 诊断

#ifdef _DEBUG
void CHomework2View::AssertValid() const
{
	CView::AssertValid();
}

void CHomework2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHomework2Doc* CHomework2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHomework2Doc)));
	return (CHomework2Doc*)m_pDocument;
}
#endif //_DEBUG


// CHomework2View 消息处理程序


void CHomework2View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CHomework2Doc* pDoc = GetDocument();
	CString str;
	str.Format(_T("鼠标单击位置：(%d,%d)"), point.x, point.y);
	pDoc->Set(point, str);

	m_point.push_back(point);

	Invalidate();

	CView::OnLButtonDown(nFlags, point);
}


void CHomework2View::OnUpdateBlack(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CHomework2Doc* pDoc = GetDocument();
	pCmdUI->SetRadio(pDoc->GetColor() == 0);
}


void CHomework2View::OnUpdateRed(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CHomework2Doc* pDoc = GetDocument();
	pCmdUI->SetRadio(pDoc->GetColor() == 1);
}


void CHomework2View::OnUpdateGreen(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CHomework2Doc* pDoc = GetDocument();
	pCmdUI->SetRadio(pDoc->GetColor() == 2);
}


void CHomework2View::OnUpdateBlue(CCmdUI* pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	CHomework2Doc* pDoc = GetDocument();
	pCmdUI->SetRadio(pDoc->GetColor() == 3);
}


//void CHomework2View::OnUpdateStartflash(CCmdUI* pCmdUI)
//{
	// TODO: 在此添加命令更新用户界面处理程序代码

//}


//void CHomework2View::OnUpdateStopflash(CCmdUI* pCmdUI)
//{
	// TODO: 在此添加命令更新用户界面处理程序代码

//}


void CHomework2View::OnStartflash()
{
	// TODO: 在此添加命令处理程序代码
	SetTimer(1, 500, NULL);
}


void CHomework2View::OnStopflash()
{
	// TODO: 在此添加命令处理程序代码
	KillTimer(1);
}


void CHomework2View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	color++;
	if (color == 4) {
		color = 0;
	}
	CHomework2Doc* pDoc = GetDocument();
	pDoc->SetColor(color);
	Invalidate();
	CView::OnTimer(nIDEvent);
}
