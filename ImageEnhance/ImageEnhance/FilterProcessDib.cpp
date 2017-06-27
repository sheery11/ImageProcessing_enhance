#include "StdAfx.h"
#include "FilterProcessDib.h"
#include <complex>
#include <math.h>
using namespace std;

CFilterProcessDib::CFilterProcessDib(CDib *pDib)
{
	m_pDib = pDib;
}

//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CFilterProcessDib::~CFilterProcessDib(void)
{

}


VOID WINAPI FFT_1D(complex<double> * pCTData, complex<double> * pCFData, int nLevel)
{
	// 循环控制变量
	int		i;
	int     j;
	int     k;

	double PI = 3.1415926;

	// 傅立叶变换点数
	int	nCount = 0;

	// 计算傅立叶变换点数
	nCount = (int)pow(2, nLevel);

	// 某一级的长度
	int		nBtFlyLen;
	nBtFlyLen = 0;

	// 变换系数的角度 ＝2 * PI * i / nCount
	double	dAngle;

	complex<double> *pCW;

	// 分配内存，存储傅立叶变化需要的系数表
	pCW = new complex<double>[nCount / 2];

	// 计算傅立叶变换的系数
	for (i = 0; i < nCount / 2; i++)
	{
		dAngle = -2 * PI * i / nCount;
		pCW[i] = complex<double>(cos(dAngle), sin(dAngle));
	}

	// 变换需要的工作空间
	complex<double> *pCWork1, *pCWork2;

	// 分配工作空间
	pCWork1 = new complex<double>[nCount];

	pCWork2 = new complex<double>[nCount];


	// 临时变量
	complex<double> *pCTmp;

	// 初始化，写入数据
	memcpy(pCWork1, pCTData, sizeof(complex<double>) * nCount);

	// 临时变量
	int nInter;
	nInter = 0;

	// 蝶形算法进行快速傅立叶变换
	for (k = 0; k < nLevel; k++)
	{
		for (j = 0; j < (int)pow(2, k); j++)
		{
			//计算长度
			nBtFlyLen = (int)pow(2, (nLevel - k));

			//倒序重排，加权计算
			for (i = 0; i < nBtFlyLen / 2; i++)
			{
				nInter = j * nBtFlyLen;
				pCWork2[i + nInter] =
					pCWork1[i + nInter] + pCWork1[i + nInter + nBtFlyLen / 2];
				pCWork2[i + nInter + nBtFlyLen / 2] =
					(pCWork1[i + nInter] - pCWork1[i + nInter + nBtFlyLen / 2])
					* pCW[(int)(i * pow(2, k))];
			}
		}

		// 交换 pCWork1和pCWork2的数据
		pCTmp = pCWork1;
		pCWork1 = pCWork2;
		pCWork2 = pCTmp;
	}

	// 重新排序
	for (j = 0; j < nCount; j++)
	{
		nInter = 0;
		for (i = 0; i < nLevel; i++)
		{
			if (j&(1 << i))
			{
				nInter += 1 << (nLevel - i - 1);
			}
		}
		pCFData[j] = pCWork1[nInter];
	}

	// 释放内存空间
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW = NULL;
	pCWork1 = NULL;
	pCWork2 = NULL;

}


VOID WINAPI DIBFFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> * pCFData)
{

	// 循环控制变量
	int	x;
	int	y;

	// 临时变量
	double	dTmpOne;
	double  dTmpTwo;

	// 进行傅立叶变换的宽度和高度，（2的整数次幂）
	// 图像的宽度和高度不一定为2的整数次幂
	int		nTransWidth;
	int 	nTransHeight;

	// 计算进行傅立叶变换的宽度	（2的整数次幂）
	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// 计算进行傅立叶变换的高度 （2的整数次幂）
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// x，y（行列）方向上的迭代次数
	int		nXLev;
	int		nYLev;

	// 计算x，y（行列）方向上的迭代次数
	nXLev = (int)(log(nTransWidth) / log(2) + 0.5);
	nYLev = (int)(log(nTransHeight) / log(2) + 0.5);

	for (y = 0; y < nTransHeight; y++)
	{
		// x方向进行快速傅立叶变换
		FFT_1D(&pCTData[nTransWidth * y], &pCFData[nTransWidth * y], nXLev);
	}

	// pCFData中目前存储了pCTData经过行变换的结果
	// 为了直接利用FFT_1D，需要把pCFData的二维数据转置，再一次利用FFT_1D进行
	// 傅立叶行变换（实际上相当于对列进行傅立叶变换）
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransHeight * x + y] = pCFData[nTransWidth * y + x];
		}
	}

	for (x = 0; x < nTransWidth; x++)
	{
		// 对x方向进行快速傅立叶变换，实际上相当于对原来的图象数据进行列方向的
		// 傅立叶变换
		FFT_1D(&pCTData[x * nTransHeight], &pCFData[x * nTransHeight], nYLev);
	}

	// pCFData中目前存储了pCTData经过二维傅立叶变换的结果，但是为了方便列方向
	// 的傅立叶变换，对其进行了转置，现在把结果转置回来
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransWidth * y + x] = pCFData[nTransHeight * x + y];
		}
	}

	memcpy(pCTData, pCFData, sizeof(complex<double>) * nTransHeight * nTransWidth);
}


