// COCCViewerDoc.cpp : implementation file
//

#include "pch.h"
#include "OCCViewer.h"

#include "OCCViewerDoc.h"
#include "OCCViewerView.h"

#include "AISMesh.h"

#include "ImportExport.h"

// COCCViewerDoc

IMPLEMENT_DYNCREATE(COCCViewerDoc, CDocument)

COCCViewerDoc::COCCViewerDoc()
{
}

BOOL COCCViewerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

COCCViewerDoc::~COCCViewerDoc()
{
}


BEGIN_MESSAGE_MAP(COCCViewerDoc, CDocument)
	ON_COMMAND(ID_2DGEOMETRY_2DSKETCH, &COCCViewerDoc::On2dgeometry2dsketch)
	ON_COMMAND(ID_3DGEOMETRY_3DSKETCH, &COCCViewerDoc::On3dgeometry3dsketch)
	ON_COMMAND(ID_3DGEOMETRY_BSPLINECURVE, &COCCViewerDoc::On3dgeometryBsplinecurve)
	ON_COMMAND(ID_3DGEOMETRY_2DTO3D, &COCCViewerDoc::On3dgeometry2dto3d)
	ON_COMMAND(ID_TOPOLOGY_EXPLORE, &COCCViewerDoc::OnTopologyExplore)
	ON_COMMAND(ID_TOPOLOGY_PRIMITIVES, &COCCViewerDoc::OnTopologyPrimitives)
	ON_COMMAND(ID_TOPOLOGY_CREATEFACE, &COCCViewerDoc::OnTopologyCreateface)
	ON_COMMAND(ID_TOPOLOGY_FACEWITHHOLES, &COCCViewerDoc::OnTopologyFacewithholes)
	ON_COMMAND(ID_3DMODELING_EXTRUDEFACE, &COCCViewerDoc::On3dmodelingExtrudeface)
	ON_COMMAND(ID_3DMODELING_REVOLVEFACE, &COCCViewerDoc::On3dmodelingRevolveface)
	ON_COMMAND(ID_FEATURES_EXTRUDE, &COCCViewerDoc::OnFeaturesExtrude)
	ON_COMMAND(ID_FEATURES_EXTRUDECUT, &COCCViewerDoc::OnFeaturesExtrudecut)
	ON_COMMAND(ID_FEATURES_REVOLVE, &COCCViewerDoc::OnFeaturesRevolve)
	ON_COMMAND(ID_FEATURES_REVOLVECUT, &COCCViewerDoc::OnFeaturesRevolvecut)
	ON_COMMAND(ID_FEATURES_OFFSET, &COCCViewerDoc::OnFeaturesOffset)
	ON_COMMAND(ID_FEATURES_SWEEP, &COCCViewerDoc::OnFeaturesSweep)
	ON_COMMAND(ID_FEATURES_LOFT, &COCCViewerDoc::OnFeaturesLoft)
	ON_COMMAND(ID_FEATURES_SHELL, &COCCViewerDoc::OnFeaturesShell)
	ON_COMMAND(ID_3DMODELING_2DFILLET, &COCCViewerDoc::On3dmodeling2dfillet)
	ON_COMMAND(ID_3DMODELING_2DCHAMFER, &COCCViewerDoc::On3dmodeling2dchamfer)
	ON_COMMAND(ID_3DMODELING_3DFILLET, &COCCViewerDoc::On3dmodeling3dfillet)
	ON_COMMAND(ID_3DMODELING_3DCHAMFER, &COCCViewerDoc::On3dmodeling3dchamfer)
	ON_COMMAND(ID_TRANSFORMATIONS_TRANSLATE, &COCCViewerDoc::OnTransformationsTranslate)
	ON_COMMAND(ID_TRANSFORMATIONS_ROTATE, &COCCViewerDoc::OnTransformationsRotate)
	ON_COMMAND(ID_TRANSFORMATIONS_MIRROR, &COCCViewerDoc::OnTransformationsMirror)
	ON_COMMAND(ID_BOOLEANOPERATIONS_ADD, &COCCViewerDoc::OnBooleanoperationsAdd)
	ON_COMMAND(ID_BOOLEANOPERATIONS_CUT, &COCCViewerDoc::OnBooleanoperationsCut)
	ON_COMMAND(ID_BOUNDINGBOX_AXIS, &COCCViewerDoc::OnBoundingboxAxis)
	ON_COMMAND(ID_BOUNDINGBOX_ORIENTED, &COCCViewerDoc::OnBoundingboxOriented)
	ON_COMMAND(ID_MODELPROPERTIES_LINEARPROPERTIES, &COCCViewerDoc::OnModelpropertiesLinearproperties)
	ON_COMMAND(ID_MODELPROPERTIES_SURFACEPROPERTIES, &COCCViewerDoc::OnModelpropertiesSurfaceproperties)
	ON_COMMAND(ID_MODELPROPERTIES_VOLUMEPROPERTIES, &COCCViewerDoc::OnModelpropertiesVolumeproperties)
	ON_COMMAND(ID_ELEMENTARY_CYLINDER, &COCCViewerDoc::OnElementaryCylinder)
	ON_COMMAND(ID_ELEMENTARY_SPHERICAL, &COCCViewerDoc::OnElementarySpherical)
	ON_COMMAND(ID_ELEMENTARY_PLANAR, &COCCViewerDoc::OnElementaryPlanar)
	ON_COMMAND(ID_SWEPT_LINEAREXTRUSION, &COCCViewerDoc::OnSweptLinearextrusion)
	ON_COMMAND(ID_SWEPT_REVOLVE, &COCCViewerDoc::OnSweptRevolve)
	ON_COMMAND(ID_SWEPT_LOFT, &COCCViewerDoc::OnSweptLoft)
	ON_COMMAND(ID_SURFACEMODELING_THICKEN, &COCCViewerDoc::OnSurfacemodelingThicken)
	ON_COMMAND(ID_SURFACEMODELING_BSPLINESURFACE, &COCCViewerDoc::OnSurfacemodelingBsplinesurface)
	ON_COMMAND(ID_ADVANCED_FILLBSPLINES, &COCCViewerDoc::OnAdvancedFillbsplines)
	ON_COMMAND(ID_ADVANCED_FILLBSPLINESURFACE, &COCCViewerDoc::OnAdvancedFillbsplinesurface)
	ON_COMMAND(ID_ADVANCED_FILLBSPLINESURFACE32836, &COCCViewerDoc::OnAdvancedFillbsplinesurface32836)
	ON_COMMAND(ID_ADVANCED_FILLCONSTRAINEDSURFACE, &COCCViewerDoc::OnAdvancedFillconstrainedsurface)
	ON_COMMAND(ID_ADVANCED_FILLBSPLINECURVESURFACE, &COCCViewerDoc::OnAdvancedFillbsplinecurvesurface)
	ON_COMMAND(ID_SURFACEMODELING_SEWSURFACES, &COCCViewerDoc::OnSurfacemodelingSewsurfaces)
	ON_COMMAND(ID_DISPLAYMODE_SHADED, &COCCViewerDoc::OnDisplaymodeShaded)
	ON_COMMAND(ID_DISPLAYMODE_WIREFRAME, &COCCViewerDoc::OnDisplaymodeWireframe)
	ON_COMMAND(ID_SELECTIONMODE_VERTEX, &COCCViewerDoc::OnSelectionmodeVertex)
	ON_COMMAND(ID_SELECTIONMODE_EDGE, &COCCViewerDoc::OnSelectionmodeEdge)
	ON_COMMAND(ID_SELECTIONMODE_FACE, &COCCViewerDoc::OnSelectionmodeFace)
	ON_COMMAND(ID_SELECTIONMODE_SOLID, &COCCViewerDoc::OnSelectionmodeSolid)
	ON_COMMAND(ID_FILTER_PLANARFACES, &COCCViewerDoc::OnFilterPlanarfaces)
	ON_COMMAND(ID_MODELCLIPPING_NORMALCLIPPING, &COCCViewerDoc::OnModelclippingNormalclipping)
	ON_COMMAND(ID_MODELCLIPPING_CLIPPINGWITHTEXTURE, &COCCViewerDoc::OnModelclippingClippingwithtexture)
	ON_COMMAND(ID_MODELCLIPPING_CLIPPINGWITHSECTION, &COCCViewerDoc::OnModelclippingClippingwithsection)
	ON_COMMAND(ID_VISUALIZATION_MESH, &COCCViewerDoc::OnVisualizationMesh)
	ON_COMMAND(ID_DIMENSIONS_LENGTH, &COCCViewerDoc::OnDimensionsLength)
	ON_COMMAND(ID_DIMENSIONS_RADIUS, &COCCViewerDoc::OnDimensionsRadius)
	ON_COMMAND(ID_DIMENSIONS_DIAMETER, &COCCViewerDoc::OnDimensionsDiameter)
	ON_COMMAND(ID_IMPORT_IGES, &COCCViewerDoc::OnImportIges)
	ON_COMMAND(ID_IMPORT_STEP, &COCCViewerDoc::OnImportStep)
	ON_COMMAND(ID_IMPORT_STL, &COCCViewerDoc::OnImportStl)
	ON_COMMAND(ID_EXPORT_IGES, &COCCViewerDoc::OnExportIges)
	ON_COMMAND(ID_EXPORT_STEP, &COCCViewerDoc::OnExportStep)
	ON_COMMAND(ID_EXPORT_STL, &COCCViewerDoc::OnExportStl)
