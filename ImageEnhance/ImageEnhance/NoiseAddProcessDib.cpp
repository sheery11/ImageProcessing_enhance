#include "StdAfx.h"
#include "NoiseAddProcessDib.h"
#include <time.h>

CNoiseAddProcessDib::CNoiseAddProcessDib(CDib *pDib)
{
	m_pDib = pDib;
}

//=======================================================
// �������ܣ� ��������
// ��������� ��
// ����ֵ��   ��
//=======================================================

CNoiseAddProcessDib::~CNoiseAddProcessDib(void)
{

}


//��������
BOOL CNoiseAddProcessDib::SaltNoiseDIB() {
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

	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
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


	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}

//��ɫͼ��������
BOOL CNoiseAddProcessDib::ColorSaltNoiseDIB() {
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

	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
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


	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight*3);
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}




//��˹����
BOOL CNoiseAddProcessDib::GauseNoiseDIB() {
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

	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
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


	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);


	return TRUE;
}


//��ɫ��˹����
BOOL CNoiseAddProcessDib::ColorGauseNoiseDIB(){
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;
	long i, j;			//ѭ������

	LPBYTE lpDIBBits = m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth = m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight = m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight*3);
	if (hNewDIBBits == NULL)
	{
		return FALSE;
	}
	lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
	lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
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

	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight*3);
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);


	return TRUE;
}