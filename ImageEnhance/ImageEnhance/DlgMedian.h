#pragma once


// CDlgMedian 对话框

class CDlgMedian : public CDialog
{
	DECLARE_DYNAMIC(CDlgMedian)

public:
	CDlgMedian(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMedian();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENHANCE_MEDIAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int miFilter_Height;
	int miFilter_Width;
	int m_nFilterCenX;
	int m_nFilterCenY;
};
