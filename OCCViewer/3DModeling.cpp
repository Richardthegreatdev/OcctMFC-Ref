#include "pch.h"
#include "framework.h"

#include "OCCViewerDoc.h"
#include "OCCViewerView.h"


void COCCViewerDoc::ExploreTopology()
{

    // Create Box
    Shape = BRepPrimAPI_MakeBox(200., 150., 100.).Shape();
    Quantity_Color acolor = Quantity_NOC_SLATEBLUE;
    GetView()->DisplayShape(Shape, acolor);

    // Data
    int NbVertices = 0;
    int NbEdges = 0;
    int NbWires = 0;
    int NbFaces = 0;

    TopExp_Explorer exp;
    TopTools_IndexedMapOfShape fmap, emap, vmap, wmap;

    // Faces
    for (exp.Init(Shape, TopAbs_FACE); exp.More(); exp.Next())
    {
        TopoDS_Face face = TopoDS::Face(exp.Current()); 
        if (fmap.FindIndex(face) < 1)
        {
            fmap.Add(face);
            NbFaces += 1;
        }
    }

    // Wires
    for (exp.Init(Shape, TopAbs_WIRE); exp.More(); exp.Next())
    {
        TopoDS_Wire wire = TopoDS::Wire(exp.Current());
        if (wmap.FindIndex(wire) < 1)
        {
            wmap.Add(wire);
            NbWires += 1;
        }
    }

    // Edges
    for (exp.Init(Shape, TopAbs_EDGE); exp.More(); exp.Next())
    {
        TopoDS_Edge edge = TopoDS::Edge(exp.Current());
        if (emap.FindIndex(edge) < 1)
        {
            emap.Add(edge);
            NbEdges += 1;
        }
    }

    // Vertices
    for (exp.Init(Shape, TopAbs_VERTEX); exp.More(); exp.Next())
    {
        TopoDS_Vertex vertex = TopoDS::Vertex(exp.Current());
        if (vmap.FindIndex(vertex) < 1)
        {
            vmap.Add(vertex);
            NbVertices += 1;
        }
    }

    CString str;

    str.Format(L"Vertices:%d , Edges:%d , Wires:%d , Faces:%d", NbVertices, NbEdges, NbWires, NbFaces);
    AfxMessageBox(str);

}

void COCCViewerDoc::CreatePrimitives()
{
    // Cylinder
    TopoDS_Shape C = BRepPrimAPI_MakeCylinder(50., 200.).Shape();

    // Box
    TopoDS_Shape B = BRepPrimAPI_MakeBox(gp_Pnt(100, 0, 0), 200., 150., 100.).Shape();

    // Cone
    TopoDS_Shape C2 = BRepPrimAPI_MakeCone(gp_Ax2(gp_Pnt(400., 0., 0.), gp_Dir(0., 0., 1.)),
        60., 0., 150., 210. * M_PI / 180).Shape();

    // Sphere
    TopoDS_Shape S = BRepPrimAPI_MakeSphere(gp_Pnt(0., 0., 400.), 80.).Shape();

    // Torus
    TopoDS_Shape T = BRepPrimAPI_MakeTorus(gp_Ax2(gp_Pnt(200., 0., 400.),
        gp_Dir(0., 0., 1.)), 60., 20.).Shape();
    
    // Wedge
    TopoDS_Shape W = BRepPrimAPI_MakeWedge(gp_Ax2(gp_Pnt(400., 0., 400.), gp_Dir(0., 0., 1.))
        , 60., 100., 80., 20.).Shape();

    GetView()->DisplayShape(B);
    GetView()->DisplayShape(C);
    GetView()->DisplayShape(C2);
    GetView()->DisplayShape(S);
    GetView()->DisplayShape(T);
    GetView()->DisplayShape(W);
}

void COCCViewerDoc::CreateSketchFace()
{
    // Create sketch
    Create2DTO3DSketch();

    // Create Face
    TopoDS_Face aFace = CreateFace(Curves3d);

    Shape = aFace;

    GetView()->DisplayShape(Shape);
}

