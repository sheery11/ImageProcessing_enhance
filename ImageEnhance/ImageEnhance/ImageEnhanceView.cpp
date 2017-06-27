
// ImageEnhanceView.cpp : CImageEnhanceView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "ImageEnhance.h"
#include <complex>
#include <math.h>
#endif
using namespace std;
#include "ImageEnhanceDoc.h"
#include "ImageEnhanceView.h"
#include "DlgAverage.h"
#include "DlgMedian.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define N 100

// CImageEnhanceView

IMPLEMENT_DYNCREATE(CImageEnhanceView, CView)

BEGIN_MESSAGE_MAP(CImageEnhanceView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_OPENPIC, &CImageEnhanceView::OnOpenpic)
	ON_COMMAND(ID_OPENJPG, &CImageEnhanceView::OnOpenjpg)
	ON_COMMAND(ID_thoutfulLow, &CImageEnhanceView::Onthoutfullow)
	ON_COMMAND(ID_MIDDLE_SMOOTH, &CImageEnhanceView::OnMiddleSmooth)
	ON_COMMAND(ID_AVERAGE_SMOOTH, &CImageEnhanceView::OnAverageSmooth)
	ON_COMMAND(ID_RestoreSaltnoise, &CImageEnhanceView::OnRestoresaltnoise)
	ON_COMMAND(ID_GauseNoise, &CImageEnhanceView::OnGausenoise)
	ON_COMMAND(ID_ButtLow, &CImageEnhanceView::OnButtlow)
	ON_COMMAND(ID_ThoughtfulHigh, &CImageEnhanceView::OnThoughtfulhigh)
	ON_COMMAND(ID_ButtHighFilter, &CImageEnhanceView::OnButthighfilter)
END_MESSAGE_MAP()

// CImageEnhanceView ����/����

CImageEnhanceView::CImageEnhanceView()
{
	// TODO: �ڴ˴���ӹ������

}

CImageEnhanceView::~CImageEnhanceView()
{
}

BOOL CImageEnhanceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CImageEnhanceView ����

void CImageEnhanceView::OnDraw(CDC* /*pDC*/)
{
	CImageEnhanceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CImageEnhanceView ��ӡ

BOOL CImageEnhanceView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CImageEnhanceView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CImageEnhanceView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CImageEnhanceView ���

#ifdef _DEBUG
void CImageEnhanceView::AssertValid() const
{
	CView::AssertValid();
}

void CImageEnhanceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageEnhanceDoc* CImageEnhanceView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageEnhanceDoc)));
	return (CImageEnhanceDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageEnhanceView ��Ϣ�������


void CImageEnhanceView::OnOpenpic()
{
	// TODO: �ڴ���������������
	// �����������Ǵ򿪲���ʾ��Ƭͼ��
    CString  filename;
    CFileDialog dlg(TRUE,_T("BMP"),_T("*.BMP"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("λͼ�ļ�(*.BMP)|*.BMP|"));	
    if(IDOK==dlg.DoModal())	
    filename.Format ("%s",dlg.GetPathName() ); 
   
    m_Image.LoadFromFile(filename);//����Ƭͼ��
    flag = TRUE;//��Ƭͼ��򿪺󣬽���Ǳ�����Ϊ��

	//��ʾͼ��
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_Image.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight=m_Image.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_Image.Draw(pDC,point,size);
}


float w[N] = {0};//
float h[N] = {0};//
int index = 0;

BOOL CImageEnhanceView::ShowImageOn(CDC* pDC,CString strPath, int x, int y)//pDC,·��,�������,���
{  
    IPicture *pPic=NULL; 
	//����jpg,gif,bmp��������ʽ��ͼƬ
    OleLoadPicturePath(CComBSTR(strPath.GetBuffer()), (LPUNKNOWN)NULL, 0, 0, IID_IPicture,(LPVOID*)&pPic);
    if (NULL==pPic)
    {
        return FALSE;
    }

    //��ȡͼ���͸�
    OLE_XSIZE_HIMETRIC hmWidth;  
    OLE_YSIZE_HIMETRIC hmHeight;  
    pPic->get_Width(&hmWidth);  
    pPic->get_Height(&hmHeight); 
	//a[index++] = hmWidth*1.0f;//170415add
	float b = (hmHeight*1.0f)/(hmWidth*1.0f);//170414add �߿��

   // ��ȡ��ʾͼƬ���ڵĿ�Ⱥ͸߶�
   RECT rtWnd;
   pDC->GetWindow()->GetWindowRect(&rtWnd);
   float iWndWidth = (rtWnd.right-rtWnd.left)*1.0f/4.0f;
   float iWndHeight = b*iWndWidth*1.0f;

   w[index+1] = w[index]+iWndWidth*1.0f;//170415
   h[index+1] = h[index]+iWndHeight*1.0f;//170415
   index++;

   if(FAILED(pPic->Render(*pDC,x,y,iWndWidth,iWndHeight,0,hmHeight,hmWidth,-hmHeight,NULL))) //��ʾͼƬ 
   {
       pPic->Release();
       return false;
   }

   pPic->Release(); 
   return true;
}

CString  file = 0;//����ͼƬ
void CImageEnhanceView::OnOpenjpg()
{
	// TODO: �ڴ���������������
	CString  filename;
	CFileDialog dlg(TRUE,"jpg","*.jpg", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
        "JPEG�ļ�(*.jpg)|*.jpg|GIF�ļ�(*.gif)|*.gif|bmp�ļ�(*.bmp)|*.bmp|",NULL); 
    if(dlg.DoModal()==IDOK) 
    { 
		filename.Format ("%s",dlg.GetPathName() );
		file = filename;
		CDC *pDC=GetDC();
        ShowImageOn(pDC,filename,0,0);
    } 
}





void CImageEnhanceView::Onthoutfullow()  //�����ͨ�˲�
{
	// TODO: �ڴ���������������
		CImageEnhanceDoc * pDoc = (CImageEnhanceDoc *)this->GetDocument();

		if (flag == FALSE)
		{
			AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
			return;
		}
		CPoint point;
		CSize size;
		point.x = 0;
		point.y = 0;
		LONG lWidth = m_Image.GetWidth();    //��ûҶ�ͼ��Ŀ��
		LONG lHeight = m_Image.GetHeight();  //��ûҶ�ͼ��ĸ߶�
		size.cx = lWidth;
		size.cy = lHeight;
		CDC *pDC = GetDC();
		m_Image.Draw(pDC, point, size);//���ƴ���ǰ��ͼ��
		CFilterProcessDib Filter(&m_Image);
		if (m_Image.GetBitCount() == 8) {
			Filter.ThoughtfulLowFilter();
		}
		else if (m_Image.GetBitCount() == 24) {
			Filter.ColorThoughtfulLowFilter();
		}

		CPoint point1;
		point1.x = lWidth + 20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
		point1.y = 0;
		m_Image.Draw(pDC, point1, size);//���ƴ�����ͼ��
}


void CImageEnhanceView::OnMiddleSmooth()
{
	// TODO: �ڴ���������������

	//���֮ǰû����ͼ�񣬽���������ʾ
	if (flag == FALSE)
	{
		AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
		return;
	}
	// �˲����ĸ߶�
	int nFilterHeight;

	// �˲����Ŀ��
	int nFilterWidth;

	// ����Ԫ�ص�X����
	int nFilterCenX;

	// ����Ԫ�ص�Y����
	int nFilterCenY;

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight = m_Image.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//���ƴ���ǰ��ͼ��

								   // �����Ի���
	CDlgMedian dlgMedian;

	// ��ʼ������ֵ
	dlgMedian.miFilter_Height = 3;
	dlgMedian.miFilter_Width = 3;
	dlgMedian.m_nFilterCenX = 1;
	dlgMedian.m_nFilterCenY = 1;

	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgMedian.DoModal() != IDOK)
	{
		// ����
		return;
	}

	// ��ȡ�û����趨
	nFilterHeight = dlgMedian.miFilter_Height;
	nFilterWidth = dlgMedian.miFilter_Width;
	nFilterCenX = dlgMedian.m_nFilterCenX;
	nFilterCenY = dlgMedian.m_nFilterCenY;

	// ɾ���Ի���
	delete dlgMedian;


	CSmoothProcessDib Smooth(&m_Image);//����һ��CSmoothProcessDib����         
	Smooth.Middle_Smooth(m_Image.GetBitCount(), nFilterWidth,
		nFilterHeight, nFilterCenX, nFilterCenY); //������ֵ�˲�ƽ������

	CPoint point1;
	point1.x = lWidth + 20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//���ƴ�����ͼ��
}


void CImageEnhanceView::OnAverageSmooth()  //��ֵ�˲�
{
	// TODO: �ڴ���������������

	//���֮ǰû����ͼ�񣬽���������ʾ
	if (flag == FALSE)
	{
		AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
		return;
	}
	// �˲����ĸ߶�
	int nFilterHeight;

	// �˲����Ŀ��
	int nFilterWidth;

	// ����Ԫ�ص�X����
	int nFilterCenX;

	// ����Ԫ�ص�Y����
	int nFilterCenY;
	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight = m_Image.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//���ƴ���ǰ��ͼ��
								   // �����Ի���
	DlgAverage dlgAverage;

	// ��ʼ������ֵ
	dlgAverage.m_nSmthTemHeight = 3;
	dlgAverage.m_nSmthTemWidth = 3;
	dlgAverage.m_nSmthTemCenX = 1;
	dlgAverage.m_nSmthTemCenY = 1;

	// ��ʾ�Ի�����ʾ�û��趨ƽ����
	if (dlgAverage.DoModal() != IDOK)
	{
		// ����
		return;
	}

	// ��ȡ�û����趨
	nFilterHeight = dlgAverage.m_nSmthTemHeight;
	nFilterWidth = dlgAverage.m_nSmthTemWidth;
	nFilterCenX = dlgAverage.m_nSmthTemCenX;
	nFilterCenY = dlgAverage.m_nSmthTemCenY;

	// ɾ���Ի���
	delete dlgAverage;
	CSmoothProcessDib Smooth(&m_Image);//����һ��CSmoothProcessDib����   
	//if (m_Image.GetBitCount() == 8) {
		Smooth.Average_Smooth(m_Image.GetBitCount(), nFilterWidth,
			nFilterHeight, nFilterCenX, nFilterCenY);  //��������ƽ��ƽ������
	//}
	

	CPoint point1;
	point1.x = lWidth + 20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//���ƴ�����ͼ��
}


void CImageEnhanceView::OnRestoresaltnoise()   //��������
{
	// TODO: �ڴ���������������
	if (flag == FALSE)
	{
		AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
		return;
	}

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight = m_Image.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//���ƴ���ǰ��ͼ��

	CNoiseAddProcessDib Noise(&m_Image);//����һ��CSmoothProcessDib����    
	if (m_Image.GetBitCount() == 8) {
		Noise.SaltNoiseDIB();
	}
	else if (m_Image.GetBitCount() == 24) {
		//AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
		Noise.ColorSaltNoiseDIB();

	}

	CPoint point1;
	point1.x = lWidth + 20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//���ƴ�����ͼ��
}


void CImageEnhanceView::OnGausenoise()
{
	// TODO: �ڴ���������������
	if (flag == FALSE)
	{
		AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
		return;
	}

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight = m_Image.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//���ƴ���ǰ��ͼ��

	CNoiseAddProcessDib Noise(&m_Image);//����һ��CSmoothProcessDib����    
	if (m_Image.GetBitCount() == 8) {
		Noise.GauseNoiseDIB();
	}
	else if (m_Image.GetBitCount() == 24) {
		//AfxMessageBox("ֻ�ܴ���8λ�Ҷ�ͼ��!");
		Noise.ColorGauseNoiseDIB();

	}
	     
	 

	CPoint point1;
	point1.x = lWidth + 20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//���ƴ�����ͼ��
}


void CImageEnhanceView::OnButtlow()
{
	// TODO: �ڴ���������������
	if (flag == FALSE)
	{
		AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
		return;
	}

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight = m_Image.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//���ƴ���ǰ��ͼ��
	CFilterProcessDib Filter(&m_Image);
	if (m_Image.GetBitCount() == 8) {
		Filter.ButtwothLowFilter();
	}


	CPoint point1;
	point1.x = lWidth + 20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//���ƴ�����ͼ��
}


void CImageEnhanceView::OnThoughtfulhigh() //�����ͨ
{
	// TODO: �ڴ���������������
	if (flag == FALSE)
	{
		AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
		return;
	}

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight = m_Image.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//���ƴ���ǰ��ͼ��
	CFilterProcessDib Filter(&m_Image);
	if (m_Image.GetBitCount() == 8) {
		Filter.ThoughtfulHighFilter();
	}


	CPoint point1;
	point1.x = lWidth + 20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//���ƴ�����ͼ��
}




void CImageEnhanceView::OnButthighfilter() //������˹��ͨ�˲�
{
	// TODO: �ڴ���������������
	if (flag == FALSE)
	{
		AfxMessageBox("���ȼ���Ҫ�������Ƭͼ��");
		return;
	}

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //��ûҶ�ͼ��Ŀ��
	LONG lHeight = m_Image.GetHeight();  //��ûҶ�ͼ��ĸ߶�
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//���ƴ���ǰ��ͼ��
	CFilterProcessDib Filter(&m_Image);
	if (m_Image.GetBitCount() == 8) {
		Filter.ButtworthHighFilter();
	}
	else if (m_Image.GetBitCount() == 24) {
		Filter.ColorButtworthHighFilter();
	}

	CPoint point1;
	point1.x = lWidth + 20;  //+20��Ϊ��������ͼ����ʾʱ�и���϶
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//���ƴ�����ͼ��
}
