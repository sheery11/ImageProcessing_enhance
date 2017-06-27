#include "StdAfx.h"
#include "NoiseAddProcessDib.h"
#include <time.h>

CNoiseAddProcessDib::CNoiseAddProcessDib(CDib *pDib)
{
	m_pDib = pDib;
}

//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CNoiseAddProcessDib::~CNoiseAddProcessDib(void)
{

}


//椒盐噪声
BOOL CNoiseAddProcessDib::SaltNoiseDIB() {
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

	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	srand((unsigned)time(NULL));
	// if (m_nBitCount == 8)
	
	for (j = 0; j < lHeight; j++) {
		for (i = 0; i < lWidth; i++) {
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			if (double(rand()) / RAND_MAX > 0.98) {
				
				*lpDst = 255;
			}
			else if(double(rand()) / RAND_MAX < 0.04){
				*lpDst = 0;
				
			}
			else {
				*lpDst = *lpSrc;
			}
			
		}
	}


	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}

//彩色图像椒盐噪声
BOOL CNoiseAddProcessDib::ColorSaltNoiseDIB() {
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

	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
	memset(lpDst, (BYTE)0, lWidth * lHeight*3);

	srand((unsigned)time(NULL));
	// if (m_nBitCount == 8)

	for (j = 0; j < lHeight; j++) {
		lpDst = (LPBYTE)lpNewDIBBits + lWidth * j * 3;
		lpSrc = (LPBYTE)lpDIBBits + lWidth * j * 3;
		for (i = 0; i < lWidth; i++) {
			//lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			//lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
			if (rand() > 32000) {
				*(lpDst++) = (rand()) % 255;
				*(lpDst++) = (rand()) % 255;
				*(lpDst++) = (rand()) % 255;
			}
			else if (rand() < 200) {
				*(lpDst++) = (rand()) % 255;
				*(lpDst++) = (rand()) % 255;
				*(lpDst++) = (rand()) % 255;
			}
			else {
				*(lpDst++) = *(lpSrc++);
				*(lpDst++) = *(lpSrc++);
				*(lpDst++) = *(lpSrc++);
			}

		}
	}


	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight*3);
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}




//高斯噪声
BOOL CNoiseAddProcessDib::GauseNoiseDIB() {
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

	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
	memset(lpDst, (BYTE)0, lWidth * lHeight);

	srand((unsigned)time(NULL));
	int rand_temp;
	int level = 50;

	for (j = 0; j < lHeight; j++) {
		for (i = 0; i < lWidth; i++) {
			rand_temp = rand() % level ;
			lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;

			*lpDst = *lpSrc+ rand_temp - 10;
			if (*lpDst > 255) {

				*lpDst = 255;  
			}
			else if (*lpDst < 0) {
				*lpDst = 0;

			}
			/*else {
				*lpDst = *lpSrc;
			}*/

		}
	}


	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);


	return TRUE;
}


//彩色高斯噪声
BOOL CNoiseAddProcessDib::ColorGauseNoiseDIB(){
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;
	long i, j;			//循环变量

	LPBYTE lpDIBBits = m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth = m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight = m_pDib->GetHeight();  //获得原图像的高度

	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight*3);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

	// 初始化新分配的内存，设定初始值为0
	lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
	memset(lpDst, (BYTE)0, lWidth * lHeight*3);
	srand((unsigned)time(NULL));
	int rand_temp;
	int level = 50;

	for (j = 0; j < lHeight; j++) {
		lpDst = (LPBYTE)lpNewDIBBits + lWidth * j * 3;
		lpSrc = (LPBYTE)lpDIBBits + lWidth * j * 3;
		for (i = 0; i < lWidth; i++) {
			rand_temp = rand() % level;


			*(lpDst++) = *(lpSrc++) + rand_temp - 10;
			*(lpDst++) = *(lpSrc++) + rand_temp - 10;
			*(lpDst++) = *(lpSrc++) + rand_temp - 10;
			if (*lpDst > 255) {

				*lpDst = 255;
			}
			else if (*lpDst < 0) {
				*lpDst = 0;

			}
			/*else {
			*lpDst = *lpSrc;
			}*/

		}
	}

	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight*3);
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);


	return TRUE;
}