END_MESSAGE_MAP()


// COCCViewerDoc diagnostics

#ifdef _DEBUG
void COCCViewerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void COCCViewerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// COCCViewerDoc serialization

void COCCViewerDoc::Serialize(CArchive& ar)
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


// COCCViewerDoc commands

COCCViewerView* COCCViewerDoc::GetView()
{
	POSITION pos = GetFirstViewPosition();
	return (COCCViewerView*)GetNextView(pos);
}

void COCCViewerDoc::DisplaySketch()
{
	if (bIs2d)
	{
		// Display 2d Curves
		for (int i = Curves2d.Lower(); i <= Curves2d.Upper(); i++)
		{
			Handle(Geom2d_Curve) L = Curves2d.Value(i);
			GetView()->DisplayCurve(L, Aspect_TOL_SOLID, Aspect_WOL_THICK, Quantity_NOC_DARKSLATEBLUE, Standard_False);
		}

		// Display 2d Points
		int cnt = 1;
		for (int i = Pnts2d.Lower(); i <= Pnts2d.Upper(); i++)
		{
			gp_Pnt2d P = Pnts2d.Value(i);
			GetView()->DisplayPoint(P);
			std::string PointName = "P" + std::to_string(cnt);
			GetView()->DisplayText(P, PointName.c_str());
			cnt += 1;
		}
	}
	else
	{
		// Display 3d Curves
		for (int i = Curves3d.Lower(); i <= Curves3d.Upper(); i++)
		{
			Handle(Geom_Curve) L = Curves3d.Value(i);
			GetView()->DisplayCurve(L, Quantity_NOC_DARKSLATEBLUE, Standard_False);
		}

		// Display 3d Points
		int cnt = 1;
		for (int i = Pnts3d.Lower(); i <= Pnts3d.Upper(); i++)
		{
			gp_Pnt P = Pnts3d.Value(i);
			GetView()->DisplayPoint(P);
			std::string PointName = "P" + std::to_string(cnt);
			GetView()->DisplayText(P, PointName.c_str());
			cnt += 1;
		}
	}
}


