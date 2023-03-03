#include "pch.h"
#include "framework.h"

#include "OCCViewerDoc.h"
#include "OCCViewerView.h"

#include "OCCSurf.h"

void COCCViewerDoc::CylindricalSurface()
{
	Standard_Real Radius = 50;
	Standard_Real Height = 100;

	gp_Pnt Pos(0, 0, 0);
	gp_Dir D(0, 1, 0);

	gp_Ax3 A3(Pos, D);

	Handle(Geom_Surface) aSurf = OCCSurf::CylindricalSurface(A3, Radius, Height);

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, aSurf);

	DisplaySurface();
}

void COCCViewerDoc::SphericalSurface()
{
	Standard_Real Radius = 50;

	gp_Pnt Pos(0, 0, 0);
	gp_Dir D(0, 1, 0);

	gp_Ax3 A3(Pos, D);

	Handle(Geom_Surface) aSurf = OCCSurf::SphericalSurface(A3, Radius);

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, aSurf);

	DisplaySurface();
}

void COCCViewerDoc::PlaneSurface()
{
	// TODO: Add your command handler code here

	gp_Pnt Pos(10, 0, 0);
	gp_Dir D(1, 0, 0);

	gp_Pln aPlane(Pos, D);

	Handle(Geom_Surface) aSurf = OCCSurf::PlaneSurface(aPlane);

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, aSurf);

	GetView()->DisplayPlane(aPlane);
}

void COCCViewerDoc::ExtrudeLinearSurface()
{
	// Create BSpline curve
	CreateBSplineCurve();

	DisplaySketch();

	::Sleep(1000);
	
	// z-direction
	gp_Dir aDir(0, 0, 1);

	// Extrude Infinite surface in z-direction
	Handle(Geom_SurfaceOfLinearExtrusion) aSurfLE =
		new Geom_SurfaceOfLinearExtrusion(aBSCrv, aDir);

	// Trimmed surface 
	Handle(Geom_RectangularTrimmedSurface) aTrimmedSurface =
		new Geom_RectangularTrimmedSurface(aSurfLE, -10, 10, false);

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, aTrimmedSurface);

	GetView()->ClearContext(1);

	DisplaySurface();
}

void COCCViewerDoc::RevolveSurface()
{
	bIs2d = false;

	TColgp_Array1OfPnt array(1, 5);
	array.SetValue(1, gp_Pnt(0, 0, 1));
	array.SetValue(2, gp_Pnt(1, 2, 2));
	array.SetValue(3, gp_Pnt(2, 3, 3));
	array.SetValue(4, gp_Pnt(4, 3, 4));
	array.SetValue(5, gp_Pnt(5, 5, 5));

	Handle(Geom_BSplineCurve) aCurve = GeomAPI_PointsToBSpline(array).Curve();

	Curves3d.Resize(1, 1, false);
	Curves3d.SetValue(1, aCurve);

	DisplaySketch();

	::Sleep(1000);

	// Create surface of revolution
	Handle(Geom_SurfaceOfRevolution) SOR =
		new Geom_SurfaceOfRevolution(aCurve, gp::OX());

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, SOR);

	GetView()->ClearContext(1);

	DisplaySurface();
}

