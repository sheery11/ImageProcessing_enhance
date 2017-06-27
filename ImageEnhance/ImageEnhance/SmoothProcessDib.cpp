#include "StdAfx.h"
#include "SmoothProcessDib.h"



//=======================================================
// 函数功能： 构造函数，初始化数据成员
// 输入参数： 位图指针
// 返回值：   无
//=======================================================
CSmoothProcessDib::CSmoothProcessDib(CDib *pDib)
{
	m_pDib = pDib;
}


//=======================================================
// 函数功能： 析构函数
// 输入参数： 无
// 返回值：   无
//=======================================================

CSmoothProcessDib::~CSmoothProcessDib(void)
{

}


//=======================================================
// 函数功能： 邻域平均平滑函数
// 输入参数： 无
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================
BOOL CSmoothProcessDib::Average_Smooth(int bitcount,int nTempWidth, int nTempHeight, int nTempCenX, int nTempCenY)
{
	LPBYTE	lpSrc;
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;
	long i, j;			//循环变量
	long k, l;    //内部循环变量
	BYTE average;	//领域均值变量
	BYTE average_r, average_g, average_b;
	FLOAT sum;
	FLOAT sum_r, sum_g, sum_b;
	//int Moban = 3;
	LPBYTE lpDIBBits = m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth = m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight = m_pDib->GetHeight();  //获得原图像的高度

										 // 暂时分配内存，以保存新图像
	if (bitcount == 8) {
		hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
		if (hNewDIBBits == NULL)
		{
			return FALSE;
		}

		lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

		// 初始化新分配的内存，设定初始值为0
		lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
		memset(lpDst, (BYTE)0, lWidth * lHeight);

		//在此添加关键程序
		//逐个扫描图像中的像素点,求其邻域均值
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
		// 复制变换后的图像
		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	}
	else if (bitcount == 24) {
		hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight*3);
		if (hNewDIBBits == NULL)
		{
			return FALSE;
		}

		lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);
		// 初始化新分配的内存，设定初始值为0
		lpDst = (LPBYTE)lpNewDIBBits;  //用来指向目标图像第j行，第i个像素的指针
		memset(lpDst, (BYTE)0, lWidth * lHeight*3);
		for (j = nTempCenY; j < lHeight - nTempHeight + nTempCenY + 1; j++)
		{
			//应该指向第一行第一列
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
					  //加i之后是灰度图像但是有左上角  不加i是彩色图像但是横条
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

		

		// 复制变换后的图像
		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight*3);

	}

	
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;


}