void COCCViewerDoc::DisplaySurface()
{
	// Display 3d Surfaces
	for (int i = Surf3D.Lower(); i <= Surf3D.Upper(); i++)
	{
		Handle(Geom_Surface) S = Surf3D.Value(i);		
		GetView()->DisplaySurface(S);
	}

}

void COCCViewerDoc::DisplayPoints(TColgp_Array2OfPnt& Pnts)
{
	int cnt = 1;
	for (int i = Pnts.LowerRow(); i <= Pnts.UpperRow(); i++)
	{
		for (int j = Pnts.LowerCol(); j <= Pnts.UpperCol(); j++)
		{
			gp_Pnt P = Pnts.Value(i,j);
			GetView()->DisplayPoint(P);
			std::string PointName = "P" + std::to_string(cnt);
			GetView()->DisplayText(P, PointName.c_str());
			cnt += 1;
		}
	}
}

void COCCViewerDoc::On2dgeometry2dsketch()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Create2DGeometry();
	DisplaySketch();
}


void COCCViewerDoc::On3dgeometry3dsketch()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Create3DGeometry();
	DisplaySketch();
}


void COCCViewerDoc::On3dgeometryBsplinecurve()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	CreateBSplineCurve();
	DisplaySketch();
}


void COCCViewerDoc::On3dgeometry2dto3d()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Create2DTO3DSketch();
	DisplaySketch();
}