void COCCViewerDoc::LoftSurface()
{
	// First profile
	bIs2d = false;

	// Create Points
	gp_Pnt P1(0, 0, 0);
	gp_Pnt P2(0, 20, 0);
	gp_Pnt P3(10, 30, 0);
	gp_Pnt P4(50, 30, 0);
	gp_Pnt P5(60, 20, 0);
	gp_Pnt P6(60, 0, 0);

	Pnts3d.Resize(1, 6, false);
	Pnts3d.SetValue(1, P1);
	Pnts3d.SetValue(2, P2);
	Pnts3d.SetValue(3, P3);
	Pnts3d.SetValue(4, P4);
	Pnts3d.SetValue(5, P5);
	Pnts3d.SetValue(6, P6);

	gp_Pnt Cnt1(10, 20, 0);
	gp_Pnt Cnt2(50, 20, 0);

	// Lines
	Handle(Geom_TrimmedCurve) L1 = GC_MakeSegment(P1, P2).Value();
	Handle(Geom_TrimmedCurve) L2 = GC_MakeSegment(P3, P4).Value();
	Handle(Geom_TrimmedCurve) L3 = GC_MakeSegment(P5, P6).Value();

	// Arcs
	gp_Dir D(0, 0, 1);

	// first circular arc
	gp_Ax2 Ax1(Cnt1, D);
	gp_Circ gC1(Ax1, 10);
	Handle(Geom_TrimmedCurve) C1 = GC_MakeArcOfCircle(gC1, P3, P2, true).Value();

	// Second circular arc
	gp_Ax2 Ax2(Cnt2, D);
	gp_Circ gC2(Ax2, 10);
	Handle(Geom_TrimmedCurve) C2 = GC_MakeArcOfCircle(gC2, P5, P4, true).Value();

	Curves3d.Resize(1, 5, false);
	Curves3d.SetValue(1, L1);
	Curves3d.SetValue(2, C1);
	Curves3d.SetValue(3, L2);
	Curves3d.SetValue(4, C2);
	Curves3d.SetValue(5, L3);

	TopoDS_Wire profile1 = CreateWire(Curves3d);

	GetView()->DisplayShape(profile1);
	::Sleep(500);

	// Second Profile

	gp_Pnt Cnt3(30, 0, -100);
	gp_Ax2 Ax3(Cnt3, D);
	gp_Circ gC3(Ax3, 60);

	gp_Pnt PC1(-30, 0, -100);
	gp_Pnt PC2(90, 0, -100);

	Handle(Geom_TrimmedCurve) C3 = GC_MakeArcOfCircle(gC3, PC2, PC1, true).Value();

	TopoDS_Wire profile2 = CreateWire(C3);

	GetView()->DisplayShape(profile2);

	BRepOffsetAPI_ThruSections BTS;
	BTS.AddWire(profile1);
	BTS.AddWire(profile2);

	Shape = BTS.Shape();

	::Sleep(500);

	GetView()->ClearContext(1);

	GetView()->DisplayShape(Shape);
}

void COCCViewerDoc::ThickenSurface()
{
	// Create Lofted surface
	LoftSurface();

	BRepOffset_MakeOffset aSolid;

	aSolid.Initialize(Shape, 3.0, 1.e-5, BRepOffset_Skin,
		false, false, GeomAbs_Intersection, true);

	aSolid.MakeOffsetShape();
	TopoDS_Shape thkSol = aSolid.Shape();

	Shape = thkSol;

	::Sleep(500);

	GetView()->ClearContext(1);

	GetView()->DisplayShape(Shape);
	
}


void COCCViewerDoc::CreateBSplineSurface()
{
	bIs2d = false;

	// Create Poles
	TColgp_Array2OfPnt Poles(1, 4, 1, 4);	// 4x4 matrix

	Poles.SetValue(1, 1, gp_Pnt(-15, 0, 15));
	Poles.SetValue(1, 2, gp_Pnt(-15, 5, 5));
	Poles.SetValue(1, 3, gp_Pnt(-15, 5, -5));
	Poles.SetValue(1, 4, gp_Pnt(-15, 0, -15));

	Poles.SetValue(2, 1, gp_Pnt(-5, 5, 15));
	Poles.SetValue(2, 2, gp_Pnt(-5, 10, 5));
	Poles.SetValue(2, 3, gp_Pnt(-5, 10, -5));
	Poles.SetValue(2, 4, gp_Pnt(-5, 5, -15));

	Poles.SetValue(3, 1, gp_Pnt(5, 5, 15));
	Poles.SetValue(3, 2, gp_Pnt(5, 10, 5));
	Poles.SetValue(3, 3, gp_Pnt(5, 10, -5));
	Poles.SetValue(3, 4, gp_Pnt(5, 5, -15));

	Poles.SetValue(4, 1, gp_Pnt(15, 0, 15));
	Poles.SetValue(4, 2, gp_Pnt(15, 5, 5));
	Poles.SetValue(4, 3, gp_Pnt(15, 5, -5));
	Poles.SetValue(4, 4, gp_Pnt(15, 0, -15));

	// Approximates a BSpline Surface passing through an array of Points
	GeomAPI_PointsToBSplineSurface PTBSS(Poles);

	// Get BSpline Surface
	Handle(Geom_BSplineSurface) S = PTBSS.Surface();

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, S);

	DisplayPoints(Poles);
}

