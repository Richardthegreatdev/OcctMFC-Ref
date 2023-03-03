
#include "pch.h"
#include "NailDesign.h"

//=======================================================================
//Class    :	Nail Driver
//purpose  :	Create Nail
//=======================================================================
IMPLEMENT_STANDARD_RTTIEXT(NailDriver, TFunction_Driver)

const Standard_GUID& NailDriver::GetID() {
	static Standard_GUID anID("D6BD6917-92AB-4A1D-8361-D933EBB879F0");
	return anID;
}

void NailDriver::Arguments(TDF_LabelList& args) const
{
	// The direct arguments, located at sub-leaves of  the function, are collected
	TDF_ChildIterator  cIterator(Label(), false);

	for (; cIterator.More(); cIterator.Next())
	{
		TDF_Label  sublabel = cIterator.Value();
		args.Append(sublabel);

		// The references to the external data are  checked.  
		Handle(TDF_Reference)  ref;
		if (sublabel.FindAttribute(TDF_Reference::GetID(), ref))
		{
			args.Append(ref->Get());
		}
	}

	// Label contains shape object 
	args.Append(Label());
}

void NailDriver::Results(TDF_LabelList& res) const
{
	// The result is kept at the function  label.  
	res.Append(Label());
}

Standard_Integer NailDriver::Execute(Handle(TFunction_Logbook)& log) const
{	

	TDF_Label Cone = Label().FindChild(1, false);
		
	TDF_Label Stem = Label().FindChild(2, false);
		
	TDF_Label Head = Label().FindChild(3, false);

	// Get the TNaming_NamedShape attributes of these labels
	Handle(TNaming_NamedShape) NSCone, NSStem, NSHead;

	if (!(Cone.FindAttribute(TNaming_NamedShape::GetID(), NSCone)))
		throw Standard_Failure("No Shape found");

	if (!(Stem.FindAttribute(TNaming_NamedShape::GetID(), NSStem)))
		throw Standard_Failure("No Shape found");

	if (!(Head.FindAttribute(TNaming_NamedShape::GetID(), NSHead)))
		throw Standard_Failure("No Shape found");

	// Now, let's get the TopoDS_Shape of these TNaming_NamedShape:
	TopoDS_Shape SHCone = NSCone->Get();
	TopoDS_Shape SHStem = NSStem->Get();
	TopoDS_Shape SHHead = NSHead->Get();

	TopoDS_Shape B1 = BooleanAdd(SHCone, SHStem);
	TopoDS_Shape Nail = BooleanAdd(B1, SHHead);

	// Build a TNaming_NamedShape using built Nail
	TNaming_Builder B(Label());
	B.Generated(Nail);

	log->SetImpacted(Label());

	return 0;
}

//=======================================================================
//Class    :	Nail Cone Driver
//purpose  :	Create Nail Cone
//=======================================================================
IMPLEMENT_STANDARD_RTTIEXT(NailConeDriver, TFunction_Driver)

const Standard_GUID& NailConeDriver::GetID() {
	static Standard_GUID anID("64902359-4ED1-47EB-9D58-4BDF0755F902");
	return anID;
}

void NailConeDriver::Arguments(TDF_LabelList& args) const
{
	// The direct arguments, located at sub-leaves of  the function, are collected
	TDF_ChildIterator  cIterator(Label(), false);

	for (; cIterator.More(); cIterator.Next())
	{
		TDF_Label  sublabel = cIterator.Value();
		args.Append(sublabel);

		// The references to the external data are  checked.  
		Handle(TDF_Reference)  ref;
		if (sublabel.FindAttribute(TDF_Reference::GetID(), ref))
		{
			args.Append(ref->Get());
		}
	}

	// Label contains shape object 
	args.Append(Label());
}

void NailConeDriver::Results(TDF_LabelList& res) const
{
	// The result is kept at the function  label.  
	res.Append(Label());
}

