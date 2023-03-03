#include "pch.h"
#include "OCAFApp.h"
#include "OCCViewer.h"

//=======================================================================
//Class    :	General tree functions
//purpose  :	
//=======================================================================

HTREEITEM AddItem(CTreeCtrl& ctrl, HTREEITEM hParent, LPCTSTR szText, int iImage, DWORD_PTR data)
{
	TVITEM tvi;
	ZeroMemory(&tvi, sizeof(TVITEM));
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.iImage = iImage;
	tvi.iSelectedImage = iImage;
	tvi.pszText = (LPTSTR)szText;
	tvi.lParam = (LPARAM)data;

	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(TVINSERTSTRUCT));
	tvis.hParent = hParent;
	tvis.item = tvi;

	return ctrl.InsertItem(&tvis);
}

void OCAFDoc::GetOCAFTree(CTreeCtrl& ctlOCAF)
{
    TDF_Label LabSat = myOcafDoc->Main();

    // Root Node
    HTREEITEM hRoot = AddItem(ctlOCAF, NULL, _T("Model"), 0, NULL);

    for (TDF_ChildIterator it(LabSat); it.More(); it.Next())
    {
        TDF_Label L = it.Value();
        if (!L.IsNull())
        {
            if (L.HasAttribute())
            {
                TraverseComponent(L, hRoot, ctlOCAF);
            }
        }
    }

}

void OCAFDoc::TraverseComponent(TDF_Label& root, HTREEITEM hParent, CTreeCtrl& ctlOCAF)
{
    Handle(TDataStd_Name) curName;
    TCollection_ExtendedString aStr;
    CString result;

    if (root.FindAttribute(TDataStd_Name::GetID(), curName))
    {
        aStr = curName->Get();
    }
    else
    {
        aStr = "Solid";
    }

    HTREEITEM aComp = AddItem(ctlOCAF, hParent, aStr.ToWideString(), 0, NULL);
    ctlOCAF.Expand(hParent, TVE_EXPAND);

    // Add Numerical Data
    Handle(TDataStd_Real) curReal;
    if (root.FindAttribute(TDataStd_Real::GetID(), curReal))
    {
        CString result;
        Standard_Real value;
        value = curReal->Get();
        result.Format(_T("value: %f"), value);
        HTREEITEM aCompval = AddItem(ctlOCAF, aComp, result, 0, NULL);
        ctlOCAF.Expand(aComp, TVE_EXPAND);
    }

    for (TDF_ChildIterator it(root); it.More(); it.Next())
    {
        TDF_Label L = it.Value();
        if (!L.IsNull())
        {
            if (L.HasAttribute())
            {
                TraverseComponent(L, aComp, ctlOCAF);
            }
        }
    }
}

//=======================================================================================

IMPLEMENT_DYNAMIC(COCCTreedlg, CDialogEx)

COCCTreedlg::COCCTreedlg(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_DIALOG2, pParent)
{

}

COCCTreedlg::~COCCTreedlg()
{
}

void COCCTreedlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);   
    DDX_Control(pDX, IDC_TREE1, m_OCCTree);
}


BEGIN_MESSAGE_MAP(COCCTreedlg, CDialogEx)
    
END_MESSAGE_MAP()


BOOL COCCTreedlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  Add extra initialization here
    if (pOCCDoc != NULL)
    {
        pOCCDoc->GetOCAFTree(m_OCCTree);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}