void COCCViewerDoc::CreateAppSurf()
{
	bIs2d = false;

	// Create Points
	gp_Pnt P1(5, 0, -20);
	gp_Pnt P2(0, 0, 0);
	gp_Pnt P3(5, 0, 20);

	gp_Pnt P4(40, 100, -20);
	gp_Pnt P5(35, 100, 0);
	gp_Pnt P6(40, 100, 20);

	gp_Pnt Cnt(100, -10, 0);

	gp_Pnt P7(40, 2, -20);
	gp_Pnt P8(17.5, 50, 0);
	gp_Pnt P9(40, 100, 20);

	gp_Pnt P33(10, 60, -17);
	gp_Pnt P44(10, 60, 17);

	Pnts3d.Resize(1, 8, false);
	Pnts3d.SetValue(1, P1);
	Pnts3d.SetValue(2, P2);
	Pnts3d.SetValue(3, P3);
	Pnts3d.SetValue(4, P4);
	Pnts3d.SetValue(5, P5);
	Pnts3d.SetValue(6, P6);
	Pnts3d.SetValue(7, P33);
	Pnts3d.SetValue(8, P44);

	// Top Arc
	Handle(Geom_TrimmedCurve) aTopCrv = GC_MakeArcOfCircle(P4, P5, P6);

	// Bottom Arc
	Handle(Geom_TrimmedCurve) aBotCrv = GC_MakeArcOfCircle(P1, P2, P3);

	// Mid line
	Handle(Geom_TrimmedCurve) aMidCrv = CreateLine(P33, P44);

	TColGeom_ListOfCurve curvs;
	curvs.Append(aBotCrv);
	curvs.Append(aMidCrv);
	curvs.Append(aTopCrv);

	Handle(Geom_BSplineSurface) aSwptSurf = OCCSurf::MakeSurface(curvs);

	Curves3d.Resize(1, 3, false);
	Curves3d.SetValue(1, aTopCrv);
	Curves3d.SetValue(2, aBotCrv);
	Curves3d.SetValue(3, aMidCrv);

	DisplaySketch();

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, aSwptSurf);

	::Sleep(1000);
}

void COCCViewerDoc::CreateAppSweepSurf()
{
	bIs2d = false;

	// Create Points
	gp_Pnt P1(5, 0, -20);
	gp_Pnt P2(0, 0, 0);
	gp_Pnt P3(5, 0, 20);
	gp_Pnt P4(35, 100, 0);


	Pnts3d.Resize(1, 4, false);
	Pnts3d.SetValue(1, P1);
	Pnts3d.SetValue(2, P2);
	Pnts3d.SetValue(3, P3);
	Pnts3d.SetValue(4, P4);

	// Bottom Arc
	Handle(Geom_TrimmedCurve) aBotCrv = GC_MakeArcOfCircle(P1, P2, P3);

	// Build Path
	gp_Vec V(0, 1, 0);
	Handle(Geom_TrimmedCurve) Path = GC_MakeArcOfCircle(P2, V, P4);

	// Create Sweep Surface
	Handle(Geom_BSplineSurface) aSwptSurf = OCCSurf::MakeSweptSurface(Path, aBotCrv);

	Curves3d.Resize(1, 2, false);
	Curves3d.SetValue(1, aBotCrv);
	Curves3d.SetValue(2, Path);

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, aSwptSurf);

	DisplaySketch();
	::Sleep(1000);
}

