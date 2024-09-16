
// Final_HomeworkView.cpp: CFinalHomeworkView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Final_Homework.h"
#endif

#include "Final_HomeworkDoc.h"
#include "Final_HomeworkView.h"

#include "CMyDlg.h"
#include<vector>
#include<algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFinalHomeworkView

IMPLEMENT_DYNCREATE(CFinalHomeworkView, CView)

BEGIN_MESSAGE_MAP(CFinalHomeworkView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_OPT_DLG, &CFinalHomeworkView::OnOptDlg)
	ON_WM_TIMER()
	ON_COMMAND(ID_Start, &CFinalHomeworkView::OnStart)
	ON_COMMAND(ID_Pause, &CFinalHomeworkView::OnPause)
	ON_COMMAND(ID_Stop, &CFinalHomeworkView::OnStop)
	ON_COMMAND(ID_FILE_OPEN, &CFinalHomeworkView::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CFinalHomeworkView::OnFileSave)
END_MESSAGE_MAP()

// CFinalHomeworkView 构造/析构

CFinalHomeworkView::CFinalHomeworkView() noexcept
{
	// TODO: 在此处添加构造代码
	m_A = 10.0;
	m_h0 = 0.0;
	m_A0 = 0.5;
	m_Ku = 1;
	m_Max = 5.0;
	m_T = 5.0;
	m_TimeLength = 20.0;
	m_TimeStep = 0.1;
	m_Kp = 4.0;
	m_Ki = 0.1;
	m_Kd = 0.0;;
	m_InputType = 0;
	m_interval = 80;
	m_num = 0;
	m_Draw = false;
	m_isPause = false;
	m_timePoints.clear();
	m_InputValue.clear();
	m_hValues.clear();
	m_uValues.clear();
	m_QValues.clear();
	m_errorValues.clear();
}

CFinalHomeworkView::~CFinalHomeworkView()
{
}

BOOL CFinalHomeworkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFinalHomeworkView 绘图

