
// MFCVView.h: CMFCVView 类的接口
//

#pragma once
#include "dcMap.h"

class CMFCVView : public CScrollView
{
protected: // 仅从序列化创建
	CMFCVView() noexcept;
	DECLARE_DYNCREATE(CMFCVView)

// 特性
public:
	CMFCVDoc* pDoc;
	dcMap dMap;


	CMFCVDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCVView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:

	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // MFCVView.cpp 中的调试版本
inline CMFCVDoc* CMFCVView::GetDocument() const
   { return reinterpret_cast<CMFCVDoc*>(m_pDocument); }
#endif