void COCCViewerDoc::CreateFaceWHoles()
{
    // Create Circle on z-plane
    gp_Pnt Pos(0, 0, 0);
    gp_Dir Dir(1, 0, 0);
    gp_Ax2 ax2(Pos, Dir);
    gp_Circ aCircle(ax2, 50);

    // Outer Wire
    Handle(Geom_Curve) L = GetCurve(aCircle);
    TopoDS_Wire OuterWire = CreateWire(L);

    // Bolt Holes
    int n = 6;
    int boltPCR = 35;
    int boltDia = 8;
    TopTools_Array1OfShape InnerWires(1, n + 1);

    double theta = (2. * M_PI) / n;
    for (int i = 0;i < n;i++)
    {
        // Bolt centers
        double x = boltPCR * cos(theta * i);
        double y = boltPCR * sin(theta * i);

        // Bolt circle
        gp_Ax2 Ax(gp_Pnt(0, y, x), gp_Dir(1, 0, 0));
        gp_Circ bolthole(Ax, boltDia * 0.5);

        // inner Wire
        Handle(Geom_Curve) BH = GetCurve(bolthole);
        TopoDS_Wire IW = CreateWire(BH);

        // Add to list
        InnerWires.SetValue(i + 1, IW);
    }

    // Add Center Hole
    gp_Circ CH(ax2, 16);
    Handle(Geom_Curve) chL = GetCurve(CH);
    TopoDS_Wire CHWire = CreateWire(chL);
    InnerWires.SetValue(n + 1, CHWire);

    // Create face with hole
    TopoDS_Face aHoleFace = CreateFaceWithHole(OuterWire, InnerWires);

    Shape = aHoleFace;
}

void COCCViewerDoc::ExtrudeFace()
{
    // Create 2D sketch
    Create2DTO3DSketch();

    // Create Face
    TopoDS_Face skFace = CreateFace(Curves3d);

    GetView()->DisplayShape(skFace);

    ::Sleep(2000);

    GetView()->ClearContext();

    // Extrude Face

    // Height
    Standard_Real height = 20;

    Standard_Boolean bMidplane = false;

    // Direction 
    gp_Vec avec(1, 0, 0);

    // Extrude
    TopoDS_Shape EBody = Extrude(skFace, TopoDS_Shape(), height, bMidplane, avec);

    Shape = EBody;

}

void COCCViewerDoc::RevolveFace()
{
    bIs2d = false;

    // Create Points
    gp_Pnt P1(0, 20, 0);
    gp_Pnt P2(50, 20, 0);
    gp_Pnt P3(50, 30, 0);
    gp_Pnt P4(40, 30, 0);
    gp_Pnt P5(40, 70, 0);
    gp_Pnt P6(10, 70, 0);
    gp_Pnt P7(10, 30, 0);
    gp_Pnt P8(0, 30, 0);

    Pnts3d.Resize(1, 8, false);
    Pnts3d.SetValue(1, P1);
    Pnts3d.SetValue(2, P2);
    Pnts3d.SetValue(3, P3);
    Pnts3d.SetValue(4, P4);
    Pnts3d.SetValue(5, P5);
    Pnts3d.SetValue(6, P6);
    Pnts3d.SetValue(7, P7);
    Pnts3d.SetValue(8, P8);

    // Create Face
    TopoDS_Face skFace = CreateFace(Pnts3d);

    GetView()->DisplayShape(skFace);

    ::Sleep(2000);

    GetView()->ClearContext();

    // Rotation Axis (x-axis)
    gp_Ax1 axis(gp_Pnt(0, 0, 0), gp::DX());	 

    // Angle of rotation
    double angleRad = 2 * M_PI;

    // Revolved shape
    TopoDS_Shape revShape = Revolve(skFace, axis, angleRad);

    Shape = revShape;

}

void COCCViewerDoc::ExtrudeFeature()
{
    // Create Base Extrude
    ExtrudeFace();

    GetView()->DisplayShape(Shape);
    
    ::Sleep(500);

    // Create Sketch Face

    // Sketch on ZY plane	x = 20

    gp_Pnt P1(20, 20, 50);
    gp_Pnt P2(20, 120, 50);
    gp_Pnt P3(20, 120, 150);
    gp_Pnt P4(20, 20, 150);

    TColgp_Array1OfPnt pPoints(1, 4);

    pPoints.SetValue(1, P1);
    pPoints.SetValue(2, P2);
    pPoints.SetValue(3, P3);
    pPoints.SetValue(4, P4);

    // Create Face
    TopoDS_Face skFace = CreateFace(pPoints);

    GetView()->DisplayShape(skFace);

    ::Sleep(2000);

    GetView()->ClearContext();

    // Initialize BrepFeat Prism 

    gp_Dir adir(1, 0, 0);		// Direction

    BRepFeat_MakePrism thePrism(Shape, skFace, TopoDS_Face(),
                                adir, Standard_True, Standard_True);
    
    double height = 20;

    thePrism.Perform(height);

    if (thePrism.IsDone())
    {
        Shape = thePrism;
    }

}

