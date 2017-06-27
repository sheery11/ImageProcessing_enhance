
// ImageEnhanceView.h : CImageEnhanceView 类的接口
//

#pragma once
#include "Dib.h"
#include "SmoothProcessDib.h"
#include "NoiseAddProcessDib.h"
#include "FilterProcessDib.h"

class CImageEnhanceView : public CView
{
protected: // 仅从序列化创建
	CImageEnhanceView();
	DECLARE_DYNCREATE(CImageEnhanceView)

// 特性
public:
	CImageEnhanceDoc* GetDocument() const;

	CDib m_Image;//照片图像
	BOOL flag;//打开照片图像标记变量
	CDib *m_pDib;
// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImageEnhanceView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL ShowImageOn(CDC* pDC,CString strPath, int x, int y);

// 生成的消息映射函数
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

#ifndef _DEBUG  // ImageEnhanceView.cpp 中的调试版本
inline CImageEnhanceDoc* CImageEnhanceView::GetDocument() const
   { return reinterpret_cast<CImageEnhanceDoc*>(m_pDocument); }
#endif

