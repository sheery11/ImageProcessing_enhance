
// ImageEnhanceView.h : CImageEnhanceView ��Ľӿ�
//

#pragma once
#include "Dib.h"
#include "SmoothProcessDib.h"
#include "NoiseAddProcessDib.h"
#include "FilterProcessDib.h"

class CImageEnhanceView : public CView
{
protected: // �������л�����
	CImageEnhanceView();
	DECLARE_DYNCREATE(CImageEnhanceView)

// ����
public:
	CImageEnhanceDoc* GetDocument() const;

	CDib m_Image;//��Ƭͼ��
	BOOL flag;//����Ƭͼ���Ǳ���
	CDib *m_pDib;
// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CImageEnhanceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL ShowImageOn(CDC* pDC,CString strPath, int x, int y);

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenpic();
	afx_msg void OnOpenjpg();
	afx_msg void Onthoutfullow();
	afx_msg void OnMiddleSmooth();
	afx_msg void OnAverageSmooth();
	afx_msg void OnRestoresaltnoise();
	afx_msg void OnGausenoise();
	afx_msg void OnButtlow();
	afx_msg void OnThoughtfulhigh();
	afx_msg void OnButthighfilter();
};

#ifndef _DEBUG  // ImageEnhanceView.cpp �еĵ��԰汾
inline CImageEnhanceDoc* CImageEnhanceView::GetDocument() const
   { return reinterpret_cast<CImageEnhanceDoc*>(m_pDocument); }
#endif