void COCCViewerDoc::ExtrudeCutFeature()
{
    // Create Base Extrude
    ExtrudeFace();

    GetView()->DisplayShape(Shape);

    ::Sleep(500);

    // Create Sketch Face

    // Sketch on ZY plane	x = 20

    gp_Pnt P1(20, 20, 50);
    gp_Pnt P2(20, 120, 50);
    gp_Pnt P3(20, 120, 150);
    gp_Pnt P4(20, 20, 150);

    TColgp_Array1OfPnt pPoints(1, 4);

    pPoints.SetValue(1, P1);
    pPoints.SetValue(2, P2);
    pPoints.SetValue(3, P3);
    pPoints.SetValue(4, P4);

    // Create Face
    TopoDS_Face skFace = CreateFace(pPoints);

    GetView()->DisplayShape(skFace);

    ::Sleep(2000);

    GetView()->ClearContext();

    // Initialize BrepFeat Prism 

    gp_Dir adir(-1, 0, 0);		// Reverse Direction

    BRepFeat_MakePrism thePrism(Shape, skFace, TopoDS_Face(),
        adir, Standard_False, Standard_True);

    double height = 20;

    thePrism.Perform(height);

    if (thePrism.IsDone())
    {
        Shape = thePrism;
    }

}

void COCCViewerDoc::RevolveFeature()
{
    // Create Shape
    RevolveFace();

    GetView()->DisplayShape(Shape);

    ::Sleep(500);

    // Create Sketch Face	
    gp_Pnt P1(12, 65, 0);
    gp_Pnt P2(38, 65, 0);
    gp_Pnt P3(38, 75, 0);
    gp_Pnt P4(12, 75, 0);

    Pnts3d.Resize(1, 4, false);
    Pnts3d.SetValue(1, P1);
    Pnts3d.SetValue(2, P2);
    Pnts3d.SetValue(3, P3);
    Pnts3d.SetValue(4, P4);

    // Create Face
    TopoDS_Face skFace = CreateFace(Pnts3d);

    GetView()->DisplayShape(skFace);

    ::Sleep(1000);

    TopoDS_Shape baseShape = Shape;		     // Basic Shape
    TopoDS_Face skPFace = skFace;		     // sketch face of feature
    TopoDS_Face skbaseFace;		             // Face in basic shape on which 
                                             // the feature sketch is drawn. 
    gp_Ax1 axis(gp_Pnt(0, 0, 0), gp::DX());	 // x-axis 
   
    // Angle of rotation
    double angleRad = 2 * M_PI;

    GetView()->ClearContext();

    // Initialize BrepFeat Revolve 
    BRepFeat_MakeRevol theRevol(baseShape, skPFace, skbaseFace,
        axis, Standard_True, Standard_True);

    theRevol.Perform(angleRad);

    if (theRevol.IsDone())
    {
        Shape = theRevol;
    }

}

void COCCViewerDoc::RevolveCutFeature()
{
    // Create Shape
    RevolveFace();

    GetView()->DisplayShape(Shape);

    ::Sleep(500);

    // Create Sketch Face	
    gp_Pnt P1(12, 65, 0);
    gp_Pnt P2(38, 65, 0);
    gp_Pnt P3(38, 75, 0);
    gp_Pnt P4(12, 75, 0);

    Pnts3d.Resize(1, 4, false);
    Pnts3d.SetValue(1, P1);
    Pnts3d.SetValue(2, P2);
    Pnts3d.SetValue(3, P3);
    Pnts3d.SetValue(4, P4);

    // Create Face
    TopoDS_Face skFace = CreateFace(Pnts3d);

    GetView()->DisplayShape(skFace);

    ::Sleep(1000);

    TopoDS_Shape baseShape = Shape;		     // Basic Shape
    TopoDS_Face skPFace = skFace;		     // sketch face of feature
    TopoDS_Face skbaseFace;		             // Face in basic shape on which 
                                             // the feature sketch is drawn. 
    gp_Ax1 axis(gp_Pnt(0, 0, 0), gp::DX());	 // x-axis 

    // Angle of rotation
    double angleRad = 2 * M_PI;

    GetView()->ClearContext();

    // Initialize BrepFeat Revolve 
    BRepFeat_MakeRevol theRevol(baseShape, skPFace, skbaseFace,
        axis, Standard_False, Standard_True);

    theRevol.Perform(angleRad);

    if (theRevol.IsDone())
    {
        Shape = theRevol;
    }

}

void COCCViewerDoc::OffsetWire()
{
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

    TopoDS_Wire profile = CreateWire(Curves3d);

    DisplaySketch();

    ::Sleep(1000);        

    // Make Offset
    BRepOffsetAPI_MakeOffset MO(profile, GeomAbs_JoinType::GeomAbs_Tangent, true);
    MO.Perform(4);

    Shape = MO.Shape();

    GetView()->ClearContext(1);

    GetView()->DisplayShape(profile);
}