void CFinalHomeworkView::OnDraw(CDC* pDC)
{
	CFinalHomeworkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if (!m_Draw)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	// 获取数据大小
	int data_size = m_InputValue.size();
	// 获取流量的最大、最小值
	auto min_Q = std::min_element(m_QValues.begin(), m_QValues.end());
	auto max_Q = std::max_element(m_QValues.begin(), m_QValues.end());
	double min_QValue = *min_Q;
	double max_QValue = *max_Q;

	CRect rect;  GetClientRect(&rect);

	// 流量（左上角）
	pDC->SetMapMode(MM_LOMETRIC);					// 设置映射模式
	CPoint ptLeft = CPoint(rect.left, rect.top);
	CPoint ptRight = CPoint(rect.right / 2, rect.bottom / 2);
	pDC->DPtoLP(&ptLeft); // 设备坐标转换为逻辑坐标
	pDC->DPtoLP(&ptRight);

	// x和y方向数据和绘图位置的比例关系，留出两侧空白
	double dXInter = (ptRight.x - ptLeft.x - m_interval * 2) / data_size;
	double dYInter = (ptLeft.y - ptRight.y - m_interval * 2) / (max_QValue - min_QValue);

	pDC->SetWindowOrg(-m_interval - ptLeft.x,
		(int)(-ptRight.y - m_interval)); // 重新设定坐标原点

	CPen NewPen;
	CPen* pOldPen;
	NewPen.CreatePen(PS_SOLID, 4, RGB(0, 0, 0));
	pOldPen = pDC->SelectObject(&NewPen);

	//	绘制横纵轴
	pDC->MoveTo(0, 0);
	pDC->LineTo(dXInter*data_size + 30, 0);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, dYInter * (max_QValue - min_QValue));

	//设置文本颜色和背景色
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetTextAlign(TA_CENTER | TA_TOP);

	// 绘制x坐标刻度
	for (int i = data_size / 5; i < data_size + data_size / 5; i = i + data_size/5) {
		if (i >= data_size) {// x轴末端绘制箭头
			pDC->MoveTo(data_size * dXInter, 10);
			CString str1;
			str1.Format(_T("%.1f"), (double(i) / double(data_size) * m_TimeLength));
			pDC->TextOut(data_size * dXInter, -20, str1);
			pDC->LineTo(data_size * dXInter, 0);

			pDC->MoveTo(data_size * dXInter + 50, 0);
			pDC->LineTo(data_size * dXInter + 40, 10);
			pDC->MoveTo(data_size * dXInter + 50, 0);
			pDC->LineTo(data_size * dXInter + 40, -10);
			CString str;
			str.Format(_T("时间（s）")); 
			pDC->TextOut(data_size * dXInter + 80, -40, str);
		}
		else {
			pDC->MoveTo(i * dXInter, 10);
			CString str1;
			str1.Format(_T("%.1f"), ceil(double(i) / double(data_size) * m_TimeLength));
			pDC->TextOut(i * dXInter, -20, str1);
			pDC->LineTo(i * dXInter, 0);

		}
	}

	// 绘制y坐标刻度
	for (int i = floor(min_QValue); i <= max_QValue + (max_QValue - min_QValue) / 5; i = i + (max_QValue-min_QValue)/5) {
		if (i >= int(max_QValue)) {// y轴末端绘制箭头
			pDC->MoveTo(0, dYInter * (max_QValue - min_QValue));
			pDC->LineTo(0, (max_QValue - floor(min_QValue)) * dYInter);

			pDC->MoveTo(0, (max_QValue - floor(min_QValue)) * dYInter );
			pDC->LineTo(-10, (max_QValue - floor(min_QValue)) * dYInter - 10);
			pDC->MoveTo(0, (max_QValue - floor(min_QValue)) * dYInter);
			pDC->LineTo(10, (max_QValue - floor(min_QValue)) * dYInter - 10);
			CString str;
			str.Format(_T("进水流量（m^3/s）"));
			pDC->TextOut(60, (max_QValue - floor(min_QValue)) * dYInter + 50, str);
		}
		else {
			pDC->MoveTo(0, (i- floor(min_QValue)) * dYInter);
			pDC->LineTo(10, (i - floor(min_QValue)) * dYInter);
			CString str1;
			str1.Format(_T("%d"), (i));
			pDC->TextOut(-20, (i - floor(min_QValue)) * dYInter, str1);
		}
	}
	
	// 恢复坐标系
	pDC->SetMapMode(MM_TEXT);
	pDC->SetWindowOrg(0, 0);


	// 误差（右上角）
	pDC->SetMapMode(MM_LOMETRIC);
	ptLeft = CPoint(rect.right / 2, rect.top);
	ptRight = CPoint(rect.right, rect.bottom / 2);
	pDC->DPtoLP(&ptLeft); // 设备坐标转换为逻辑坐标
	pDC->DPtoLP(&ptRight);
	pDC->SetWindowOrg(-m_interval - ptLeft.x,
		(int)(-ptRight.y - m_interval)); // 重新设定坐标原点

	// 误差的最值
	auto min_error = std::min_element(m_errorValues.begin(), m_errorValues.end());
	auto max_error = std::max_element(m_errorValues.begin(), m_errorValues.end());
	double min_errorValue = *min_error;
	double max_errorValue = *max_error;

	// x和y方向数据和绘图位置的比例关系，留出两侧空白
	dXInter = (ptRight.x - ptLeft.x - m_interval * 2) / data_size;
	dYInter = (ptLeft.y - ptRight.y - m_interval * 2) / (max_errorValue - min_errorValue);

	//	绘制横纵轴
	pDC->MoveTo(0, 0);
	pDC->LineTo(dXInter * data_size + 30, 0);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, dYInter * (max_errorValue - min_errorValue));
	
	//设置文本颜色和背景色
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetTextAlign(TA_CENTER | TA_TOP);

	// 绘制x坐标刻度
	for (int i = data_size / 5; i < data_size + data_size / 5; i = i + data_size / 5) {
		if (i >= data_size) {// x轴末端绘制箭头
			pDC->MoveTo(data_size * dXInter, 10);
			CString str1;
			str1.Format(_T("%.1f"), (double(i) / double(data_size) * m_TimeLength));
			pDC->TextOut(data_size * dXInter, -20, str1);
			pDC->LineTo(data_size * dXInter, 0);

			pDC->MoveTo(data_size * dXInter + 50, 0);
			pDC->LineTo(data_size * dXInter + 40, 10);
			pDC->MoveTo(data_size * dXInter + 50, 0);
			pDC->LineTo(data_size * dXInter + 40, -10);
			CString str;
			str.Format(_T("时间（s）"));
			pDC->TextOut(data_size * dXInter + 80, -40, str);
		}
		else {
			pDC->MoveTo(i * dXInter, 10);
			CString str1;
			str1.Format(_T("%.1f"), ceil(double(i) / double(data_size) * m_TimeLength));
			pDC->TextOut(i * dXInter, -20, str1);
			pDC->LineTo(i * dXInter, 0);

		}
	}

	// 绘制y坐标刻度
	for (double i = min_errorValue; i <= max_errorValue + (max_errorValue - min_errorValue) / 5; i = i + ceil((max_errorValue - min_errorValue) / 5)) {
		if (i >= int(max_errorValue)) {// y轴末端绘制箭头
			pDC->MoveTo(0, dYInter* (max_errorValue - min_errorValue));
			pDC->LineTo(0, (floor(max_errorValue) - floor(min_errorValue))* dYInter);

			pDC->MoveTo(0, (floor(max_errorValue) - floor(min_errorValue)) * dYInter);
			pDC->LineTo(-10, (floor(max_errorValue) - floor(min_errorValue))* dYInter - 10);
			pDC->MoveTo(0, (floor(max_errorValue) - floor(min_errorValue))* dYInter );
			pDC->LineTo(10, (floor(max_errorValue) - floor(min_errorValue))* dYInter - 10);
			CString str;
			str.Format(_T("误差（m）"));
			pDC->TextOut(-70, (floor(max_errorValue) - floor(min_errorValue)) * dYInter, str);
		}
		else {
			pDC->MoveTo(0, (floor(i) - floor(min_errorValue)) * dYInter);
			pDC->LineTo(10, (floor(i) - floor(min_errorValue)) * dYInter);
			CString str1;
			int a = floor(i);
			int b = floor(i) - floor(min_errorValue);
			str1.Format(_T("%.2f"), (floor(i)));
			pDC->TextOut(-40, (floor(i) - floor(min_errorValue)) * dYInter, str1);
		}
	}

	// 恢复坐标系
	pDC->SetMapMode(MM_TEXT);
	pDC->SetWindowOrg(0, 0);

	// 液位（左下角）
	pDC->SetMapMode(MM_LOMETRIC);
	ptLeft = CPoint(rect.left, rect.bottom / 2);
	ptRight = CPoint(rect.right / 2, rect.bottom);
	pDC->DPtoLP(&ptLeft); // 设备坐标转换为逻辑坐标
	pDC->DPtoLP(&ptRight);
	pDC->SetWindowOrg(-m_interval - ptLeft.x,
		(int)(-ptRight.y - m_interval)); // 重新设定坐标原点

	// 误差的最值
	auto min_h = std::min_element(m_hValues.begin(), m_hValues.end());
	auto max_h = std::max_element(m_hValues.begin(), m_hValues.end());
	double min_hValue = *min_h;
	double max_hValue = *max_h;

	auto min_i = std::min_element(m_InputValue.begin(), m_InputValue.end());
	auto max_i = std::max_element(m_InputValue.begin(), m_InputValue.end());
	double min_iValue = *min_i;
	double max_iValue = *max_i;

	if (max_iValue > max_hValue)
		max_hValue = max_iValue;

	// x和y方向数据和绘图位置的比例关系，留出两侧空白
	dXInter = (ptRight.x - ptLeft.x - m_interval * 2) / data_size;
	dYInter = (ptLeft.y - ptRight.y - m_interval * 2) / (max_hValue - 0);

	//	绘制横纵轴
	pDC->MoveTo(0, 0);
	pDC->LineTo(dXInter* data_size + 30, 0);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, dYInter* (max_hValue - 0));

	//设置文本颜色和背景色
	pDC->SetTextColor(RGB(0, 0, 0));
	pDC->SetTextAlign(TA_CENTER | TA_TOP);

	// 绘制x坐标刻度
	for (int i = data_size / 5; i < data_size + data_size / 5; i = i + data_size / 5) {
		if (i >= data_size) {// x轴末端绘制箭头
			pDC->MoveTo(data_size * dXInter, 10);
			CString str1;
			str1.Format(_T("%.1f"), (double(i) / double(data_size) * m_TimeLength));
			pDC->TextOut(data_size * dXInter, -20, str1);
			pDC->LineTo(data_size * dXInter, 0);

			pDC->MoveTo(data_size * dXInter + 50, 0);
			pDC->LineTo(data_size * dXInter + 40, 10);
			pDC->MoveTo(data_size * dXInter + 50, 0);
			pDC->LineTo(data_size * dXInter + 40, -10);
			CString str;
			str.Format(_T("时间（s）"));
			pDC->TextOut(data_size * dXInter + 80, -40, str);
		}
		else {
			pDC->MoveTo(i * dXInter, 10);
			CString str1;
			str1.Format(_T("%.1f"), ceil(double(i) / double(data_size) * m_TimeLength));
			pDC->TextOut(i * dXInter, -20, str1);
			pDC->LineTo(i * dXInter, 0);

		}
	}

	// 绘制y坐标刻度
	for (int i = 0; i <= max_hValue; i = i + max_hValue / 4) {
		if (i == int(max_hValue)) {// y轴末端绘制箭头
			pDC->MoveTo(0, i * dYInter);
			pDC->LineTo(-10, i * dYInter - 10);
			pDC->MoveTo(0, i * dYInter);
			pDC->LineTo(10, i * dYInter - 10);
			CString str;
			str.Format(_T("液位高度（m）"));
			pDC->TextOut(60, i * dYInter + 60, str);
		}
		else {
			pDC->MoveTo(0, i * dYInter);
			pDC->LineTo(10, i * dYInter);
			CString str1;
			str1.Format(_T("%d"), (int(i)));
			pDC->TextOut(-20, i * dYInter, str1);
		}
	}

	// 恢复坐标系
	pDC->SetMapMode(MM_TEXT);
	pDC->SetWindowOrg(0, 0);

	// 水槽（右下角）
	pDC->SetMapMode(MM_LOMETRIC);
	ptLeft = CPoint(rect.right / 2, rect.bottom / 2);
	ptRight = CPoint(rect.right, rect.bottom);
	pDC->DPtoLP(&ptLeft); // 设备坐标转换为逻辑坐标
	pDC->DPtoLP(&ptRight);
	pDC->SetWindowOrg(-m_interval - ptLeft.x,
		(int)(-ptRight.y - m_interval)); // 重新设定坐标原点

	// 绘制水桶
	pDC->MoveTo(0, 0);
	pDC->LineTo(600, 0);
	pDC->MoveTo(0, 0);
	pDC->LineTo(0, dYInter * (max_hValue - 0) + 50);
	pDC->MoveTo(600, 0);
	pDC->LineTo(600, dYInter * (max_hValue - 0) + 50);
}


