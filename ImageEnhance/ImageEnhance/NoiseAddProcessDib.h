#pragma once

#include "Dib.h"

#include "afx.h"

class CNoiseAddProcessDib {
public:
	// 构造函数，初始化数据成员
	CNoiseAddProcessDib(CDib *pDib);

	// 析构函数	
	~CNoiseAddProcessDib(void);

	BOOL SaltNoiseDIB();
	BOOL ColorSaltNoiseDIB();

	BOOL GauseNoiseDIB();

	BOOL ColorGauseNoiseDIB();  //彩色图像高斯噪声
private:
	CDib *m_pDib;
};