void COCCViewerDoc::Sweep()
{
    // Path
    Create3DGeometry();
    TopoDS_Wire Path = CreateWire(Curves3d);

    GetView()->DisplayShape(Path);

    ::Sleep(1000);

    // Profile
    gp_Dir D(1, 0, 0);
    gp_Pnt Pos(0, 0, 0);
    gp_Ax2 Ax1(Pos, D);
    gp_Circ gC1(Ax1, 10);
    Handle(Geom_Circle) C = new Geom_Circle(gC1);

    // Outer circle
    TopoDS_Wire W1 = CreateWire(C);

    GetView()->DisplayShape(W1);

    // Offset circle
    BRepOffsetAPI_MakeOffset MO(W1, GeomAbs_JoinType::GeomAbs_Tangent, true);
    MO.Perform(-2);

    // Inner circle
    TopoDS_Wire W2 = TopoDS::Wire(MO.Shape());

    GetView()->DisplayShape(W2);

    ::Sleep(1000);
        
    TopoDS_Face F = CreateFaceWithHole(W1, W2);

    GetView()->DisplayShape(F);

    ::Sleep(1000);

    // Sweep body
    BRepOffsetAPI_MakePipe MP(Path, F);
    MP.Build();

    Shape = MP.Shape();

    GetView()->ClearContext();
}

void COCCViewerDoc::Loft()
{
    // Top Circular Profile
    gp_Dir D(0, 1, 0);
    gp_Pnt Pos(0, 100, 0);
    gp_Ax2 Ax1(Pos, D);
    gp_Circ gC1(Ax1, 25);
    Handle(Geom_Circle) C = new Geom_Circle(gC1);

    // Outer circle
    TopoDS_Wire profile1 = CreateWire(C);

    // Bottom Rectangle profile
    Standard_Real Length = 70;
    Standard_Real Width = 50;

    // Create Plane defined by plane normal and local y-axis direction

    gp_Dir N(0, 1, 0);	                    // y-axis in global CS
    gp_Dir Vx(1, 0, 0);	                    // x-axis in global CS
    gp_Pnt P(-Length / 2, 0, Width / 2);

    gp_Ax2 Plane(P, N, Vx);	                // Right hand coordinate system

    gp_Pnt2d Pos2d(0, 0);					// Position of sketch in 2d plane
    TopoDS_Wire profile2 = CreateRectangle(Plane, Pos2d, Length, Width);

    BRepOffsetAPI_ThruSections BTS(true);
    BTS.AddWire(profile1);
    BTS.AddWire(profile2);

    GetView()->DisplayShape(profile1);
    ::Sleep(500);
    GetView()->DisplayShape(profile2);

    ::Sleep(1000);

    GetView()->ClearContext();

    Shape = BTS.Shape();
}


void COCCViewerDoc::ThickSolid()
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
    Handle(Geom_TrimmedCurve) L4 = GC_MakeSegment(P6, P1).Value();

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

    Curves3d.Resize(1, 6, false);
    Curves3d.SetValue(1, L1);
    Curves3d.SetValue(2, C1);
    Curves3d.SetValue(3, L2);
    Curves3d.SetValue(4, C2);
    Curves3d.SetValue(5, L3);
    Curves3d.SetValue(6, L4);

    TopoDS_Wire profile1 = CreateWire(Curves3d);

    TopoDS_Face skface = CreateFace(profile1);

    double height = 0;
    gp_Vec avec(0, 0, -100);
    bool bMidplane = false;

    Shape = Extrude(skface, TopoDS_Shape(), height, bMidplane, avec);

}

void COCCViewerDoc::Shelling()
{
    // Box
    ThickSolid();

    GetView()->DisplayShape(Shape);    

    // Get Bottom face
    gp_Pnt bp(30, 0, -50);

    TopoDS_Face RF = GetNearestFace(Shape, bp);

    // Shell Operation
    BRepOffsetAPI_MakeThickSolid MTS;

    // Add faces to remove from the base solid
    TopTools_ListOfShape ClosingFaces;
    ClosingFaces.Append(RF);

    // define thickness of solid walls
    Standard_Real Offset = 2;

    Standard_Real Tol = Precision::Confusion();
    BRepOffset_Mode 	Mode = BRepOffset_Skin;
    Standard_Boolean 	Intersection = Standard_False;
    Standard_Boolean 	SelfInter = Standard_False;
    GeomAbs_JoinType 	Join = GeomAbs_JoinType::GeomAbs_Intersection;
    Standard_Boolean 	RemoveIntEdges = Standard_False;

    MTS.MakeThickSolidByJoin(Shape,
        ClosingFaces,
        Offset,
        Tol, Mode, Intersection, SelfInter, Join, RemoveIntEdges);

    if (MTS.IsDone())
        Shape = MTS.Shape();

    ::Sleep(1000);

    GetView()->ClearContext();
}

