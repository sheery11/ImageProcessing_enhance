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
// �������ܣ� ��������
// ��������� ��
// ����ֵ��   ��
//=======================================================

CFilterProcessDib::~CFilterProcessDib(void)
{

}


VOID WINAPI FFT_1D(complex<double> * pCTData, complex<double> * pCFData, int nLevel)
{
	// ѭ�����Ʊ���
	int		i;
	int     j;
	int     k;

	double PI = 3.1415926;

	// ����Ҷ�任����
	int	nCount = 0;

	// ���㸵��Ҷ�任����
	nCount = (int)pow(2, nLevel);

	// ĳһ���ĳ���
	int		nBtFlyLen;
	nBtFlyLen = 0;

	// �任ϵ���ĽǶ� ��2 * PI * i / nCount
	double	dAngle;

	complex<double> *pCW;

	// �����ڴ棬�洢����Ҷ�仯��Ҫ��ϵ����
	pCW = new complex<double>[nCount / 2];

	// ���㸵��Ҷ�任��ϵ��
	for (i = 0; i < nCount / 2; i++)
	{
		dAngle = -2 * PI * i / nCount;
		pCW[i] = complex<double>(cos(dAngle), sin(dAngle));
	}

	// �任��Ҫ�Ĺ����ռ�
	complex<double> *pCWork1, *pCWork2;

	// ���乤���ռ�
	pCWork1 = new complex<double>[nCount];

	pCWork2 = new complex<double>[nCount];


	// ��ʱ����
	complex<double> *pCTmp;

	// ��ʼ����д������
	memcpy(pCWork1, pCTData, sizeof(complex<double>) * nCount);

	// ��ʱ����
	int nInter;
	nInter = 0;

	// �����㷨���п��ٸ���Ҷ�任
	for (k = 0; k < nLevel; k++)
	{
		for (j = 0; j < (int)pow(2, k); j++)
		{
			//���㳤��
			nBtFlyLen = (int)pow(2, (nLevel - k));

			//�������ţ���Ȩ����
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

		// ���� pCWork1��pCWork2������
		pCTmp = pCWork1;
		pCWork1 = pCWork2;
		pCWork2 = pCTmp;
	}

	// ��������
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

	// �ͷ��ڴ�ռ�
	delete pCW;
	delete pCWork1;
	delete pCWork2;
	pCW = NULL;
	pCWork1 = NULL;
	pCWork2 = NULL;

}


VOID WINAPI DIBFFT_2D(complex<double> * pCTData, int nWidth, int nHeight, complex<double> * pCFData)
{

	// ѭ�����Ʊ���
	int	x;
	int	y;

	// ��ʱ����
	double	dTmpOne;
	double  dTmpTwo;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���2���������ݣ�
	// ͼ��Ŀ�Ⱥ͸߶Ȳ�һ��Ϊ2����������
	int		nTransWidth;
	int 	nTransHeight;

	// ������и���Ҷ�任�Ŀ��	��2���������ݣ�
	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ�
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// x��y�����У������ϵĵ�������
	int		nXLev;
	int		nYLev;

	// ����x��y�����У������ϵĵ�������
	nXLev = (int)(log(nTransWidth) / log(2) + 0.5);
	nYLev = (int)(log(nTransHeight) / log(2) + 0.5);

	for (y = 0; y < nTransHeight; y++)
	{
		// x������п��ٸ���Ҷ�任
		FFT_1D(&pCTData[nTransWidth * y], &pCFData[nTransWidth * y], nXLev);
	}

	// pCFData��Ŀǰ�洢��pCTData�����б任�Ľ��
	// Ϊ��ֱ������FFT_1D����Ҫ��pCFData�Ķ�ά����ת�ã���һ������FFT_1D����
	// ����Ҷ�б任��ʵ�����൱�ڶ��н��и���Ҷ�任��
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTData[nTransHeight * x + y] = pCFData[nTransWidth * y + x];
		}
	}

	for (x = 0; x < nTransWidth; x++)
	{
		// ��x������п��ٸ���Ҷ�任��ʵ�����൱�ڶ�ԭ����ͼ�����ݽ����з����
		// ����Ҷ�任
		FFT_1D(&pCTData[x * nTransHeight], &pCFData[x * nTransHeight], nYLev);
	}

	// pCFData��Ŀǰ�洢��pCTData������ά����Ҷ�任�Ľ��������Ϊ�˷����з���
	// �ĸ���Ҷ�任�����������ת�ã����ڰѽ��ת�û���
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
	// ѭ�����Ʊ���
	int	x;
	int	y;

	// ��ʱ����
	double	dTmpOne;
	double  dTmpTwo;

	// ���и���Ҷ�任�Ŀ�Ⱥ͸߶ȣ���2���������ݣ�
	// ͼ��Ŀ�Ⱥ͸߶Ȳ�һ��Ϊ2����������
	int		nTransWidth;
	int 	nTransHeight;

	// ������и���Ҷ�任�Ŀ��	��2���������ݣ�
	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// ������и���Ҷ�任�ĸ߶� ��2���������ݣ�
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// ���乤����Ҫ���ڴ�ռ�
	complex<double> *pCWork = new complex<double>[nTransWidth * nTransHeight];

	//��ʱ����
	complex<double> *pCTmp;

	// Ϊ�����ø���Ҷ���任,���԰Ѹ���ҶƵ�������ȡ����
	// Ȼ��ֱ���������任�����������Ǹ���Ҷ���任����Ĺ���
	for (y = 0; y < nTransHeight; y++)
	{
		for (x = 0; x < nTransWidth; x++)
		{
			pCTmp = &pCFData[nTransWidth * y + x];
			pCWork[nTransWidth * y + x] = complex<double>(pCTmp->real(), -pCTmp->imag());
		}
	}

	// ���ø���Ҷ���任
	::DIBFFT_2D(pCWork, nWidth, nHeight, pCTData);

	// ��ʱ���Ĺ��������ս��
	// ���ݸ���Ҷ�任ԭ�����������ķ�����õĽ����ʵ�ʵ�ʱ������
	// ���һ��ϵ��
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
	// ѭ�����Ʊ���
	int y;
	int x;

	double dTmpOne;
	double dTmpTwo;

	// ����Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth;
	int nTransHeight;

	// ͼ������ֵ
	unsigned char unchValue;

	// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCTData;
	// ָ��Ƶ�����ݵ�ָ��
	complex<double> * pCFData;

	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// ����Ҷ�任��ʵ�����鲿
	double dReal;
	double dImag;

	// ��ͨ�˲��İ뾶���ܳ���Ƶ������뾶
	if (nRadius>nTransWidth - 1 || nRadius>nTransHeight - 1)
	{
		return;
	}

	// �����ڴ�
	pCTData = new complex<double>[nTransWidth * nTransHeight];
	pCFData = new complex<double>[nTransWidth * nTransHeight];

	// ��ʼ��
	// ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData
	// ��һ����������Ҫ��0
	for (y = 0; y<nTransHeight; y++)
	{
		for (x = 0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x] = complex<double>(0, 0);
		}
	}

	// ��ͼ�����ݴ���pCTData
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth + x];
			pCTData[y*nTransWidth + x] = complex<double>(unchValue, 0);
		}
	}

	// ����Ҷ���任
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData);

	// ���濪ʼʵʩ��ͨ�˲��������д���nRadius�ĸ�Ƶ��������Ϊ0
	//  ע�������Ƶ�������õķ�������ŷʽ���룬�����������
	//  || (u,v)-(0,0) || = max(|u|,|v|) 
	for (y = nRadius; y<nTransHeight; y++)
	{
		for (x = nRadius; x<nTransWidth; x++)
		{
			pCFData[y*nTransWidth + x] = complex<double>(0, 0);
		}
	}

	// ������ͨ�˲���ͼ����з��任
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// ���任�����ݴ���lpImage
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


	// �ͷ��ڴ�
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;

}