// CFinalHomeworkView 打印

BOOL CFinalHomeworkView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFinalHomeworkView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFinalHomeworkView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CFinalHomeworkView 诊断

#ifdef _DEBUG
void CFinalHomeworkView::AssertValid() const
{
	CView::AssertValid();
}

void CFinalHomeworkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFinalHomeworkDoc* CFinalHomeworkView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFinalHomeworkDoc)));
	return (CFinalHomeworkDoc*)m_pDocument;
}
#endif //_DEBUG

void CFinalHomeworkView::RungeKutta4()
{
	m_hValues.clear();
	m_uValues.clear();
	m_QValues.clear();
	m_errorValues.clear();

	double h = m_h0;
	double I = 0.0; // 积分项初值
	double previousError = 0.0;

	for (size_t i = 0; i < m_timePoints.size(); ++i) {
		double t = m_timePoints[i];
		double r = m_InputValue[i];
		double e = r - h;
		I += e * m_TimeStep;
		double D = (e - previousError) / m_TimeStep;
		double u = m_Kp * e + m_Ki * I + m_Kd * D;
		previousError = e;
		double q = m_Ku * u;

		double k1 = (m_Ku * u - m_A0 * sqrt(2 * 9.81 * h)) / m_A;
		double k2 = (m_Ku * u - m_A0 * sqrt(2 * 9.81 * (h + 0.5 * m_TimeStep * k1))) / m_A;
		double k3 = (m_Ku * u - m_A0 * sqrt(2 * 9.81 * (h + 0.5 * m_TimeStep * k2))) / m_A;
		double k4 = (m_Ku * u - m_A0 * sqrt(2 * 9.81 * (h + m_TimeStep * k3))) / m_A;

		h += (m_TimeStep / 6.0) * (k1 + 2 * k2 + 2 * k3 + k4);

		m_hValues.push_back(h);
		m_uValues.push_back(u);
		m_QValues.push_back(q);
		m_errorValues.push_back(e);
	}
}