void COCCViewerDoc::Fillet2D()
{
    bIs2d = false;

    // Create 2D Face

    // Create Points
    gp_Pnt P1(0, 0, 0);
    gp_Pnt P2(100, 0, 0);
    gp_Pnt P3(100, 50, 0);
    gp_Pnt P4(0, 50, 0);

    Pnts3d.Resize(1, 4, false);
    Pnts3d.SetValue(1, P1);
    Pnts3d.SetValue(2, P2);
    Pnts3d.SetValue(3, P3);
    Pnts3d.SetValue(4, P4);

    TopoDS_Face skFace = CreateFace(Pnts3d);

    GetView()->DisplayShape(skFace);

    // Initialize fillet class
    BRepFilletAPI_MakeFillet2d builder(skFace);

    // Get the vertex from face
    TopoDS_Vertex aV1 = GetVertex(skFace, P3);
    TopoDS_Vertex aV2 = GetVertex(skFace, P4);

    // Define radius
    Standard_Real Radius = 10;

    // Add fillet 
    builder.AddFillet(aV1, Radius);
    builder.AddFillet(aV2, Radius);

    // Build fillet
    TopoDS_Face afilletFace = TopoDS::Face(builder.Shape());

    Shape = afilletFace;

    ::Sleep(1000);

    GetView()->ClearContext();
}

void COCCViewerDoc::Chamfer2D()
{
    bIs2d = false;

    // Create 2D Face

    // Create Points
    gp_Pnt P1(0, 0, 0);
    gp_Pnt P2(100, 0, 0);
    gp_Pnt P3(100, 50, 0);
    gp_Pnt P4(0, 50, 0);

    gp_Pnt Pe1(100, 25, 0);
    gp_Pnt Pe2(50, 50, 0);
    gp_Pnt Pe3(0, 25, 0);

    Pnts3d.Resize(1, 4, false);
    Pnts3d.SetValue(1, P1);
    Pnts3d.SetValue(2, P2);
    Pnts3d.SetValue(3, P3);
    Pnts3d.SetValue(4, P4);

    TopoDS_Face skFace = CreateFace(Pnts3d);

    GetView()->DisplayShape(skFace);

    // Initialize fillet class
    BRepFilletAPI_MakeFillet2d builder(skFace);

    // Get edges for chamfer
    TopoDS_Edge E1 = GetNearestEdge(skFace, Pe1);
    TopoDS_Edge E2 = GetNearestEdge(skFace, Pe2);
    TopoDS_Edge E3 = GetNearestEdge(skFace, Pe3);

    // Add chamfer
    builder.AddChamfer(E1, E2, 10, 10);
    builder.AddChamfer(E2, E3, 10, 16);

    // Build fillet
    TopoDS_Face afilletFace = TopoDS::Face(builder.Shape());

    Shape = afilletFace;

    ::Sleep(1000);

    GetView()->ClearContext();

}

void COCCViewerDoc::Fillet3D()
{
    // Create Sketch Face

    // Sketch on XY plane	z = 0
    gp_Pnt P1(0, 0, 0);
    gp_Pnt P2(100, 0, 0);
    gp_Pnt P3(100, 50, 0);
    gp_Pnt P4(60, 100, 0);
    gp_Pnt P5(0, 100, 0);

    TColgp_Array1OfPnt pPoints(1, 5);

    pPoints.SetValue(1, P1);
    pPoints.SetValue(2, P2);
    pPoints.SetValue(3, P3);
    pPoints.SetValue(4, P4);
    pPoints.SetValue(5, P5);

    TopoDS_Face skFace = CreateFace(pPoints);

    // Extrude Face
    Standard_Real height = 20;
    Standard_Boolean bMidplane = false;
    gp_Vec avec(1, 0, 0);
    TopoDS_Shape EBody = Extrude(skFace, TopoDS_Shape(), height, bMidplane, avec);

    GetView()->DisplayShape(EBody);

    // Select Edge for fillet
    gp_Pnt Pe1(60, 100, 20);
    gp_Pnt Pe2(0, 100, 20);
    TopoDS_Edge E1 = GetNearestEdge(EBody, Pe1);
    TopoDS_Edge E2 = GetNearestEdge(EBody, Pe2);

    TopTools_Array1OfShape Edges(1, 2);
    Edges.SetValue(1, E1);
    Edges.SetValue(2, E2);
    double radius = 10;

    TopoDS_Shape FBody = Add3DFillet(EBody, Edges, radius);

    Shape = FBody;

    ::Sleep(1000);

    GetView()->ClearContext();
}

