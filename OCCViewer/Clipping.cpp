#include "pch.h"
#include "framework.h"

#include "OCCViewerView.h"

void COCCViewerView::EnableClipping(bool bON)
{
	// Get objects from context
	AIS_ListOfInteractive aList;
	myAISContext->ObjectsInside(aList);

	// Iterate on the list to get shape objects
	if (aList.Size() > 0)
	{
		for (AIS_ListIteratorOfListOfInteractive aLI(aList); aLI.More(); aLI.Next())
		{
			if (aLI.Value()->Type() == AIS_KindOfInteractive::AIS_KOI_Shape)
			{
				
			}
		}
	}
}

void COCCViewerView::AddClipPlane(const Handle(Graphic3d_ClipPlane)& thePlane)
{
	// Get objects from context
	AIS_ListOfInteractive aList;
	myAISContext->ObjectsInside(aList);

	// Iterate on the list to get shape objects
	if (aList.Size() > 0)
	{
		for (AIS_ListIteratorOfListOfInteractive aLI(aList); aLI.More(); aLI.Next())
		{
			if (aLI.Value()->Type() == AIS_KindOfInteractive::AIS_KOI_Shape)
			{
				aLI.Value()->AddClipPlane(thePlane);
			}
		}
	}
}

void COCCViewerView::CreateClipSection(gp_Pln& aPlane)
{
	// Show Section outline

	// Get all 3d objects in a compound
	TopoDS_Compound allShapes = GetAllShapes();

	// Create a section from all bodies
	BRepAlgoAPI_Section aSection(allShapes, aPlane, Standard_False);
	aSection.ComputePCurveOn1(Standard_True);
	aSection.Approximation(Standard_True);
	aSection.Build();

	// Get Section wire body
	TopoDS_Shape topoSection = aSection.Shape();

	// Display Section
	Handle(AIS_Shape) aisSection = new AIS_Shape(topoSection);
	aisSection->SetColor(Quantity_NOC_RED);
	aisSection->SetWidth(2);
	Display(aisSection, true);

}

void COCCViewerView::SetCapping(bool showSec)
{
	Handle(V3d_View) myView = GetView();

	// Geometric plane
	gp_Dir N(0, 0, -1);	    // z-axis in global CS
	gp_Dir Vx(1, 0, 0); 	// x-axis in global CS
	gp_Pnt P(0, 0, 0);	    // Position

	// Right hand coordinate system
	gp_Ax3 Plane(P, N, Vx);

	// Plane
	gp_Pln aPln(Plane);

	// Clipping plane
	Handle(Graphic3d_ClipPlane) aclipPlane = new Graphic3d_ClipPlane(aPln);

	// Turn on capping
	aclipPlane->SetCapping(true);
	aclipPlane->SetCappingColor(Quantity_NOC_CYAN3);

	// Activate clip plane
	aclipPlane->SetOn(true);

	// Add clip plane to view
	GetView()->AddClipPlane(aclipPlane);

	// update the view
	myView->Redraw();

	ShowAISTrihedron(false);

	if (showSec)
	{
		CreateClipSection(aPln);
	}

}

void COCCViewerView::SetCappingTex(bool showSec,CString TexturePath)
{
	// Geometric plane
	gp_Dir N(0, 0, -1);	    // z-axis in global CS
	gp_Dir Vx(1, 0, 0); 	// x-axis in global CS
	gp_Pnt P(0, 0, 0);	    // Position

	// Right hand coordinate system
	gp_Ax3 Plane(P, N, Vx);

	// Plane
	gp_Pln aPln(Plane);

	// Clipping plane
	Handle(Graphic3d_ClipPlane) aclipPlane = new Graphic3d_ClipPlane(aPln);

	// Turn on capping
	aclipPlane->SetCapping(true);

	// set the texture of clipping plane	
	TCollection_AsciiString aHatchText(TexturePath);

	Handle(Graphic3d_Texture2Dmanual) aTexture = new Graphic3d_Texture2Dmanual(aHatchText);

	if (!aTexture->IsDone())
	{
		aclipPlane->SetCappingTexture(NULL);
		aclipPlane->SetCappingColor(Quantity_NOC_CYAN3);
	}
	else
	{
		// Texture Properties
		aclipPlane->SetCappingColor(Quantity_NOC_WHITE);
		aTexture->EnableModulate();
		aTexture->EnableRepeat();
		aclipPlane->SetCappingTexture(aTexture);

		// Scale Texture 
		aclipPlane->CappingTexture()->GetParams()->SetScale(Graphic3d_Vec2(0.02, -0.02));
	}

	if (aclipPlane->CappingTexture().IsNull())
	{
		Message::SendFail("Error: no texture is set");
	}

	// Activate clip plane
	aclipPlane->SetOn(true);

	// Add clip plane to view
	AddClipPlane(aclipPlane);

	// update the view
	myView->Redraw();

	// Show Section outline
	if (showSec)
	{
		CreateClipSection(aPln);
	}
	
	ShowAISTrihedron(false);
}