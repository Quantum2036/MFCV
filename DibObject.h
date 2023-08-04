//DibObject.h

#ifndef __DibObject_H__
#define __DibObject_H__

#define POPULARITY_PALETTE 0
#define MEDIAN_CUT_PALETTE 1
#define FIXED_PALETTE 2

#define GETRGB555( a, b, c, d ) { WORD *wData = (WORD *) d; a = (unsigned char) ( ( (*wData) & 0x7c00 ) >> 7 ); b = (unsigned char) ( ( (*wData) & 0x03e0 ) >> 2 ); c = (unsigned char) ( ( (*wData) & 0x001f ) << 3 ); }
#define GETRGB565( a, b, c, d ) { WORD *wData = (WORD *) d; a = (unsigned char) ( ( (*wData) & 0xf800 ) >> 8 ); b = (unsigned char) ( ( (*wData) & 0x07e0 ) >> 3 ); c = (unsigned char) ( ( (*wData) & 0x001f ) << 3 ); }
#define GETRGB888( a, b, c, d ) { DWORD *dwData = (DWORD *) d; a = (unsigned char) ( (*dwData) >> 16 ); b = (unsigned char) ( ( (*dwData) & 0x0000ff00 ) >> 8 ); c = (unsigned char) ( (*dwData) & 0x000000ff ); }
#define PUTRGB555( a, b, c, d ) { WORD *wData = (WORD *) d; *wData = ( ( ( (WORD) a & 0x00f8 ) << 7 ) | ( ( (WORD) b & 0x00f8 ) << 2 ) | ( (WORD) c >> 3 ) ); }
#define PUTRGB565( a, b, c, d ) { WORD *wData = (WORD *) d; *wData = ( ( ( (WORD) a & 0x00f8 ) << 8 ) | ( ( (WORD) b & 0x00fc ) << 3 ) | ( (WORD) c >> 3 ) ); }
#define PUTRGB888( a, b, c, d ) { DWORD *dwData = (DWORD *) d; *dwData = ( (DWORD) a << 16 ) | ( (DWORD) b << 8 ) | (DWORD) c; }

#define _RGB(r,g,b) (WORD)(((b)&~7)<<7)|(((g)&~7)<<2)|((r)>>3)

class CDibObject : public CObject
{
	DECLARE_DYNCREATE(CDibObject)
public:
	//�޲ι��캯��
	CDibObject();
	//�������캯��
	CDibObject(CDibObject* pDibObject);
	//�вι��캯��
	CDibObject( const char *, CDC *pDC = NULL, int nX = -1, int nY = -1 );
	//��������
	~CDibObject();

	//�ȺŲ���������
	void operator= (const CDibObject &DibObject);
	
	//����ͼ��
	BOOL Load( const char *, CDC *pDC = NULL, int nX = -1, int nY = -1 );
	
	//�õ���������Ĵ�����
	int GetLastError( void );
	//����ͼ��
	BOOL Save( const char *, int nType = -1 );
	//�õ���ȣ����ص�λ��
	int GetWidth( void );
	//�õ��߶ȣ����ص�λ��
	int GetHeight( void );
	//�õ�ÿ������ɫλ��
	int GetNumBits( void );
	//�õ���ɫ��
	int GetNumColors( void );
	//�õ���ɫ��
	BOOL GetPaletteData( RGBQUAD * );
	RGBQUAD *GetPaletteData( void );
	//�õ��ļ�����
	int GetImageType( const char * );
	int GetImageType( void );
	//�õ�ͼ����Ϣ
	BOOL GetImageInfo( const char *, int *pnWidth = NULL,
					   int *pnHeight = NULL, int *pnPlanes = NULL,
					   int *pnBitsPerPixel = NULL, int *pnNumColors = NULL );
	//����ͼ��
	BOOL Draw( CDC *, int x = -1, int y = -1 );
	//���õ�ɫ��
	BOOL SetPalette( CDC * );
	//���õ�ɫ������
	void SetPaletteCreationType( int );
	//�õ���ɫ������
	int GetPaletteCreationType( void );

	//ͼ���Ƿ�����
	BOOL IsLoaded( void );
	//��չ������
	int ExtensionIndex( const char * );

	//�ı�ͼ���ʽ
	BOOL ChangeFormat( int );

	//�õ���ɫ���ֽ���
	int GetPaletteBytes( void );
	//�õ�λͼ���
	HGLOBAL GetDib( void );
	//�õ���ɫ��ָ��
	CPalette *GetPalette( void );

