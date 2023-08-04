//DibObject.cpp

#include "pch.h"
#include "DibObject.h"

IMPLEMENT_DYNCREATE(CDibObject, CObject)

char *CDibObject::szExtensions[] = { ".BMP", ".GIF", ".PCX", ".TGA", ".JPG", ".TIF", "" };
        
//Diagnostics and dump member functions, overridden
#ifdef _DEBUG
void CDibObject::Dump(CDumpContext &dc) const
{
	//call base class function first
	CObject::Dump(dc);
	//now do the stuff for our specific class
	dc<<"File Name:"<<m_pszFilename<<"\n";
}
#endif

#ifdef _DEBUG
void CDibObject::AssertValid() const
{
	//call inherited AssertValid first
	CObject::AssertValid();
	//check CDibObject members...
	ASSERT(m_pszFilename != NULL); //must exist
	ASSERT(m_hDib != NULL); //must exist
}
#endif

////////////////////////////////////////////////////////////////////////
//CDibObject��Ĺ��캯��		    
//----------------------------------------------------------------------
//�������ܣ�����һ���޲����Ĺ��캯�������򵥵ش���һ��CDibObject����
//			��ʼ�����ڲ�������
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ����
////////////////////////////////////////////////////////////////////////
CDibObject::CDibObject()
{

	//���ø���������ʼ�����ڲ�������
	InitVars();

}

////////////////////////////////////////////////////////////////////////
//CDibObject��Ĺ��캯��		    
//----------------------------------------------------------------------
//�������ܣ���һ��CDibObject����ͨ�����Ʋ���������һ��CDibObject����
//----------------------------------------------------------------------
//����˵����CDibObject *pDibObject
//----------------------------------------------------------------------
//�� �� ֵ����
////////////////////////////////////////////////////////////////////////
CDibObject::CDibObject(CDibObject *pDibObject)
{
	InitVars();

	KillImage();
	
	m_nWidth = pDibObject->m_nWidth;
	m_nHeight = pDibObject->m_nHeight;
	m_nPlanes = pDibObject->m_nPlanes;
	m_nBits = pDibObject->m_nBits;
	m_nColors = pDibObject->m_nColors;
	m_nImageType = pDibObject->m_nImageType;
	m_nX = pDibObject->m_nX;
	m_nY = pDibObject->m_nY;
	m_nLastError = pDibObject->m_nLastError;
	m_nScreenPlanes = pDibObject->m_nScreenPlanes;
	m_nScreenBits = pDibObject->m_nScreenBits;
	m_nPaletteBytes = pDibObject->m_nPaletteBytes;
	m_nQuality = pDibObject->m_nQuality;
	m_nPaletteCreationType = pDibObject->m_nPaletteCreationType;
	
	int nNumColors = m_nColors;
	int nWidthBytes = WidthBytes( m_nBits, m_nWidth );
	
	if( pDibObject->m_hDib != NULL )
	{
		DWORD dwSize = ::GlobalSize( pDibObject->m_hDib );
		char *pData = (char *) ::GlobalLock( pDibObject->m_hDib );
		if( pData != NULL )
		{
			HGLOBAL hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
			if( hGlobal != NULL ){
				
				char *pDestData = (char *) ::GlobalLock( hGlobal );
				if( pDestData != NULL )
				{
					memcpy( pDestData, pData, dwSize );
					::GlobalUnlock( hGlobal );
					m_hDib = hGlobal;
				}
				else ::GlobalFree( hGlobal );
			}
			::GlobalUnlock( pDibObject->m_hDib );
		}
	}
}

////////////////////////////////////////////////////////////////////////
//CDibObject��Ĺ��캯��		    
//----------------------------------------------------------------------
//�������ܣ�����һ��CDibObject����Ψһ��Ҫ���һ���������ļ��������
//			������CDC�豸�����Ĳ�����ͼ����غ�������ڸ��豸����������ʾ
//			���������������nX��nY������ͼ�����ʾ�ڸ�����ָ����λ�ã���
//			��ͼ��������ʾ������Ϊ��0, 0����λ�á�
//----------------------------------------------------------------------
//����˵����const char *pszFilename
//			CDC *pDC��Ĭ��ΪNULL
//			int nX��Ĭ��Ϊ-1
//			int nY��Ĭ��Ϊ-1
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CDibObject::CDibObject( const char *pszFilename,
						   CDC *pDC, int nX, int nY )
{

	InitVars();
	Load( pszFilename, pDC, nX, nY );

}

////////////////////////////////////////////////////////////////////////
//CDibObject�����������		    
//----------------------------------------------------------------------
//�������ܣ�����CDibObject����ɾ���ö����е�ͼ����Ӧ�ı���
//----------------------------------------------------------------------
//����˵������
////////////////////////////////////////////////////////////////////////
CDibObject::~CDibObject()
{

	if( m_hDib ) ::GlobalFree( m_hDib );
	if( m_pszFilename != NULL ) delete [] m_pszFilename;
	if( m_pLogPal != NULL ) delete [] m_pLogPal;

}

////////////////////////////////////////////////////////////////////////
//InitVars()����		    
//----------------------------------------------------------------------
//�������ܣ���ʼ��CDibObject���ڲ�����
//----------------------------------------------------------------------
//����˵����BOOL bFlag	��������Ƿ���ͼ��򿪣�Ĭ��ֵΪFALSE
////////////////////////////////////////////////////////////////////////
void CDibObject::InitVars( BOOL bFlag )
{

	m_nWidth = m_nHeight = m_nBits = m_nColors = m_nImageType = 0;
	m_nX = m_nY = 0;
	m_nLastError = 0;
	m_hDib = NULL;
	m_nPaletteCreationType = FIXED_PALETTE;
	if( !bFlag )
	{
		m_pszFilename = NULL;
		m_pLogPal = NULL;
	}
	m_nQuality = 50;

}

