
// Final_HomeworkDoc.h: CFinalHomeworkDoc 类的接口
//


#pragma once


class CFinalHomeworkDoc : public CDocument
{
private:
	double m_A;
	double m_h0;
	double m_A0;
	double m_Ku;
	double m_Max;
	double m_T;
	double m_TimeLength;
	double m_TimeStep;
	double m_Kp;
	double m_Ki;
	double m_Kd;
	int m_InputType;

protected: // 仅从序列化创建
	CFinalHomeworkDoc() noexcept;
	DECLARE_DYNCREATE(CFinalHomeworkDoc)

// 特性
public:

// 操作
public:
	void Set(double A, double h0, double A0, double Ku, double Max, double T, double TimeLength, double TimeStep, double Kp, double Ki, double Kd, int InputType);
	void Get(double& A, double& h0, double& A0, double& Ku, double& Max, double& T, double& TimeLength, double& TimeStep, double& Kp, double& Ki, double& Kd, int& InputType);

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
	virtual ~CFinalHomeworkDoc();
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
};