Standard_Integer NailConeDriver::Execute(Handle(TFunction_Logbook)& log) const
{
	    //   0:1 : 1 : 1 Cone Label--->Name--->Named shape--->Function2
		//					|
		//				0:1:1:1:1 -- Pos
		//					|
		//				0:1:1:1:2 -- aConeRadius
		//					|
		//				0:1:1:1:3 -- aConeHeight

	gp_Pnt Pos;
	TDataXtd_Geometry::Point(Label().FindChild(1, false), Pos);
	
	Handle(TDataStd_Real) TSR;
	Standard_Real aConeRadius, aConeHeight;

	if (!Label().FindChild(2).FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
	aConeRadius = TSR->Get();

	if (!Label().FindChild(3).FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
	aConeHeight = TSR->Get();

	TopoDS_Shape cone = BRepPrimAPI_MakeCone(gp_Ax2(Pos, gp_Dir(0., 1., 0.)),
		0, aConeRadius, aConeHeight, 2. * M_PI).Shape();

	// Build a TNaming_NamedShape using built cone
	TNaming_Builder B(Label());
	B.Generated(cone);

	log->SetImpacted(Label());
	log->SetImpacted(Label().FindChild(2));
	log->SetImpacted(Label().FindChild(3));

	return 0;
}

//=======================================================================
//Class    :	Nail Stem Driver
//purpose  :	Create Nail Stem
//=======================================================================
IMPLEMENT_STANDARD_RTTIEXT(NailStemDriver, TFunction_Driver)

const Standard_GUID& NailStemDriver::GetID() {
	static Standard_GUID anID("8B7C02D4-A832-459B-B9C4-AF1809110C76");
	return anID;
}

void NailStemDriver::Arguments(TDF_LabelList& args) const
{
	// The direct arguments, located at sub-leaves of  the function, are collected
	TDF_ChildIterator  cIterator(Label(), false);

	for (; cIterator.More(); cIterator.Next())
	{
		TDF_Label  sublabel = cIterator.Value();
		args.Append(sublabel);

		// The references to the external data are  checked.  
		Handle(TDF_Reference)  ref;
		if (sublabel.FindAttribute(TDF_Reference::GetID(), ref))
		{
			args.Append(ref->Get());
		}
	}

	// Label contains shape object 
	args.Append(Label());
}

void NailStemDriver::Results(TDF_LabelList& res) const
{
	// The result is kept at the function  label.  
	res.Append(Label());
}

Standard_Integer NailStemDriver::Execute(Handle(TFunction_Logbook)& log) const
{

//			0:1:1:2	Stem Label ---> Name --->  Named shape ---> Function3
//					|
//				0:1:1:2:1 -- "Cone" position translated for "Cone" height along Y
//					|
//				0:1:1:2:2 -- "Cone" radius
//					|
//				0:1:1:2:3 -- "Cone" height multiplied by 3

	// Get Reference Labels
	Handle(TDF_Reference)  RefPos, RefRadius, RefHeight;

	// Position
	if (!Label().FindChild(1).FindAttribute(TDF_Reference::GetID(), RefPos)) return 1;
	TDF_Label refPosLab = RefPos->Get();

	gp_Pnt Pos;
	TDataXtd_Geometry::Point(refPosLab, Pos);

	// Radius
	if (!Label().FindChild(2).FindAttribute(TDF_Reference::GetID(), RefRadius)) return 1;
	TDF_Label refradLab = RefRadius->Get();

	// Ref Height
	if (!Label().FindChild(3).FindAttribute(TDF_Reference::GetID(), RefHeight)) return 1;
	TDF_Label refHtLab = RefHeight->Get();


	// Get Values

	Handle(TDataStd_Real) TSR;
	Standard_Real aConeRadius, aConeHeight;

	if (!refradLab.FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
	aConeRadius = TSR->Get();

	if (!refHtLab.FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
	aConeHeight = TSR->Get();


	// Stem Values
	gp_Pnt stemPos(Pos.X(), Pos.Y() + aConeHeight, Pos.Z());
	Standard_Real stemRadius, stemHeight;

	stemRadius = aConeRadius;
	stemHeight = aConeHeight * 3;

	TDataXtd_Point::Set(Label().FindChild(1, false), stemPos);
	TDataStd_Real::Set(Label().FindChild(2, false), stemRadius);
	TDataStd_Real::Set(Label().FindChild(3, false), stemHeight);

	gp_Ax2 axes(stemPos, gp_Dir(0., 1., 0.));
	TopoDS_Shape stem = BRepPrimAPI_MakeCylinder(axes, stemRadius, stemHeight);

	// Build a TNaming_NamedShape using built cone
	TNaming_Builder B(Label());
	B.Generated(stem);

	log->SetImpacted(Label());
	log->SetImpacted(Label().FindChild(1));
	log->SetImpacted(Label().FindChild(2));
	log->SetImpacted(Label().FindChild(3));

	return 0;
}

//=======================================================================
//Class    :	Nail Head Driver
//purpose  :	Create Nail Head
//=======================================================================
IMPLEMENT_STANDARD_RTTIEXT(NailHeadDriver, TFunction_Driver)

const Standard_GUID& NailHeadDriver::GetID() {
	static Standard_GUID anID("2E4230D5-EF3D-4C74-992C-06BB68C1D376");
	return anID;
}

void NailHeadDriver::Arguments(TDF_LabelList& args) const
{
	// The direct arguments, located at sub-leaves of  the function, are collected
	TDF_ChildIterator  cIterator(Label(), false);

	for (; cIterator.More(); cIterator.Next())
	{
		TDF_Label  sublabel = cIterator.Value();
		args.Append(sublabel);

		// The references to the external data are  checked.  
		Handle(TDF_Reference)  ref;
		if (sublabel.FindAttribute(TDF_Reference::GetID(), ref))
		{
			args.Append(ref->Get());
		}
	}

	// Label contains shape object 
	args.Append(Label());
}

void NailHeadDriver::Results(TDF_LabelList& res) const
{
	// The result is kept at the function  label.  
	res.Append(Label());
}

Standard_Integer NailHeadDriver::Execute(Handle(TFunction_Logbook)& log) const
{

	//			0:1:1:3	Head Label ---> Name --->  Named shape ---> Function4
	//					|
	//				0:1:1:3:1 -- "Long cylinder" position translated for "Long cylinder" height along Y
	//					|
	//				0:1:1:3:2 -- "Long cylinder" radius multiplied by 1.5
	//					|
	//				0:1:1:3:3 -- "Cone" height divided by 2

	// Get Reference Labels
	Handle(TDF_Reference)  RefPos, RefRadius, RefHeight;

	// Position
	if (!Label().FindChild(1).FindAttribute(TDF_Reference::GetID(), RefPos)) return 1;
	TDF_Label refPosLab = RefPos->Get();

	gp_Pnt Pos;
	TDataXtd_Geometry::Point(refPosLab, Pos);

	// Radius
	if (!Label().FindChild(2).FindAttribute(TDF_Reference::GetID(), RefRadius)) return 1;
	TDF_Label refradLab = RefRadius->Get();

	// Height
	if (!Label().FindChild(3).FindAttribute(TDF_Reference::GetID(), RefHeight)) return 1;
	TDF_Label refHtLab = RefHeight->Get();

	// Get Values

	Handle(TDataStd_Real) TSR;
	Standard_Real aStemRadius, aStemHeight , aConeHeight;

	if (!refradLab.FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
	aStemRadius = TSR->Get();

	if (!refHtLab.FindAttribute(TDataStd_Real::GetID(), TSR)) return 1;
	aConeHeight = TSR->Get();

	// Stem Values
	aStemHeight = aConeHeight * 3;
	gp_Pnt headPos(Pos.X(), Pos.Y() + aStemHeight, Pos.Z());
	Standard_Real headRadius, headHeight;

	headRadius = aStemRadius * 1.5;
	headHeight = aConeHeight * 0.5;

	TDataXtd_Point::Set(Label().FindChild(1, false), headPos);
	TDataStd_Real::Set(Label().FindChild(2, false), headRadius);
	TDataStd_Real::Set(Label().FindChild(3, false), headHeight);

	gp_Ax2 axes(headPos, gp_Dir(0., 1., 0.));
	TopoDS_Shape head = BRepPrimAPI_MakeCylinder(axes, headRadius, headHeight);

	// Build a TNaming_NamedShape using built cone
	TNaming_Builder B(Label());
	B.Generated(head);

	log->SetImpacted(Label());

	return 0;
}

//=======================================================================
//Class    :	Nail 
//purpose  :	Create Nail
//=======================================================================

TDF_Label NailDesign::CreateNail(TDF_Label& MainLab, gp_Pnt& Pos, Standard_Real& aConeRadius, Standard_Real& aConeHeight)
{
	// Data Structure
	// 0:1 MainLab
	//        |
	//      0:1:1 Nail Label ---> Name --->  Named shape ---> Function1
	//              |
	//			0:1:1:1	Cone Label ---> Name --->  Named shape ---> Function2
	//					|
	//				0:1:1:1:1 -- Pos
	//					|
	//				0:1:1:1:2 -- aConeRadius
	//					|
	//				0:1:1:1:3 -- aConeHeight
	//				|
	//			0:1:1:2	Stem Label ---> Name --->  Named shape ---> Function3
	//					|
	//				0:1:1:2:1 -- "Cone" position translated for "Cone" height along Y
	//					|
	//				0:1:1:2:2 -- "Cone" radius
	//					|
	//				0:1:1:2:3 -- "Cone" height multiplied by 3
	//				|
	//			0:1:1:3	Head Label ---> Name --->  Named shape ---> Function4
	//					|
	//				0:1:1:3:1 -- "Long cylinder" position translated for "Long cylinder" height along Y
	//					|
	//				0:1:1:3:2 -- "Long cylinder" radius multiplied by 1.5
	//					|
	//				0:1:1:3:3 -- "Cone" height divided by 2


	// Create a new label in the data structure for the Nail
	TDF_Label NAIL = TDF_TagSource::NewChild(MainLab);    
	TCollection_ExtendedString Name = "Nail";
	TDataStd_Name::Set(NAIL, "Nail");

	// Cone 
	TDF_Label CONE = NAIL.FindChild(1);
	Name = "Nail Cone";
	TDataStd_Name::Set(CONE, "Cone");

	TDF_Label aPosCone = CONE.FindChild(1);
	TDataXtd_Point::Set(aPosCone, Pos);
	TDataStd_Name::Set(aPosCone, "Position");

	TDF_Label aRadiusCone = CONE.FindChild(2);
	TDataStd_Real::Set(aRadiusCone, aConeRadius);
	TDataStd_Name::Set(aRadiusCone, "Cone Radius");

	TDF_Label aHeightCone = CONE.FindChild(3);
	TDataStd_Real::Set(aHeightCone, aConeHeight);
	TDataStd_Name::Set(aHeightCone, "Cone Height");

	// Stem
	TDF_Label STEM = NAIL.FindChild(2);
	Name = "Nail Stem";
	TDataStd_Name::Set(STEM, "Stem");

	TDF_Label aPosStem = STEM.FindChild(1);
	TDF_Reference::Set(aPosStem, aPosCone);
	TDataStd_Name::Set(aPosStem, "Position");

	TDF_Label aRadiusStem = STEM.FindChild(2);
	TDF_Reference::Set(aRadiusStem, aRadiusCone);
	TDataStd_Name::Set(aRadiusStem, "Stem Radius");

	TDF_Label aHeightStem = STEM.FindChild(3);
	TDF_Reference::Set(aHeightStem, aHeightCone);
	TDataStd_Name::Set(aHeightStem, "Stem Height");

	// Head
	TDF_Label HEAD = NAIL.FindChild(3);
	Name = "Nail Head";
	TDataStd_Name::Set(HEAD, "Head");

	TDF_Label aPosHead = HEAD.FindChild(1);
	TDF_Reference::Set(aPosHead, aPosStem);
	TDataStd_Name::Set(aPosHead, "Position");

	TDF_Label aRadiusHead = HEAD.FindChild(2);
	TDF_Reference::Set(aRadiusHead, aRadiusStem);
	TDataStd_Name::Set(aRadiusHead, "Head Radius");

	TDF_Label aHeightHead = HEAD.FindChild(3);
	TDF_Reference::Set(aHeightHead, aHeightCone);
	TDataStd_Name::Set(aHeightHead, "Head Height");

	// Initialise functions
	Handle(TFunction_Function) myNailFunction = 
		TFunction_Function::Set(NAIL, NailDriver::GetID());

	Handle(TFunction_Function) myNailConeFunction = 
		TFunction_Function::Set(CONE, NailConeDriver::GetID());

	Handle(TFunction_Function) myNailStemFunction = 
		TFunction_Function::Set(STEM, NailStemDriver::GetID());

	Handle(TFunction_Function) myNailHeadFunction = 
		TFunction_Function::Set(HEAD, NailHeadDriver::GetID());

	// Set function scope for nail label
	Handle(TFunction_Scope) scope = TFunction_Scope::Set(NAIL);
	scope->AddFunction(CONE);
	scope->AddFunction(STEM);
	scope->AddFunction(HEAD);
	scope->AddFunction(NAIL);

	Handle(TFunction_Logbook) log = TFunction_Logbook::Set(NAIL);
	log->SetTouched(CONE);

	UpdateNail(NAIL);

	return NAIL;
}

void NailDesign::GetDimensions(TDF_Label& Nail, gp_Pnt& Pos, Standard_Real& aConeRadius, Standard_Real& aConeHeight)
{
	// get position
	TDF_Label Cone = Nail.FindChild(1, false);
	TDataXtd_Geometry::Point(Cone.FindChild(1, false), Pos);

	// get Radius
	Handle(TDataStd_Real) curReal;
	Cone.FindChild(2).FindAttribute(TDataStd_Real::GetID(), curReal);
	aConeRadius = curReal->Get();

	// get Height
	Cone.FindChild(3).FindAttribute(TDataStd_Real::GetID(), curReal);
	aConeHeight = curReal->Get();

}

void NailDesign::EditNail(TDF_Label& Nail, gp_Pnt& Pos, Standard_Real& aConeRadius, Standard_Real& aConeHeight)
{
	// The information on  modifications in the model is received.  
	Handle(TFunction_Logbook) log;
	log = TFunction_Logbook::Set(Nail);

	// Check position
	TDF_Label Cone = Nail.FindChild(1, false);
	gp_Pnt oldPos;
	TDataXtd_Geometry::Point(Cone.FindChild(1, false), oldPos);

	if (!oldPos.IsEqual(Pos, 0.001))
	{
		TDataXtd_Point::Set(Cone.FindChild(1, false), Pos);
		log->SetTouched(Cone.FindChild(1, false));
		log->SetTouched(Cone);
	}

	// Check Radius
	Handle(TDataStd_Real) curReal;

	Cone.FindChild(2).FindAttribute(TDataStd_Real::GetID(), curReal);
	if (aConeRadius != curReal->Get())
	{
		TDataStd_Real::Set(Cone.FindChild(2), aConeRadius);
		// Set the label of the attribute as touched for next recomputation
		log->SetTouched(curReal->Label());
		log->SetTouched(Cone);
	}
	
	// Check Height
	Cone.FindChild(3).FindAttribute(TDataStd_Real::GetID(), curReal);
	if (aConeHeight != curReal->Get())
	{
		TDataStd_Real::Set(Cone.FindChild(3), aConeHeight);
		// Set the label of the attribute as touched for next recomputation
		log->SetTouched(curReal->Label());
		log->SetTouched(Cone);
	}

	UpdateNail(Nail);
}


void NailDesign::UpdateNail(TDF_Label& Nail)
{
	// Get Scope
	Handle(TFunction_Scope) scope;
	scope = TFunction_Scope::Set(Nail);

	// The information on  modifications in the model is received.  
	Handle(TFunction_Logbook) log;
	log = TFunction_Logbook::Set(Nail);

	// The iterator is iInitialized by  the scope of functions.
	TFunction_DoubleMapIteratorOfDoubleMapOfIntegerLabel itrm(scope->GetFunctions());

	for (; itrm.More(); itrm.Next())
	{
		const TDF_Label& L = itrm.Key2();
		
		//  An interface for the function is created.  
		TFunction_IFunction  TFinterface(L);

		//  The function driver is retrieved.  
		Handle(TFunction_Driver)  driver = TFinterface.GetDriver();

		if (!driver.IsNull())
		{
			//  The dependency of the function on the  modified data is checked. 
			if (driver->MustExecute(log))
			{
				// The function is executed.  
				int  ret = driver->Execute(log);
				if (ret)
					return;

			}
		}
	}	
}
