
// MFCVDoc.h: CMFCVDoc 类的接口
//


#pragma once


class CMFCVDoc : public CDocument
{
protected: // 仅从序列化创建
	CMFCVDoc() noexcept;
	DECLARE_DYNCREATE(CMFCVDoc)

// 特性
public:

	//OpenCV图像格式
	Mat image;

	//是否有图像数据
	BOOL isImgRead;

	//图像文件的全路径
	CString imageFileName;
	
// 操作
public:
	BOOL Draw(CDC* pDC, int nx, int ny);

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
	virtual ~CMFCVDoc();
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
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
	// 读入图像文件
	BOOL LoadImgToDoc();
	BOOL LoadImgToDoc(CString strFilePath);

	// inline-从文件路径返回Mat变量，未检验有效性
	static Mat LoadImg(CString strFileName)
	{
		String filename = CT2A(strFileName.GetString());
		return imread(filename);
	}
};