void COCCViewerDoc::Chamfer3D()
{
    // Create 2D Face

    // Create Points
    gp_Pnt P1(0, 0, 0);
    gp_Pnt P2(100, 0, 0);
    gp_Pnt P3(100, 50, 0);
    gp_Pnt P4(0, 50, 0);

    Pnts3d.Resize(1, 4, false);
    Pnts3d.SetValue(1, P1);
    Pnts3d.SetValue(2, P2);
    Pnts3d.SetValue(3, P3);
    Pnts3d.SetValue(4, P4);

    TopoDS_Face skFace = CreateFace(Pnts3d);

    // Extrude Face
    Standard_Real height = 20;
    Standard_Boolean bMidplane = false;
    gp_Vec avec(1, 0, 0);
    TopoDS_Shape EBody = Extrude(skFace, TopoDS_Shape(), height, bMidplane, avec);

    GetView()->DisplayShape(EBody);

    // Select Edge for Chamfer
    gp_Pnt Pe1(100, 50, 10);
    TopoDS_Edge E1 = GetNearestEdge(EBody, Pe1);

    // Select face for chamfer
    gp_Pnt Pf1(50, 50, 10);
    TopoDS_Face F = GetNearestFace(EBody, Pf1);

    double dist1 = 30;
    double dist2 = 15;

    TopoDS_Shape CBody = Add3DChamfer(EBody, E1, dist1, dist2, F);

    Shape = CBody;

    ::Sleep(1000);

    GetView()->ClearContext();
}

void COCCViewerDoc::CreateBrg()
{

    Standard_Real xPos = 0;
    Standard_Real yPos = 0;
    Standard_Real zPos = 0;
    Standard_Real W = 16;
    Standard_Real ID = 60;
    Standard_Real OD = 100;

    float Ra, Rb, X1, X2, X3, X4, Y1, Y2, Y4, Y3;

    Ra = OD / 2;
    Rb = ID / 2;

    X1 = xPos;
    X2 = X1 + W;

    Y1 = yPos + Rb;
    Y4 = yPos + Ra;
    Y2 = Y1;
    Y3 = Y4;

    // Creaate Face
    TColgp_Array1OfPnt pPoints(1, 4);

    gp_Pnt Pos(xPos, yPos, zPos);
    gp_Pnt p1(X1, Y1, zPos);
    gp_Pnt p2(X2, Y2, zPos);
    gp_Pnt p3(X2, Y3, zPos);
    gp_Pnt p4(X1, Y4, zPos);

    pPoints.SetValue(1, p1);
    pPoints.SetValue(2, p2);
    pPoints.SetValue(3, p3);
    pPoints.SetValue(4, p4);

    TopoDS_Shape rFace = CreateFace(pPoints);

    gp_Dir xDir(1, 0, 0);
    gp_Ax1 xAxis(Pos, xDir);
    double angle = 2 * M_PI;

    TopoDS_Shape rotor = Revolve(rFace, xAxis, angle);

    Shape = rotor;
}

void COCCViewerDoc::Translate()
{
    // Create Bearing 
    CreateBrg();

    // Define translation vector
    gp_Vec vec(200, 0, 0);

    // Get translated shape
    TopoDS_Shape TS = TranslatedShape(Shape, vec);

    GetView()->DisplayShape(Shape);
    ::Sleep(500);
    GetView()->DisplayShape(TS);

}

void COCCViewerDoc::Rotate()
{
    // Create Bearing 
    CreateBrg();

    // Define Axis for rotation
    gp_Pnt Pos(0, 100, 0);
    gp_Dir zDir(0, 0, 1);

    gp_Ax1 zAxis(Pos, zDir);

    double angle = 0.5 * M_PI;

    // Get rotated shape
    TopoDS_Shape RS = RotatedShape(Shape, zAxis, angle);

    GetView()->DisplayShape(Shape);
    ::Sleep(500);
    GetView()->DisplayShape(RS);
}

void COCCViewerDoc::Mirror()
{
    // Create Bearing 
    CreateBrg();

    // Define Plane
    gp_Pnt Pos(100, 0, 0);
    gp_Dir Dir(1, 0, 0);

    gp_Pln aPlane(Pos, Dir);

    // Get mirror shape
    TopoDS_Shape MS = MirrorShape(Shape, aPlane);

    GetView()->DisplayShape(Shape);

    ::Sleep(500);

    GetView()->DisplayPlane(aPlane);

    ::Sleep(500);

    GetView()->DisplayShape(MS);

}