VOID WINAPI IFFT_2D(complex<double> * pCFData, complex<double> * pCTData, int nWidth, int nHeight)
{
	// 循环控制变量
	int	x;
	int	y;

	// 临时变量
	double	dTmpOne;
	double  dTmpTwo;

	// 进行傅立叶变换的宽度和高度，（2的整数次幂）
	// 图像的宽度和高度不一定为2的整数次幂
	int		nTransWidth;
	int 	nTransHeight;

	// 计算进行傅立叶变换的宽度	（2的整数次幂）
	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// 计算进行傅立叶变换的高度 （2的整数次幂）
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// 分配工作需要的内存空间
	complex<double> *pCWork = new complex<double>[nTransWidth * nTransHeight];

	//临时变量
	complex<double> *pCTmp;

	// 为了利用傅立叶正变换,可以把傅立叶频域的数据取共轭
	// 然后直接利用正变换，输出结果就是傅立叶反变换结果的共轭
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCFData[nTransWidth * y + x];
			pCWork[nTransWidth * y + x] = complex<double>(pCTmp->real(), -pCTmp->imag());
		}
	}

	// 调用傅立叶正变换
	::DIBFFT_2D(pCWork, nWidth, nHeight, pCTData);

	// 求时域点的共轭，求得最终结果
	// 根据傅立叶变换原理，利用这样的方法求得的结果和实际的时域数据
	// 相差一个系数
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCTData[nTransWidth * y + x];
			pCTData[nTransWidth * y + x] =
				complex<double>(pCTmp->real() / (nTransWidth*nTransHeight),
					-pCTmp->imag() / (nTransWidth*nTransHeight));
		}
	}
	delete pCWork;
	pCWork = NULL;
}

void LowPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// 循环控制变量
	int y;
	int x;

	double dTmpOne;
	double dTmpTwo;

	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth;
	int nTransHeight;

	// 图象象素值
	unsigned char unchValue;

	// 指向时域数据的指针
	complex<double> * pCTData;
	// 指向频域数据的指针
	complex<double> * pCFData;

	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// 傅立叶变换的实部和虚部
	double dReal;
	double dImag;

	// 低通滤波的半径不能超过频域的最大半径
	if (nRadius>nTransWidth - 1 || nRadius>nTransHeight - 1)
	{
		return;
	}

	// 分配内存
	pCTData = new complex<double>[nTransWidth * nTransHeight];
	pCFData = new complex<double>[nTransWidth * nTransHeight];

	// 初始化
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData
	// 有一部分数据需要补0
	for (y = 0; y<nTransHeight; y++)
	{
		for (x = 0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x] = complex<double>(0, 0);
		}
	}

	// 把图像数据传给pCTData
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth + x];
			pCTData[y*nTransWidth + x] = complex<double>(unchValue, 0);
		}
	}

	// 傅立叶正变换
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData);

	// 下面开始实施低通滤波，把所有大于nRadius的高频分量设置为0
	//  注意这里高频分量采用的范数不是欧式距离，而是无穷大范数
	//  || (u,v)-(0,0) || = max(|u|,|v|) 
	for (y = nRadius; y<nTransHeight; y++)
	{
		for (x = nRadius; x<nTransWidth; x++)
		{
			pCFData[y*nTransWidth + x] = complex<double>(0, 0);
		}
	}

	// 经过低通滤波的图象进行反变换
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// 反变换的数据传给lpImage
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			dReal = pCTData[y*nTransWidth + x].real();
			dImag = pCTData[y*nTransWidth + x].imag();
			unchValue = (unsigned char)max(0, min(255, sqrt(dReal*dReal + dImag*dImag)));
			lpImage[y*nWidth + x] = unchValue;
		}
	}


	// 释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;

}