void COCCViewerDoc::OnTopologyExplore()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	ExploreTopology();
}


void COCCViewerDoc::OnTopologyPrimitives()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	CreatePrimitives();
}


void COCCViewerDoc::OnTopologyCreateface()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	CreateSketchFace();
}


void COCCViewerDoc::OnTopologyFacewithholes()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	CreateFaceWHoles();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::On3dmodelingExtrudeface()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	ExtrudeFace();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::On3dmodelingRevolveface()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	RevolveFace();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnFeaturesExtrude()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	ExtrudeFeature();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnFeaturesExtrudecut()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	ExtrudeCutFeature();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnFeaturesRevolve()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	RevolveFeature();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnFeaturesRevolvecut()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	RevolveCutFeature();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnFeaturesOffset()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	OffsetWire();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnFeaturesSweep()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Sweep();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnFeaturesLoft()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Loft();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnFeaturesShell()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Shelling();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::On3dmodeling2dfillet()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Fillet2D();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::On3dmodeling2dchamfer()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Chamfer2D();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::On3dmodeling3dfillet()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Fillet3D();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::On3dmodeling3dchamfer()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Chamfer3D();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnTransformationsTranslate()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Translate();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnTransformationsRotate()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Rotate();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnTransformationsMirror()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext();
	Mirror();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnBooleanoperationsAdd()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	AddBodies();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnBooleanoperationsCut()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	CutBody();
	GetView()->DisplayShape(Shape);
}


void COCCViewerDoc::OnBoundingboxAxis()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	BoundingBox();
}


void COCCViewerDoc::OnBoundingboxOriented()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	OBBBoundingBox();
}


void COCCViewerDoc::OnModelpropertiesLinearproperties()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	LinearProperties();
}


void COCCViewerDoc::OnModelpropertiesSurfaceproperties()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	SurfaceProperties();
}


void COCCViewerDoc::OnModelpropertiesVolumeproperties()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	VolumeProperties();
}


void COCCViewerDoc::OnElementaryCylinder()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	CylindricalSurface();
}


void COCCViewerDoc::OnElementarySpherical()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	SphericalSurface();
}


void COCCViewerDoc::OnElementaryPlanar()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	PlaneSurface();
}


void COCCViewerDoc::OnSweptLinearextrusion()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	ExtrudeLinearSurface();
}


void COCCViewerDoc::OnSweptRevolve()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	RevolveSurface();
}


void COCCViewerDoc::OnSweptLoft()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	LoftSurface();
}


void COCCViewerDoc::OnSurfacemodelingThicken()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	ThickenSurface();
}

void COCCViewerDoc::OnSurfacemodelingBsplinesurface()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	CreateBSplineSurface();
	DisplaySurface();
	//GetView()->OnWireFrame();
}

void COCCViewerDoc::OnAdvancedFillbsplines()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	CreateAppSurf();
	DisplaySurface();
	//GetView()->OnWireFrame();
}


void COCCViewerDoc::OnAdvancedFillbsplinesurface()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	CreateAppSweepSurf();
	DisplaySurface();
	//GetView()->OnWireFrame();
}


void COCCViewerDoc::OnAdvancedFillbsplinesurface32836()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	CreateLoftSurf();
	DisplaySurface();
	//GetView()->OnWireFrame();
}


void COCCViewerDoc::OnAdvancedFillconstrainedsurface()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	CreateConstrainedSurface();
	DisplaySurface();
	//GetView()->OnWireFrame();
}


void COCCViewerDoc::OnAdvancedFillbsplinecurvesurface()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	FillBSplineSurface();
	DisplaySurface();
	//GetView()->OnWireFrame();
}


void COCCViewerDoc::OnSurfacemodelingSewsurfaces()
{
	// TODO: Add your command handler code here
	GetView()->ClearContext(1);
	MakeSewedSolid();
	//GetView()->OnWireFrame();
}

// Visualization Functions
//==========================================================================

void COCCViewerDoc::OnDisplaymodeShaded()
{
	// TODO: Add your command handler code here
}



void COCCViewerDoc::OnDisplaymodeWireframe()
{
	// TODO: Add your command handler code here
}