void COCCViewerDoc::CreateFlange()
{
    Standard_Real xPos = 0;
    Standard_Real yPos = 0;
    Standard_Real zPos = 0;
    Standard_Real W = 16;
    Standard_Real ID = 50;
    Standard_Real OD = 100;

    float Ra, Rb, X1, X2, X3, X4, Y1, Y2, Y4, Y3;

    Ra = OD / 2;
    Rb = ID / 2;

    X1 = xPos;
    X2 = X1 + W;

    Y1 = yPos + Rb;
    Y4 = yPos + Ra;
    Y2 = Y1;
    Y3 = Y4;

    // Creaate Face
    TColgp_Array1OfPnt pPoints(1, 4);

    gp_Pnt Pos(xPos, yPos, zPos);
    gp_Pnt p1(X1, Y1, zPos);
    gp_Pnt p2(X2, Y2, zPos);
    gp_Pnt p3(X2, Y3, zPos);
    gp_Pnt p4(X1, Y4, zPos);

    pPoints.SetValue(1, p1);
    pPoints.SetValue(2, p2);
    pPoints.SetValue(3, p3);
    pPoints.SetValue(4, p4);

    TopoDS_Shape rFace = CreateFace(pPoints);

    gp_Dir xDir(1, 0, 0);
    gp_Ax1 xAxis(Pos, xDir);
    double angle = 2 * M_PI;

    TopoDS_Shape rotor = Revolve(rFace, xAxis, angle);

    Shape = rotor;

}

void COCCViewerDoc::CreatePipe()
{
    // Create Circle on z-plane
    gp_Pnt Pos(16, 0, 0);
    gp_Dir Dir(1, 0, 0);
    gp_Ax2 ax2(Pos, Dir);
    gp_Circ aCircle(ax2, 30);

    Handle(Geom_Curve) L = GetCurve(aCircle);
    TopoDS_Face aFace = CreateFace(L);

    // Define Axis for rotation
    gp_Pnt Pos2(16, 84, 0);
    gp_Dir zDir(0, 0, 1);
    gp_Ax1 zAxis(Pos2, zDir);
    double angle = 0.5 * M_PI;

    TopoDS_Shape pipe = Revolve(aFace, zAxis, angle);

    Shape = pipe;
}

void COCCViewerDoc::AddBodies()
{

    // Flange 1
    CreateFlange();
    TopoDS_Shape FG1 = Shape;

    // Flange 2
    CreateFlange();

    // Define Axis for rotation
    gp_Pnt Pos(0, 100, 0);
    gp_Dir zDir(0, 0, 1);
    gp_Ax1 zAxis(Pos, zDir);

    double angle = 0.5 * M_PI;

    // Get rotated shape
    TopoDS_Shape FG2 = RotatedShape(Shape, zAxis, angle);

    // Translate FG2 down by 16mm
    gp_Vec TV(0, -16, 0);
    FG2 = TranslatedShape(FG2, TV);

    // Pipe
    CreatePipe();
    TopoDS_Shape Pipe = Shape;    

    // Add Bodies

    TopoDS_Shape B1 = BooleanAdd(FG1, Pipe);
    TopoDS_Shape BentPipe = BooleanAdd(B1, FG2);

    Shape = BentPipe;

    /*GetView()->DisplayShape(FG1);
    ::Sleep(500);
    GetView()->DisplayShape(Pipe);
    ::Sleep(500);
    GetView()->DisplayShape(FG2);

    ::Sleep(1000);

    GetView()->ClearContext();*/

}

void COCCViewerDoc::CutBody()
{
    // Inner pipe body
    // Create Circle on z-plane
    gp_Pnt Pos(16, 0, 0);
    gp_Dir Dir(1, 0, 0);
    gp_Ax2 ax2(Pos, Dir);
    gp_Circ aCircle(ax2, 25);

    Handle(Geom_Curve) L = GetCurve(aCircle);

    TopoDS_Face aFace = CreateFace(L);

    // Define Axis for rotation
    gp_Pnt Pos2(16, 84, 0);
    gp_Dir zDir(0, 0, 1);
    gp_Ax1 zAxis(Pos2, zDir);
    double angle = 0.5 * M_PI;

    TopoDS_Shape pipe = Revolve(aFace, zAxis, angle);

    AddBodies();

    TopoDS_Shape BentPipe = BooleanCut(Shape, pipe);

    Shape = BentPipe;
}

void COCCViewerDoc::BoundingBox()
{
    Standard_Real aXmin, aYmin, aZmin, aXmax, aYmax, aZmax;

    // Create pipe body
    CutBody();

    Bnd_Box aBox;
    BRepBndLib::Add(Shape, aBox);

    GetView()->DisplayShape(Shape);
    ::Sleep(500);
    GetView()->DisplayBoundingBox(aBox);
}

