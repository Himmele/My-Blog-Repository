// cdib.h Deklaration der Klasse CDib aus dem Buch Inside Visual C++

#ifndef _INSIDE_VISUAL_CPP_CDIB
#define _INSIDE_VISUAL_CPP_CDIB

class CDib : public CObject
{
	enum Alloc {noAlloc, crtAlloc, heapAlloc};
	DECLARE_SERIAL(CDib)
public:
	LPVOID m_lpvColorTable;
	HBITMAP m_hBitmap;
	LPBYTE m_lpImage;  // Startadresse der DIB-Bits
	LPBITMAPINFOHEADER m_lpBMIH; //  Puffer für BITMAPINFOHEADER
private:
	HGLOBAL m_hGlobal; // muß für externe Fenster freigegeben werden;
	                   //  kann in dieser Klasse oder extern alloziert werden
	Alloc m_nBmihAlloc;
	Alloc m_nImageAlloc;
	DWORD m_dwSizeImage; // die eigentlichen Bilddaten ohne BITMAPINFOHEADER oder BITMAPFILEHEADER
	int m_nColorTableEntries;
	
	HANDLE m_hFile;
	HANDLE m_hMap;
	LPVOID m_lpvFile;
	HPALETTE m_hPalette;
public:
	CDib();
	CDib(CSize size, int nBitCount);	// erstellt BITMAPINFOHEADER
	~CDib();
	int GetSizeImage() {return m_dwSizeImage;}
	int GetSizeHeader()
		{return sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * m_nColorTableEntries;}
	CSize GetDimensions();
	BOOL AttachMapFile(const char* strPathname, BOOL bShare = FALSE);
	BOOL CopyToMapFile(const char* strPathname);
	BOOL AttachMemory(LPVOID lpvMem, BOOL bMustDelete = FALSE, HGLOBAL hGlobal = NULL);
	BOOL Draw(CDC* pDC, CPoint origin, CSize size);  // bis wir CreateDibSection implementieren
	HBITMAP CreateSection(CDC* pDC = NULL);
	UINT UsePalette(CDC* pDC, BOOL bBackground = FALSE);
	BOOL MakePalette();
	BOOL SetSystemPalette(CDC* pDC);
	BOOL Compress(CDC* pDC, BOOL bCompress = TRUE); // FALSE heißt Dekomprimieren
	HBITMAP CreateBitmap(CDC* pDC);
	BOOL Read(CFile* pFile);
	BOOL ReadSection(CFile* pFile, CDC* pDC = NULL);
	BOOL Write(CFile* pFile);
	void Serialize(CArchive& ar);
	void Empty();
private:
	void DetachMapFile();
	void ComputePaletteSize(int nBitCount);
	void ComputeMetrics();
};
#endif // _INSIDE_VISUAL_CPP_CDIB