// CFinalHomeworkView 消息处理程序


void CFinalHomeworkView::OnOptDlg()
{
	// TODO: 在此添加命令处理程序代码
	CMyDlg Dlg(m_A, m_h0, m_A0, m_Ku, m_Max, m_T, m_TimeLength, m_TimeStep, m_Kp, m_Ki, m_Kd, m_InputType, nullptr);

	if (Dlg.DoModal() == IDOK) {
		m_hValues.clear();
		m_uValues.clear();
		m_QValues.clear();
		m_errorValues.clear();
		m_timePoints.clear();
		m_InputValue.clear();

		m_A = Dlg.m_A;
		m_h0 = Dlg.m_h0;
		m_A0 = Dlg.m_A0;
		m_Ku = Dlg.m_Ku;
		m_Max = Dlg.m_Max;
		m_T = Dlg.m_T;
		m_TimeLength = Dlg.m_TimeLength;
		m_TimeStep = Dlg.m_TimeStep;
		m_Kp = Dlg.m_Kp;
		m_Ki = Dlg.m_Ki;
		m_Kd = Dlg.m_Kd;
		m_InputType = Dlg.m_InputType;

		if (m_InputType == 0) {
			if (m_Max < 0 || m_Max>10) {
				AfxMessageBox(_T("数值超出范围"), MB_ICONERROR | MB_OK);
				return;
			}
			for (double t = 0.0; t < m_TimeLength - 1e-10; t += m_TimeStep)
			{
				m_timePoints.push_back(t);
				m_InputValue.push_back(m_Max);
			}
		}
		else {
			if (m_Max < 0 || m_Max>5 || m_T < 5 || m_T > 10) {
				AfxMessageBox(_T("数值超出范围"), MB_ICONERROR | MB_OK);
				return;
			}
			for (double t = 0.0; t < m_TimeLength - 1e-10; t += m_TimeStep)
			{
				double h = m_Max * sin(2 * 3.14159265 / m_TimeLength * t) + m_h0;
				m_timePoints.push_back(t);
				m_InputValue.push_back(h);
			}
		}
		RungeKutta4();
		m_Draw = true;
		Invalidate();
	}
}


