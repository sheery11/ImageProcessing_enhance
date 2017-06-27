// DlgAverage.cpp : 实现文件
//

#include "stdafx.h"
#include "ImageEnhance.h"
#include "DlgAverage.h"
#include "afxdialogex.h"


// DlgAverage 对话框

IMPLEMENT_DYNAMIC(DlgAverage, CDialog)

DlgAverage::DlgAverage(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ENHANCE_AVERAGE, pParent)
	, m_nSmthTemHeight(0)
	, m_nSmthTemWidth(0)
	, m_nSmthTemCenX(0)
	, m_nSmthTemCenY(0)
{

}

DlgAverage::~DlgAverage()
{
}

void DlgAverage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AVERAGE_FILH, m_nSmthTemHeight);
	DDX_Text(pDX, IDC_EDIT_AVERAGE_FILW, m_nSmthTemWidth);
	DDX_Text(pDX, IDC_EDIT_AVERAGE_FILCENX, m_nSmthTemCenX);
	DDX_Text(pDX, IDC_EDIT_AVERAGE_FILCENY, m_nSmthTemCenY);
}


BEGIN_MESSAGE_MAP(DlgAverage, CDialog)
END_MESSAGE_MAP()


// DlgAverage 消息处理程序
