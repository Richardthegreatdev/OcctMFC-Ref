// ParametricDoc.cpp : implementation file
//

#include "pch.h"
#include "OCCViewer.h"
#include "ParametricDoc.h"

#include "FlangeDesign.h"
#include "FlangeDlg.h"
#include "NailDesign.h"
#include "NailDlg.h"

// ParametricDoc

IMPLEMENT_DYNCREATE(ParametricDoc, OCAFDoc)

ParametricDoc::ParametricDoc()
{
}

BOOL ParametricDoc::OnNewDocument()
{
	if (!OCAFDoc::OnNewDocument())
		return FALSE;
	return TRUE;
}

ParametricDoc::~ParametricDoc()
{
}


BEGIN_MESSAGE_MAP(ParametricDoc, OCAFDoc)
	ON_COMMAND(ID_FLANGEDESIGN_CREATEFLANGE, &ParametricDoc::OnFlangedesignCreateflange)
	ON_COMMAND(ID_FLANGEDESIGN_EDITFLANGE, &ParametricDoc::OnFlangedesignEditflange)
	ON_UPDATE_COMMAND_UI(ID_FLANGEDESIGN_EDITFLANGE, &ParametricDoc::OnUpdateFlangedesignEditflange)
	ON_COMMAND(ID_OCAF_OCAFTREE, &ParametricDoc::OnOcafOcaftree)
	ON_UPDATE_COMMAND_UI(ID_OCAF_OCAFTREE, &ParametricDoc::OnUpdateOcafOcaftree)
	ON_COMMAND(ID_NAILDESIGN_CREATENAIL, &ParametricDoc::OnNaildesignCreatenail)
	ON_COMMAND(ID_NAILDESIGN_EDITNAIL, &ParametricDoc::OnNaildesignEditnail)
END_MESSAGE_MAP()


// ParametricDoc diagnostics

#ifdef _DEBUG
void ParametricDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void ParametricDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// ParametricDoc serialization

void ParametricDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}
#endif

void ParametricDoc::UpdateDocument()
{
	if(idesT==1)
		UpdateFlange();
	
	if(idesT==2)
		UpdateNail();
}

// ParametricDoc commands

void ParametricDoc::CreateFlange()
{
	CFlangeDlg flg;

	Standard_Real OD;
	Standard_Real ID;
	Standard_Real flgthk;
	Standard_Real NbBolts;
	Standard_Real BoltDia;
	Standard_Real BoltPCD;

	if (flg.DoModal() != IDOK) return;

	OD = flg.pOD;
	ID = flg.pID;
	flgthk = flg.pThk;
	NbBolts = flg.pNbBolts;
	BoltDia = flg.pBoltDia;
	BoltPCD = flg.pBoltPCD;
	
	// Open a new command (for undo)
	myOcafDoc->NewCommand();

	TDF_Label FLG = Flange::CreateFlangeDS(myOcafDoc->Main(),
		OD, ID, flgthk, NbBolts, BoltDia, BoltPCD);

	// Attach an integer attribute to flange to memorize it's displayed
	TDataStd_Integer::Set(FLG, 1);

	// Set the TPrsStd_AISPresentation of the new Flange TNaming_NamedShape
	TPrsStd_AISPresentation::Set(FLG, TNaming_NamedShape::GetID());

	// Close the command (for undo)
	myOcafDoc->CommitCommand();

	DisplayPrs();
	GetView()->FitAll();

	idesT = 1;
}

void ParametricDoc::OnFlangedesignCreateflange()
{
	// TODO: Add your command handler code here
	CreateFlange();	
}


void ParametricDoc::EditFlange()
{
	CFlangeDlg flg;

	Standard_Real OD;
	Standard_Real ID;
	Standard_Real flgthk;
	Standard_Real NbBolts;
	Standard_Real BoltDia;
	Standard_Real BoltPCD;

	// Get Root Label
	TDF_Label Root = myOcafDoc->Main();

	// Get Flange Label
	TDF_Label FLG = Root.FindChild(1, false);

	// Get Dimensions
	Flange::GetDimensions(FLG, OD, ID, flgthk, NbBolts, BoltDia, BoltPCD);

	flg.pOD = OD;
	flg.pID = ID;
	flg.pThk = flgthk;
	flg.pNbBolts = NbBolts;
	flg.pBoltDia = BoltDia;
	flg.pBoltPCD = BoltPCD;

	if (flg.DoModal() != IDOK) return;

	OD = flg.pOD;
	ID = flg.pID;
	flgthk = flg.pThk;
	NbBolts = flg.pNbBolts;
	BoltDia = flg.pBoltDia;
	BoltPCD = flg.pBoltPCD;

	// Open a new command (for undo)
	myOcafDoc->NewCommand();

	Flange::ModifyFlange(FLG, OD, ID, flgthk, NbBolts, BoltDia, BoltPCD);

	// Close the command (for undo)
	myOcafDoc->CommitCommand();

	DisplayPrs();
	GetView()->FitAll();
}

