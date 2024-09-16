
// Homework2Doc.h: CHomework2Doc 类的接口
//
#include<vector>

#pragma once


class CHomework2Doc : public CDocument
{
private:
	int m_nColorIndex;	// 颜色
	std::vector<CString> m_str;	// 字符串
	std::vector<CPoint> m_point; //鼠标点击位置
protected: // 仅从序列化创建
	CHomework2Doc() noexcept;
	DECLARE_DYNCREATE(CHomework2Doc)

// 特性
public:
	void Set(CPoint point, CString string);
	void Get(CPoint &point, CString &string, int index);
	void SetColor(int index);
	int GetColor();
	//afx_msg void OnOptColorChange(UINT nID);

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 实现
public:
	virtual ~CHomework2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnBlack();
	afx_msg void OnRed();
	afx_msg void OnBlue();
	afx_msg void OnGreen();
//	afx_msg void OnFileSave();
//	afx_msg void OnFileOpen();
};
