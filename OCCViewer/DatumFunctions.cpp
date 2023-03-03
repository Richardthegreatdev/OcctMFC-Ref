#include "pch.h"
#include "framework.h"

#include "OCCViewerView.h"

void COCCViewerView::DisplayPlane(gp_Pln& aPlane)
{
	// Get Coordinate System of plane
	gp_Ax3 ax3 = aPlane.Position();
	gp_Ax2 ax2 = ax3.Ax2();

	// Geometric Plane
	Handle(Geom_Plane) aGPlane = GC_MakePlane(aPlane).Value();

	// AIS Plane
	Handle(AIS_Plane) aisPlane = new AIS_Plane(aGPlane);

	Display(aisPlane, Standard_False);
}

void COCCViewerView::SetupCsys()
{
	aXPl = new Geom_Axis2Placement(gp_Ax2());

	aXaxis = new AIS_Axis(aXPl, AIS_TypeOfAxis::AIS_TOAX_XAxis);
	aYaxis = new AIS_Axis(aXPl, AIS_TypeOfAxis::AIS_TOAX_YAxis);
	aZaxis = new AIS_Axis(aXPl, AIS_TypeOfAxis::AIS_TOAX_ZAxis);

	aXaxis->SetColor(Quantity_NOC_GREEN);
	aYaxis->SetColor(Quantity_NOC_RED);
	aZaxis->SetColor(Quantity_NOC_BLUE);

	aXaxis->Attributes()->DatumAspect()->SetAxisLength(50, 50, 50);
	aYaxis->Attributes()->DatumAspect()->SetAxisLength(50, 50, 50);
	aZaxis->Attributes()->DatumAspect()->SetAxisLength(50, 50, 50);
}

void COCCViewerView::DisplayCSys(bool bOn, gp_Ax2 aCsys)
{

	if (bOn)
	{
		// Set new coordinate system
		aXPl = new Geom_Axis2Placement(aCsys);
		aXaxis->SetAxis2Placement(aXPl, AIS_TypeOfAxis::AIS_TOAX_XAxis);
		aYaxis->SetAxis2Placement(aXPl, AIS_TypeOfAxis::AIS_TOAX_YAxis);
		aZaxis->SetAxis2Placement(aXPl, AIS_TypeOfAxis::AIS_TOAX_ZAxis);

		if (GetAISContext()->IsDisplayed(aXaxis))
		{
			GetAISContext()->Redisplay(aXaxis, true);
			GetAISContext()->Redisplay(aYaxis, true);
			GetAISContext()->Redisplay(aZaxis, true);
		}
		else
		{
			GetAISContext()->Display(aXaxis, true);
			GetAISContext()->Display(aYaxis, true);
			GetAISContext()->Display(aZaxis, true);
		}

		GetAISContext()->UpdateCurrentViewer();
	}
	else
	{
		// Erase previous Csys
		GetAISContext()->Remove(aXaxis, true);
		GetAISContext()->Remove(aYaxis, true);
		GetAISContext()->Remove(aZaxis, true);
		GetAISContext()->UpdateCurrentViewer();
	}
}

void COCCViewerView::SetupAISTrihedron()
{
	// Axis position
	Handle(Geom_Axis2Placement) aTrihedronAxis = new Geom_Axis2Placement(gp::XOY());

	// Initialize trihedron
	aTrihedron = new AIS_Trihedron(aTrihedronAxis);

	// Display mode
	aTrihedron->SetDisplayMode((int)AIS_DisplayMode::AIS_Shaded);

	// Text color
	aTrihedron->SetTextColor(Quantity_NOC_BLACK);
}

void COCCViewerView::ShowAISTrihedron(bool bOn)
{
	if (bOn)
	{
		bShowAISTrihedron = true;
		myAISContext->Display(aTrihedron, Standard_True);
	}
	else
	{
		bShowAISTrihedron = false;
		myAISContext->Erase(aTrihedron, Standard_True);
	}
}

void COCCViewerView::InitializeViewCube()
{
	aVCube = new AIS_ViewCube();
	aVCube->SetYup(Standard_True);
	aVCube->SetDrawAxes(Standard_False);
	aVCube->SetSize(60);
	aVCube->SetTransformPersistence(new Graphic3d_TransformPers(Graphic3d_TMF_TriedronPers,
		Aspect_TOTP_RIGHT_UPPER, Graphic3d_Vec2i(70, 70)));
}

void COCCViewerView::ShowCube(bool bOn)
{
	if (bOn)
	{
		bShowCube = true;
		myAISContext->Display(aVCube, Standard_True);
	}
	else
	{
		bShowCube = false;
		myAISContext->Erase(aVCube, Standard_True);
	}
}