void ParametricDoc::UpdateFlange()
{
	Standard_Real OD;
	Standard_Real ID;
	Standard_Real flgthk;
	Standard_Real NbBolts;
	Standard_Real BoltDia;
	Standard_Real BoltPCD;

	// Get Root Label
	TDF_Label Root = myOcafDoc->Main();

	// Get Flange Label
	TDF_Label FLG = Root.FindChild(1, false);

	// Get Dimensions
	Flange::GetDimensions(FLG, OD, ID, flgthk, NbBolts, BoltDia, BoltPCD);

	// Open a new command (for undo)
	myOcafDoc->NewCommand();

	Flange::ModifyFlange(FLG, OD, ID, flgthk, NbBolts, BoltDia, BoltPCD);

	// Close the command (for undo)
	myOcafDoc->CommitCommand();

	DisplayPrs();
	GetView()->FitAll();
}

void ParametricDoc::OnFlangedesignEditflange()
{
	EditFlange();
}


void ParametricDoc::OnUpdateFlangedesignEditflange(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (GetView()->GetAISContext()->NbSelected() == 1)
	{
		pCmdUI->Enable(1);
	}
	else
	{
		pCmdUI->Enable(0);
	}
}


void ParametricDoc::OnOcafOcaftree()
{
	// TODO: Add your command handler code here
	CreateOCAFTree();
}

void ParametricDoc::OnUpdateOcafOcaftree(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (GetView()->GetAISContext()->NbSelected() == 1)
	{
		pCmdUI->Enable(1);
	}
	else
	{
		pCmdUI->Enable(0);
	}
}

void ParametricDoc::CreateOCAFTree()
{
	COCCTreedlg occTDlg;
	occTDlg.pOCCDoc = this;
	occTDlg.DoModal();
}

void ParametricDoc::CreateNail()
{
	NailDlg dlg;

	Standard_Real xpos = 0;
	Standard_Real ypos = 0;
	Standard_Real zpos = 0;

	Standard_Real aConeRadius = 10;
	Standard_Real aConeHeight = 10;

	if (dlg.DoModal() != IDOK) return;

	xpos = dlg.posX;
	ypos = dlg.posY;
	zpos = dlg.posZ;

	gp_Pnt Pos(xpos, ypos, zpos);

	aConeRadius = dlg.pConeRadius;
	aConeHeight = dlg.pConeHeight;

	// Open a new command (for undo)
	myOcafDoc->NewCommand();

	TDF_Label Nail = NailDesign::CreateNail(myOcafDoc->Main(), Pos, aConeRadius, aConeHeight);

	// Attach an integer attribute to L to memorize it's displayed
	TDataStd_Integer::Set(Nail, 1);

	// Close the command (for undo)
	myOcafDoc->CommitCommand();

	// Get the TPrsStd_AISPresentation of the new Flange TNaming_NamedShape
	Handle(TPrsStd_AISPresentation) prs = TPrsStd_AISPresentation::Set(Nail, TNaming_NamedShape::GetID());

	DisplayPrs();
	GetView()->FitAll();

	idesT = 2;
}

void ParametricDoc::EditNail()
{
	gp_Pnt Pos;
	Standard_Real aConeRadius;
	Standard_Real aConeHeight;

	// Get Root Label
	TDF_Label Root = myOcafDoc->Main();

	TDF_Label NAIL = GetSelectedObjLabel();

	// Get Dimensions
	NailDesign::GetDimensions(NAIL, Pos, aConeRadius, aConeHeight);

	NailDlg dlg;

	dlg.posX = Pos.X();
	dlg.posY = Pos.Y();
	dlg.posZ = Pos.Z();
	dlg.pConeRadius = aConeRadius;
	dlg.pConeHeight = aConeHeight;

	if (dlg.DoModal() != IDOK) return;

	Pos.SetX(dlg.posX);
	Pos.SetY(dlg.posY);
	Pos.SetZ(dlg.posZ);

	aConeRadius = dlg.pConeRadius;
	aConeHeight = dlg.pConeHeight;

	// Open a new command (for undo)
	myOcafDoc->NewCommand();

	NailDesign::EditNail(NAIL, Pos, aConeRadius, aConeHeight);

	// Close the command (for undo)
	myOcafDoc->CommitCommand();

	DisplayPrs();
	GetView()->FitAll();
}

void ParametricDoc::UpdateNail()
{
	gp_Pnt Pos;
	Standard_Real aConeRadius;
	Standard_Real aConeHeight;

	// Get Root Label
	TDF_Label Root = myOcafDoc->Main();

	// Get Flange Label
	TDF_Label NAIL = Root.FindChild(1, false);

	// Get Dimensions
	NailDesign::GetDimensions(NAIL, Pos, aConeRadius, aConeHeight);

	// Open a new command (for undo)
	myOcafDoc->NewCommand();

	NailDesign::EditNail(NAIL, Pos, aConeRadius, aConeHeight);

	// Close the command (for undo)
	myOcafDoc->CommitCommand();

	DisplayPrs();
	GetView()->FitAll();
}


void ParametricDoc::OnNaildesignCreatenail()
{
	CreateNail();
}


void ParametricDoc::OnNaildesignEditnail()
{
	EditNail();
}