void COCCViewerDoc::CreateLoftSurf()
{
	bIs2d = false;

	// Create Points
	gp_Pnt P1(0, 0, -20);
	gp_Pnt P2(0, 0, 20);
	gp_Pnt P3(10, 60, -17);
	gp_Pnt P4(10, 60, 17);
	gp_Pnt P5(40, 100, -14);
	gp_Pnt P6(40, 100, 14);

	Pnts3d.Resize(1, 6, false);
	Pnts3d.SetValue(1, P1);
	Pnts3d.SetValue(2, P2);
	Pnts3d.SetValue(3, P3);
	Pnts3d.SetValue(4, P4);
	Pnts3d.SetValue(5, P5);
	Pnts3d.SetValue(6, P6);

	gp_Pnt Mid(35, (P5.Y() + P6.Y()) / 2., (P5.Z() + P6.Z()) / 2.);

	// Top line
	Handle(Geom_TrimmedCurve) aTopCrv = CreateLine(P5, P6);

	// Bottom line
	Handle(Geom_TrimmedCurve) aBotCrv = CreateLine(P1, P2);

	// Mid line
	Handle(Geom_TrimmedCurve) aMidCrv = CreateLine(P3, P4);

	Curves3d.Resize(1, 3, false);
	Curves3d.SetValue(1, aBotCrv);
	Curves3d.SetValue(2, aMidCrv);
	Curves3d.SetValue(3, aTopCrv);

	TColGeom_SequenceOfCurve curvs;
	curvs.Append(aBotCrv);
	curvs.Append(aMidCrv);
	curvs.Append(aTopCrv);

	GeomFill_NSections surf(curvs);
	surf.ComputeSurface();

	Handle(Geom_BSplineSurface) aSwptSurf = surf.BSplineSurface();

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, aSwptSurf);

	DisplaySketch();
	::Sleep(1000);
}

void COCCViewerDoc::CreateConstrainedSurface()
{
	bIs2d = false;

	// Create Points
	gp_Pnt P1(0, 0, 0);
	gp_Pnt P2(50, 0, 0);
	gp_Pnt P3(50, 0, -50);
	gp_Pnt P4(0, 0, -50);

	Pnts3d.Resize(1, 4, false);
	Pnts3d.SetValue(1, P1);
	Pnts3d.SetValue(2, P2);
	Pnts3d.SetValue(3, P3);
	Pnts3d.SetValue(4, P4);

	Handle(Geom_TrimmedCurve) L1 = CreateLine(P1, P2);
	Handle(Geom_TrimmedCurve) L2 = CreateLine(P2, P3);
	Handle(Geom_TrimmedCurve) L3 = CreateLine(P3, P4);
	Handle(Geom_TrimmedCurve) L4 = CreateLine(P4, P1);

	Curves3d.Resize(1, 4, false);
	Curves3d.SetValue(1, L1);
	Curves3d.SetValue(2, L2);
	Curves3d.SetValue(3, L3);
	Curves3d.SetValue(4, L4);

	// Adapter curves
	Handle(GeomAdaptor_HCurve) cL1 = new GeomAdaptor_HCurve(L1);
	Handle(GeomAdaptor_HCurve) cL2 = new GeomAdaptor_HCurve(L2);
	Handle(GeomAdaptor_HCurve) cL3 = new GeomAdaptor_HCurve(L3);
	Handle(GeomAdaptor_HCurve) cL4 = new GeomAdaptor_HCurve(L4);

	// Boundary 
	Handle(GeomFill_SimpleBound) B1 = new GeomFill_SimpleBound(cL1, 0.0001, 0.0001);
	Handle(GeomFill_SimpleBound) B2 = new GeomFill_SimpleBound(cL2, 0.0001, 0.0001);
	Handle(GeomFill_SimpleBound) B3 = new GeomFill_SimpleBound(cL3, 0.0001, 0.0001);
	Handle(GeomFill_SimpleBound) B4 = new GeomFill_SimpleBound(cL4, 0.0001, 0.0001);

	// Constrained filling
	Standard_Integer aMaxDeg = BSplCLib::MaxDegree();
	Standard_Integer aMaxSeg = 10;

	GeomFill_ConstrainedFilling fillSurf(aMaxDeg, aMaxSeg);
	fillSurf.Init(B1, B2, B3, B4);

	Handle(Geom_BSplineSurface) aPlnSurf = fillSurf.Surface();

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, aPlnSurf);

	DisplaySketch();
	::Sleep(1000);
}

