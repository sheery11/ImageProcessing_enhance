#pragma once


// DlgAverage �Ի���

class DlgAverage : public CDialog
{
	DECLARE_DYNAMIC(DlgAverage)

public:
	DlgAverage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgAverage();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENHANCE_AVERAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nSmthTemHeight;
	int m_nSmthTemWidth;
	int m_nSmthTemCenX;
	int m_nSmthTemCenY;
};
