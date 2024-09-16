
// Final_HomeworkDoc.cpp: CFinalHomeworkDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Final_Homework.h"
#endif

#include "Final_HomeworkDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFinalHomeworkDoc

IMPLEMENT_DYNCREATE(CFinalHomeworkDoc, CDocument)

BEGIN_MESSAGE_MAP(CFinalHomeworkDoc, CDocument)
END_MESSAGE_MAP()


// CFinalHomeworkDoc 构造/析构

CFinalHomeworkDoc::CFinalHomeworkDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
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
	m_Kd = 0.0;
	m_InputType = 0;
}

CFinalHomeworkDoc::~CFinalHomeworkDoc()
{
}

BOOL CFinalHomeworkDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CFinalHomeworkDoc 序列化

void CFinalHomeworkDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
		ar << m_A << m_h0 << m_A0 << m_Ku << m_Max << m_T << m_TimeLength << m_TimeStep << m_Kp << m_Ki << m_Kd << m_InputType;
	}
	else
	{
		// TODO: 在此添加加载代码
		ar >> m_A >> m_h0 >> m_A0 >> m_Ku >> m_Max >> m_T >> m_TimeLength >> m_TimeStep >> m_Kp >> m_Ki >> m_Kd >> m_InputType;
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CFinalHomeworkDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CFinalHomeworkDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CFinalHomeworkDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CFinalHomeworkDoc 诊断

#ifdef _DEBUG
void CFinalHomeworkDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CFinalHomeworkDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CFinalHomeworkDoc 命令
void CFinalHomeworkDoc::Set(double A, double h0, double A0, double Ku, double Max, double T, double TimeLength, double TimeStep, double Kp, double Ki, double Kd, int InputType) 
{
	m_A = A;
	m_h0 = h0;
	m_A0 = A0;
	m_Ku = Ku;
	m_Max = Max;
	m_T = T;
	m_TimeLength = TimeLength;
	m_TimeStep = TimeStep;
	m_Kp = Kp;
	m_Ki = Ki;
	m_Kd = Kd;
	m_InputType = InputType;
}

void CFinalHomeworkDoc::Get(double& A, double& h0, double& A0, double& Ku, double& Max, double& T, double& TimeLength, double& TimeStep, double& Kp, double& Ki, double& Kd, int& InputType)
{
	A = m_A;
	h0 = m_h0;
	A0 = m_A0;
	Ku = m_Ku;
	Max = m_Max;
	T = m_T;
	TimeLength = m_TimeLength;
	TimeStep = m_TimeStep;
	Kp = m_Kp;
	Ki = m_Ki;
	Kd = m_Kd;
	InputType = m_InputType;
}