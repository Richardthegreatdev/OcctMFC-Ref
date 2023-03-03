#include "pch.h"
#include "framework.h"

#include "OCCViewerView.h"

void COCCViewerView::DisplayLengthDimension(gp_Pnt& P1, gp_Pnt& P2, gp_Pnt& P3, bool isText3D)
{
	GC_MakePlane aMkPlane(P1, P2, P3);
	Handle(Geom_Plane) aPlane = aMkPlane.Value();

	// Dimension Properties
	Handle(Prs3d_DimensionAspect) anAspect = new Prs3d_DimensionAspect();

	anAspect->MakeArrows3d(false);
	anAspect->MakeText3d(isText3D);
	anAspect->MakeTextShaded(true);
	anAspect->SetCommonColor(Quantity_NOC_BLACK);

	// Length Dimension
	Handle(PrsDim_LengthDimension) aLenDim = new PrsDim_LengthDimension(P1, P2, aPlane->Pln());
	aLenDim->SetDimensionAspect(anAspect);

	Display(aLenDim, true);
}

void COCCViewerView::DisplayLengthDimension(gp_Dir aDir, gp_Pnt& P1, gp_Pnt& P2, gp_Pnt& P3, bool isText3D)
{
	GC_MakePlane aMkPlane(P1, P2, P3);
	Handle(Geom_Plane) aPlane = aMkPlane.Value();

	// Dimension Properties
	Handle(Prs3d_DimensionAspect) anAspect = new Prs3d_DimensionAspect();

	anAspect->MakeArrows3d(false);
	anAspect->MakeText3d(isText3D);
	anAspect->MakeTextShaded(true);
	anAspect->SetCommonColor(Quantity_NOC_BLACK);

	// Length Dimension
	Handle(PrsDim_LengthDimension) aLenDim = new PrsDim_LengthDimension(P1, P2, aPlane->Pln());
	aLenDim->SetDimensionAspect(anAspect);

	// Set Direction
	aLenDim->SetDirection(aDir);

	Display(aLenDim, true);
}

void COCCViewerView::DisplayDiameterDimension(TopoDS_Edge& aCircularEdge)
{
	Handle(PrsDim_DiameterDimension) aDiadim =
		new PrsDim_DiameterDimension(aCircularEdge);

	Handle(Prs3d_DimensionAspect) anAspect = 
		            new Prs3d_DimensionAspect();
	anAspect->SetCommonColor(Quantity_NOC_BLACK);

	aDiadim->SetDimensionAspect(anAspect);

	Display(aDiadim, true);
}

void COCCViewerView::DisplayRadiusDimension(TopoDS_Edge& aCircularEdge)
{
	Handle(PrsDim_RadiusDimension) aRaddim =
		new PrsDim_RadiusDimension(aCircularEdge);

	Handle(Prs3d_DimensionAspect) anAspect = 
		            new Prs3d_DimensionAspect();
	anAspect->SetCommonColor(Quantity_NOC_BLACK);

	aRaddim->SetDimensionAspect(anAspect);

	Display(aRaddim, true);
}

void COCCViewerView::DisplayRadiusDimension(gp_Pnt& ancP, TopoDS_Edge& aCircularEdge)
{
	Handle(PrsDim_RadiusDimension) aRaddim =
		new PrsDim_RadiusDimension(aCircularEdge);

	Handle(Prs3d_DimensionAspect) anAspect = new Prs3d_DimensionAspect();
	anAspect->SetCommonColor(Quantity_NOC_BLACK);
	aRaddim->SetMeasuredGeometry(aCircularEdge, ancP);
	aRaddim->SetDimensionAspect(anAspect);

	Display(aRaddim, true);
}