////////////////////////////////////////////////////////////////////////
//BOOL Load()		    
//----------------------------------------------------------------------
//�������ܣ���������һ��ͼ���ļ�����CDibObject�ࡣ�����һ���������ļ�
//			�������������CDC�豸�����Ĳ�����ͼ��һ���ؾͱ����ơ������
//			����nX��nY������ͼ����ʾ�ڸ�����ָ����λ�á�����ͼ������
//			��ʾ������Ϊ��0,0����λ�á�
//----------------------------------------------------------------------
//����˵����const char *pszFilename
//			CDC *pDC��Ĭ��ΪNULL
//			int nX��Ĭ��Ϊ-1
//			int nY��Ĭ��Ϊ-1
//----------------------------------------------------------------------
//�� �� ֵ��BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::Load( const char *pszFilename, 
						CDC *pDC, int nX, int nY )
{

	//��ȡͼ���ļ�����
	m_nImageType = FileType( pszFilename );
	if( m_nImageType == 0 )
	{
		m_nLastError = IMAGELIB_UNSUPPORTED_FILETYPE;
		return( FALSE );
	}

	//ɾ���Ѵ��ڵ�ͼ��
	KillImage();

	m_pszFilename = new char [strlen(pszFilename)+1];
	if( m_pszFilename != NULL ) strcpy( m_pszFilename, pszFilename );

	//�����ļ����͵���ImageLoad.dll��̬���ӿ��е���Ӧ������ͼ���ļ�
	switch( m_nImageType )
	{
	case IMAGETYPE_BMP:
		m_hDib = ::LoadBMP( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	case IMAGETYPE_GIF:
		m_hDib = ::LoadGIF( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	case IMAGETYPE_JPG:
		m_hDib = ::LoadJPG( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	case IMAGETYPE_PCX:
		m_hDib = ::LoadPCX( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	case IMAGETYPE_TGA:
		m_hDib = ::LoadTGA( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	case IMAGETYPE_TIF:
		m_hDib = ::LoadTIF( pszFilename );
		if( m_hDib == NULL ){
			m_nLastError = ::GetLastPicLibError();
			return( FALSE );
		}
		break;
	}

	//����ͼ����Ϣͷ
	ProcessImageHeader();
	//�����ɫ��
	ProcessPalette();

	//���������豸������ָ�룬����ָ�����豸�������л���ͼ��
	if( pDC != NULL ) Draw( pDC, nX, nY );

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//BOOL GetImageInfo()		    
//----------------------------------------------------------------------
//�������ܣ���������ȡͼ���ȡ��߶ȡ�λƽ������ÿ�����ص��λ�����Լ���
//			ɫ������Ϣ��ֵΪNULL�Ĳ������������롣
//----------------------------------------------------------------------
//����˵����const char *pszFilename
//			int *pnWidth��			Ĭ��ΪNULL
//			int *pnHeight��			Ĭ��ΪNULL
//			int *pnPlanes��			Ĭ��ΪNULL
//			int *pnBitsPerPixel��	Ĭ��ΪNULL
//			int *pnNumColors��		Ĭ��ΪNULL
//----------------------------------------------------------------------
//�� �� ֵ��BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::GetImageInfo( const char *pszFilename, int *pnWidth,
	int *pnHeight, int *pnPlanes, int *pnBitsPerPixel, int *pnNumColors )
{
	int nImageType;

	//��ȡͼ���ļ�����
	nImageType = FileType( pszFilename );
	if( nImageType == 0 )
	{
		m_nLastError = IMAGELIB_UNSUPPORTED_FILETYPE;
		return( FALSE );
	}
	
	//�����ļ����͵���ImageLoad.dll��̬���ӿ��е���Ӧ������ȡͼ����Ϣ
	switch( nImageType )
	{
	case IMAGETYPE_BMP:
		return( ::GetBMPInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	case IMAGETYPE_GIF:
		return( ::GetGIFInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	case IMAGETYPE_JPG:
		return( ::GetJPGInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	case IMAGETYPE_PCX:
		return( ::GetPCXInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	case IMAGETYPE_TGA:
		return( ::GetTGAInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	case IMAGETYPE_TIF:
		return( ::GetTIFInfo( pszFilename, pnWidth, pnHeight, pnPlanes, pnBitsPerPixel, pnNumColors ) );
	}
	
	return( FALSE );

}

////////////////////////////////////////////////////////////////////////
//int GetLastError()		    
//----------------------------------------------------------------------
//�������ܣ����������������������һ��CDibObject�������ϸ������Ϣ��
//			"ImageErrors.h"ͷ�ļ���
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��int nErrorNumber
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::GetLastError( void )
{

	return( m_nLastError );

}

////////////////////////////////////////////////////////////////////////
//BOOL Save()		    
//----------------------------------------------------------------------
//�������ܣ�����������פ����CDibObject�����еĵ�ǰͼ��ͼ�����Ͷ����
//			GetImageType()������
//----------------------------------------------------------------------
//����˵����const char *pszFilename
//			int nType��Ĭ��Ϊ-1
//----------------------------------------------------------------------
//�� �� ֵ��BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::Save( const char *pszFilename, int nType )
{

	//��û��ָ���ļ����ͣ�������ļ����ж����ļ�����
	if( nType == -1 ) nType = ExtensionIndex( pszFilename );

	if( nType < IMAGETYPE_FIRSTTYPE || nType > IMAGETYPE_LASTTYPE ) return( FALSE );
	m_nImageType = nType;
	delete [] m_pszFilename;
	m_pszFilename = new char [strlen(pszFilename)+1];
	if( m_pszFilename != NULL ) strcpy( m_pszFilename, pszFilename );

	//�����ļ����͵���ImageLoad.dll��̬���ӿ��е���Ӧ��������ͼ��
	switch( m_nImageType )
	{
	case IMAGETYPE_BMP:
		return( ::SaveBMP( pszFilename, m_hDib ) );
		break;
	case IMAGETYPE_GIF:
		return( ::SaveGIF( pszFilename, m_hDib ) );
		break;
	case IMAGETYPE_JPG:
		return( ::SaveJPG( pszFilename, m_hDib, m_nQuality ) );
		break;
	case IMAGETYPE_PCX:
		return( ::SavePCX( pszFilename, m_hDib ) );
		break;
	case IMAGETYPE_TGA:
		return( ::SaveTGA( pszFilename, m_hDib ) );
		break;
	case IMAGETYPE_TIF:
		return( ::SaveTIF( pszFilename, m_hDib ) );
		break;
	}
	
	return( TRUE );
	
}

////////////////////////////////////////////////////////////////////////
//int ExtensionIndex()
//----------------------------------------------------------------------
//�������ܣ������������ļ���ָ����ͼ�����͡�
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��int nWidth
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::ExtensionIndex( const char *pszFilename )
{
	int Index = 0;
	char *pszExtension;

	pszExtension = (char *) &pszFilename[strlen(pszFilename)-4];

	while( szExtensions[Index][0] )
	{
		if( !stricmp( pszExtension, szExtensions[Index] ) ) return( Index + 1 );
		Index++;
	}

	return( -1 );

}

////////////////////////////////////////////////////////////////////////
//int GetWidth()
//----------------------------------------------------------------------
//�������ܣ�����������ͼ������ؿ������
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��int nWidth
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::GetWidth( void )
{

	return( m_nWidth );

}

////////////////////////////////////////////////////////////////////////
//int GetHeight( void )
//----------------------------------------------------------------------
//�������ܣ�����������ͼ������ظ߶�����
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��int nHeight
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::GetHeight( void )
{

	return( m_nHeight );

}

////////////////////////////////////////////////////////////////////////
//int GetNumBits( void )
//----------------------------------------------------------------------
//�������ܣ�����������ͼ�����������1��4��8��16��24��32��
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��int nBits
////////////////////////////////////////////////////////////////////////
int CDibObject::GetNumBits( void )
{

	return( m_nBits );

}

////////////////////////////////////////////////////////////////////////
//int GetNumColors( void )
//----------------------------------------------------------------------
//�������ܣ�����������ͼ���ɫ���е���ɫ��ͼ��Ϊ16λɫ�����ʱ����ֵΪ0��
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��int nColors
////////////////////////////////////////////////////////////////////////
int CDibObject::GetNumColors( void )
{

	return( m_nColors );

}

////////////////////////////////////////////////////////////////////////
//BOOL GetPaletteData()		    
//----------------------------------------------------------------------
//�������ܣ�����������ǰRGB��ɫ���������뵽�������ָ���С����ͼ����16
//			λɫ�����ʱ�򲻽����κβ�����
//----------------------------------------------------------------------
//����˵����RGBQUAD *pRGBPalette
//----------------------------------------------------------------------
//�� �� ֵ��BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::GetPaletteData( RGBQUAD *pRGBPalette )
{

	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return( FALSE );

	char *pTemp;
	pTemp = (char *) ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL ) return( FALSE );

	memcpy( pRGBPalette, &pTemp[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)],
								m_nPaletteBytes );

	m_nLastError = IMAGELIB_SUCCESS;
	::GlobalUnlock( m_hDib );

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//RGBQUAD GetPaletteData()		    
//----------------------------------------------------------------------
//�������ܣ�����������һ��ָ��CDibObject��ɫ���������е�ָ�롣���ͼ��
//			��16λɫ�����ʱ�򷵻�NULL��
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ������һ��ָ��CDibObject��ɫ���������е�ָ��
////////////////////////////////////////////////////////////////////////
RGBQUAD *CDibObject::GetPaletteData( void )
{
	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return( NULL );

	m_nLastError = IMAGELIB_NO_PALETTE_FOR_HIGH_COLOR;
	if( m_nColors <= 0 || m_nColors > 256 ) return( NULL );
	
	RGBQUAD *pRGBPalette;
	pRGBPalette = new RGBQUAD [m_nColors];
	if( pRGBPalette == NULL )
	{
		m_nLastError = IMAGELIB_MEMORY_ALLOCATION_ERROR;
		return( NULL );
	}
	
	char *pTemp;
	pTemp = (char *) ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL )
	{
		delete [] pRGBPalette;
		return( NULL );
	}
	
	memcpy( pRGBPalette, &pTemp[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], 
								m_nColors * sizeof( RGBQUAD ) );
	
	m_nLastError = IMAGELIB_SUCCESS;
	::GlobalUnlock( m_hDib );
	
	return( pRGBPalette );
	
}

////////////////////////////////////////////////////////////////////////
//int GetImageType()		    
//----------------------------------------------------------------------
//�������ܣ�����������ͼ������͡�ͼ�����Ͷ������£�
//			#define IMAGETYPE_NONE		0
//			#define IMAGETYPE_BMP		1
//			#define IMAGETYPE_GIF		2
//			#define IMAGETYPE_PCX		3
//			#define IMAGETYPE_TGA		4
//			#define IMAGETYPE_JPG		5
//			#define IMAGETYPE_TIF		6
//----------------------------------------------------------------------
//����˵����const char *pFilename
//----------------------------------------------------------------------
//�� �� ֵ��int nImageType
////////////////////////////////////////////////////////////////////////
int CDibObject::GetImageType( const char *pFilename )
{

	return( ::FileType( pFilename ) );

}

////////////////////////////////////////////////////////////////////////
//int GetImageType()		    
//----------------------------------------------------------------------
//�������ܣ�����������ͼ������͡�ͼ�����Ͷ������£�
//			#define IMAGETYPE_NONE		0
//			#define IMAGETYPE_BMP		1
//			#define IMAGETYPE_GIF		2
//			#define IMAGETYPE_PCX		3
//			#define IMAGETYPE_TGA		4
//			#define IMAGETYPE_JPG		5
//			#define IMAGETYPE_TIF		6
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��int nImageType
////////////////////////////////////////////////////////////////////////
int CDibObject::GetImageType( void )
{

	return( m_nImageType );

}

////////////////////////////////////////////////////////////////////////
//BOOL Draw( CDC *pDC, int nX, int nY )		    
//----------------------------------------------------------------------
//�������ܣ������������豸�������ϻ���ͼ�����X��Y����û�и�����ͼ���
//			������(0,0)������ϴδ������Ч���괦��
//----------------------------------------------------------------------
//����˵����CDC *pDC
//			int nX��Ĭ��Ϊ-1
//			int nY��Ĭ��Ϊ-1
//----------------------------------------------------------------------
//�� �� ֵ��BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::Draw( CDC *pDC, int nX, int nY )
{
	if( nX != -1 ) m_nX = nX;
	if( nY != -1 ) m_nY = nY;

	m_nLastError = IMAGELIB_HDIB_NULL;

	//û�д�ͼ��
	if( m_hDib == NULL ) return( FALSE );

	char *pTemp;
	//����ͼ�������
	pTemp = (char *) ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL ) return( NULL );

	//ͼ����Ϣͷ
	BITMAPINFOHEADER *pBIH;
	pBIH = (BITMAPINFOHEADER *) &pTemp[sizeof(BITMAPFILEHEADER)];

	int nRet = ::StretchDIBits( pDC->m_hDC, m_nX, m_nY, m_nWidth, m_nHeight, 0, 0, 
								m_nWidth, m_nHeight, 
								(const void FAR *) &pTemp[sizeof(BITMAPFILEHEADER)+
														  sizeof(BITMAPINFOHEADER)+
														  m_nPaletteBytes], 
								(BITMAPINFO *) pBIH, DIB_RGB_COLORS, SRCCOPY );

	::GlobalUnlock( m_hDib );
	m_nLastError = IMAGELIB_STRETCHDIBITS_ERROR;
	if( nRet != m_nHeight ) return( FALSE );
	m_nLastError = IMAGELIB_SUCCESS;

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//BOOL CreateLogPalette()		    
//----------------------------------------------------------------------
//�������ܣ�����������ָ���ĵ�ɫ��ָ�뼰��ɫ������һ���߼���ɫ�塣
//----------------------------------------------------------------------
//����˵����CDC *pDC
//			int nX��Ĭ��Ϊ-1
//			int nY��Ĭ��Ϊ-1
//----------------------------------------------------------------------
//�� �� ֵ���������߼���ɫ��ָ��
////////////////////////////////////////////////////////////////////////
LOGPALETTE *CDibObject::CreateLogPalette( RGBQUAD *pPalette, 
										   int nNumColors )
{
	LOGPALETTE *pLogPal;
	int i;

	if( pPalette == NULL ) return( NULL );

	pLogPal = (LOGPALETTE *) new char [sizeof(LOGPALETTE) + nNumColors * sizeof(PALETTEENTRY)];
	if( pLogPal == NULL ) return( NULL );

	//ָ����ɫ��汾��
	pLogPal->palVersion = 0x300;
	//ָ����ɫ����ɫ��
	pLogPal->palNumEntries = (unsigned short) nNumColors;
	
	//���Ƶ�ɫ�����ɫ��
	for( i=0; i<nNumColors; i++ )
	{
		pLogPal->palPalEntry[i].peRed = pPalette[i].rgbRed;
		pLogPal->palPalEntry[i].peGreen = pPalette[i].rgbGreen;
		pLogPal->palPalEntry[i].peBlue = pPalette[i].rgbBlue;
		pLogPal->palPalEntry[i].peFlags = 0;
	}

	return( pLogPal );

}

////////////////////////////////////////////////////////////////////////
//void ProcessImageHeader()		    
//----------------------------------------------------------------------
//�������ܣ�����������ͼ����Ϣͷ�������ݡ�
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ����
////////////////////////////////////////////////////////////////////////
void CDibObject::ProcessImageHeader( void )
{

	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return;

	char *pTemp;
	BITMAPINFOHEADER *pBIH;
	pTemp = (char *) ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL ) return;
	
	//�õ�ͼ����Ϣͷָ��
	pBIH = (BITMAPINFOHEADER *) &pTemp[sizeof(BITMAPFILEHEADER)];
	m_nWidth = pBIH->biWidth;
	m_nHeight = pBIH->biHeight;
	m_nPlanes = pBIH->biPlanes;
	m_nBits = pBIH->biBitCount;
	m_nColors = 1 << m_nBits;
	if( m_nPlanes > 1 ) m_nColors <<= ( m_nPlanes - 1 );
	if( m_nBits >= 16 ) m_nColors = 0;

		
	int nWidthBytes = WidthBytes(m_nBits, m_nWidth);
	pBIH->biSizeImage = nWidthBytes * m_nHeight;

	::GlobalUnlock( m_hDib );

	m_nLastError = IMAGELIB_SUCCESS;

}

////////////////////////////////////////////////////////////////////////
//void ProcessPalette()		    
//----------------------------------------------------------------------
//�������ܣ��������ɫ�塣
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ����
////////////////////////////////////////////////////////////////////////
void CDibObject::ProcessPalette( void )
{

	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return;

	CWindowDC WindowDC( NULL );
	m_nScreenPlanes = WindowDC.GetDeviceCaps( PLANES );
	m_nScreenBits = WindowDC.GetDeviceCaps( BITSPIXEL );

	m_nPaletteBytes = 0;
	m_Palette.DeleteObject();

	if( m_nBits <= 8 ) m_nPaletteBytes = m_nColors * sizeof( RGBQUAD );
	if( m_nScreenBits >= 16 ) return;

	char *pTemp;
	pTemp = (char *) ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL ) return;

	if( m_nBits <= 8 ){
		RGBQUAD *pRGBPalette;
		pRGBPalette = (RGBQUAD *) &pTemp[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
		LOGPALETTE *pPalette;
		pPalette = CreateLogPalette( pRGBPalette, m_nColors );
		if( pPalette == NULL ){
			m_nLastError = IMAGELIB_LOGICAL_PALETTE_CREATION_ERROR;
			goto ProcessPaletteExit;
			}
		m_Palette.CreatePalette( pPalette );
		delete [] pPalette;
		}

	m_nLastError = IMAGELIB_SUCCESS;

ProcessPaletteExit:
	::GlobalUnlock( m_hDib );

}

////////////////////////////////////////////////////////////////////////
//void KillImage()		    
//----------------------------------------------------------------------
//�������ܣ������ǰCDibObject���е�ͼ�����
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ����
////////////////////////////////////////////////////////////////////////
void CDibObject::KillImage( void )
{

	if( m_hDib ) ::GlobalFree( m_hDib );
	m_hDib = NULL;

	if( m_pLogPal != NULL ) delete [] m_pLogPal;
	m_pLogPal = NULL;

	if( m_pszFilename != NULL ) delete [] m_pszFilename;
	m_pszFilename = NULL;

	m_Palette.DeleteObject();

}

////////////////////////////////////////////////////////////////////////
//BOOL SetPalette( CDC *pDC )		    
//----------------------------------------------------------------------
//�������ܣ������������豸������ĵ�ɫ��������Ӧ��ͼ�񡣵�һ��ͼ����
//			��ʱ�����ĵ�ɫ����Ϣ�㱻����CDibObject�����С�
//----------------------------------------------------------------------
//����˵����CDC *pDC
//----------------------------------------------------------------------
//�� �� ֵ��BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::SetPalette( CDC *pDC )
{

	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return( FALSE );

	pDC->SelectPalette( &m_Palette, FALSE );
	pDC->RealizePalette();

	m_nLastError = IMAGELIB_SUCCESS;

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//BOOL IsLoaded( void )		    
//----------------------------------------------------------------------
//�������ܣ��������ж��Ƿ��Ѿ�������һ��ͼ��
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::IsLoaded( void )
{

	return( m_hDib != NULL );

}

////////////////////////////////////////////////////////////////////////
//void GetDIBPointer()		    
//----------------------------------------------------------------------
//�������ܣ��õ�ͼ��DIB����ָ�롣
//----------------------------------------------------------------------
//����˵����int *nWidthBytes	ͼ����ֽڿ��
//			int nNewBits		�����õ�ͼ����ɫλ��
//			int *nNewWidthBytes	ͼ����ɫλ���ı����ֽڿ��
//			int nNewWidth		�����õ�ͼ���ȣ����أ�
//----------------------------------------------------------------------
//�� �� ֵ����
////////////////////////////////////////////////////////////////////////
void *CDibObject::GetDIBPointer( int *nWidthBytes,
								 int nNewBits, 
								 int *nNewWidthBytes,
								 int nNewWidth )
{

	m_nLastError = IMAGELIB_HDIB_NULL;
	if( m_hDib == NULL ) return( NULL );

	void *pTemp;
	pTemp = ::GlobalLock( m_hDib );
	m_nLastError = IMAGELIB_MEMORY_LOCK_ERROR;
	if( pTemp == NULL ) return( NULL );

	if( nWidthBytes != NULL ) *nWidthBytes = WidthBytes( m_nBits, m_nWidth );

	if( nNewWidthBytes != NULL )
	{
		if( nNewWidth == -1 ) nNewWidth = m_nWidth;
		*nNewWidthBytes = WidthBytes( nNewBits, nNewWidth );
	}

	return( pTemp );

}

////////////////////////////////////////////////////////////////////////
//int WidthBytes()		    
//----------------------------------------------------------------------
//�������ܣ����������ݴ��������λ����ͼ���ȣ����أ��������ֽڿ�ȡ�
//----------------------------------------------------------------------
//����˵����int nBits	ÿ����λ��
//			int nWidth	ͼ��Ŀ��
//----------------------------------------------------------------------
//�� �� ֵ��int	ͼ����ֽڿ��
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::WidthBytes( int nBits, int nWidth )
{
	int nWidthBytes;

	nWidthBytes = nWidth;
	if( nBits == 1 ) nWidthBytes = ( nWidth + 7 ) / 8;
	else if( nBits == 4 ) nWidthBytes = ( nWidth + 1 ) / 2;
	else if( nBits == 16 ) nWidthBytes = nWidth * 2;
	else if( nBits == 24 ) nWidthBytes = nWidth * 3;
	else if( nBits == 32 ) nWidthBytes = nWidth * 4;

	//һ�����ĺ���λΪ�㣬�����Ϊ4��������
	while( ( nWidthBytes & 3 ) != 0 ) nWidthBytes++;

	return( nWidthBytes );

}


////////////////////////////////////////////////////////////////////////
//void NormalizeCoordinates()		    
//----------------------------------------------------------------------
//�������ܣ���񻯴����ͼ������
//----------------------------------------------------------------------
//����˵����int *nX1				�����������Ͻ�X����
//			int *nY1				�����������Ͻ�Y����
//			int *nX2				�����������½�X����
//			int *nY2				�����������½�Y����
//			BOOL *bCompleteImage	���������Ƿ�Ϊȫ������
//			BOOL *bLessThanHalf		���������Ƿ�Ϊһ������
//----------------------------------------------------------------------
//�� �� ֵ����
////////////////////////////////////////////////////////////////////////
void CDibObject::NormalizeCoordinates( int *nX1, int *nY1, int *nX2, int *nY2, 
									  BOOL *bCompleteImage, BOOL *bLessThanHalf )
{

	if( *nX1 == -1 ) *nX1 = 0;
	if( *nY1 == -1 ) *nY1 = 0;
	if( *nX2 == -1 ) *nX2 = m_nWidth - 1;
	if( *nY2 == -1 ) *nY2 = m_nHeight - 1;

	if( *nX1 > *nX2 ){
		int nTemp;
		nTemp = *nX1;
		*nX1 = *nX2;
		*nX2 = nTemp;		
		}

	if( *nY1 > *nY2 ){
		int nTemp;
		nTemp = *nY1;
		*nY1 = *nY2;
		*nY2 = nTemp;		
		}

	if( *nX1 < 0 ) *nX1 = 0;
	if( *nX2 > m_nWidth - 1 ) *nX2 = m_nWidth - 1;

	if( *nX2 < 0 ) *nX2 = 0;
	if( *nX2 > m_nWidth - 1 ) *nX2 = m_nWidth - 1;

	if( *nY1 < 0 ) *nY1 = 0;
	if( *nY1 > m_nHeight - 1 ) *nY1 = m_nHeight - 1;

	if( *nY2 < 0 ) *nY2 = 0;
	if( *nY2 > m_nHeight - 1 ) *nY2 = m_nHeight - 1;

	if( bCompleteImage != NULL ) *bCompleteImage = TRUE;
	if( bLessThanHalf != NULL ) *bLessThanHalf = FALSE;
	if( *nX1 > 0 || *nY1 > 0 || *nX2 < m_nWidth - 1 || *nY2 < m_nHeight - 1 ){
		if( bCompleteImage != NULL ) *bCompleteImage = FALSE;
		DWORD dwTotalPixels, dwThesePixels;
		dwTotalPixels = (DWORD) m_nWidth * m_nHeight;
		dwThesePixels = (DWORD) ( (*nX2) - (*nX1) + 1 ) * (DWORD) ( (*nY2) - (*nY1) + 1 );
		if( bLessThanHalf != NULL && dwThesePixels <= dwTotalPixels / 2 ) *bLessThanHalf = TRUE;
		}

}

////////////////////////////////////////////////////////////////////////
//void operator=()		    
//----------------------------------------------------------------------
//�������ܣ���=�����������ء�
//----------------------------------------------------------------------
//����˵����CDibObject &DibObject	��һ��CDibObjectͼ�����
//----------------------------------------------------------------------
//�� �� ֵ����
////////////////////////////////////////////////////////////////////////
void CDibObject::operator= (const CDibObject &DibObject)
{

	KillImage();

	m_nWidth = DibObject.m_nWidth;
	m_nHeight = DibObject.m_nHeight;
	m_nPlanes = DibObject.m_nPlanes;
	m_nBits = DibObject.m_nBits;
	m_nColors = DibObject.m_nColors;
	m_nImageType = DibObject.m_nImageType;
	m_nX = DibObject.m_nX;
	m_nY = DibObject.m_nY;
	m_nLastError = DibObject.m_nLastError;
	m_nScreenPlanes = DibObject.m_nScreenPlanes;
	m_nScreenBits = DibObject.m_nScreenBits;
	m_nPaletteBytes = DibObject.m_nPaletteBytes;
	m_nQuality = DibObject.m_nQuality;
	m_nPaletteCreationType = DibObject.m_nPaletteCreationType;

	int nNumColors = m_nColors;
	int nWidthBytes = WidthBytes( m_nBits, m_nWidth );

	if( DibObject.m_hDib != NULL ){

		DWORD dwSize = ::GlobalSize( DibObject.m_hDib );

		char *pData = (char *) ::GlobalLock( DibObject.m_hDib );
		if( pData != NULL ){

			HGLOBAL hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
			if( hGlobal != NULL ){

				char *pDestData = (char *) ::GlobalLock( hGlobal );
				if( pDestData != NULL ){

					memcpy( pDestData, pData, dwSize );

					if( nNumColors != 0 ) CreatePaletteFromDIB( (RGBQUAD *) &pData[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)], nNumColors );

					else if( DibObject.m_pLogPal != NULL ){
						m_pLogPal = (LOGPALETTE *) new char [sizeof(LOGPALETTE)+DibObject.m_pLogPal->palNumEntries*sizeof(PALETTEENTRY)];
						if( m_pLogPal != NULL ){
							for( int i=0; i<DibObject.m_pLogPal->palNumEntries; i++ ) m_pLogPal[i] = DibObject.m_pLogPal[i];
							m_Palette.CreatePalette( m_pLogPal );
							}
						}

					::GlobalUnlock( hGlobal );
					m_hDib = hGlobal;
					}
				else ::GlobalFree( hGlobal );
				}
			::GlobalUnlock( DibObject.m_hDib );
			}
		}

	if( DibObject.m_pszFilename != NULL ){
		m_pszFilename = new char [strlen(DibObject.m_pszFilename)+1];
		strcpy( m_pszFilename, DibObject.m_pszFilename );
		}
}

////////////////////////////////////////////////////////////////////////
//void CreatePaletteFromDIB()		    
//----------------------------------------------------------------------
//�������ܣ��ӵ�ǰCDibObjectλͼ�����д�����ɫ�塣
//----------------------------------------------------------------------
//����˵����RGBQUAD *pRGBPalette	��ɫ��ָ��
//			int nNumColors			ͼ����ɫ��
//----------------------------------------------------------------------
//�� �� ֵ����
////////////////////////////////////////////////////////////////////////
void CDibObject::CreatePaletteFromDIB( RGBQUAD *pRGBPalette, int nNumColors )
{

	if( pRGBPalette != NULL ){
		LOGPALETTE *pPalette;
		pPalette = CreateLogPalette( pRGBPalette, nNumColors );
		if( pPalette != NULL ){
			m_Palette.CreatePalette( pPalette );
			delete [] pPalette;
			}
		else m_nLastError = IMAGELIB_LOGICAL_PALETTE_CREATION_ERROR;
		}

}

////////////////////////////////////////////////////////////////////////
//int GetNearestIndex()		    
//----------------------------------------------------------------------
//�������ܣ���ȡ�����ɫ����ֵ��
//----------------------------------------------------------------------
//����˵����unsigned char ucRed		��ɫ����
//			unsigned char ucGreen	��ɫ����
//			unsigned char ucBlue	��ɫ����
//			RGBQUAD *pRGBPalette	��ɫ��ָ��
//			int nNumColors			��ɫ��
//----------------------------------------------------------------------
//�� �� ֵ��int	�����ɫ����ֵ
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::GetNearestIndex( unsigned char ucRed,
								 unsigned char ucGreen, 
								 unsigned char ucBlue, 
								 RGBQUAD *pRGBPalette, 
								 int nNumColors )
{
	int i, Index = 0;
	DWORD NewDiff, Diff = 100000L;
	DWORD Red, Green, Blue;
	
	for( i=0; i<nNumColors; i++ )
	{
		if( ucRed > pRGBPalette[i].rgbRed ) Red = (DWORD) ( pRGBPalette[i].rgbRed - ucRed );
		else Red = (DWORD) ( ucRed - pRGBPalette[i].rgbRed );
		if( ucGreen > pRGBPalette[i].rgbGreen ) Green = (DWORD) ( pRGBPalette[i].rgbGreen - ucGreen );
		else Green = (DWORD) ( ucGreen - pRGBPalette[i].rgbGreen );
		if( ucBlue > pRGBPalette[i].rgbBlue ) Blue = (DWORD) ( pRGBPalette[i].rgbBlue - ucBlue );
		else Blue = (DWORD) ( ucBlue - pRGBPalette[i].rgbBlue );
		NewDiff = ( Red * Red ) + ( Green * Green ) + ( Blue * Blue );
		if( NewDiff < Diff )
		{
			if( NewDiff <= 1 ) return( i );
			Diff = NewDiff;
			Index = i;
		}
	}
	
	return( Index );

}

////////////////////////////////////////////////////////////////////////
//BOOL ChangeFormat( int nNewBitsPerPixel )		    
//----------------------------------------------------------------------
//�������ܣ���������ͼ��ӵ�ǰ�ֱ���ת��Ϊָ���ֱ��ʡ�
//----------------------------------------------------------------------
//����˵����int nNewBitsPerPixel
//----------------------------------------------------------------------
//�� �� ֵ��BOOL���ɹ�����TRUE��ʧ�ܷ���FALSE
////////////////////////////////////////////////////////////////////////
BOOL CDibObject::ChangeFormat( int nNewBitsPerPixel )
{

	m_nLastError = IMAGELIB_SUCCESS;
	if( nNewBitsPerPixel == m_nBits ) return( TRUE );

	int nOldWidthBytes, nNewWidthBytes;
	char *pBuffer = (char *) GetDIBPointer( &nOldWidthBytes, nNewBitsPerPixel, &nNewWidthBytes );
	if( pBuffer == NULL ) return( FALSE );

	BITMAPINFOHEADER *pOldBIH, *pNewBIH;
	BITMAPFILEHEADER *pOldBFH, *pNewBFH;
	RGBQUAD *pOldRGBPalette, *pNewRGBPalette;
	unsigned char *pOldBits, *pNewBits;
	int nNumColors, nNumNewColors;

	pOldBFH = (BITMAPFILEHEADER *) pBuffer;
	pOldBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	pOldRGBPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	nNumColors = m_nColors;
	nNumNewColors = 1 << nNewBitsPerPixel;
	if( nNewBitsPerPixel > 8 ) nNumNewColors = 0;
	pOldBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumColors*sizeof(RGBQUAD)];

	if( m_nBits >= 16 && nNewBitsPerPixel < 16 ){
		if( m_pLogPal != NULL ) delete [] m_pLogPal;
		m_pLogPal = CreatePaletteFromBitmap( nNumNewColors, pOldBits, m_nBits, m_nWidth, m_nHeight );
		}

	HGLOBAL hGlobal;
	DWORD dwSize;
	dwSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNumNewColors * sizeof( RGBQUAD ) + m_nHeight * nNewWidthBytes;
	hGlobal = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_ZEROINIT, dwSize );
	if( hGlobal == NULL ){
		::GlobalUnlock( m_hDib );
		return( FALSE );
		}

	pBuffer = (char *) ::GlobalLock( hGlobal );
	if( pBuffer == NULL ){
		::GlobalFree( hGlobal );
		::GlobalUnlock( m_hDib );
		return( FALSE );
		}

	pNewBFH = (BITMAPFILEHEADER *) pBuffer;
	pNewBIH = (BITMAPINFOHEADER *) &pBuffer[sizeof(BITMAPFILEHEADER)];
	pNewRGBPalette = (RGBQUAD *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)];
	*pNewBFH = *pOldBFH;
	*pNewBIH = *pOldBIH;
	int i, j = nNumNewColors;

	if( m_nBits < 16 && nNewBitsPerPixel < 16 ){
		for( i=0; i<j; i++ ) pNewRGBPalette[i] = pOldRGBPalette[i];
		}
	else if( m_nBits >= 16 ){
 		for( i=0; i<j; i++ ){
			pNewRGBPalette[i].rgbRed = m_pLogPal->palPalEntry[i].peRed;
			pNewRGBPalette[i].rgbGreen = m_pLogPal->palPalEntry[i].peGreen;
			pNewRGBPalette[i].rgbBlue = m_pLogPal->palPalEntry[i].peBlue;
			}
		}
	pNewBIH->biBitCount = nNewBitsPerPixel;
	pNewBIH->biSizeImage = nNewWidthBytes * m_nHeight;
	pNewBIH->biClrUsed = nNumNewColors;
	pNewBFH->bfSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNumNewColors * sizeof( RGBQUAD ) + pNewBIH->biSizeImage;
	pNewBFH->bfOffBits = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + nNumNewColors * sizeof( RGBQUAD );
	pNewBits = (unsigned char *) &pBuffer[sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+nNumNewColors*sizeof(RGBQUAD)];
	m_nPaletteBytes = nNumNewColors * sizeof( RGBQUAD );

	for( int y=0; y<m_nHeight; y++ ){
		unsigned char ucRed, ucGreen, ucBlue;
		unsigned char *pSrc, *pDest;
		pSrc = pOldBits;
		pSrc += ( nOldWidthBytes * y );
		pDest = pNewBits;
		pDest += ( nNewWidthBytes * y );
		for( int x=0; x<m_nWidth; x++ ){
			switch( m_nBits ){
				case 1:
					if( pSrc[x/8] & ( 0x80 >> ( x & 7 ) ) )
						ucRed = ucGreen = ucBlue = 0xff;
					else
						ucRed = ucGreen = ucBlue = 0x00;
					break;
				case 4:
					if( !( x & 1 ) ){
						ucRed = pOldRGBPalette[pSrc[x/2]>>4].rgbRed;
						ucGreen = pOldRGBPalette[pSrc[x/2]>>4].rgbGreen;
						ucBlue = pOldRGBPalette[pSrc[x/2]>>4].rgbBlue;
						}
					else{
						ucRed = pOldRGBPalette[pSrc[x/2]&15].rgbRed;
						ucGreen = pOldRGBPalette[pSrc[x/2]&15].rgbGreen;
						ucBlue = pOldRGBPalette[pSrc[x/2]&15].rgbBlue;
						}
					break;
				case 8:
					ucRed = pOldRGBPalette[pSrc[x]].rgbRed;
					ucGreen = pOldRGBPalette[pSrc[x]].rgbGreen;
					ucBlue = pOldRGBPalette[pSrc[x]].rgbBlue;
					break;
				case 16:
					GETRGB555( ucRed, ucGreen, ucBlue, &pSrc[x*2] );
					break;
				case 24:
					ucRed = pSrc[x*3+2];
					ucGreen = pSrc[x*3+1];
					ucBlue = pSrc[x*3];
					break;
				case 32:
					GETRGB888( ucRed, ucGreen, ucBlue, &pSrc[x*4] );
					break;
				}

			switch( nNewBitsPerPixel ){
				case 1:
					if( !( x & 7 ) ) pDest[x/8] = 0;
					pDest[x/8] |= (unsigned char) GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNumNewColors ) << ( x & 7 );
					break;
				case 4:
					if( !( x & 1 ) )
						pDest[x/2] = (unsigned char) GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNumNewColors ) << 4;
					else
						pDest[x/2] |= (unsigned char) GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNumNewColors );
					break;
				case 8:
					pDest[x] = (unsigned char) GetNearestIndex( ucRed, ucGreen, ucBlue, pNewRGBPalette, nNumNewColors );
					break;
				case 16:
					PUTRGB555( ucRed, ucGreen, ucBlue, &pDest[x*2] );
					break;
				case 24:
					pDest[x*3+2] = ucRed;
					pDest[x*3+1] = ucGreen;
					pDest[x*3] = ucBlue;
					break;
				case 32:
					PUTRGB888( ucRed, ucGreen, ucBlue, &pDest[x*4] );
					break;
				}
			}
		}

	::GlobalUnlock( m_hDib );
	::GlobalFree( m_hDib );
	::GlobalUnlock( hGlobal );
	m_hDib = hGlobal;

	ProcessImageHeader();

	return( TRUE );

}

////////////////////////////////////////////////////////////////////////
//LOGPALETTE CreatePaletteFromBitmap()		    
//----------------------------------------------------------------------
//�������ܣ��ӵ�ǰCDibObjectλͼ�����д����߼���ɫ�塣
//----------------------------------------------------------------------
//����˵����int nNumColors			��ɫ��
//			unsigned char *pBits	λͼ����ָ��
//			int nBits				��ɫλ��
//			int nWidth				ͼ����
//			int nHeight				ͼ��߶�
//----------------------------------------------------------------------
//�� �� ֵ��LOGPALETTE	�߼���ɫ��ָ��
////////////////////////////////////////////////////////////////////////
LOGPALETTE *CDibObject::CreatePaletteFromBitmap(int nNumColors, 
												unsigned char *pBits, int nBits, 
												int nWidth, int nHeight )
{

	RGBQUAD *pRGBPalette = NULL;
	if( nBits != 8 && m_nPaletteCreationType == POPULARITY_PALETTE ) pRGBPalette = MakePopularityPalette( nNumColors, pBits, nBits, nWidth, nHeight );
	else if( nBits != 8 && m_nPaletteCreationType == MEDIAN_CUT_PALETTE ) pRGBPalette = MakeMedianCutPalette( nNumColors, pBits, nBits, nWidth, nHeight );
	else if( m_nPaletteCreationType == FIXED_PALETTE ) pRGBPalette = MakeFixedPalette( nNumColors );
	if( pRGBPalette == NULL ) return( NULL );

	LOGPALETTE *pLogPal = CreateLogPalette( pRGBPalette, nNumColors );

	delete [] pRGBPalette;

	return( pLogPal );

}

////////////////////////////////////////////////////////////////////////
//void SetPaletteCreationType( int nType )		    
//----------------------------------------------------------------------
//�������ܣ���ϣ��ͨ����ɫ��ԭ�õ�һ�����е�ɫ���ͼ��ʱ����������������
//			��ɫ��Ĵ������͡������������£�
//			#define POPULARITY_PALETTE	0
//			#define MEDIAN_CUT_PALETTE	1
//			#define FIXED_PALETTE		2
//----------------------------------------------------------------------
//����˵����int nType
//----------------------------------------------------------------------
//�� �� ֵ����
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
void CDibObject::SetPaletteCreationType( int nType )
{

	m_nPaletteCreationType = nType;

}

////////////////////////////////////////////////////////////////////////
//int GetPaletteCreationType( void )		    
//----------------------------------------------------------------------
//�������ܣ�����������������һ����ɫ��ԭ�����ĵ�ɫ�����͡�
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��int nPaletteCreationType
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
int CDibObject::GetPaletteCreationType( void )
{

	return( m_nPaletteCreationType );

}

RGBQUAD *CDibObject::MakePopularityPalette( int nNumColors, unsigned char *pBits, int nBits, int nWidth, int nHeight )
{
	RGBQUAD *pRGBQuad = new RGBQUAD[nNumColors];
	if( pRGBQuad == NULL ) return( MakeFixedPalette( nNumColors ) );
	memset( pRGBQuad, 0, nNumColors * sizeof( RGBQUAD ) );

	BYTE ColMap[256][3];
	if( !Popularity( pBits, nBits, nWidth, nHeight, ColMap ) ){
		delete [] pRGBQuad;
		return( MakeFixedPalette( nNumColors ) );
		}

	for( int i=0; i<nNumColors; i++ ){
		pRGBQuad[i].rgbRed = ColMap[i][0];
		pRGBQuad[i].rgbGreen = ColMap[i][1];
		pRGBQuad[i].rgbBlue = ColMap[i][2];
		}

	return( pRGBQuad );

}

RGBQUAD *CDibObject::MakeMedianCutPalette( int nNumColors, unsigned char *pBits, int nBits, int nWidth, int nHeight )
{
	RGBQUAD *pRGBQuad = new RGBQUAD[nNumColors];
	if( pRGBQuad == NULL ) return( MakeFixedPalette( nNumColors ) );
	memset( pRGBQuad, 0, nNumColors * sizeof( RGBQUAD ) );

	BYTE ColMap[256][3];
	WORD *Hist = new WORD[32768];
	if( Hist == NULL ){
		delete [] pRGBQuad;
		return( MakeFixedPalette( nNumColors ) );
		}
	memset( Hist, 0, 32768 * sizeof( WORD ) );

	int nWidthBytes = WidthBytes( nBits, nWidth );

	for( int y=0; y<nHeight; y++ ){
		unsigned char* pData = pBits;
		unsigned char ucRed = 0, ucGreen = 0, ucBlue = 0;
		WORD color;
		pData += ( y * nWidthBytes );
		for( int x=0; x<nWidth; x++ ){
			switch( nBits ){
				case 16:
					GETRGB555( ucRed, ucGreen, ucBlue, &pData[x*2] );
					break;
				case 24:
					ucRed = pData[x*3+2];
					ucGreen = pData[x*3+1];
					ucBlue = pData[x*3];
					break;
				case 32:
					GETRGB888( ucRed, ucGreen, ucBlue, &pData[x*4] );
					break;
				}
			color = _RGB( ucRed, ucGreen, ucBlue );
			if( Hist[color] < 65535 ) Hist[color]++;
			}
		}

	MedianCut( Hist, ColMap, (int) 256 );
	for( int i=0; i<nNumColors; i++ ){
		pRGBQuad[i].rgbRed = ColMap[i][0];
		pRGBQuad[i].rgbGreen = ColMap[i][1];
		pRGBQuad[i].rgbBlue = ColMap[i][2];
		}
	delete [] Hist;

	return( pRGBQuad );

}

RGBQUAD *CDibObject::MakeFixedPalette( int nNumColors )
{
	RGBQUAD *pRGBQuad = new RGBQUAD[nNumColors];
	if( pRGBQuad == NULL ) return( NULL );

	static int colors[] = {
		255, 255, 255,
		0, 0, 0,
		255, 0, 0,
		0, 0, 255,
		0, 255, 0,
		150, 150, 150,
		255, 255, 0,
		0, 150, 150,
		150, 0, 150,
		150, 150, 0,
		0, 255, 255,
		255, 0, 255,
		255, 120, 120,
		120, 255, 120,
		120, 120, 255,
		90, 90, 90 };

	int nSteps = ( ( nNumColors + 15 ) / 16 );

	for( int i=0; i<nSteps; i++ ){
		for( int j=0; j<16; j++ ){
			if( i * 16 + j < nNumColors ){
				int r, g, b;
				r = colors[j*3];
				g = colors[j*3+1];
				b = colors[j*3+2];
				r = ( ( nSteps - i ) * r ) / nSteps;
				g = ( ( nSteps - i ) * g ) / nSteps;
				b = ( ( nSteps - i ) * b ) / nSteps;
				pRGBQuad[i*16+j].rgbRed = (unsigned char) r;
				pRGBQuad[i*16+j].rgbGreen = (unsigned char) g;
				pRGBQuad[i*16+j].rgbBlue = (unsigned char) b;
				}
			}
		}

	return( pRGBQuad );

}

//�õ���ɫ���ֽ���
int CDibObject::GetPaletteBytes( void )
{

	return( m_nPaletteBytes );

}

////////////////////////////////////////////////////////////////////////
//HGLOBALBOOL GetDib( void )	    
//----------------------------------------------------------------------
//�������ܣ����������ص�ǰ�����ͼ��Dib�����
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��HGLOBALBOOL hDib
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
HGLOBAL CDibObject::GetDib( void )
{

	return( m_hDib );

}

////////////////////////////////////////////////////////////////////////
//CPalette GetPalette( void )	    
//----------------------------------------------------------------------
//�������ܣ�����������һ��ָ��CPalette�����ָ�롣
//----------------------------------------------------------------------
//����˵������
//----------------------------------------------------------------------
//�� �� ֵ��CPalette *Palette
//----------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////
CPalette *CDibObject::GetPalette( void )
{

	return( &m_Palette );

}

//�õ��߼���ɫ��
LOGPALETTE *CDibObject::GetLogPal( void )
{

	return( m_pLogPal );

}

void CDibObject::ProcImgHead()
{
	ProcessImageHeader();
}

char * CDibObject::GetImageName()
{
	return( m_pszFilename );
}