void COCCViewerDoc::OnSelectionmodeVertex()
{
	// TODO: Add your command handler code here
	GetView()->SetSelectionFilter(AIS_Shape::SelectionMode(TopAbs_ShapeEnum::TopAbs_VERTEX));
}


void COCCViewerDoc::OnSelectionmodeEdge()
{
	// TODO: Add your command handler code here
	GetView()->SetSelectionFilter(AIS_Shape::SelectionMode(TopAbs_ShapeEnum::TopAbs_EDGE));
}


void COCCViewerDoc::OnSelectionmodeFace()
{
	// TODO: Add your command handler code here
	GetView()->SetSelectionFilter(AIS_Shape::SelectionMode(TopAbs_ShapeEnum::TopAbs_FACE));
}


void COCCViewerDoc::OnSelectionmodeSolid()
{
	// TODO: Add your command handler code here
	GetView()->SetSelectionFilter(0);
}


void COCCViewerDoc::OnFilterPlanarfaces()
{
	// TODO: Add your command handler code here
	GetView()->SetFaceFilter(StdSelect_Plane);
}


void COCCViewerDoc::OnModelclippingNormalclipping()
{
	// TODO: Add your command handler code here
	GetView()->SetCapping(false);
}


void COCCViewerDoc::OnModelclippingClippingwithtexture()
{
	// TODO: Add your command handler code here
	CString strTexturePath;
	strTexturePath = "E:\\IngentaCal\\Courses\\Courses\\Learn Opencascade for Custom 3D Application Development\\imgs\\hatch_1.png";

	GetView()->SetCappingTex(false, strTexturePath);
}


void COCCViewerDoc::OnModelclippingClippingwithsection()
{
	// TODO: Add your command handler code here
	CString strTexturePath;

	// Change path
	strTexturePath = "E:\\IngentaCal\\Courses\\Courses\\Learn Opencascade for Custom 3D Application Development\\imgs\\hatch_1.png";

	GetView()->SetCappingTex(true, strTexturePath);
}


void COCCViewerDoc::ShowMesh()
{
	//
	Handle(MeshDS) aMeshDS = new MeshDS();
	aMeshDS->CreateTempMesh();

	Handle(AISMesh) aisMesh = new AISMesh(aMeshDS);

	aisMesh->SetDisplayMode(0);
	GetView()->Display(aisMesh, true);
	//aMeshDS->DisplayNodes(true, GetView()->GetAISContext());
	//aMeshDS->DisplayNodeLabels(true, GetView()->GetAISContext());
}

void COCCViewerDoc::OnVisualizationMesh()
{
	// TODO: Add your command handler code here
	ShowMesh();
	GetView()->ShowAISTrihedron(false);
}

void COCCViewerDoc::LengthDimension()
{
	// Create the profile
	OffsetWire();

	// Dimension points
	gp_Pnt P1(0, 0, 0);
	gp_Pnt P2(0, 20, 0);
	gp_Pnt P3(10, 30, 0);
	gp_Pnt P4(50, 30, 0);
	gp_Pnt P5(60, 20, 0);
	gp_Pnt P6(60, 0, 0);

	gp_Pnt Cnt1(30, -5, 0);
	GetView()->DisplayLengthDimension(P1, P6, Cnt1, false);


	gp_Pnt Cnt2(65, 5, 0);
	gp_Dir aHDir(0, -1, 0);
	GetView()->DisplayLengthDimension(aHDir,P4, P6, Cnt2, false);

}

void COCCViewerDoc::OnDimensionsLength()
{
	// TODO: Add your command handler code here
	LengthDimension();
}


void COCCViewerDoc::OnDimensionsRadius()
{
	// TODO: Add your command handler code here

	gp_Ax2 Ax(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0));
	gp_Circ aCir(Ax, 50);

	Handle(Geom_Curve) aCrv = GetCurve(aCir);
	TopoDS_Edge E = CreateEdge(aCrv);

	GetView()->DisplayShape(E);
	GetView()->DisplayRadiusDimension(E);
}


void COCCViewerDoc::OnDimensionsDiameter()
{
	// TODO: Add your command handler code here

	gp_Ax2 Ax(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0));
	gp_Circ aCir(Ax, 50);

	Handle(Geom_Curve) aCrv = GetCurve(aCir);
	TopoDS_Edge E = CreateEdge(aCrv);

	GetView()->DisplayShape(E);
	GetView()->DisplayDiameterDimension(E);
}

