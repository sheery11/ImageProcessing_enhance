// DlgMedian.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageEnhance.h"
#include "DlgMedian.h"
#include "afxdialogex.h"


// CDlgMedian 对话框

IMPLEMENT_DYNAMIC(CDlgMedian, CDialog)

CDlgMedian::CDlgMedian(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ENHANCE_MEDIAN, pParent)
	, miFilter_Height(0)
	, miFilter_Width(0)
	, m_nFilterCenX(0)
	, m_nFilterCenY(0)
{

}

CDlgMedian::~CDlgMedian()
{
}

void CDlgMedian::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MEDIAN_FILH, miFilter_Height);
	DDX_Text(pDX, IDC_EDIT_MEDIAN_FILW, miFilter_Width);
	DDX_Text(pDX, IDC_EDIT_MEDIAN_FILCENX, m_nFilterCenX);
	DDX_Text(pDX, IDC_EDIT_MEDIAN_FILCENY, m_nFilterCenY);
}


BEGIN_MESSAGE_MAP(CDlgMedian, CDialog)
END_MESSAGE_MAP()


// CDlgMedian 消息处理程序
