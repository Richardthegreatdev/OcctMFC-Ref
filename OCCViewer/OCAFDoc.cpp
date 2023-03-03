#include "pch.h"
#include "OCAFApp.h"
#include "OCCViewer.h"

#include <TDocStd_PathParser.hxx>

//=======================================================================
//Class    :	OCAFDoc
//purpose  :	Create OCAF based application
//=======================================================================

IMPLEMENT_DYNCREATE(OCAFDoc, CDocument)

BEGIN_MESSAGE_MAP(OCAFDoc, CDocument)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_COMMAND(ID_FILE_SAVE, OCAFDoc::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OCAFDoc::OnFileSaveAs)
END_MESSAGE_MAP()

OCAFView* OCAFDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	return (OCAFView*)GetNextView(pos);
}

Handle(AIS_InteractiveContext)& OCAFDoc::GetAISContext()
{
	// Get View
	POSITION pos = GetFirstViewPosition();
	OCAFView* pView = (OCAFView*)GetNextView(pos);

	return pView->GetAISContext();
}

void OCAFDoc::InitializeOCAFApp()
{
	myOccApp = new OCAFApp();
	BinDrivers::DefineFormat(myOccApp);
	XmlDrivers::DefineFormat(myOccApp);
}

BOOL OCAFDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// Get an Handle on the current Ocaf Application 
	InitializeOCAFApp();

	if (myOccApp.IsNull())
	{
		return FALSE;
	}

	// Create a new Ocaf document
	myOccApp->NewDocument("BinOcaf", myOcafDoc);

	// Set the maximum number of available "undo" actions
	myOcafDoc->SetUndoLimit(10);

	return TRUE;
}

void OCAFDoc::InitialisePrs()
{
	GetAISContext()->SetDisplayMode(AIS_Shaded, Standard_True);
	myTAISViewer = TPrsStd_AISViewer::New(myOcafDoc->Main(), GetAISContext());	
	DisplayPrs();
}

BOOL OCAFDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
	{
		return FALSE;
	}	

	// Initialize OCAF App
	InitializeOCAFApp();

	if (myOccApp.IsNull())
	{
		return FALSE;
	}

	CWaitCursor aWaitCursor;
	PathName = lpszPathName;

	const wchar_t* aPathName = lpszPathName;
	TCollection_ExtendedString anOccPathName(aPathName);

	// Open the document in the current application
	myOccApp->Open(anOccPathName, myOcafDoc);

	myOcafDoc->SetUndoLimit(10);

	return TRUE;
}

void OCAFDoc::DisplayPrs()
{
	TDF_Label LabSat = myOcafDoc->Main();

	for (TDF_ChildIterator it(LabSat); it.More(); it.Next())
	{
		TDF_Label L = it.Value();
		Handle(TNaming_NamedShape) TNS;
		Handle(TDataStd_Name) TNA;
		TCollection_ExtendedString Name;

		if (L.FindAttribute(TDataStd_Name::GetID(), TNA))
			Name = TNA->Get();

		if (!L.FindAttribute(TNaming_NamedShape::GetID(), TNS)) continue;
		Handle(TDataStd_Integer) TDI;

		// To know if the object was displayed
		if (L.FindAttribute(TDataStd_Integer::GetID(), TDI))
			if (!TDI->Get())  continue;

		Handle(TPrsStd_AISPresentation) prs;
		if (!L.FindAttribute(TPrsStd_AISPresentation::GetID(), prs))
			prs = TPrsStd_AISPresentation::Set(L, TNaming_NamedShape::GetID());

		if (TDI->Get() == 1)
			prs->Display(1);
	}

	GetAISContext()->UpdateCurrentViewer();
	GetView()->FitAll();
}

void OCAFDoc::OnCloseDocument()
{	
	myOccApp->Close(myOcafDoc);
	CDocument::OnCloseDocument();
}

void OCAFDoc::OnEditRedo()
{
	myOcafDoc->Redo();
	myOcafDoc->CommitCommand();
	UpdateDocument();
	GetAISContext()->UpdateCurrentViewer();

	UpdateAllViews(NULL);
}

void OCAFDoc::OnEditUndo()
{
	myOcafDoc->Undo();
	myOcafDoc->CommitCommand();
	UpdateDocument();
	GetAISContext()->UpdateCurrentViewer();

	UpdateAllViews(NULL);
}

void OCAFDoc::OnUpdateEditRedo(CCmdUI* pCmdUI)
{
	// Disable the "redo" button if there is no availlables redo actions
	if (myOcafDoc->GetAvailableRedos() > 0) pCmdUI->Enable(Standard_True);
	else pCmdUI->Enable(Standard_False);
}

void OCAFDoc::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	// Disable the "undo" button if there is no availlables undo actions
	if (myOcafDoc->GetAvailableUndos() > 0) pCmdUI->Enable(Standard_True);
	else pCmdUI->Enable(Standard_False);
}

void OCAFDoc::OnFileSaveAs()
{
	const wchar_t* SPathName = PathName;
	TCollection_ExtendedString TPathName(SPathName);

	CString Filter;
	Filter = "OCC Document(Binary) (*.cbf)|*.cbf|OCC Document(Xml) (*.xml)|*.xml||";

	CFileDialog dlg(FALSE,
		L"cbf",
		GetTitle(),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		Filter,
		NULL);


	if (dlg.DoModal() != IDOK) return;

	SetTitle(dlg.GetFileTitle());

	CWaitCursor aWaitCursor;
	CString CSPath = dlg.GetPathName();

	PathName = CSPath;
	const wchar_t* SPath = CSPath;
	TCollection_ExtendedString TPath(SPath);

	if (TPath.SearchFromEnd(".xml") > 0)
	{
		// The document must be saved in XML format
		myOcafDoc->ChangeStorageFormat("XmlOcaf");
	}
	else if (TPath.SearchFromEnd(".bin") > 0)
	{
		// The document must be saved in binary format
		myOcafDoc->ChangeStorageFormat("BinOcaf");
	}

	try
	{
		// Saves the document in the current application
		myOccApp->SaveAs(myOcafDoc, TPath);
	}
	catch (...)
	{
		AfxMessageBox(L"Error! The file wasn't saved.");
		return;
	}

	SetPathName(SPath);
	PathName = CSPath;
}

void OCAFDoc::OnFileSave()
{	
	if (myOcafDoc.IsNull())
	{
		AfxMessageBox(L"Error during saving! Empty document.");
		return;
	}

	if (PathName != "")
	{
		const wchar_t* SPath = PathName;
		TCollection_ExtendedString TPath(SPath);

		if (TPath.SearchFromEnd(".xml") > 0)
		{
			// The document must be saved in XML format
			myOcafDoc->ChangeStorageFormat("XmlOcaf");
		}
		else if (TPath.SearchFromEnd(".bin") > 0)
		{
			// The document must be saved in binary format
			myOcafDoc->ChangeStorageFormat("BinOcaf");
		}

		try
		{
			// Saves the document in the current application
			myOccApp->SaveAs(myOcafDoc, TPath);
		}
		catch (...)
		{
			AfxMessageBox(L"Error! The file wasn't saved.");
			return;
		}

		return;
	}
	else
	{
		OnFileSaveAs();
	}

}

TDF_Label OCAFDoc::GetSelectedObjLabel()
{
	// Get Label from selected object
	Handle(AIS_InteractiveObject) curAISObject = GetView()->GetSelectedObj();

	Handle(TPrsStd_AISPresentation) ObjectPrs =
		Handle(TPrsStd_AISPresentation)::DownCast(curAISObject->GetOwner());

	return ObjectPrs->Label();
}
