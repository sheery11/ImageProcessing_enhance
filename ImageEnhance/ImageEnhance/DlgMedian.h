#pragma once


// CDlgMedian �Ի���

class CDlgMedian : public CDialog
{
	DECLARE_DYNAMIC(CDlgMedian)

public:
	CDlgMedian(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMedian();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENHANCE_MEDIAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int miFilter_Height;
	int miFilter_Width;
	int m_nFilterCenX;
	int m_nFilterCenY;
};
