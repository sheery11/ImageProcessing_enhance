#pragma once


#include "Dib.h"

#include "afx.h"

class CFilterProcessDib {
public:
	// ���캯������ʼ�����ݳ�Ա
	CFilterProcessDib(CDib *pDib);

	// ��������	
	~CFilterProcessDib(void);

	BOOL ButtwothLowFilter();
	BOOL ThoughtfulLowFilter();
	BOOL ColorThoughtfulLowFilter();  //�����ɫ��ͨ
	BOOL ThoughtfulHighFilter();
	BOOL ButtworthHighFilter();
	BOOL ColorButtworthHighFilter();
private:
	CDib *m_pDib;
};