void COCCViewerDoc::FillBSplineSurface()
{
	bIs2d = false;

	Curves3d.Resize(1, 2, false);

	// Points for 1st curve
	gp_Pnt P1(0, 0, 0);
	gp_Pnt P2(50, 25, 0);
	gp_Pnt P3(100, 0, 0);
	gp_Pnt P4(150, 20, 0);
	gp_Pnt P5(200, 0, 0);

	TColgp_Array1OfPnt aCrv1(1, 5);
	aCrv1.SetValue(1, P1);
	aCrv1.SetValue(2, P2);
	aCrv1.SetValue(3, P3);
	aCrv1.SetValue(4, P4);
	aCrv1.SetValue(5, P5);

	Handle(Geom_BSplineCurve) aBSCrv1 = GetBSplineCurve(aCrv1);
	Curves3d.SetValue(1, aBSCrv1);

	// Points for 2st curve
	gp_Pnt P6(0, 0, 100);
	gp_Pnt P7(50, -25, 100);
	gp_Pnt P8(100, 0, 100);
	gp_Pnt P9(150, -20, 100);
	gp_Pnt P10(200, 0, 100);

	TColgp_Array1OfPnt aCrv2(1, 5);
	aCrv2.SetValue(1, P6);
	aCrv2.SetValue(2, P7);
	aCrv2.SetValue(3, P8);
	aCrv2.SetValue(4, P9);
	aCrv2.SetValue(5, P10);

	Handle(Geom_BSplineCurve) aBSCrv2 = GetBSplineCurve(aCrv2);
	Curves3d.SetValue(2, aBSCrv2);

	TColGeom_Array1OfBSplineCurve array(1, 2);

	// Make Surfaces

	// Surf bottom
	array.SetValue(1, aBSCrv1);
	array.SetValue(2, aBSCrv2);
	Handle(Geom_BSplineSurface) aSurf = OCCSurf::MakeBoundarySurface(array);

	Pnts3d.Resize(1, 10, false);
	Pnts3d.SetValue(1, P1);
	Pnts3d.SetValue(2, P2);
	Pnts3d.SetValue(3, P3);
	Pnts3d.SetValue(4, P4);
	Pnts3d.SetValue(5, P5);
	Pnts3d.SetValue(6, P6);
	Pnts3d.SetValue(7, P7);
	Pnts3d.SetValue(8, P8);
	Pnts3d.SetValue(9, P9);
	Pnts3d.SetValue(10, P10);

	Surf3D.Resize(1, 1, false);
	Surf3D.SetValue(1, aSurf);

	DisplaySketch();
	::Sleep(1000);
}

