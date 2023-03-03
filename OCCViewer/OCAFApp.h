#pragma once

#include "OccFunctions.h"
#include "OCCViewerView.h"

/// <summary>
/// OCAF Application Class.
/// Declare handle of OCAF class in main application class in 
/// InitInstance function.
/// </summary>
class OCAFApp : public TDocStd_Application
{
public:
	OCAFApp();
	~OCAFApp() {};

	// OCCT RTTI
	DEFINE_STANDARD_RTTIEXT(OCAFApp, TDocStd_Application)

public:
	virtual void InitializeDrivers();
};

class OCAFView;
//=======================================================================
//Class    :	OCAFDoc
//purpose  :	Create OCAF based document
//=======================================================================
class OCAFDoc : public CDocument
{
	DECLARE_DYNCREATE(OCAFDoc)

protected:
	OCAFDoc() {};           // protected constructor used by dynamic creation
	virtual ~OCAFDoc() {};

protected:
	Handle(OCAFApp)          myOccApp;
	Handle(TDocStd_Document) myOcafDoc;
	Handle(TPrsStd_AISViewer) myTAISViewer;
	CString PathName;

	virtual void InitializeOCAFApp();
	virtual void UpdateDocument() {};
	TDF_Label& GetRoot() { return myOcafDoc->Main(); };	
	virtual void DisplayPrs();
	TDF_Label GetSelectedObjLabel();

public:
	void InitialisePrs();
	Handle(TDocStd_Document) GetOcafDoc() { return myOcafDoc; };
	Handle(AIS_InteractiveContext)& GetAISContext();
	OCAFView* GetView();

	void GetOCAFTree(CTreeCtrl& ctlOCAF);
	void TraverseComponent(TDF_Label& root, HTREEITEM hParent, CTreeCtrl& ctlOCAF);

public:
	virtual void OnCloseDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnNewDocument();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();

};

//=======================================================================
//Class    :	OCAFView
//purpose  :	Create OCAF based view class
//=======================================================================
class OCAFView : public COCCViewerView
{
	DECLARE_DYNCREATE(OCAFView)

protected:
	OCAFView() {};           // protected constructor used by dynamic creation
	virtual ~OCAFView() {};

	// Attributes
public:
	OCAFDoc* GetDocument() const;
	virtual void OnInitialUpdate();
};

//=======================================================================
//Class    :	General tree functions
//purpose  :	
//=======================================================================

extern HTREEITEM AddItem(CTreeCtrl& ctrl, HTREEITEM hParent, LPCTSTR szText, int iImage, DWORD_PTR data);


class COCCTreedlg : public CDialogEx
{
	DECLARE_DYNAMIC(COCCTreedlg)

public:
	COCCTreedlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~COCCTreedlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CTreeCtrl m_OCCTree;
	OCAFDoc* pOCCDoc = NULL;

	virtual BOOL OnInitDialog();
};