void CFinalHomeworkView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1) {
		// 数据量
		int data_size = m_InputValue.size();
		// 获取流量的最大、最小值
		auto min_Q = std::min_element(m_QValues.begin(), m_QValues.end());
		auto max_Q = std::max_element(m_QValues.begin(), m_QValues.end());
		double min_QValue = *min_Q;
		double max_QValue = *max_Q;

		CRect rect;  GetClientRect(&rect);
		CClientDC dc(this);

		// 流量（左上角）
		dc.SetMapMode(MM_LOMETRIC);					// 设置映射模式
		CPoint ptLeft = CPoint(rect.left, rect.top);
		CPoint ptRight = CPoint(rect.right / 2, rect.bottom / 2);
		dc.DPtoLP(&ptLeft); // 设备坐标转换为逻辑坐标
		dc.DPtoLP(&ptRight);

		// x和y方向数据和绘图位置的比例关系，留出两侧空白
		double dXInter = (ptRight.x - ptLeft.x - m_interval * 2) / data_size;
		double dYInter = (ptLeft.y - ptRight.y - m_interval * 2) / (max_QValue - min_QValue);

		dc.SetWindowOrg(-m_interval - ptLeft.x,
			(int)(-ptRight.y - m_interval)); // 重新设定坐标原点

		CPen m_brownPen;
		m_brownPen.CreatePen(PS_SOLID, 3, RGB(210, 160, 90));
		dc.SelectObject(m_brownPen);

		int x, y;
		x = (int)(m_num * dXInter);
		y = (int)((m_QValues[m_num] - floor(min_QValue)) * dYInter);
		dc.MoveTo(x, y);
		x = (int)((m_num + 1) * dXInter);
		y = (int)((m_QValues[m_num + 1] - floor(min_QValue)) * dYInter);
		dc.LineTo(x, y);

		// 恢复坐标系
		dc.SetMapMode(MM_TEXT);
		dc.SetWindowOrg(0, 0);


		// 误差（右上角）
		dc.SetMapMode(MM_LOMETRIC);
		ptLeft = CPoint(rect.right / 2, rect.top);
		ptRight = CPoint(rect.right, rect.bottom / 2);
		dc.DPtoLP(&ptLeft); // 设备坐标转换为逻辑坐标
		dc.DPtoLP(&ptRight);
		dc.SetWindowOrg(-m_interval - ptLeft.x,
			(int)(-ptRight.y - m_interval)); // 重新设定坐标原点

		// 误差的最值
		auto min_error = std::min_element(m_errorValues.begin(), m_errorValues.end());
		auto max_error = std::max_element(m_errorValues.begin(), m_errorValues.end());
		double min_errorValue = *min_error;
		double max_errorValue = *max_error;

		// x和y方向数据和绘图位置的比例关系，留出两侧空白
		dXInter = (ptRight.x - ptLeft.x - m_interval * 2) / data_size;
		dYInter = (ptLeft.y - ptRight.y - m_interval * 2) / (max_errorValue - min_errorValue);

		//绘制误差图像
		CPen m_bluePen;
		m_bluePen.CreatePen(PS_SOLID, 3, RGB(0, 0, 255));
		dc.SelectObject(m_bluePen);

		x = (int)(m_num * dXInter);
		y = (int)((m_errorValues[m_num] - floor(min_errorValue)) * dYInter);
		dc.MoveTo(x, y);
		x = (int)((m_num + 1) * dXInter);
		y = (int)((m_errorValues[m_num + 1] - floor(min_errorValue)) * dYInter);
		dc.LineTo(x, y);

		CString str2;
		str2.Format(_T("当前误差：%.2f m"), m_errorValues[m_num]);
		dc.TextOut(data_size * dXInter - 100, 300, str2);

		// 恢复坐标系
		dc.SetMapMode(MM_TEXT);
		dc.SetWindowOrg(0, 0);

		// 液位（左下角）
		dc.SetMapMode(MM_LOMETRIC);
		ptLeft = CPoint(rect.left, rect.bottom / 2);
		ptRight = CPoint(rect.right / 2, rect.bottom);
		dc.DPtoLP(&ptLeft); // 设备坐标转换为逻辑坐标
		dc.DPtoLP(&ptRight);
		dc.SetWindowOrg(-m_interval - ptLeft.x,
			(int)(-ptRight.y - m_interval)); // 重新设定坐标原点

		// 误差的最值
		auto min_h = std::min_element(m_hValues.begin(), m_hValues.end());
		auto max_h = std::max_element(m_hValues.begin(), m_hValues.end());
		double min_hValue = *min_h;
		double max_hValue = *max_h;

		auto min_i = std::min_element(m_InputValue.begin(), m_InputValue.end());
		auto max_i = std::max_element(m_InputValue.begin(), m_InputValue.end());
		double min_iValue = *min_i;
		double max_iValue = *max_i;

		if (max_iValue > max_hValue)
			max_hValue = max_iValue;

		// x和y方向数据和绘图位置的比例关系，留出两侧空白
		dXInter = (ptRight.x - ptLeft.x - m_interval * 2) / data_size;
		dYInter = (ptLeft.y - ptRight.y - m_interval * 2) / (max_hValue - 0);

		//绘制液位图像
		CPen m_redPen;
		m_redPen.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
		dc.SelectObject(m_redPen);

		x = (int)(m_num * dXInter);
		y = (int)(m_hValues[m_num] * dYInter);
		dc.MoveTo(x, y);
		x = (int)((m_num + 1) * dXInter);
		y = (int)(m_hValues[m_num + 1] * dYInter);
		dc.LineTo(x, y);

		// 绘制期望液位图像
		CPen m_greenPen;
		m_greenPen.CreatePen(PS_SOLID, 3, RGB(0, 255, 0));
		dc.SelectObject(m_greenPen);

		x = (int)(m_num * dXInter) + 10;
		y = (int)(m_InputValue[m_num] * dYInter);
		dc.MoveTo(x, y);
		x = (int)((m_num + 1) * dXInter) + 10;
		y = (int)(m_InputValue[m_num + 1] * dYInter);
		dc.LineTo(x, y);

		// 恢复坐标系
		dc.SetMapMode(MM_TEXT);
		dc.SetWindowOrg(0, 0);

		// 绘制水槽
		dc.SetMapMode(MM_LOMETRIC);
		ptLeft = CPoint(rect.right / 2, rect.bottom / 2);
		ptRight = CPoint(rect.right, rect.bottom);
		dc.DPtoLP(&ptLeft); // 设备坐标转换为逻辑坐标
		dc.DPtoLP(&ptRight);
		dc.SetWindowOrg(-m_interval - ptLeft.x,
			(int)(-ptRight.y - m_interval)); // 重新设定坐标原点

		// 水槽区域
		CRect m_bucketRect = CRect(rect.right / 2, rect.bottom / 2, rect.right, rect.bottom); // 右下角区域
		CRect m_waterRect = CRect(0, dYInter * (m_hValues[m_num + 1] - 0), 600, 0);	// 水的区域
		//InvalidateRect(&m_bucketRect); // 重绘
		RedrawWindow(&m_bucketRect);

		// 水
		dc.FillSolidRect(m_waterRect, RGB(0, 0, 255));

		// 期望液位
		dc.MoveTo(0, dYInter * (m_InputValue[m_num + 1] - 0));
		dc.LineTo(600, dYInter * (m_InputValue[m_num + 1] - 0));

		m_num++;
		if (m_num == data_size - 1)  KillTimer(1);
	}


	CView::OnTimer(nIDEvent);
	
}


