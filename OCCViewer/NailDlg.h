#pragma once


// NailDlg dialog

class NailDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NailDlg)

public:
	NailDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~NailDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	double posX;
	double posY;
	double posZ;
	double pConeRadius;
	double pConeHeight;
};
