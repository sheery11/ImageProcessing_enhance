#pragma once

#include "Dib.h"

#include "afx.h"

class CNoiseAddProcessDib {
public:
	// ���캯������ʼ�����ݳ�Ա
	CNoiseAddProcessDib(CDib *pDib);

	// ��������	
	~CNoiseAddProcessDib(void);

	BOOL SaltNoiseDIB();
	BOOL ColorSaltNoiseDIB();

	BOOL GauseNoiseDIB();

	BOOL ColorGauseNoiseDIB();  //��ɫͼ���˹����
private:
	CDib *m_pDib;
};