void CFinalHomeworkView::OnStart()
{
	// TODO: 在此添加命令处理程序代码
	SetTimer(1, 1000 * m_TimeStep, NULL);
}


void CFinalHomeworkView::OnPause()
{
	// TODO: 在此添加命令处理程序代码
	if (m_isPause) {		// 现在暂定，开启定时器
		SetTimer(1, 1000*m_TimeStep, NULL);   m_isPause = false;
	}
	else {			// 正在运行，关闭定时器
		KillTimer(1);  m_isPause = true;
	}

}


void CFinalHomeworkView::OnStop()
{
	// TODO: 在此添加命令处理程序代码
	KillTimer(1);
	m_num = 0;
	Invalidate();
}


void CFinalHomeworkView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	m_hValues.clear();
	m_uValues.clear();
	m_QValues.clear();
	m_errorValues.clear();
	m_timePoints.clear();
	m_InputValue.clear();
	m_num = 0;

	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, _T("All Files (*.*)|*.*||"));
	if (fileDlg.DoModal() == IDOK)
	{
		CString filePath = fileDlg.GetPathName();
		CFile file;
		if (file.Open(filePath, CFile::modeRead))
		{
			CArchive ar(&file, CArchive::load);
			CFinalHomeworkDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->Serialize(ar);
			pDoc->Get(m_A, m_h0, m_A0, m_Ku, m_Max, m_T, m_TimeLength, m_TimeStep, m_Kp, m_Ki, m_Kd, m_InputType);
			ar.Close();
			file.Close();
			Invalidate();
		}
		else
		{
			AfxMessageBox(_T("无法打开文件"), MB_ICONERROR | MB_OK);
		}
	}

	if (m_InputType == 0) {
		if (m_Max < 0 || m_Max>10) {
			AfxMessageBox(_T("数值超出范围"), MB_ICONERROR | MB_OK);
			return;
		}
		for (double t = 0.0; t < m_TimeLength - 1e-10; t += m_TimeStep)
		{
			m_timePoints.push_back(t);
			m_InputValue.push_back(m_Max);
		}
	}
	else {
		if (m_Max < 0 || m_Max>5 || m_T < 5 || m_T > 10) {
			AfxMessageBox(_T("数值超出范围"), MB_ICONERROR | MB_OK);
			return;
		}
		for (double t = 0.0; t < m_TimeLength - 1e-10; t += m_TimeStep)
		{
			double h = m_Max * sin(2 * 3.14159265 / m_TimeLength * t) + m_h0;
			m_timePoints.push_back(t);
			m_InputValue.push_back(h);
		}
	}
	RungeKutta4();
	m_Draw = true;
	Invalidate();
}


void CFinalHomeworkView::OnFileSave()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog fileDlg(FALSE, NULL, NULL, OFN_OVERWRITEPROMPT, _T("All Files (*.*)|*.*||"));
	if (fileDlg.DoModal() == IDOK)
	{
		CString filePath = fileDlg.GetPathName();
		CFile file;
		if (file.Open(filePath, CFile::modeCreate | CFile::modeWrite))
		{
			CArchive ar(&file, CArchive::store);
			CFinalHomeworkDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			pDoc->Set(m_A, m_h0, m_A0, m_Ku, m_Max, m_T, m_TimeLength, m_TimeStep, m_Kp, m_Ki, m_Kd, m_InputType);
			pDoc->Serialize(ar);
			ar.Close();
			file.Close();
		}
		else
		{
			AfxMessageBox(_T("无法保存文件"), MB_ICONERROR | MB_OK);
		}
	}
}
