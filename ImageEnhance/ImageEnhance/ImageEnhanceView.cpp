
// ImageEnhanceView.cpp : CImageEnhanceView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
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

// CImageEnhanceView 构造/析构

CImageEnhanceView::CImageEnhanceView()
{
	// TODO: 在此处添加构造代码

}

CImageEnhanceView::~CImageEnhanceView()
{
}

BOOL CImageEnhanceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CImageEnhanceView 绘制

void CImageEnhanceView::OnDraw(CDC* /*pDC*/)
{
	CImageEnhanceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CImageEnhanceView 打印

BOOL CImageEnhanceView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImageEnhanceView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImageEnhanceView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CImageEnhanceView 诊断

#ifdef _DEBUG
void CImageEnhanceView::AssertValid() const
{
	CView::AssertValid();
}

void CImageEnhanceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageEnhanceDoc* CImageEnhanceView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageEnhanceDoc)));
	return (CImageEnhanceDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageEnhanceView 消息处理程序


void CImageEnhanceView::OnOpenpic()
{
	// TODO: 在此添加命令处理程序代码
	// 本函数功能是打开并显示照片图像
    CString  filename;
    CFileDialog dlg(TRUE,_T("BMP"),_T("*.BMP"),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("位图文件(*.BMP)|*.BMP|"));	
    if(IDOK==dlg.DoModal())	
    filename.Format ("%s",dlg.GetPathName() ); 
   
    m_Image.LoadFromFile(filename);//打开照片图像
    flag = TRUE;//照片图像打开后，将标记变量置为真

	//显示图像
	CPoint point;
	CSize size;
	point.x=0;
	point.y=0;
	LONG lWidth=m_Image.GetWidth();    //获得灰度图像的宽度
	LONG lHeight=m_Image.GetHeight();  //获得灰度图像的高度
	size.cx= lWidth	;
	size.cy= lHeight;
	CDC *pDC=GetDC();
	m_Image.Draw(pDC,point,size);
}


float w[N] = {0};//
float h[N] = {0};//
int index = 0;

BOOL CImageEnhanceView::ShowImageOn(CDC* pDC,CString strPath, int x, int y)//pDC,路径,起点坐标,宽高
{  
    IPicture *pPic=NULL; 
	//读入jpg,gif,bmp等少数格式的图片
    OleLoadPicturePath(CComBSTR(strPath.GetBuffer()), (LPUNKNOWN)NULL, 0, 0, IID_IPicture,(LPVOID*)&pPic);
    if (NULL==pPic)
    {
        return FALSE;
    }

    //获取图像宽和高
    OLE_XSIZE_HIMETRIC hmWidth;  
    OLE_YSIZE_HIMETRIC hmHeight;  
    pPic->get_Width(&hmWidth);  
    pPic->get_Height(&hmHeight); 
	//a[index++] = hmWidth*1.0f;//170415add
	float b = (hmHeight*1.0f)/(hmWidth*1.0f);//170414add 高宽比

   // 获取显示图片窗口的宽度和高度
   RECT rtWnd;
   pDC->GetWindow()->GetWindowRect(&rtWnd);
   float iWndWidth = (rtWnd.right-rtWnd.left)*1.0f/4.0f;
   float iWndHeight = b*iWndWidth*1.0f;

   w[index+1] = w[index]+iWndWidth*1.0f;//170415
   h[index+1] = h[index]+iWndHeight*1.0f;//170415
   index++;

   if(FAILED(pPic->Render(*pDC,x,y,iWndWidth,iWndHeight,0,hmHeight,hmWidth,-hmHeight,NULL))) //显示图片 
   {
       pPic->Release();
       return false;
   }

   pPic->Release(); 
   return true;
}

CString  file = 0;//传递图片
void CImageEnhanceView::OnOpenjpg()
{
	// TODO: 在此添加命令处理程序代码
	CString  filename;
	CFileDialog dlg(TRUE,"jpg","*.jpg", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, 
        "JPEG文件(*.jpg)|*.jpg|GIF文件(*.gif)|*.gif|bmp文件(*.bmp)|*.bmp|",NULL); 
    if(dlg.DoModal()==IDOK) 
    { 
		filename.Format ("%s",dlg.GetPathName() );
		file = filename;
		CDC *pDC=GetDC();
        ShowImageOn(pDC,filename,0,0);
    } 
}





void CImageEnhanceView::Onthoutfullow()  //理想低通滤波
{
	// TODO: 在此添加命令处理程序代码
		CImageEnhanceDoc * pDoc = (CImageEnhanceDoc *)this->GetDocument();

		if (flag == FALSE)
		{
			AfxMessageBox("请先加载要处理的照片图像！");
			return;
		}
		CPoint point;
		CSize size;
		point.x = 0;
		point.y = 0;
		LONG lWidth = m_Image.GetWidth();    //获得灰度图像的宽度
		LONG lHeight = m_Image.GetHeight();  //获得灰度图像的高度
		size.cx = lWidth;
		size.cy = lHeight;
		CDC *pDC = GetDC();
		m_Image.Draw(pDC, point, size);//绘制处理前的图像
		CFilterProcessDib Filter(&m_Image);
		if (m_Image.GetBitCount() == 8) {
			Filter.ThoughtfulLowFilter();
		}
		else if (m_Image.GetBitCount() == 24) {
			Filter.ColorThoughtfulLowFilter();
		}

		CPoint point1;
		point1.x = lWidth + 20;  //+20是为了让两个图像显示时有个间隙
		point1.y = 0;
		m_Image.Draw(pDC, point1, size);//绘制处理后的图像
}


void CImageEnhanceView::OnMiddleSmooth()
{
	// TODO: 在此添加命令处理程序代码

	//如果之前没加载图像，将在这里提示
	if (flag == FALSE)
	{
		AfxMessageBox("请先加载要处理的照片图像！");
		return;
	}
	// 滤波器的高度
	int nFilterHeight;

	// 滤波器的宽度
	int nFilterWidth;

	// 中心元素的X坐标
	int nFilterCenX;

	// 中心元素的Y坐标
	int nFilterCenY;

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //获得灰度图像的宽度
	LONG lHeight = m_Image.GetHeight();  //获得灰度图像的高度
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//绘制处理前的图像

								   // 创建对话框
	CDlgMedian dlgMedian;

	// 初始化变量值
	dlgMedian.miFilter_Height = 3;
	dlgMedian.miFilter_Width = 3;
	dlgMedian.m_nFilterCenX = 1;
	dlgMedian.m_nFilterCenY = 1;

	// 显示对话框，提示用户设定平移量
	if (dlgMedian.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户的设定
	nFilterHeight = dlgMedian.miFilter_Height;
	nFilterWidth = dlgMedian.miFilter_Width;
	nFilterCenX = dlgMedian.m_nFilterCenX;
	nFilterCenY = dlgMedian.m_nFilterCenY;

	// 删除对话框
	delete dlgMedian;


	CSmoothProcessDib Smooth(&m_Image);//创建一个CSmoothProcessDib对象         
	Smooth.Middle_Smooth(m_Image.GetBitCount(), nFilterWidth,
		nFilterHeight, nFilterCenX, nFilterCenY); //调用中值滤波平滑函数

	CPoint point1;
	point1.x = lWidth + 20;  //+20是为了让两个图像显示时有个间隙
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//绘制处理后的图像
}


void CImageEnhanceView::OnAverageSmooth()  //均值滤波
{
	// TODO: 在此添加命令处理程序代码

	//如果之前没加载图像，将在这里提示
	if (flag == FALSE)
	{
		AfxMessageBox("请先加载要处理的照片图像！");
		return;
	}
	// 滤波器的高度
	int nFilterHeight;

	// 滤波器的宽度
	int nFilterWidth;

	// 中心元素的X坐标
	int nFilterCenX;

	// 中心元素的Y坐标
	int nFilterCenY;
	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //获得灰度图像的宽度
	LONG lHeight = m_Image.GetHeight();  //获得灰度图像的高度
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//绘制处理前的图像
								   // 创建对话框
	DlgAverage dlgAverage;

	// 初始化变量值
	dlgAverage.m_nSmthTemHeight = 3;
	dlgAverage.m_nSmthTemWidth = 3;
	dlgAverage.m_nSmthTemCenX = 1;
	dlgAverage.m_nSmthTemCenY = 1;

	// 显示对话框，提示用户设定平移量
	if (dlgAverage.DoModal() != IDOK)
	{
		// 返回
		return;
	}

	// 获取用户的设定
	nFilterHeight = dlgAverage.m_nSmthTemHeight;
	nFilterWidth = dlgAverage.m_nSmthTemWidth;
	nFilterCenX = dlgAverage.m_nSmthTemCenX;
	nFilterCenY = dlgAverage.m_nSmthTemCenY;

	// 删除对话框
	delete dlgAverage;
	CSmoothProcessDib Smooth(&m_Image);//创建一个CSmoothProcessDib对象   
	//if (m_Image.GetBitCount() == 8) {
		Smooth.Average_Smooth(m_Image.GetBitCount(), nFilterWidth,
			nFilterHeight, nFilterCenX, nFilterCenY);  //调用邻域平均平滑函数
	//}
	

	CPoint point1;
	point1.x = lWidth + 20;  //+20是为了让两个图像显示时有个间隙
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//绘制处理后的图像
}


void CImageEnhanceView::OnRestoresaltnoise()   //椒盐噪声
{
	// TODO: 在此添加命令处理程序代码
	if (flag == FALSE)
	{
		AfxMessageBox("请先加载要处理的照片图像！");
		return;
	}

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //获得灰度图像的宽度
	LONG lHeight = m_Image.GetHeight();  //获得灰度图像的高度
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//绘制处理前的图像

	CNoiseAddProcessDib Noise(&m_Image);//创建一个CSmoothProcessDib对象    
	if (m_Image.GetBitCount() == 8) {
		Noise.SaltNoiseDIB();
	}
	else if (m_Image.GetBitCount() == 24) {
		//AfxMessageBox("只能处理8位灰度图像!");
		Noise.ColorSaltNoiseDIB();

	}

	CPoint point1;
	point1.x = lWidth + 20;  //+20是为了让两个图像显示时有个间隙
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//绘制处理后的图像
}


void CImageEnhanceView::OnGausenoise()
{
	// TODO: 在此添加命令处理程序代码
	if (flag == FALSE)
	{
		AfxMessageBox("请先加载要处理的照片图像！");
		return;
	}

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //获得灰度图像的宽度
	LONG lHeight = m_Image.GetHeight();  //获得灰度图像的高度
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//绘制处理前的图像

	CNoiseAddProcessDib Noise(&m_Image);//创建一个CSmoothProcessDib对象    
	if (m_Image.GetBitCount() == 8) {
		Noise.GauseNoiseDIB();
	}
	else if (m_Image.GetBitCount() == 24) {
		//AfxMessageBox("只能处理8位灰度图像!");
		Noise.ColorGauseNoiseDIB();

	}
	     
	 

	CPoint point1;
	point1.x = lWidth + 20;  //+20是为了让两个图像显示时有个间隙
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//绘制处理后的图像
}


void CImageEnhanceView::OnButtlow()
{
	// TODO: 在此添加命令处理程序代码
	if (flag == FALSE)
	{
		AfxMessageBox("请先加载要处理的照片图像！");
		return;
	}

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //获得灰度图像的宽度
	LONG lHeight = m_Image.GetHeight();  //获得灰度图像的高度
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//绘制处理前的图像
	CFilterProcessDib Filter(&m_Image);
	if (m_Image.GetBitCount() == 8) {
		Filter.ButtwothLowFilter();
	}


	CPoint point1;
	point1.x = lWidth + 20;  //+20是为了让两个图像显示时有个间隙
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//绘制处理后的图像
}


void CImageEnhanceView::OnThoughtfulhigh() //理想高通
{
	// TODO: 在此添加命令处理程序代码
	if (flag == FALSE)
	{
		AfxMessageBox("请先加载要处理的照片图像！");
		return;
	}

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //获得灰度图像的宽度
	LONG lHeight = m_Image.GetHeight();  //获得灰度图像的高度
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//绘制处理前的图像
	CFilterProcessDib Filter(&m_Image);
	if (m_Image.GetBitCount() == 8) {
		Filter.ThoughtfulHighFilter();
	}


	CPoint point1;
	point1.x = lWidth + 20;  //+20是为了让两个图像显示时有个间隙
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//绘制处理后的图像
}




void CImageEnhanceView::OnButthighfilter() //巴特沃斯高通滤波
{
	// TODO: 在此添加命令处理程序代码
	if (flag == FALSE)
	{
		AfxMessageBox("请先加载要处理的照片图像！");
		return;
	}

	CPoint point;
	CSize size;
	point.x = 0;
	point.y = 0;
	LONG lWidth = m_Image.GetWidth();    //获得灰度图像的宽度
	LONG lHeight = m_Image.GetHeight();  //获得灰度图像的高度
	size.cx = lWidth;
	size.cy = lHeight;
	CDC *pDC = GetDC();
	m_Image.Draw(pDC, point, size);//绘制处理前的图像
	CFilterProcessDib Filter(&m_Image);
	if (m_Image.GetBitCount() == 8) {
		Filter.ButtworthHighFilter();
	}
	else if (m_Image.GetBitCount() == 24) {
		Filter.ColorButtworthHighFilter();
	}

	CPoint point1;
	point1.x = lWidth + 20;  //+20是为了让两个图像显示时有个间隙
	point1.y = 0;
	m_Image.Draw(pDC, point1, size);//绘制处理后的图像
}