void COCCViewerDoc::MakeSewedSolid()
{
	bIs2d = false;

	// Create Points
	gp_Pnt P1(0, 0, 0);
	gp_Pnt P2(50, 0, 0);
	gp_Pnt P3(50, 0, -50);
	gp_Pnt P4(0, 0, -50);

	gp_Pnt P5(0, 50, 0);
	gp_Pnt P6(50, 50, 0);
	gp_Pnt P7(50, 50, -50);
	gp_Pnt P8(0, 50, -50);

	Pnts3d.Resize(1, 8, false);
	Pnts3d.SetValue(1, P1);
	Pnts3d.SetValue(2, P2);
	Pnts3d.SetValue(3, P3);
	Pnts3d.SetValue(4, P4);
	Pnts3d.SetValue(5, P5);
	Pnts3d.SetValue(6, P6);
	Pnts3d.SetValue(7, P7);
	Pnts3d.SetValue(8, P8);

	Handle(Geom_TrimmedCurve) L1 = CreateLine(P1, P2);
	Handle(Geom_TrimmedCurve) L2 = CreateLine(P2, P3);
	Handle(Geom_TrimmedCurve) L3 = CreateLine(P3, P4);
	Handle(Geom_TrimmedCurve) L4 = CreateLine(P4, P1);

	Handle(Geom_TrimmedCurve) L5 = CreateLine(P5, P6);
	Handle(Geom_TrimmedCurve) L6 = CreateLine(P6, P7);
	Handle(Geom_TrimmedCurve) L7 = CreateLine(P7, P8);
	Handle(Geom_TrimmedCurve) L8 = CreateLine(P8, P5);

	Handle(Geom_TrimmedCurve) L9 = CreateLine(P1, P5);
	Handle(Geom_TrimmedCurve) L10 = CreateLine(P2, P6);
	Handle(Geom_TrimmedCurve) L11 = CreateLine(P3, P7);
	Handle(Geom_TrimmedCurve) L12 = CreateLine(P8, P4);

	// Make Spline Curves
	Handle(Geom_BSplineCurve) aBSPL1 = OCCSurf::CurveToBSplineCurve(L1);
	Handle(Geom_BSplineCurve) aBSPL2 = OCCSurf::CurveToBSplineCurve(L2);
	Handle(Geom_BSplineCurve) aBSPL3 = OCCSurf::CurveToBSplineCurve(L3);
	Handle(Geom_BSplineCurve) aBSPL4 = OCCSurf::CurveToBSplineCurve(L4);

	Handle(Geom_BSplineCurve) aBSPL5 = OCCSurf::CurveToBSplineCurve(L5);
	Handle(Geom_BSplineCurve) aBSPL6 = OCCSurf::CurveToBSplineCurve(L6);
	Handle(Geom_BSplineCurve) aBSPL7 = OCCSurf::CurveToBSplineCurve(L7);
	Handle(Geom_BSplineCurve) aBSPL8 = OCCSurf::CurveToBSplineCurve(L8);

	Handle(Geom_BSplineCurve) aBSPL9 = OCCSurf::CurveToBSplineCurve(L9);
	Handle(Geom_BSplineCurve) aBSPL10 = OCCSurf::CurveToBSplineCurve(L10);
	Handle(Geom_BSplineCurve) aBSPL11 = OCCSurf::CurveToBSplineCurve(L11);
	Handle(Geom_BSplineCurve) aBSPL12 = OCCSurf::CurveToBSplineCurve(L12);

	Curves3d.Resize(1, 12, false);
	Curves3d.SetValue(1, aBSPL1);
	Curves3d.SetValue(2, aBSPL2);
	Curves3d.SetValue(3, aBSPL3);
	Curves3d.SetValue(4, aBSPL4);
	Curves3d.SetValue(5, aBSPL5);
	Curves3d.SetValue(6, aBSPL6);
	Curves3d.SetValue(7, aBSPL7);
	Curves3d.SetValue(8, aBSPL8);
	Curves3d.SetValue(9, aBSPL9);
	Curves3d.SetValue(10, aBSPL10);
	Curves3d.SetValue(11, aBSPL11);
	Curves3d.SetValue(12, aBSPL12);

	DisplaySketch();
	::Sleep(1000);

	GetView()->ClearContext(1);

	// Make Surfaces
	TColGeom_Array1OfBSplineCurve array(1, 4);
	Surf3D.Resize(1, 1, false);
	
	// Surf bottom
	array.SetValue(1, aBSPL1);
	array.SetValue(2, aBSPL2);
	array.SetValue(3, aBSPL3);
	array.SetValue(4, aBSPL4);
	Handle(Geom_BSplineSurface) abotSurf = OCCSurf::MakeBoundarySurface(array);

	Surf3D.SetValue(1, abotSurf);
	DisplaySurface();
	::Sleep(500);

	// Surf Top
	array.SetValue(1, aBSPL5);
	array.SetValue(2, aBSPL6);
	array.SetValue(3, aBSPL7);
	array.SetValue(4, aBSPL8);
	Handle(Geom_BSplineSurface) aTopSurf = OCCSurf::MakeBoundarySurface(array);

	Surf3D.SetValue(1, aTopSurf);
	DisplaySurface();
	::Sleep(500);

	// Surf Side1
	array.SetValue(1, aBSPL1);
	array.SetValue(2, aBSPL10);
	array.SetValue(3, aBSPL5);
	array.SetValue(4, aBSPL9);
	Handle(Geom_BSplineSurface) aSide1Surf = OCCSurf::MakeBoundarySurface(array);

	Surf3D.SetValue(1, aSide1Surf);
	DisplaySurface();
	::Sleep(500);

	// Surf Side2
	array.SetValue(1, aBSPL10);
	array.SetValue(2, aBSPL2);
	array.SetValue(3, aBSPL11);
	array.SetValue(4, aBSPL6);
	Handle(Geom_BSplineSurface) aSide2Surf = OCCSurf::MakeBoundarySurface(array);

	Surf3D.SetValue(1, aSide2Surf);
	DisplaySurface();
	::Sleep(500);

	// Surf Side3
	array.SetValue(1, aBSPL3);
	array.SetValue(2, aBSPL11);
	array.SetValue(3, aBSPL7);
	array.SetValue(4, aBSPL12);
	Handle(Geom_BSplineSurface) aSide3Surf = OCCSurf::MakeBoundarySurface(array);

	Surf3D.SetValue(1, aSide3Surf);
	DisplaySurface();
	::Sleep(500);

	// Surf Side4
	array.SetValue(1, aBSPL4);
	array.SetValue(2, aBSPL12);
	array.SetValue(3, aBSPL8);
	array.SetValue(4, aBSPL9);
	Handle(Geom_BSplineSurface) aSide4Surf = OCCSurf::MakeBoundarySurface(array);

	Surf3D.SetValue(1, aSide4Surf);
	DisplaySurface();
	::Sleep(500);

	// Create Shells
	TopoDS_Shell S1 = CreateShell(abotSurf);
	TopoDS_Shell S2 = CreateShell(aTopSurf);
	TopoDS_Shell S3 = CreateShell(aSide1Surf);
	TopoDS_Shell S4 = CreateShell(aSide2Surf);
	TopoDS_Shell S5 = CreateShell(aSide3Surf);
	TopoDS_Shell S6 = CreateShell(aSide4Surf);

	BRepBuilderAPI_Sewing Sew;
	Sew.Add(S1);
	Sew.Add(S2);
	Sew.Add(S3);
	Sew.Add(S4);
	Sew.Add(S5);
	Sew.Add(S6);

	GetView()->ClearContext(1);

	Sew.Perform();
	TopoDS_Shape result = Sew.SewedShape();

	GetView()->DisplayShape(result);
	::Sleep(1000);

	TopTools_Array1OfShape aSol(1, 1);
	aSol.SetValue(1, result);

	TopoDS_Solid Sol = CreateSolid(aSol);

	// Fix shells for orientation
	ShapeFix_Solid fixSol(Sol);
	fixSol.Perform();

	Shape = fixSol.Solid();

	GetView()->ClearContext(1);
	GetView()->DisplayShape(Shape);
}