void COCCViewerDoc::OBBBoundingBox()
{
    Standard_Real aXmin, aYmin, aZmin, aXmax, aYmax, aZmax;

    // Create pipe body
    CutBody();

    Bnd_OBB aOBBox;
    BRepBndLib::AddOBB(Shape, aOBBox);
    
    GetView()->DisplayShape(Shape);
    ::Sleep(500);
    GetView()->DisplayBoundingBox(aOBBox);
}

void COCCViewerDoc::LinearProperties()
{
    // Create Curve
    CreateBSplineCurve();

    // Create Wire from curve
    TopoDS_Wire aCrv = CreateWire(Curves3d);

    GProp_GProps System;
    Standard_Real aQuantity;

    BRepGProp::LinearProperties(aCrv, System);
    aQuantity = System.Mass();

    GetView()->DisplayShape(aCrv);

    ::Sleep(500);

    CString str;
    str.Format(L"Length of Curve : %f" ,aQuantity);

    AfxMessageBox(str);
}

void COCCViewerDoc::SurfaceProperties()
{
    // Create Face
    CreateFaceWHoles();

    GProp_GProps System;
    Standard_Real Area;
    Standard_Real Ixx, Iyy, Izz, Ixy, Iyz, Izx;

    BRepGProp::SurfaceProperties(Shape, System);
    Area = System.Mass();
    gp_Pnt G = System.CentreOfMass();

    gp_Mat I = System.MatrixOfInertia();
    Ixx = I.Value(1, 1);Iyy = I.Value(2, 2);Izz = I.Value(3, 3);
    Ixy = I.Value(1, 2);Iyz = I.Value(2, 3);Izx = I.Value(1, 3);

    TCollection_ExtendedString string("Area Of the Face =");
    TCollection_ExtendedString string1(Area);

    string += string1;
    string += "\nCenterOfMass : \n   X=";
    string1 = G.X();
    string += string1;
    string += " Y=";
    string1 = G.Y();
    string += string1;
    string += " Z=";
    string1 = G.Z();
    string += string1;
    string += "\n";

    string += "Matrix of Inertia :\n     ";
    string1 = I(1, 1);
    string += string1;
    string += " ";
    string1 = I(1, 2);
    string += string1;
    string += " ";
    string1 = I(1, 3);
    string += string1;
    string += "\n     ";
    string1 = I(2, 1);
    string += string1;
    string += " ";
    string1 = I(2, 2);
    string += string1;
    string += " ";
    string1 = I(2, 3);
    string += string1;
    string += "\n     ";
    string1 = I(3, 1);
    string += string1;
    string += " ";
    string1 = I(3, 2);
    string += string1;
    string += " ";
    string1 = I(3, 3);
    string += string1;
    string += "\n";

    GetView()->DisplayShape(Shape);

    ::Sleep(500);

    MessageBoxW(AfxGetApp()->m_pMainWnd->m_hWnd, string.ToWideString(), L"Surface Properties", MB_OK);
}

void COCCViewerDoc::VolumeProperties()
{
    // Create Body
    ExtrudeFace();

    GProp_GProps System;
    Standard_Real Volume;
    Standard_Real Ixx, Iyy, Izz, Ixy, Iyz, Izx;

    BRepGProp::VolumeProperties(Shape, System);
    Volume = System.Mass();
    gp_Pnt G = System.CentreOfMass();

    gp_Mat I = System.MatrixOfInertia();
    Ixx = I.Value(1, 1);Iyy = I.Value(2, 2);Izz = I.Value(3, 3);
    Ixy = I.Value(1, 2);Iyz = I.Value(2, 3);Izx = I.Value(1, 3);

    TCollection_ExtendedString string("Volume Of the Body =");
    TCollection_ExtendedString string1(Volume);

    string += string1;
    string += "\nCenterOfMass : \n   X=";
    string1 = G.X();
    string += string1;
    string += " Y=";
    string1 = G.Y();
    string += string1;
    string += " Z=";
    string1 = G.Z();
    string += string1;
    string += "\n";

    string += "Matrix of Inertia :\n     ";
    string1 = I(1, 1);
    string += string1;
    string += " ";
    string1 = I(1, 2);
    string += string1;
    string += " ";
    string1 = I(1, 3);
    string += string1;
    string += "\n     ";
    string1 = I(2, 1);
    string += string1;
    string += " ";
    string1 = I(2, 2);
    string += string1;
    string += " ";
    string1 = I(2, 3);
    string += string1;
    string += "\n     ";
    string1 = I(3, 1);
    string += string1;
    string += " ";
    string1 = I(3, 2);
    string += string1;
    string += " ";
    string1 = I(3, 3);
    string += string1;
    string += "\n";

    GetView()->DisplayShape(Shape);

    ::Sleep(500);

    MessageBoxW(AfxGetApp()->m_pMainWnd->m_hWnd, string.ToWideString(), L"Volume Properties", MB_OK);

}