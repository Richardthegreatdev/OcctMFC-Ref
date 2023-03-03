#pragma once


// CFlangeDlg dialog

class CFlangeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CFlangeDlg)

public:
	CFlangeDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CFlangeDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	bool isEdit = false;
	double pOD;
	double pID;
	double pThk;
	int pNbBolts;
	double pBoltDia;
	double pBoltPCD;
};
