// NailDlg.cpp : implementation file
//

#include "pch.h"
#include "OCCViewer.h"
#include "NailDlg.h"
#include "afxdialogex.h"


// NailDlg dialog

IMPLEMENT_DYNAMIC(NailDlg, CDialogEx)

NailDlg::NailDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, posX(0)
	, posY(0)
	, posZ(0)
	, pConeRadius(10)
	, pConeHeight(10)
{

}

NailDlg::~NailDlg()
{
}

void NailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, posX);
	DDX_Text(pDX, IDC_EDIT4, posY);
	DDX_Text(pDX, IDC_EDIT5, posZ);
	DDX_Text(pDX, IDC_EDIT2, pConeRadius);
	DDX_Text(pDX, IDC_EDIT3, pConeHeight);
}


BEGIN_MESSAGE_MAP(NailDlg, CDialogEx)
END_MESSAGE_MAP()


// NailDlg message handlers
