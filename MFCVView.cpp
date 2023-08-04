
// MFCVView.cpp: CMFCVView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCV.h"
#endif

#include "MFCVDoc.h"
#include "MFCVView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCVView

IMPLEMENT_DYNCREATE(CMFCVView, CScrollView)

BEGIN_MESSAGE_MAP(CMFCVView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMFCVView 构造/析构

CMFCVView::CMFCVView() noexcept
{
	pDoc = NULL;
}

CMFCVView::~CMFCVView()
{
}

BOOL CMFCVView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CMFCVView 绘图

void CMFCVView::OnDraw(CDC* pDC)
{
	debug.UpdateLog("执行 CMFCVView::OnDraw");
	
	
	dMap.Draw(pDC);

}

void CMFCVView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc) {
		debug.UpdateLog("WARNING: CMFCVView::GetDocument() == NULL");
		return;
	}
	if (!pDoc->isImgRead) {
		if (!pDoc->LoadImgToDoc()) {
			debug.UpdateLog("未读取到图像");
			return;
		}
	}
	
	dMap = dcMap::dcMap(pDoc->image);


	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
	
}


// CMFCVView 打印

BOOL CMFCVView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCVView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCVView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCVView 诊断

#ifdef _DEBUG
void CMFCVView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMFCVView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMFCVDoc* CMFCVView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCVDoc)));
	return (CMFCVDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCVView 消息处理程序


void CMFCVView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	return;
}
