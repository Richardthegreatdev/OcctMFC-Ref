#include "pch.h"
#include "FlangeDesign.h"


//=======================================================================
//Class    :	Flange 
//purpose  :	Create Flange
//=======================================================================

TDF_Label Flange::CreateFlangeDS(TDF_Label& MainLab,
    Standard_Real OD, Standard_Real ID, Standard_Real flgthk,
    Standard_Real NbBolts, Standard_Real BoltDia, Standard_Real BoltPCD)
{
    // Data Structure
    // 0:1 MainLab
    //        |
    //      0:1:1 Flange Label ---> Name --->  Named shape ---> Function
    //              |
    //            0:1:1:1 -- OD
    //              |
    //            0:1:1:2 -- ID
    //              |
    //            0:1:1:3 -- flgthk
    //              |
    //            0:1:1:4 -- NbBolts
    //              |
    //            0:1:1:5 -- BoltDia  
    //              |
    //            0:1:1:6 -- BoltPCD

    // Create a new label in the data structure for the Flange
    TDF_Label FLG = TDF_TagSource::NewChild(MainLab);     // Geometry dimensions
    TCollection_ExtendedString Name = "Flange";

    // Create the data structure : Set the dimensions, position and name attributes
    TDataStd_Real::Set(FLG.FindChild(1), OD);
    TDataStd_Name::Set(FLG.FindChild(1, false), "Outside Diameter");

    TDataStd_Real::Set(FLG.FindChild(2), ID);
    TDataStd_Name::Set(FLG.FindChild(2, false), "Inside Diameter");

    TDataStd_Real::Set(FLG.FindChild(3), flgthk);
    TDataStd_Name::Set(FLG.FindChild(3, false), "Flange thickness");

    TDataStd_Real::Set(FLG.FindChild(4), NbBolts);
    TDataStd_Name::Set(FLG.FindChild(4, false), "No.of Bolts");

    TDataStd_Real::Set(FLG.FindChild(5), BoltDia);
    TDataStd_Name::Set(FLG.FindChild(5, false), "Bolt Diameter");

    TDataStd_Real::Set(FLG.FindChild(6), BoltPCD);
    TDataStd_Name::Set(FLG.FindChild(6, false), "Bolt Pitch Diameter");

    TDataStd_Name::Set(FLG, Name);

    // Instanciate a TFunction_Function attribute connected to the current driver
    // and attach it to the data structure as an attribute of the Flange Label
    Handle(TFunction_Function) myFunction = TFunction_Function::Set(FLG, FlangeDriver::GetID());

    // Initialize logbook
    Handle(TFunction_Logbook) log = TFunction_Logbook::Set(FLG);

    Handle(FlangeDriver) myflgDriver;

    // Find the FlangeDriver in the TFunction_DriverTable using its GUID
    if (!TFunction_DriverTable::Get()->FindDriver(FlangeDriver::GetID(), myflgDriver)) return FLG;

    myflgDriver->Init(FLG);
    if (myflgDriver->Execute(log))
    {
        AfxMessageBox(L"Cannot create flange", MB_OK | MB_ICONERROR);
    }

    return FLG;
}

