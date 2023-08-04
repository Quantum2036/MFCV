
// MFCVDoc.cpp: CMFCVDoc 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCV.h"
#endif

#include "MFCVDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

LPCTSTR szFilter = L"Windows位图(*.BMP, *.dib)			|*.BMP;*.dib|		\
					JPEG 文件(*.jpeg, *.jpg, *.jpe)		|*.jpeg;*.jpg;*.jpe|\
					JPEG 2000 文件(*.jp2)				|*.jp2|				\
					便携式网络图形(*.png)					|*.png|				\
					WebP(*.webp)						|*.webp|			\
					便携式图像格式(*.pbm, *.pgm, *.ppm *.pxm, *.pnm)		\
														|*.pbm;*.pgm;*.ppm;*.pxm;*.pnm|	\
					PFM 文件(*.pfm)						|*.pfm|				\
					Sun rasters(*.sr, *.ras)			|*.sr;*.ras|		\
					TIFF 文件(*.tiff, *.tif)				|*.tiff;*.tif|		\
					OpenEXR Image 文件(*.exr)			|*.exr|				\
					Radiance HDR(*.hdr, *.pic)			|*.hdr;*.pic||";

// CMFCVDoc

IMPLEMENT_DYNCREATE(CMFCVDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCVDoc, CDocument)
	ON_COMMAND(ID_FILE_OPEN, &CMFCVDoc::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMFCVDoc::OnFileSaveAs)
END_MESSAGE_MAP()


// CMFCVDoc 构造/析构

CMFCVDoc::CMFCVDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	isImgRead = FALSE;
	imageFileName.Empty();
}

CMFCVDoc::~CMFCVDoc()
{
	image.~Mat();
	isImgRead = FALSE;
	imageFileName.Empty();
}

BOOL CMFCVDoc::Draw(CDC* pDC, int nX, int nY)
{
	//没有打开图像
	if (isImgRead == NULL) 
		return FALSE;

	int nWidth = image.cols;
	int nHeight = image.rows;

	//char* pTemp;
	////锁定图像对象句柄
	//pTemp = (char*) ::GlobalLock(m_hDib);
	//if (pTemp == NULL) return(NULL);

	////图像信息头
	//BITMAPINFOHEADER* pBIH;
	//pBIH = (BITMAPINFOHEADER*)&pTemp[sizeof(BITMAPFILEHEADER)];

	//int nRet = ::StretchDIBits(pDC->m_hDC, nX, nY, nWidth, nHeight, 0, 0,
	//							nWidth, nHeight,
	//							(const void FAR*) & pTemp[sizeof(BITMAPFILEHEADER) +
	//							sizeof(BITMAPINFOHEADER) +
	//							m_nPaletteBytes],
	//							(BITMAPINFO*)pBIH, DIB_RGB_COLORS, SRCCOPY);

	//::GlobalUnlock(m_hDib);
	//if (nRet != nHeight)
	//	return FALSE ;

	return TRUE;
}

BOOL CMFCVDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

// CMFCVDoc 方法



// CMFCVDoc 序列化

void CMFCVDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CMFCVDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void CMFCVDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CMFCVDoc::SetSearchContent(const CString& value)
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

// CMFCVDoc 诊断

#ifdef _DEBUG
void CMFCVDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCVDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCVDoc 命令


void CMFCVDoc::OnFileOpen()
{
	debug.UpdateLog("执行 CMFCVDoc::OnFileOpen");

	static DWORD nIndex = 1;
	CFileDialog FileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	FileDlg.m_ofn.nFilterIndex = (DWORD)nIndex;

	if (FileDlg.DoModal() == IDOK)
	{
		CString strPathName = FileDlg.GetPathName();

		AfxGetApp()->OpenDocumentFile(strPathName);
		nIndex = FileDlg.m_ofn.nFilterIndex;

		if (LoadImgToDoc()) {
			debug.UpdateLog("OnFileOpen: 文件打开成功");
		}
		else {
			debug.UpdateLog("WARNING: 文件打开失败");
		}
	}
}


void CMFCVDoc::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	debug.UpdateLog("执行 CMFCVDoc::OnFileSaveAs");

	static DWORD nIndex = 1;
	CFileDialog DialogSaveAs(FALSE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	DialogSaveAs.m_ofn.nFilterIndex = nIndex;

	//if (DialogSaveAs.DoModal() == IDOK)
	//{
	//	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	//	CChildFrame* pChildFrame = (CChildFrame*)pMainFrame->MDIGetActive();
	//	CDipView* pDipView = (CDipView*)pChildFrame->GetActiveView();
	//	nIndex = (int)DialogSaveAs.m_ofn.nFilterIndex;
	//	if (nIndex == 5)
	//	{
	//		if (m_pDibObject->GetNumBits() != 24)
	//		{
	//			AfxMessageBox(TEXT("24位真彩色图像才能存为JPEG格式！"));
	//			return;
	//		}
	//	}
	//	if (m_pDibObject != NULL)
	//	{
	//		CString strPathName = DialogSaveAs.GetPathName();
	//		int nFindIndex = strPathName.Find(TEXT("."));

	//		if (nFindIndex != -1)
	//			strPathName = strPathName.Left(nFindIndex);

	//		strPathName += CDibObject::szExtensions[nIndex - 1];
	//		m_pDibObject->Save(CStrtoChar(strPathName));
	//		CString strFileName = DialogSaveAs.GetFileName();
	//		nFindIndex = strFileName.Find(TEXT("."));

	//		if (nFindIndex != -1)
	//			strFileName = strFileName.Left(nFindIndex);

	//		strFileName += CDibObject::szExtensions[nIndex - 1];
	//		pChildFrame->SetWindowText(strFileName);
	//		SetPathName(strPathName);

	//		if (nIndex == 5)
	//		{
	//			m_pDibObject->Load(CStrtoChar(strPathName));
	//			pDipView->InvalidateRect(NULL, FALSE);
	//			pDipView->UpdateWindow();
	//			debug.UpdateLog("文件另存成功");
	//		}
	//	}
	//}
}


// 读入图像文件
BOOL CMFCVDoc::LoadImgToDoc()
{
	debug.UpdateLog("执行 CMFCVDoc::LoadImgToDoc");

	CString strPathName = GetPathName();

	BeginWaitCursor(); 	     //设置等待光标
	image = LoadImg(strPathName);
	EndWaitCursor();     //取消等待光标

	if ( image.empty() ) {
		debug.UpdateLog("WARNING: 图像加载失败");
		this->~CMFCVDoc();
	}
	else {
		debug.UpdateLog("LoadImgToDoc: 图像加载成功");
		isImgRead = TRUE;
		imageFileName = strPathName;
	}

	return isImgRead;
}

BOOL CMFCVDoc::LoadImgToDoc(CString strFilePath)
{
	debug.UpdateLog("执行 CMFCVDoc::LoadImgToDoc");

	//若文档类中image已有数据，则清除原有数据
	if (isImgRead) {
		this->~CMFCVDoc();
	}

	BeginWaitCursor(); 	     //设置等待光标
	image = LoadImg(strFilePath);
	EndWaitCursor();     //取消等待光标

	if (image.empty()) {
		debug.UpdateLog("WARNING: 图像加载失败");
		this->~CMFCVDoc();
	}
	else {
		debug.UpdateLog("LoadImgToDoc: 图像加载成功");
		isImgRead = TRUE;
		imageFileName = strFilePath;
	}

	return isImgRead;
}