BOOL CFilterProcessDib::ThoughtfulLowFilter() {  //�����ͨ�˲�
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;
	long i, j;			//ѭ������
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth = m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight = m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

										 // ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight];
	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
	memset(lpDst, (BYTE)0, lWidth * lHeight);
	for (j = 0; j < lHeight; j++) {
		for (i = 0; i < lWidth; i++) {
			//lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
			
			pUnchImage[j*lWidth +i] = m_pDib->m_lpData[j*nSaveWidth + i];
			//lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
		}
	}

	LowPassFilterEnhance(pUnchImage, lWidth, lHeight, lWidth / 16);
	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
	for (j = 0; j<lHeight; j++)
	{
		for (i = 0; i<lWidth; i++)
		{
			m_pDib->m_lpData[j*nSaveWidth + i] = pUnchImage[j*lWidth +i];
		}
	}

	// �ͷ��ڴ�
	delete[]pUnchImage;
	pUnchImage = NULL;
	// ���Ʊ任���ͼ��
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// �ָ������״

	return TRUE;
}


BOOL CFilterProcessDib::ColorThoughtfulLowFilter() {  //�����ɫͼ���ͨ�˲�
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;
	long i, j;			//ѭ������
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth = m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight = m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

										 // ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight*3);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight*3];
	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	//lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
	//memset(lpDst, (BYTE)0, lWidth * lHeight);
	for (j = 0; j < lHeight; j++) {
		for (i = 0; i < lWidth; i++) {
			//lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;

			pUnchImage[j*lWidth*3 + i] = m_pDib->m_lpData[j*nSaveWidth + i];
			//lpSrc = (LPBYTE)lpDIBBits + lWidth * j + i;
		}
	}

	LowPassFilterEnhance(pUnchImage, lWidth*3, lHeight, lWidth / 16);
	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
	for (j = 0; j<lHeight; j++)
	{
		for (i = 0; i<lWidth; i++)
		{
			m_pDib->m_lpData[j*nSaveWidth + i] = pUnchImage[j*lWidth*3 + i];
		}
	}

	// �ͷ��ڴ�
	delete[]pUnchImage;
	pUnchImage = NULL;
	// ���Ʊ任���ͼ��
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	// �ָ������״

	return TRUE;
}

void ButterWorthLowPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius) 
{
	// ѭ�����Ʊ���
	int y;
	int x;

	double dTmpOne;
	double dTmpTwo;

	// ButterWorth �˲�ϵ��
	double H;

	// ����Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth;
	int nTransHeight;

	double dReal;
	double dImag;
	// ͼ������ֵ
	unsigned char unchValue;

	// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCTData;
	// ָ��Ƶ�����ݵ�ָ��
	complex<double> * pCFData;

	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// �����ڴ�
	pCTData = new complex<double>[nTransWidth * nTransHeight];
	pCFData = new complex<double>[nTransWidth * nTransHeight];

	// ��ʼ��
	// ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData
	// ��һ����������Ҫ��0
	for (y = 0; y<nTransHeight; y++)
	{
		for (x = 0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x] = complex<double>(0, 0);
		}
	}

	// ��ͼ�����ݴ���pCTData
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth + x];
			pCTData[y*nTransWidth + x] = complex<double>(unchValue, 0);
		}
	}

	// ����Ҷ���任
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData);

	// ���濪ʼʵʩButterWorth��ͨ�˲�
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

	// ����ButterWorth��ͨ�˲���ͼ����з��任
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// ���任�����ݴ���lpImage
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


	// �ͷ��ڴ�
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;

}




BOOL CFilterProcessDib::ButtwothLowFilter() {  //������˹��ͨ
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;
	long i, j;			//ѭ������
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth = m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight = m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

										 // ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight];
	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
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

	// �ͷ��ڴ�
	delete[]pUnchImage;
	pUnchImage = NULL;
	// ���Ʊ任���ͼ��
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	return TRUE;
}



void HighPassFilterEnhance(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)//�����ͨ����
{
	// ѭ�����Ʊ���
	int y;
	int x;

	double dTmpOne;
	double dTmpTwo;

	// ����Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth;
	int nTransHeight;

	// ͼ������ֵ
	unsigned char unchValue;

	// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCTData;
	// ָ��Ƶ�����ݵ�ָ��
	complex<double> * pCFData;

	double dReal;
	double dImag;

	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// �˲��İ뾶���ܳ���Ƶ������뾶
	if (nRadius>nTransWidth - 1 || nRadius>nTransHeight - 1)
	{
		return;
	}

	// �����ڴ�
	pCTData = new complex<double>[nTransWidth * nTransHeight];
	pCFData = new complex<double>[nTransWidth * nTransHeight];

	// ��ʼ��
	// ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData
	// ��һ����������Ҫ��0
	for (y = 0; y<nTransHeight; y++)
	{
		for (x = 0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth + x] = complex<double>(0, 0);
		}
	}

	// ��ͼ�����ݴ���pCTData
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth + x];
			pCTData[y*nTransWidth + x] = complex<double>(unchValue, 0);
		}
	}

	// ����Ҷ���任
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData);

	// ���濪ʼ�˲���������С��nRadius�ĵ�Ƶ��������Ϊ0
	// ���õķ�������ŷʽ���룬�����������
	//  || (u,v)-(0,0) || = max(|u|,|v|) 
	for (y = 0; y<nRadius; y++)
	{
		for (x = 0; x<nRadius; x++)
		{
			pCFData[y*nTransWidth + x] = complex<double>(0, 0);
		}
	}
	// �����˲���ͼ����з��任
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// ���任�����ݴ���lpImage
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			// ��Ϊ��ͨ�˲�����ѵ�Ƶ�ɷ�ȥ��������ͼ��������Ƚϰ���Ϊ�˽���������
			// �ھ�����ǿ���ͼ�����һ��������ʹͼ�������������ϸ��������ִ����൱��
			// ������һЩ��Ƶ��������Ƶ��(0,0)���������������һ����������ͨЧ���ڹۿ�
			// �ϾͲ��Ǻܷ��㡣
			dReal = pCTData[y*nTransWidth + x].real();
			dImag = pCTData[y*nTransWidth + x].imag();
			unchValue = (unsigned char)max(0, min(255, (sqrt(dReal*dReal + dImag*dImag) + 100)));
			lpImage[y*nWidth + x] = unchValue;
		}
	}


	// �ͷ��ڴ�
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;

}