void Flange::ModifyFlange(TDF_Label& FLG,
    Standard_Real OD, Standard_Real ID, Standard_Real flgthk,
    Standard_Real NbBolts, Standard_Real BoltDia, Standard_Real BoltPCD)
{

    Handle(TFunction_Logbook) log = TFunction_Logbook::Set(FLG);

    // Set the dimensions and position attributes (if changed)
    Handle(TDataStd_Real) curReal;

    FLG.FindChild(1).FindAttribute(TDataStd_Real::GetID(), curReal);
    if (OD != curReal->Get())
    {
        TDataStd_Real::Set(FLG.FindChild(1), OD);
        // Set the label of the attribute as touched for next recomputation
        log->SetTouched(curReal->Label());
    }

    FLG.FindChild(2).FindAttribute(TDataStd_Real::GetID(), curReal);
    if (ID != curReal->Get())
    {
        TDataStd_Real::Set(FLG.FindChild(2), ID);
        // Set the label of the attribute as touched for next recomputation
        log->SetTouched(curReal->Label());
    }

    FLG.FindChild(3).FindAttribute(TDataStd_Real::GetID(), curReal);
    if (flgthk != curReal->Get())
    {
        TDataStd_Real::Set(FLG.FindChild(3), flgthk);
        // Set the label of the attribute as touched for next recomputation
        log->SetTouched(curReal->Label());
    }

    FLG.FindChild(4).FindAttribute(TDataStd_Real::GetID(), curReal);
    if (NbBolts != curReal->Get())
    {
        TDataStd_Real::Set(FLG.FindChild(4), NbBolts);
        // Set the label of the attribute as touched for next recomputation
        log->SetTouched(curReal->Label());
    }

    FLG.FindChild(5).FindAttribute(TDataStd_Real::GetID(), curReal);
    if (BoltDia != curReal->Get())
    {
        TDataStd_Real::Set(FLG.FindChild(5), BoltDia);
        // Set the label of the attribute as touched for next recomputation
        log->SetTouched(curReal->Label());
    }

    FLG.FindChild(6).FindAttribute(TDataStd_Real::GetID(), curReal);
    if (BoltPCD != curReal->Get())
    {
        TDataStd_Real::Set(FLG.FindChild(6), BoltPCD);
        // Set the label of the attribute as touched for next recomputation
        log->SetTouched(curReal->Label());
    }

    Handle(TFunction_Function) TFF;
    if (!FLG.FindAttribute(TFunction_Function::GetID(), TFF))
    {
        MessageBox(NULL, L"Object cannot be modified.", L"Modification", MB_ICONEXCLAMATION);
        return;
    }    

    // Get the TFunction_FunctionDriver GUID used with the TFunction_Function
    Standard_GUID myDriverID = TFF->GetDriverGUID();

    Handle(FlangeDriver) myFlangeDriver;
    TFunction_DriverTable::Get()->FindDriver(myDriverID, myFlangeDriver);

    // Execute (if an attribute changes)
    myFlangeDriver->Init(FLG);
    log->SetTouched(FLG);
    if (myFlangeDriver->MustExecute(log))
    {
        // Set the box touched, it will be usefull to recompute an object which used this box as attribute
        if (myFlangeDriver->Execute(log))
            MessageBox(NULL, L"Recompute failed", L"Modify Flange", MB_ICONEXCLAMATION);
    }

}

TopoDS_Shape Flange::CreateFlangeBody(gp_Ax2 Pos,
    Standard_Real OD, Standard_Real ID, Standard_Real flgthk,
    Standard_Real NbBolts, Standard_Real BoltDia, Standard_Real BoltPCD)
{
    // Geometry

    // Create Outer Circle on plane
    gp_Circ aOD(Pos, OD * 0.5);

    // Outer Wire
    Handle(Geom_Curve) cOD = GetCurve(aOD);
    TopoDS_Wire ODWire = CreateWire(cOD);

    // Create Inner Circle on plane
    gp_Circ aID(Pos, ID * 0.5);

    Handle(Geom_Curve) cID = GetCurve(aID);
    TopoDS_Wire IDWire = CreateWire(cID);

    // Bolt Holes
    TopTools_Array1OfShape InnerWires(1, NbBolts + 1);

    int boltPCR = BoltPCD * 0.5;
    double theta = (2. * M_PI) / NbBolts;

    for (int i = 0;i < NbBolts;i++)
    {
        // Bolt centers
        double x = boltPCR * cos(theta * i);
        double y = boltPCR * sin(theta * i);

        // Bolt circle
        gp_Ax2 Ax(gp_Pnt(x, y, 0), gp_Dir(0, 0, 1));
        gp_Circ bolthole(Ax, BoltDia * 0.5);

        // inner Wire
        Handle(Geom_Curve) BH = GetCurve(bolthole);
        TopoDS_Wire IW = CreateWire(BH);

        // Add to list
        InnerWires.SetValue(i + 1, IW);
    }

    InnerWires.SetValue(NbBolts + 1, IDWire);

    // Create face with hole
    TopoDS_Face aHoleFace = CreateFaceWithHole(ODWire, InnerWires);

    // Extrude Flange
    Standard_Real height = flgthk;
    Standard_Boolean bMidplane = false;
    gp_Vec avec(1, 0, 0); // dummy

    TopoDS_Shape FlangeBody = Extrude(aHoleFace, TopoDS_Shape(), height, bMidplane, avec);

    return FlangeBody;
}


