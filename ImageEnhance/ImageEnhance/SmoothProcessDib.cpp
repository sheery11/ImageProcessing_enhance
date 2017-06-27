#include "StdAfx.h"
#include "SmoothProcessDib.h"



//=======================================================
// �������ܣ� ���캯������ʼ�����ݳ�Ա
// ��������� λͼָ��
// ����ֵ��   ��
//=======================================================
CSmoothProcessDib::CSmoothProcessDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// �������ܣ� ��������
// ��������� ��
// ����ֵ��   ��
//=======================================================

CSmoothProcessDib::~CSmoothProcessDib(void)
{

}


//=======================================================
// �������ܣ� ����ƽ��ƽ������
// ��������� ��
// ����ֵ��   �ɹ�����TRUE�����򷵻�FALSE
//=======================================================
BOOL CSmoothProcessDib::Average_Smooth(int bitcount,int nTempWidth, int nTempHeight, int nTempCenX, int nTempCenY)
{
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;
	long i, j;			//ѭ������
	long k, l;    //�ڲ�ѭ������
	BYTE average;	//�����ֵ����
	BYTE average_r, average_g, average_b;
	FLOAT sum;
	FLOAT sum_r, sum_g, sum_b;
	//int Moban = 3;
	LPBYTE lpDIBBits = m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth = m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight = m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

										 // ��ʱ�����ڴ棬�Ա�����ͼ��
	if (bitcount == 8) {
		hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
		if (hNewDIBBits == NULL)
		{
			return FALSE;
		}

		lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

		// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
		lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
		memset(lpDst, (BYTE)0, lWidth * lHeight);

		//�ڴ���ӹؼ�����
		//���ɨ��ͼ���е����ص�,���������ֵ
		for (j = nTempCenY; j < lHeight - nTempHeight + nTempCenY + 1; j++)
		{
			for (i = nTempCenX; i < lWidth - nTempWidth + nTempCenX + 1; i++)
			{

				lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
				average = 0;
				sum = 0;

				for (k = 0; k < nTempHeight; k++)
				{
					for (l = 0; l < nTempWidth; l++)
					{

						lpSrc = (LPBYTE)lpDIBBits + lWidth * (j - nTempCenY + k) + i - nTempCenX + l;

						sum += *lpSrc;
					}
				}

				sum = (float)fabs(sum);
				average = (sum) / (nTempWidth*nTempHeight*1.0f);
				if (average > 255)
				{
					*lpDst = 255;
				}
				else
				{
					*lpDst = (BYTE)(average+0.5);
				}
			}
		}
		// ���Ʊ任���ͼ��
		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	}
	else if (bitcount == 24) {
		hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight*3);
		if (hNewDIBBits == NULL)
		{
			return FALSE;
		}

		lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
		// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
		lpDst = (LPBYTE)lpNewDIBBits;  //����ָ��Ŀ��ͼ���j�У���i�����ص�ָ��
		memset(lpDst, (BYTE)0, lWidth * lHeight*3);
		for (j = nTempCenY; j < lHeight - nTempHeight + nTempCenY + 1; j++)
		{
			//Ӧ��ָ���һ�е�һ��
			for (i = nTempCenX; i < lWidth *3 - nTempWidth + nTempCenX + 1; i++)
			{
				lpDst = (LPBYTE)lpNewDIBBits + lWidth * j * 3 + i;
				sum_b = 0;
				sum_g = 0;
				sum_r = 0;
				average_r = 0;
				average_g = 0;
				average_b = 0;
				for (k = 0; k < nTempHeight; k++)
				{
					  //��i֮���ǻҶ�ͼ���������Ͻ�  ����i�ǲ�ɫͼ���Ǻ���
					for (l = 0; l < nTempWidth; l++)
					{
						lpSrc = (LPBYTE)lpDIBBits + lWidth *(j - nTempCenY + k) * 3 + i ;
						sum_b += *(lpSrc++);
						sum_g += *(lpSrc++);
						sum_r += *(lpSrc++);

					}
				}		
				sum_r = (float)fabs(sum_r);
				sum_g = (float)fabs(sum_g);
				sum_b = (float)fabs(sum_b);

                average_b = (sum_b) / (nTempWidth*nTempHeight*1.0f);
				average_g = (sum_g) / (nTempWidth*nTempHeight*1.0f);
				average_r = (sum_r) / (nTempWidth*nTempHeight*1.0f);
			  


				//if (average_b > 255)
				//{
				//	average_b = 255;
				//}
				//else
				//{
				//	average_b = (BYTE)average_b;
				//}

				//if (average_g > 255)
				//{
				//	average_g = 255;
				//}
				//else
				//{
				//	average_g = (BYTE)average_g;
				//}

				//if (average_r > 255)
				//{
				//	average_r = 255;
				//}
				//else
				//{
				//	average_r = (BYTE)average_r;
				//}
				*(lpDst++) = (BYTE)average_b;
				*(lpDst++) = (BYTE)average_g;
				*(lpDst++) = (BYTE)average_r;
			}
		}

		

		// ���Ʊ任���ͼ��
		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight*3);

	}

	
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;


}