	//�������������е��������Ƿ񳬳�ͼ���С
	void NormalizeCoordinates( int *, int *, int *, int *, BOOL *bCompleteImage = NULL, BOOL *bLessThanHalf = NULL );
	//�õ�DIBλͼ�ڴ�ָ��
	void *GetDIBPointer( int *nWidthBytes = NULL, int nNewBits = 0, int *nNewWidthBytes = NULL, int nNewWidth = -1 );
	//�õ��߼���ɫ��
	LOGPALETTE *GetLogPal( void );
	//�����߼���ɫ��
	LOGPALETTE *CreateLogPalette( RGBQUAD *Palette, int NumColors );
	//�õ������ɫ����
	int GetNearestIndex( unsigned char, unsigned char, unsigned char, RGBQUAD *, int );

	//��������
	void SetQuality( int nQual ){ m_nQuality = nQual; }
	//���ÿ��
	void SetWidth( int nWidth ){ m_nWidth = nWidth; }
	//���ø߶�
	void SetHeight( int nHeight ){ m_nHeight = nHeight; }
	//����ƽ����
	void SetPlanes( int nPlanes ){ m_nPlanes = nPlanes; }
	//����ƽ����
	void SetScreenPlanes( int nScreenPlanes ){ m_nScreenPlanes = nScreenPlanes; }
	//������ɫλ��
	void SetBits( int nBits ){ m_nBits = nBits; }
	//������ɫλ��
	void SetScreenBits( int nScreenBits ){ m_nScreenBits = nScreenBits; }
	//������ɫ��
	void SetColors( int nColors ){ m_nColors = nColors; }
	//����ͼ������
	void SetImageType( int nImageType ){ m_nImageType = nImageType; }
	//��������������
	void SetLastError( int nLastError ){ m_nLastError = nLastError; }
	//��������������
	void SetPaletteBytes( int nPaletteBytes ){ m_nPaletteBytes = nPaletteBytes; }
	//����m_nX
	void SetX( int nX ){ m_nX = nX; }
	//����m_nY
	void SetY( int nY ){ m_nY = nY; }
	//��ȡm_nPlansֵ
	int GetPlanes( ){ return m_nPlanes; }
	//��ȡm_nScreenPlansֵ
	int GetScreenPlanes( ){ return m_nScreenPlanes; }
	//��ȡm_nScreenBitsֵ
	int GetScreenBits( ){ return m_nScreenBits; }
	//��ȡm_nQualityֵ
	int GetQuality( ){ return m_nQuality; }
	//��ȡm_nColorsֵ
	int GetColors( ){ return m_nColors; }
	//��ȡm_nXֵ
	int GetX( ){ return m_nX; }
	//��ȡm_nYֵ
	int GetY( ){ return m_nY; }

	//�����ɫ��
	void ProcessPalette( void );
	//�ֽڿ��
	int WidthBytes( int, int );
	//����λͼ
	void SetDib( HGLOBAL hDib ){ m_hDib = hDib; }
	//�����߼���ɫ��
	void SetLogPal( LOGPALETTE *pLogPal ){ m_pLogPal = pLogPal; }
	//���ͼ��
	void KillImage( void );
	//����ͼ��ͷ
	void ProcessImageHeader( void );

	static char *szExtensions[7];			//��չ��
	int m_nLastError;						//�������
	int m_nScreenPlanes, m_nScreenBits, m_nPaletteBytes;			

//��ϵ��Ժ���
#ifdef _DEBUG
	virtual void Dump( CDumpContext &dc) const;
	virtual void AssertValid() const;
#endif

protected:
	//��ʼ������
	void InitVars( BOOL bFlag = FALSE );
	//��λͼ������ɫ��
	void CreatePaletteFromDIB( RGBQUAD *, int );
	LOGPALETTE *CreatePaletteFromBitmap( int, unsigned char *, int, int, int );

	RGBQUAD *MakePopularityPalette( int, unsigned char *, int, int, int );
	RGBQUAD *MakeMedianCutPalette( int, unsigned char *, int, int, int );
	RGBQUAD *MakeFixedPalette( int );


	int m_nWidth, m_nHeight, m_nPlanes, m_nBits, m_nColors, m_nImageType;
	int m_nX, m_nY;
	int m_nQuality;
	int m_nPaletteCreationType;
	CPalette m_Palette;
	HGLOBAL m_hDib;
public:
	char * GetImageName();
	void ProcImgHead(void);
	char *m_pszFilename;
	LOGPALETTE *m_pLogPal;


};

#endif