void COCCViewerDoc::ImportStep()
{
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Step Files (*.step , *.stp)|*.step; *.stp|All Files (*.*)|*.*||",
		NULL, 0, TRUE);

	if (dlg.DoModal() == IDOK)
	{
		Handle(TopTools_HSequenceOfShape) TopoDsShapes;

		CStringA C(dlg.GetPathName());
		LPCSTR aFileName = C;

		TopoDsShapes = OCCDataExchange::ImportStep(aFileName);

		if (!TopoDsShapes.IsNull())
		{
			for (int i = 1; i <= TopoDsShapes->Length(); i++)
			{
				TopoDS_Shape aShape = TopoDsShapes->Value(i);
				GetView()->DisplayShape(aShape);
			}
		}

	}

}

void COCCViewerDoc::ImportIGES()
{
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Iges Files (*.iges , *.igs)|*.iges; *.igs|All Files (*.*)|*.*||",
		NULL, 0, TRUE);

	if (dlg.DoModal() == IDOK)
	{
		CStringA C(dlg.GetPathName());
		LPCSTR aFileName = C;

		TopoDS_Shape aShape = OCCDataExchange::ImportIges(aFileName);
		GetView()->DisplayShape(aShape);		

	}
}

void COCCViewerDoc::ImportStl()
{
	CFileDialog dlg(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Stl Files (*.stl)|*.stl|All Files (*.*)|*.*||",
		NULL, 0, TRUE);

	if (dlg.DoModal() == IDOK)
	{
		CStringA C(dlg.GetPathName());
		LPCSTR aFileName = C;

		Handle(Poly_Triangulation) aShape = OCCDataExchange::ImportStl(aFileName);

		Handle(AIS_Triangulation) aStlShape = new AIS_Triangulation(aShape);
		aStlShape->SetDisplayMode(0);
		GetView()->Display(aStlShape, true);

	}

}

void COCCViewerDoc::ExportStep()
{
	// Get all 3d objects in a compound
	TopoDS_Compound allShapes = GetView()->GetAllShapes();

	CFileDialog dlg(FALSE,
		L"step",
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Step Files (*.step , *.stp)|*.step; *.stp|All Files (*.*)|*.*||",
		NULL, 0, TRUE);

	if (dlg.DoModal() == IDOK)
	{
		CStringA C(dlg.GetPathName());
		LPCSTR aFileName = C;

		bool ares = OCCDataExchange::ExportStep(aFileName, allShapes);

	}

}

void COCCViewerDoc::ExportIGES()
{
	// Get all 3d objects in a compound
	TopoDS_Compound allShapes = GetView()->GetAllShapes();

	CFileDialog dlg(FALSE,
		L"iges",
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Iges Files (*.iges , *.igs)|*.iges; *.igs|All Files (*.*)|*.*||",
		NULL, 0, TRUE);

	if (dlg.DoModal() == IDOK)
	{
		CStringA C(dlg.GetPathName());
		LPCSTR aFileName = C;

		bool ares = OCCDataExchange::ExportIges(aFileName, allShapes);

	}
}

void COCCViewerDoc::ExportStl()
{
	// Get all 3d objects in a compound
	TopoDS_Compound allShapes = GetView()->GetAllShapes();

	CFileDialog dlg(FALSE,
		L"stl",
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Stl Files (*.stl)|*.stl|All Files (*.*)|*.*||",
		NULL, 0, TRUE);

	if (dlg.DoModal() == IDOK)
	{
		CStringA C(dlg.GetPathName());
		LPCSTR aFileName = C;

		bool ares = OCCDataExchange::ExportStl(aFileName, allShapes);

	}
}

void COCCViewerDoc::OnImportIges()
{
	// TODO: Add your command handler code here
	ImportIGES();
}


void COCCViewerDoc::OnImportStep()
{
	// TODO: Add your command handler code here
	ImportStep();
}


void COCCViewerDoc::OnImportStl()
{
	// TODO: Add your command handler code here
	ImportStl();
}


void COCCViewerDoc::OnExportIges()
{
	// TODO: Add your command handler code here
	ExportIGES();
}


void COCCViewerDoc::OnExportStep()
{
	// TODO: Add your command handler code here
	ExportStep();
}


void COCCViewerDoc::OnExportStl()
{
	// TODO: Add your command handler code here
	ExportStl();
}
