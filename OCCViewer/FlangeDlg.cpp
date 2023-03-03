// FlangeDlg.cpp : implementation file
//

#include "pch.h"
#include "OCCViewer.h"
#include "FlangeDlg.h"
#include "afxdialogex.h"


// CFlangeDlg dialog

IMPLEMENT_DYNAMIC(CFlangeDlg, CDialogEx)

CFlangeDlg::CFlangeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, pOD(100)
	, pID(32)
	, pThk(25)
	, pNbBolts(8)
	, pBoltDia(8)
	, pBoltPCD(70)
{

}

CFlangeDlg::~CFlangeDlg()
{
}

void CFlangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, pOD);
	DDX_Text(pDX, IDC_EDIT2, pID);
	DDX_Text(pDX, IDC_EDIT3, pThk);
	DDX_Text(pDX, IDC_EDIT4, pNbBolts);
	DDX_Text(pDX, IDC_EDIT5, pBoltDia);
	DDX_Text(pDX, IDC_EDIT6, pBoltPCD);
}


BEGIN_MESSAGE_MAP(CFlangeDlg, CDialogEx)
	
END_MESSAGE_MAP()


// CFlangeDlg message handlers


