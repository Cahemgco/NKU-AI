
// HomeworkView.cpp: CHomeworkView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Homework.h"
#endif

#include "HomeworkDoc.h"
#include "HomeworkView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "math.h"
#define PI 3.1415926

// CHomeworkView

IMPLEMENT_DYNCREATE(CHomeworkView, CView)

BEGIN_MESSAGE_MAP(CHomeworkView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

// CHomeworkView 构造/析构

CHomeworkView::CHomeworkView() noexcept
{
	// TODO: 在此处添加构造代码
	cx = 6;
	cy = 4;
	dx = 1;
	dy = 5;
	for (int i = 0; i < DATA_NUM; i++) {
		m_array[i] = 10 * sin(i * 2 * PI / DATA_NUM);
	}
}

CHomeworkView::~CHomeworkView()
{
}

BOOL CHomeworkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CHomeworkView 绘图

void CHomeworkView::OnDraw(CDC* pDC)
{
	CHomeworkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect rect;  //获取窗口的大小
	GetClientRect(&rect);

	// 限制绘图区域为左上四分之一矩形
	CRgn rgn;
	rgn.CreateRectRgn(0, 0, rect.Width() / 2, rect.Height() / 2);
	pDC->SelectObject(rgn);

	CBrush NewBrush;
	CBrush* pOldBrush;
	NewBrush.CreateSolidBrush(RGB(0, 0, 0));
	pOldBrush = pDC->SelectObject(&NewBrush);
	pDC->Rectangle(rect);
	pDC->SelectObject(pOldBrush);

	//设置逻辑坐标系原点
	CPoint ptLeft(rect.left, rect.top);
	CPoint ptRight(rect.right, rect.bottom);
	pDC->SetWindowOrg(0, -rect.Height() / 4);

	CPen NewPen;
	CPen* pOldPen;
	NewPen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	pOldPen = pDC->SelectObject(&NewPen);

	//	绘制横纵坐标
	pDC->MoveTo(0, 0);
	pDC->LineTo(rect.Width() / 2, 0);
	pDC->MoveTo(10, -rect.Height() / 4);
	pDC->LineTo(10, rect.Height() / 4);

	//设置文本颜色和背景色
	pDC->SetBkColor(RGB(0, 0, 0));
	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetTextAlign(TA_CENTER | TA_TOP);

	//绘制x轴刻度
	for (int i = 0; i < cx; i++) {
		if (i == cx - 1) {// x轴末端绘制箭头
			pDC->MoveTo(10 + (rect.Width() / 2 - 10) * (i + 1) / cx, 0);
			pDC->LineTo((rect.Width() / 2 - 10) * (i + 1) / cx, 5);
			pDC->MoveTo(10 + (rect.Width() / 2 - 10) * (i + 1) / cx, 0);
			pDC->LineTo((rect.Width() / 2 - 10) * (i + 1) / cx, -5);
		}
		else {
			pDC->MoveTo(10 + (rect.Width() / 2 - 10) * (i + 1) / cx, 0);
			CString str1;
			str1.Format(_T("%d"), i + 1);
			pDC->TextOut(10 + (rect.Width() / 2 - 10) * (i + 1) / cx, 10, str1);
			pDC->LineTo(10 + (rect.Width() / 2 - 10) * (i + 1) / cx, -10);
		}
	}

	//绘制y坐标刻度
	for (int i = 0; i < cy; i++) {
		if (i == cy - 1) {// y轴末端绘制箭头
			pDC->MoveTo(10, -rect.Height() / 2 * (i + 1) / cy + rect.Height() / 4);
			pDC->LineTo(15, -rect.Height() / 2 * (i + 1) / cy + rect.Height() / 4 + 10);
			pDC->MoveTo(10, -rect.Height() / 2 * (i + 1) / cy + rect.Height() / 4);
			pDC->LineTo(5, -rect.Height() / 2 * (i + 1) / cy + rect.Height() / 4 + 10);
		}
		else {
			pDC->MoveTo(10, -rect.Height() / 2 * (i + 1) / cy + rect.Height() / 4);
			pDC->LineTo(20, -rect.Height() / 2 * (i + 1) / cy + rect.Height() / 4);
			CString str1;
			str1.Format(_T("%d"), -(cy / 2 - i - 1) * dy);
			pDC->TextOut(25, -rect.Height() / 2 * (i + 1) / cy + rect.Height() / 4 + 5, str1);
		}
	}

	//计算横向数据点之间的间距
	double x_step = (ptRight.x / 2 - ptLeft.x) / ((double)DATA_NUM);


	double arrayMax = -1, arrayMin = 1000;  //获取数据最大值最小值
	for (int i = 0; i < DATA_NUM; i++) {
		if (m_array[i] > arrayMax) arrayMax = m_array[i];
		if (m_array[i] < arrayMin) arrayMin = m_array[i];
	}

	//计算纵向数据点之间的间距
	double y_step = (ptLeft.y - ptRight.y / 2) / (arrayMax - arrayMin);

	CPen m_redPen;
	m_redPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	pDC->SelectObject(m_redPen);
	int x, y;

	//绘制正弦图像
	for (int i = 0; i < DATA_NUM - 1; i++) {
		x = (int)(i * x_step) + 10; 
		y = (int)(m_array[i] * y_step); 
		pDC->MoveTo(x, y);
		x = (int)((i + 1) * x_step) + 10; 
		y = (int)(m_array[i + 1] * y_step); 
		pDC->LineTo(x, y);
	}
	pDC->SelectObject(pOldPen);
}


// CHomeworkView 打印

BOOL CHomeworkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHomeworkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHomeworkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CHomeworkView 诊断

#ifdef _DEBUG
void CHomeworkView::AssertValid() const
{
	CView::AssertValid();
}

void CHomeworkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHomeworkDoc* CHomeworkView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHomeworkDoc)));
	return (CHomeworkDoc*)m_pDocument;
}
#endif //_DEBUG


// CHomeworkView 消息处理程序


void CHomeworkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnLButtonDown(nFlags, point);
	CClientDC dc(this);
	CPen Pen;
	Pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	dc.SelectObject(Pen);

	CRect rect;
	GetClientRect(&rect);
	if (point.x < rect.Width() / 2 && point.y < rect.Height() / 2) {
		// 绘制十字
		dc.MoveTo(point.x - 10, point.y);
		dc.LineTo(point.x + 10, point.y);
		dc.MoveTo(point.x, point.y - 10);
		dc.LineTo(point.x, point.y + 10);

		// 显示坐标
		CString str1;
		double x, y;
		x = double(point.x) / rect.Width() * dx * cx * 2;
		y = 10 - double(point.y) / rect.Height() * dy * cy * 2;
		str1.Format(_T("x=%f,y=%f"), x, y);
		dc.TextOut(point.x, point.y, str1);
	}
}


void CHomeworkView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Invalidate(); // 清除十字与坐标显示
	CView::OnRButtonDown(nFlags, point);
}