void Flange::GetDimensions(TDF_Label& FLG,
    Standard_Real& OD, Standard_Real& ID, Standard_Real& flgthk,
    Standard_Real& NbBolts, Standard_Real& BoltDia, Standard_Real& BoltPCD)
{
    Handle(TDataStd_Real) TSR;

    FLG.FindChild(1).FindAttribute(TDataStd_Real::GetID(), TSR);
    OD = TSR->Get();

    FLG.FindChild(2).FindAttribute(TDataStd_Real::GetID(), TSR);
    ID = TSR->Get();

    FLG.FindChild(3).FindAttribute(TDataStd_Real::GetID(), TSR);
    flgthk = TSR->Get();

    FLG.FindChild(4).FindAttribute(TDataStd_Real::GetID(), TSR);
    NbBolts = TSR->Get();

    FLG.FindChild(5).FindAttribute(TDataStd_Real::GetID(), TSR);
    BoltDia = TSR->Get();

    FLG.FindChild(6).FindAttribute(TDataStd_Real::GetID(), TSR);
    BoltPCD = TSR->Get();

}

//=======================================================================
//Class    :	Flange Driver
//purpose  :	Create Flange
//=======================================================================

IMPLEMENT_STANDARD_RTTIEXT(FlangeDriver, TFunction_Driver)

const Standard_GUID& FlangeDriver::GetID() {
	static Standard_GUID anID("22D22E52-D69A-11d4-8F1A-0060B0EE18E8");
	return anID;
}

FlangeDriver::FlangeDriver()
{

}

void FlangeDriver::Validate(Handle(TFunction_Logbook)& log) const
{
	// We validate the object label ( Label() ), all the arguments and the results of the object:
	log->SetValid(Label(), Standard_True);
}

Standard_Boolean FlangeDriver::MustExecute(const Handle(TFunction_Logbook)& log) const
{
    // If the object's label is modified:
    if (log->IsModified(Label())) return Standard_True;

    // Let's check them:
    if (log->IsModified(Label().FindChild(1))) return Standard_True; // OD.
    if (log->IsModified(Label().FindChild(2))) return Standard_True; // ID,
    if (log->IsModified(Label().FindChild(3))) return Standard_True; // flgthk.
    if (log->IsModified(Label().FindChild(4))) return Standard_True; // NbBolts,
    if (log->IsModified(Label().FindChild(5))) return Standard_True; // BoltDia,
    if (log->IsModified(Label().FindChild(6))) return Standard_True; // BoltPCD,

    // if there are no any modifications concerned the flange,
    // it's not necessary to recompute (to call the method Execute()):
    return Standard_False;
}

Standard_Integer FlangeDriver::Execute(Handle(TFunction_Logbook)& log) const
{
    // Data Structure
    // 0:1 MainLab
    //        |
    //      0:1:1 Flange Label ---> Name --->  Named shape ---> Function
    //              |
    //            0:1:1:1 -- OD
    //              |
    //            0:1:1:2 -- ID
    //              |
    //            0:1:1:3 -- flgthk
    //              |
    //            0:1:1:4 -- NbBolts
    //              |
    //            0:1:1:5 -- BoltDia  
    //              |
    //            0:1:1:6 -- BoltPCD
    // 
    
    Handle(TDataStd_Real) TSR;
    Standard_Real ID, OD, flgthk, NbBolts, BoltDia, BoltPCD;

    if (!Label().FindChild(1).FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
    OD = TSR->Get();

    if (!Label().FindChild(2).FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
    ID = TSR->Get();

    if (!Label().FindChild(3).FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
    flgthk = TSR->Get();

    if (!Label().FindChild(4).FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
    NbBolts = TSR->Get();

    if (!Label().FindChild(5).FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
    BoltDia = TSR->Get();

    if (!Label().FindChild(6).FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
    BoltPCD = TSR->Get();
    
    // Create Circle on xy-plane
    gp_Pnt Pos(0, 0, 0);
    gp_Dir Dir(0, 0, 1);
    gp_Ax2 ax2(Pos, Dir);    
    
    TopoDS_Shape aFlg = Flange::CreateFlangeBody(ax2, OD, ID, flgthk, NbBolts, BoltDia, BoltPCD);
    
    // Build a TNaming_NamedShape using built flange
    TNaming_Builder B(Label());
    B.Generated(aFlg);

    // That's all:
      // If there are no any mistakes we return 0:

    return 0;
}