BOOL CFilterProcessDib::ThoughtfulHighFilter() {
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;
	long i, j;			//ѭ������
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth = m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight = m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

										 // ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight];
	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
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

	// �ͷ��ڴ�
	delete[]pUnchImage;
	pUnchImage = NULL;
	// ���Ʊ任���ͼ��
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}


void ButterWorthHighPass(LPBYTE lpImage, int nWidth, int nHeight, int nRadius)
{
	// ѭ�����Ʊ���
	int y;
	int x;

	double dTmpOne;
	double dTmpTwo;

	// ButterWorth �˲�ϵ��
	double H;

	// ����Ҷ�任�Ŀ�Ⱥ͸߶ȣ�2���������ݣ�
	int nTransWidth;
	int nTransHeight;

	double dReal;
	double dImag;
	// ͼ������ֵ
	unsigned char unchValue;

	// ָ��ʱ�����ݵ�ָ��
	complex<double> * pCTData;
	// ָ��Ƶ�����ݵ�ָ��
	complex<double> * pCFData;

	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
	dTmpOne = log(nWidth*3) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransWidth = (int)dTmpTwo;

	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
	dTmpOne = log(nHeight) / log(2);
	dTmpTwo = ceil(dTmpOne);
	dTmpTwo = pow(2, dTmpTwo);
	nTransHeight = (int)dTmpTwo;

	// �����ڴ�
	pCTData = new complex<double>[nTransWidth * nTransHeight*3];
	pCFData = new complex<double>[nTransWidth * nTransHeight*3];

	// ��ʼ��
	// ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData
	// ��һ����������Ҫ��0
	for (y = 0; y<nTransHeight; y++)
	{
		for (x = 0; x<nTransWidth; x++)
		{
			pCTData[y*nTransWidth*3 + x] = complex<double>(0, 0);
		}
	}

	// ��ͼ�����ݴ���pCTData
	for (y = 0; y<nHeight; y++)
	{
		for (x = 0; x<nWidth; x++)
		{
			unchValue = lpImage[y*nWidth*3 + x];
			pCTData[y*nTransWidth + x] = complex<double>(unchValue, 0);
		}
	}

	// ����Ҷ���任
	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData);

	// ���濪ʼʵʩButterWorth��ͨ�˲�
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

	// ����ButterWorth��ͨ�˲���ͼ����з��任
	IFFT_2D(pCFData, pCTData, nWidth, nHeight);

	// ���任�����ݴ���lpImage
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


	// �ͷ��ڴ�
	delete pCTData;
	delete pCFData;
	pCTData = NULL;
	pCFData = NULL;

}

BOOL CFilterProcessDib::ButtworthHighFilter() {
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;
	long i, j;			//ѭ������
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth = m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight = m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

										 // ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;  //256
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight];
	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
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

	// �ͷ��ڴ�
	delete[]pUnchImage;
	pUnchImage = NULL;
	// ���Ʊ任���ͼ��
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}


BOOL CFilterProcessDib::ColorButtworthHighFilter() {
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;
	long i, j;			//ѭ������
						//int RAND_MAX = 100;
	LPBYTE lpDIBBits = m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth = m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight = m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

										 // ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight*3);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	int nSaveWidth = m_pDib->GetDibSaveDim().cx;  //768
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
	unsigned char * pUnchImage = new unsigned char[lWidth*lHeight*3];
	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	//lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
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

	// �ͷ��ڴ�
	delete[]pUnchImage;
	pUnchImage = NULL;
	// ���Ʊ任���ͼ��
	//memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}