//=======================================================
// 函数功能： 中值滤波平滑函数
// 输入参数： 无
// 返回值：   成功返回TRUE，否则返回FALSE
//=======================================================
BOOL CSmoothProcessDib::Middle_Smooth(int bitcount, int nTempWidth, int nTempHeight,int nTempCenX, int nTempCenY)
{
	LPBYTE	lpSrc;   //自己加的
	LPBYTE	lpDst;			// 指向缓存图像的指针	
	LPBYTE	lpNewDIBBits;	// 指向缓存DIB图像的指针
	HLOCAL	hNewDIBBits;
	int i, j, x, y, m, n;			//循环变量
	int flag = 1;  //循环标志变量
				   //BYTE pixel[9],mid; //窗口像素值及中值

	BYTE mid; //窗口像素值及中值
	unsigned char* pixel;
	BYTE pixel_r[3*3], pixel_g[3*3], pixel_b[3*3],mid_r,mid_g,mid_b;
	BYTE temp;//中间变量
	BYTE temp_b, temp_g, temp_r;
	LPBYTE lpDIBBits = m_pDib->GetData();//找到原图像的起始位置
	LONG lWidth = m_pDib->GetWidth();    //获得原图像的宽度
	LONG lHeight = m_pDib->GetHeight();  //获得原图像的高度

										 // 暂时分配内存，以保存新图像
	pixel = new unsigned char[nTempHeight * nTempWidth];
	if (bitcount == 8) {
		hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
		if (hNewDIBBits == NULL)
		{
			return FALSE;
		}

		lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

		// 初始化新分配的内存，设定初始值为0
		lpDst = (LPBYTE)lpNewDIBBits;
		memset(lpDst, (BYTE)0, lWidth * lHeight);

		//在此添加中值滤波的关键程序

		for (j = nTempCenY; j < lHeight -nTempHeight + nTempCenY + 1; j++)
		{
			for (i = nTempCenX; i < lWidth - nTempWidth + nTempCenX + 1; i++)
			{
				//int k = 0;
				lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;    //第j行第i个像素  第1行第1列

				for (x = 0; x <nTempHeight; x++)   //行
				{
					for (y = 0; y < nTempWidth; y++)     //列
					{

						//lpSrc = (LPBYTE)lpNewDIBBits + lWidth * x + y;    
						pixel[x * nTempWidth + y] = lpDIBBits[lWidth*(j - nTempCenY + x) + i - nTempCenX + y];         //得到了pixel[0]-pixel[8]的值
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
							pixel[n + 1] = temp;     //排序完pixel数组里的元素为从小到大排，pixel[4]为中值
						}
					}
				}
				// 计算中值
				if ((nTempHeight * nTempWidth & 1) > 0)
				{
					// 数组有奇数个元素，返回中间一个元素
					mid = pixel[(nTempHeight * nTempWidth - 1) / 2];
				}
				else
				{
					// 数组有偶数个元素，返回中间两个元素平均值
					mid = (pixel[nTempHeight * nTempWidth / 2 -1] + pixel[nTempHeight * nTempWidth / 2]) / 2;
				}
	
				*lpDst = (BYTE)mid;

			}
		}



		//如果是3*3模板的话，求中值mid	如下：		
		//	mid=pixel[4];

		// 指向目标图像第j行，第i个像素的指针			
		//	lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;
		//将中值赋给目标图像的当前点
		//    *lpDst = (BYTE)mid;

		// 复制变换后的图像
		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	}
	else if (bitcount == 24) {
		hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight*3);
		if (hNewDIBBits == NULL)
		{
			return FALSE;
		}

		lpNewDIBBits = (LPBYTE)LocalLock(hNewDIBBits);

		// 初始化新分配的内存，设定初始值为0
		lpDst = (LPBYTE)lpNewDIBBits;
		memset(lpDst, (BYTE)0, lWidth * lHeight * 3);

		for (j = nTempCenY; j < lHeight - nTempHeight + nTempCenY + 1; j++)
		{
			
			for (i = 1; i < lWidth*3 - 1; i++)
			{
				lpDst = (LPBYTE)lpNewDIBBits + lWidth * j * 3 + i;
				//int k = 0;
				//lpDst = (LPBYTE)lpNewDIBBits + lWidth * j + i;    //第j行第i个像素  第1行第1列
			//	lpDst = (LPBYTE)lpNewDIBBits + lWidth * j * 3+i;
				for (x = 0; x < nTempHeight; x++)   //行
				{
					for (y = 0; y < nTempWidth; y++)     //列
					{  
						pixel_b[x * nTempWidth + y] = lpDIBBits[lWidth*(j - nTempCenY + x)*3 + i - nTempCenX +1 ];         //得到了pixel[0]-pixel[8]的值
						pixel_g[x * nTempWidth + y] = lpDIBBits[lWidth*(j - nTempCenY + x)*3 + i - nTempCenX +2 ];
						pixel_r[x * nTempWidth + y] = lpDIBBits[lWidth*(j - nTempCenY + x)*3 + i - nTempCenX +3 ]; //加0 1 2没什么影响  加y之后变灰度图
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
							pixel_b[n + 1] = temp_b;     //排序完pixel数组里的元素为从小到大排，pixel[4]为中值
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
							pixel_g[n + 1] = temp_g;     //排序完pixel数组里的元素为从小到大排，pixel[4]为中值
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
							pixel_r[n + 1] = temp_r;     //排序完pixel数组里的元素为从小到大排，pixel[4]为中值
						}
					}
				}
				
				*(lpDst++) = (BYTE)pixel_r[(nTempHeight * nTempWidth - 1) / 2];

			}
		}

		memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight * 3);
	}
	
	//释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);

	return TRUE;
}