//=======================================================
// �������ܣ� ��ֵ�˲�ƽ������
// ��������� ��
// ����ֵ��   �ɹ�����TRUE�����򷵻�FALSE
//=======================================================
BOOL CSmoothProcessDib::Middle_Smooth(int bitcount, int nTempWidth, int nTempHeight,int nTempCenX, int nTempCenY)
{
	LPBYTE	lpSrc;   //�Լ��ӵ�
	LPBYTE	lpDst;			// ָ�򻺴�ͼ���ָ��	
	LPBYTE	lpNewDIBBits;	// ָ�򻺴�DIBͼ���ָ��
	HLOCAL	hNewDIBBits;
	int i, j, x, y, m, n;			//ѭ������
	int flag = 1;  //ѭ����־����
				   //BYTE pixel[9],mid; //��������ֵ����ֵ

	BYTE mid; //��������ֵ����ֵ
	unsigned char* pixel;
	BYTE pixel_r[3*3], pixel_g[3*3], pixel_b[3*3],mid_r,mid_g,mid_b;
	BYTE temp;//�м����
	BYTE temp_b, temp_g, temp_r;
	LPBYTE lpDIBBits = m_pDib->GetData();//�ҵ�ԭͼ�����ʼλ��
	LONG lWidth = m_pDib->GetWidth();    //���ԭͼ��Ŀ��
	LONG lHeight = m_pDib->GetHeight();  //���ԭͼ��ĸ߶�

										 // ��ʱ�����ڴ棬�Ա�����ͼ��
	pixel = new unsigned char[nTempHeight * nTempWidth];
	if (bitcount == 8) {
		hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
		if (hNewDIBBits == NULL)
		{
			return FALSE;
		}

		lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

		// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
		lpDst = (LPBYTE)lpNewDIBBits;
		memset(lpDst, (BYTE)0, lWidth * lHeight);

		//�ڴ������ֵ�˲��Ĺؼ�����

		for (j = nTempCenY; j < lHeight -nTempHeight + nTempCenY + 1; j++)
		{
			for (i = nTempCenX; i < lWidth - nTempWidth + nTempCenX + 1; i++)
			{
				//int k = 0;
				lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;    //��j�е�i������  ��1�е�1��

				for (x = 0; x <nTempHeight; x++)   //��
				{
					for (y = 0; y < nTempWidth; y++)     //��
					{

						//lpSrc = (LPBYTE)lpNewDIBBits + lWidth * x + y;    
						pixel[x * nTempWidth + y] = lpDIBBits[lWidth*(j - nTempCenY + x) + i - nTempCenX + y];         //�õ���pixel[0]-pixel[8]��ֵ
						//k++;
					}
				}

				for (m = 0; m < nTempHeight * nTempWidth -1; m++)
				{
					for (n = 0; n < nTempHeight * nTempWidth - 1 - m; n++)
					{
						if (pixel[n]>pixel[n + 1])
						{
							temp = pixel[n];
							pixel[n] = pixel[n + 1];
							pixel[n + 1] = temp;     //������pixel�������Ԫ��Ϊ��С�����ţ�pixel[4]Ϊ��ֵ
						}
					}
				}
				// ������ֵ
				if ((nTempHeight * nTempWidth & 1) > 0)
				{
					// ������������Ԫ�أ������м�һ��Ԫ��
					mid = pixel[(nTempHeight * nTempWidth - 1) / 2];
				}
				else
				{
					// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
					mid = (pixel[nTempHeight * nTempWidth / 2 -1] + pixel[nTempHeight * nTempWidth / 2]) / 2;
				}
	
				*lpDst = (BYTE)mid;

			}
		}



		//�����3*3ģ��Ļ�������ֵmid	���£�		
		//	mid=pixel[4];

		// ָ��Ŀ��ͼ���j�У���i�����ص�ָ��			
		//	lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
		//����ֵ����Ŀ��ͼ��ĵ�ǰ��
		//    *lpDst = (BYTE)mid;

		// ���Ʊ任���ͼ��
		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	}
	else if (bitcount == 24) {
		hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight*3);
		if (hNewDIBBits == NULL)
		{
			return FALSE;
		}

		lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

		// ��ʼ���·�����ڴ棬�趨��ʼֵΪ0
		lpDst = (LPBYTE)lpNewDIBBits;
		memset(lpDst, (BYTE)0, lWidth * lHeight * 3);

		for (j = nTempCenY; j < lHeight - nTempHeight + nTempCenY + 1; j++)
		{
			
			for (i = 1; i < lWidth*3 - 1; i++)
			{
				lpDst = (LPBYTE)lpNewDIBBits + lWidth * j * 3 + i;
				//int k = 0;
				//lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;    //��j�е�i������  ��1�е�1��
			//	lpDst = (LPBYTE)lpNewDIBBits + lWidth * j * 3+i;
				for (x = 0; x < nTempHeight; x++)   //��
				{
					for (y = 0; y < nTempWidth; y++)     //��
					{  
						pixel_b[x * nTempWidth + y] = lpDIBBits[lWidth*(j - nTempCenY + x)*3 + i - nTempCenX +1 ];         //�õ���pixel[0]-pixel[8]��ֵ
						pixel_g[x * nTempWidth + y] = lpDIBBits[lWidth*(j - nTempCenY + x)*3 + i - nTempCenX +2 ];
						pixel_r[x * nTempWidth + y] = lpDIBBits[lWidth*(j - nTempCenY + x)*3 + i - nTempCenX +3 ]; //��0 1 2ûʲôӰ��  ��y֮���Ҷ�ͼ
						//k++;
					}
				}

				for (m = 0; m < nTempHeight * nTempWidth - 1; m++)
				{
					for (n = 0; n < nTempHeight * nTempWidth - 1 - m; n++)
					{
						if (pixel_b[n]>pixel_b[n + 1])
						{
							temp_b = pixel_b[n];
							pixel_b[n] = pixel_b[n + 1];
							pixel_b[n + 1] = temp_b;     //������pixel�������Ԫ��Ϊ��С�����ţ�pixel[4]Ϊ��ֵ
						}
					}
				}
				*(lpDst++) = (BYTE)pixel_b[(nTempHeight * nTempWidth - 1) / 2];

				for (m = 0; m < nTempHeight * nTempWidth - 1; m++)
				{
					for (n = 0; n < nTempHeight * nTempWidth - 1 - m; n++)
					{
						if (pixel_g[n]>pixel_g[n + 1])
						{
							temp_g = pixel_g[n];
							pixel_g[n] = pixel_g[n + 1];
							pixel_g[n + 1] = temp_g;     //������pixel�������Ԫ��Ϊ��С�����ţ�pixel[4]Ϊ��ֵ
						}
					}
				}
				*(lpDst++) = (BYTE)pixel_g[(nTempHeight * nTempWidth - 1) / 2];

				for (m = 0; m <nTempHeight * nTempWidth - 1; m++)
				{
					for (n = 0; n <nTempHeight * nTempWidth - 1 - m; n++)
					{
						if (pixel_r[n]>pixel_r[n + 1])
						{
							temp_r = pixel_r[n];
							pixel_r[n] = pixel_r[n + 1];
							pixel_r[n + 1] = temp_r;     //������pixel�������Ԫ��Ϊ��С�����ţ�pixel[4]Ϊ��ֵ
						}
					}
				}
				
				*(lpDst++) = (BYTE)pixel_r[(nTempHeight * nTempWidth - 1) / 2];

			}
		}

		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight * 3);
	}
	
	//�ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}