BOOL CFilterProcessDib::ThoughtfulLowFilter() {  //理想低通滤波
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;
	long i, j;			//循环变量
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth = m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight = m_pDib->GetHeight();  //获得原图像的高度

										 // 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight];
	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
	memset(lpDst, (BYTE)0, lWidth * lHeight);
	for (j = 0; j < lHeight; j++) {
		for (i = 0; i < lWidth; i++) {
			//lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			
			pUnchImage[j*lWidth +i] = m_pDib->m_lpData[j*nSaveWidth + i];
			//lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
		}
	}

	LowPassFilterEnhance(pUnchImage, lWidth, lHeight, lWidth / 16);
	// 增强以后的图象拷贝到pDib中，进行显示
	for (j = 0; j<lHeight; j++)
	{
		for (i = 0; i<lWidth; i++)
		{
			m_pDib->m_lpData[j*nSaveWidth + i] = pUnchImage[j*lWidth +i];
		}
	}

	// 释放内存
	delete[]pUnchImage;
	pUnchImage = NULL;
	// 复制变换后的图像
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 恢复光标形状

	return TRUE;
}


BOOL CFilterProcessDib::ColorThoughtfulLowFilter() {  //理想彩色图像低通滤波
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;
	long i, j;			//循环变量
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth = m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight = m_pDib->GetHeight();  //获得原图像的高度

										 // 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight*3);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight*3];
	// 初始化新分配的内存，设定初始值为0
	//lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
	//memset(lpDst, (BYTE)0, lWidth * lHeight);
	for (j = 0; j < lHeight; j++) {
		for (i = 0; i < lWidth; i++) {
			//lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;

			pUnchImage[j*lWidth*3 + i] = m_pDib->m_lpData[j*nSaveWidth + i];
			//lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
		}
	}

	LowPassFilterEnhance(pUnchImage, lWidth*3, lHeight, lWidth / 16);
	// 增强以后的图象拷贝到pDib中，进行显示
	for (j = 0; j<lHeight; j++)
	{
		for (i = 0; i<lWidth; i++)
		{
			m_pDib->m_lpData[j*nSaveWidth + i] = pUnchImage[j*lWidth*3 + i];
		}
	}

	// 释放内存
	delete[]pUnchImage;
	pUnchImage = NULL;
	// 复制变换后的图像
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// 恢复光标形状

	return TRUE;
}

void ButterWorthLowPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius) 
{
	// 循环控制变量
	int y;
	int x;

	double dTmpOne;
	double dTmpTwo;

	// ButterWorth 滤波系数
	double H;

	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth;
	int nTransHeight;

	double dReal;
	double dImag;
	// 图象象素值
	unsigned char unchValue;

	// 指向时域数据的指针
	complex<double> * pCTData;
	// 指向频域数据的指针
	complex<double> * pCFData;

	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// 分配内存
	pCTData = new complex<double>[nTransWidth * nTransHeight];
	pCFData = new complex<double>[nTransWidth * nTransHeight];

	// 初始化
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData
	// 有一部分数据需要补0
	for (y = 0; y<nTransHeight; y++)
	{
		for (x = 0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x] = complex<double>(0, 0);
		}
	}

	// 把图像数据传给pCTData
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth + x];
			pCTData[y*nTransWidth + x] = complex<double>(unchValue, 0);
		}
	}

	// 傅立叶正变换
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData);

	// 下面开始实施ButterWorth低通滤波
	for (y = 0; y<nTransHeight; y++)
	{
		for (x = 0; x<nTransWidth; x++)
		{
			H = (double)(y*y + x*x);
			H = H / (nRadius * nRadius);
			H = 1 / (1 + H);
			pCFData[y*nTransWidth + x] = complex<double>(pCFData[y*nTransWidth + x].real()*H,
				pCFData[y*nTransWidth + x].imag()*H);
		}
	}

	// 经过ButterWorth低通滤波的图象进行反变换
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// 反变换的数据传给lpImage
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			dReal = pCTData[y*nTransWidth + x].real();
			dImag = pCTData[y*nTransWidth + x].imag();
			unchValue = (unsigned char)max(0, min(255, sqrt(dReal*dReal + dImag*dImag)));
			lpImage[y*nWidth + x] = unchValue;
		}
	}


	// 释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;

}




