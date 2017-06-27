#pragma once


// DlgAverage 对话框

class DlgAverage : public CDialog
{
	DECLARE_DYNAMIC(DlgAverage)

public:
	DlgAverage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgAverage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENHANCE_AVERAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nSmthTemHeight;
	int m_nSmthTemWidth;
	int m_nSmthTemCenX;
	int m_nSmthTemCenY;
};
