
// Homework2Doc.cpp: CHomework2Doc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Homework2.h"
#endif

#include "Homework2Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CHomework2Doc

IMPLEMENT_DYNCREATE(CHomework2Doc, CDocument)

BEGIN_MESSAGE_MAP(CHomework2Doc, CDocument)
	ON_COMMAND(ID_Black, &CHomework2Doc::OnBlack)
	ON_COMMAND(ID_Red, &CHomework2Doc::OnRed)
	ON_COMMAND(ID_Blue, &CHomework2Doc::OnBlue)
	ON_COMMAND(ID_Green, &CHomework2Doc::OnGreen)
	//ON_COMMAND_RANGE(ID_BLACK, ID_BLUE, OnOptColorChange)
//	ON_COMMAND(ID_FILE_SAVE, &CHomework2Doc::OnFileSave)
//	ON_COMMAND(ID_FILE_OPEN, &CHomework2Doc::OnFileOpen)
END_MESSAGE_MAP()


// CHomework2Doc 构造/析构

CHomework2Doc::CHomework2Doc() noexcept
{
	// TODO: 在此添加一次性构造代码
	m_nColorIndex = 0;	// 颜色
}

CHomework2Doc::~CHomework2Doc()
{
}

BOOL CHomework2Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

// CHomework2Doc 序列化

void CHomework2Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
		for (int i = 0; i < m_point.size(); i++)
		{
			ar << m_point[i] << m_str[i] << m_nColorIndex;
		}
	}
	else
	{
		// TODO: 在此添加加载代码
		for (int i = 0; i < m_point.size(); i++)
		{
			ar >> m_point[i] >> m_str[i] >> m_nColorIndex;
		}
		UpdateAllViews(NULL);
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CHomework2Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CHomework2Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CHomework2Doc::SetSearchContent(const CString& value)
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

// CHomework2Doc 诊断

#ifdef _DEBUG
void CHomework2Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHomework2Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHomework2Doc 命令
void CHomework2Doc::Set(CPoint point, CString string)
{
	m_point.push_back(point);
	m_str.push_back(string);
}

void CHomework2Doc::Get(CPoint &point, CString &string, int index)
{
	point = m_point[index];
	string = m_str[index];
}

void CHomework2Doc::SetColor(int index)
{
	m_nColorIndex = index;
}

int CHomework2Doc::GetColor()
{
	return m_nColorIndex;
}

void CHomework2Doc::OnBlack()
{
	// TODO: 在此添加命令处理程序代码
	m_nColorIndex = 0;
	UpdateAllViews(NULL);
}


void CHomework2Doc::OnRed()
{
	// TODO: 在此添加命令处理程序代码
	m_nColorIndex = 1;
	UpdateAllViews(NULL);
}

void CHomework2Doc::OnGreen()
{
	// TODO: 在此添加命令处理程序代码
	m_nColorIndex = 2;
	UpdateAllViews(NULL);
}

void CHomework2Doc::OnBlue()
{
	// TODO: 在此添加命令处理程序代码
	m_nColorIndex = 3;
	UpdateAllViews(NULL);
}

//void CHomework2Doc::OnOptColorChange(UINT nID)
//{
//	m_nColorIndex = nID - ID_BLACK;
//	UpdateAllViews(NULL);
//}



//void CHomework2Doc::OnFileSave()
//{
	// TODO: 在此添加命令处理程序代码
//}


//void CHomework2Doc::OnFileOpen()
//{
	// TODO: 在此添加命令处理程序代码
//}