BOOL CFilterProcessDib::ButtwothLowFilter() {  //巴特沃斯低通
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;
	long i, j;			//循环变量
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth = m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight = m_pDib->GetHeight();  //获得原图像的高度

										 // 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight];
	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	for (j = 0; j < lHeight; j++) {
		for (i = 0; i < lWidth; i++) {
			pUnchImage[j*lWidth + i] = m_pDib->m_lpData[j*nSaveWidth + i];
		}
	}

	ButterWorthLowPass(pUnchImage, lWidth, lHeight, lWidth / 2);
	for (j = 0; j<lHeight; j++)
	{
		for (i = 0; i<lWidth; i++)
		{
			m_pDib->m_lpData[j*nSaveWidth + i] = pUnchImage[j*lWidth + i];
		}
	}

	// 释放内存
	delete[]pUnchImage;
	pUnchImage = NULL;
	// 复制变换后的图像
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	return TRUE;
}



void HighPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)//理想高通函数
{
	// 循环控制变量
	int y;
	int x;

	double dTmpOne;
	double dTmpTwo;

	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth;
	int nTransHeight;

	// 图象象素值
	unsigned char unchValue;

	// 指向时域数据的指针
	complex<double> * pCTData;
	// 指向频域数据的指针
	complex<double> * pCFData;

	double dReal;
	double dImag;

	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// 滤波的半径不能超过频域的最大半径
	if (nRadius>nTransWidth - 1 || nRadius>nTransHeight - 1)
	{
		return;
	}

	// 分配内存
	pCTData = new complex<double>[nTransWidth * nTransHeight];
	pCFData = new complex<double>[nTransWidth * nTransHeight];

	// 初始化
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData
	// 有一部分数据需要补0
	for (y = 0; y<nTransHeight; y++)
	{
		for (x = 0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x] = complex<double>(0, 0);
		}
	}

	// 把图像数据传给pCTData
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth + x];
			pCTData[y*nTransWidth + x] = complex<double>(unchValue, 0);
		}
	}

	// 傅立叶正变换
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData);

	// 下面开始滤波，把所有小于nRadius的低频分量设置为0
	// 采用的范数不是欧式距离，而是无穷大范数
	//  || (u,v)-(0,0) || = max(|u|,|v|) 
	for (y = 0; y<nRadius; y++)
	{
		for (x = 0; x<nRadius; x++)
		{
			pCFData[y*nTransWidth + x] = complex<double>(0, 0);
		}
	}
	// 经过滤波的图象进行反变换
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// 反变换的数据传给lpImage
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			// 因为高通滤波器会把低频成分去掉，所以图象看起来会比较暗，为了解决这个问题
			// 在经过增强后的图象加上一个常数，使图象变得亮起来。严格来讲这种处理相当于
			// 增加了一些低频分量（在频率(0,0)）。不过如果不加一个常数，高通效果在观看
			// 上就不是很方便。
			dReal = pCTData[y*nTransWidth + x].real();
			dImag = pCTData[y*nTransWidth + x].imag();
			unchValue = (unsigned char)max(0, min(255, (sqrt(dReal*dReal + dImag*dImag) + 100)));
			lpImage[y*nWidth + x] = unchValue;
		}
	}


	// 释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;

}

BOOL CFilterProcessDib::ThoughtfulHighFilter() {
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;
	long i, j;			//循环变量
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth = m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight = m_pDib->GetHeight();  //获得原图像的高度

										 // 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight];
	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	for (j = 0; j < lHeight; j++) {
		for (i = 0; i < lWidth; i++) {
			pUnchImage[j*lWidth + i] = m_pDib->m_lpData[j*nSaveWidth + i];
		}
	}

	HighPassFilterEnhance(pUnchImage, lWidth, lHeight, 50);
	for (j = 0; j<lHeight; j++)
	{
		for (i = 0; i<lWidth; i++)
		{
			m_pDib->m_lpData[j*nSaveWidth + i] = pUnchImage[j*lWidth + i];
		}
	}

	// 释放内存
	delete[]pUnchImage;
	pUnchImage = NULL;
	// 复制变换后的图像
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}


void ButterWorthHighPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// 循环控制变量
	int y;
	int x;

	double dTmpOne;
	double dTmpTwo;

	// ButterWorth 滤波系数
	double H;

	// 傅立叶变换的宽度和高度（2的整数次幂）
	int nTransWidth;
	int nTransHeight;

	double dReal;
	double dImag;
	// 图象象素值
	unsigned char unchValue;

	// 指向时域数据的指针
	complex<double> * pCTData;
	// 指向频域数据的指针
	complex<double> * pCFData;

	// 计算进行傅立叶变换的点数	（2的整数次幂）
	dTmpOne = log(nWidth*3) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// 计算进行傅立叶变换的点数 （2的整数次幂）
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// 分配内存
	pCTData = new complex<double>[nTransWidth * nTransHeight*3];
	pCFData = new complex<double>[nTransWidth * nTransHeight*3];

	// 初始化
	// 图像数据的宽和高不一定是2的整数次幂，所以pCTData
	// 有一部分数据需要补0
	for (y = 0; y<nTransHeight; y++)
	{
		for (x = 0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth*3 + x] = complex<double>(0, 0);
		}
	}

	// 把图像数据传给pCTData
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth*3 + x];
			pCTData[y*nTransWidth + x] = complex<double>(unchValue, 0);
		}
	}

	// 傅立叶正变换
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData);

	// 下面开始实施ButterWorth高通滤波
	for (y = 0; y<nTransHeight; y++)
	{
		for (x = 0; x<nTransWidth; x++)
		{

			H = (double)(y*y + x*x);
			H = (nRadius * nRadius) / H;
			H = 1 / (1 + H);
			pCFData[y*nTransWidth + x] = complex<double>(H*(pCFData[y*nTransWidth + x].real()),
				H*(pCFData[y*nTransWidth + x].imag()));
		}
	}

	// 经过ButterWorth高通滤波的图象进行反变换
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// 反变换的数据传给lpImage
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			dReal = pCTData[y*nTransWidth + x].real();
			dImag = pCTData[y*nTransWidth+ x].imag();
			unchValue = (unsigned char)max(0, min(255, sqrt(dReal*dReal + dImag*dImag) + 100));
			lpImage[y*nWidth*3 + x] = unchValue;
		}
	}


	// 释放内存
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;

}

BOOL CFilterProcessDib::ButtworthHighFilter() {
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;
	long i, j;			//循环变量
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth = m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight = m_pDib->GetHeight();  //获得原图像的高度

										 // 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;  //256
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight];
	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	for (j = 0; j < lHeight; j++) {
		for (i = 0; i < lWidth; i++) {
			pUnchImage[j*lWidth + i] = m_pDib->m_lpData[j*nSaveWidth + i];
		}
	}

	ButterWorthHighPass(pUnchImage, lWidth, lHeight, lWidth/2);
	for (j = 0; j<lHeight; j++)
	{
		for (i = 0; i<lWidth; i++)
		{
			m_pDib->m_lpData[j*nSaveWidth + i] = pUnchImage[j*lWidth + i];
		}
	}

	// 释放内存
	delete[]pUnchImage;
	pUnchImage = NULL;
	// 复制变换后的图像
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}


BOOL CFilterProcessDib::ColorButtworthHighFilter() {
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;
	long i, j;			//循环变量
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth = m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight = m_pDib->GetHeight();  //获得原图像的高度

										 // 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight*3);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;  //768
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight*3];
	// 初始化新分配的内存，设定初始值为0
	//lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
	//memset(lpDst, (BYTE)0, lWidth * lHeight*3);

	for (j = 0; j < lHeight; j++) {
		for (i = 0; i < lWidth; i++) {
			pUnchImage[j*lWidth*3 + i] = m_pDib->m_lpData[j*nSaveWidth + i];
		}
	}

	ButterWorthHighPass(pUnchImage, lWidth, lHeight, lWidth / 2 *3);

	for (j = 0; j<lHeight; j++)
	{
		for (i = 0; i<lWidth; i++)
		{
			m_pDib->m_lpData[j*nSaveWidth + i] = pUnchImage[j*lWidth*3 + i];
		}
	}

	// 释放内存
	delete[]pUnchImage;
	pUnchImage = NULL;
	// 复制变换后的图像
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}