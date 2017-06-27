#pragma once


#include "Dib.h"

#include "afx.h"

class CFilterProcessDib {
public:
	// 构造函数，初始化数据成员
	CFilterProcessDib(CDib *pDib);

	// 析构函数	
	~CFilterProcessDib(void);

	BOOL ButtwothLowFilter();
	BOOL ThoughtfulLowFilter();
	BOOL ColorThoughtfulLowFilter();  //理想彩色低通
	BOOL ThoughtfulHighFilter();
	BOOL ButtworthHighFilter();
	BOOL ColorButtworthHighFilter();
private:
	CDib *m_pDib;
};