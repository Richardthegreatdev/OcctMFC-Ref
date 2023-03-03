#pragma once

#include "OccFunctions.h"

//=======================================================================
//Class    :	Flange Driver
//purpose  :	Create Flange
//=======================================================================
class TFunction_Logbook;

class FlangeDriver : public TFunction_Driver
{
public:

	static const Standard_GUID& GetID();

	FlangeDriver();
	virtual  void Validate(Handle(TFunction_Logbook)& log) const;
	virtual  Standard_Boolean MustExecute(const Handle(TFunction_Logbook)& log) const;
	virtual  Standard_Integer Execute(Handle(TFunction_Logbook)& log) const;

	// OCCT RTTI
	DEFINE_STANDARD_RTTIEXT(FlangeDriver, TFunction_Driver)
};

//=======================================================================
//Class    :	Flange 
//purpose  :	Create Flange
//=======================================================================

class Flange
{
public:

	static int NbFlanges;

	/// <summary>
	/// Create Flange Data Structure
	/// </summary>
	/// <param name="MainLab">Root label for flange</param>
	/// <param name="OD">Outside Diameter</param>
	/// <param name="ID">Inside Diameter</param>
	/// <param name="flgthk">Flange Thickness</param>
	/// <param name="NbBolts">No.of Bolts</param>
	/// <param name="BoltDia">Bolt Diameter</param>
	/// <param name="BoltPCD">Bolt P.C.D</param>
	/// <returns></returns>
	static TDF_Label CreateFlangeDS(TDF_Label& MainLab,
		Standard_Real OD, Standard_Real ID, Standard_Real flgthk,
		Standard_Real NbBolts, Standard_Real BoltDia, Standard_Real BoltPCD);

	static TopoDS_Shape CreateFlangeBody(gp_Ax2 Pos,
		Standard_Real OD, Standard_Real ID, Standard_Real flgthk,
		Standard_Real NbBolts, Standard_Real BoltDia, Standard_Real BoltPCD);

	/// <summary>
	/// Modify Flange
	/// </summary>
	/// <param name="FLG">Flange Label</param>
	/// <param name="OD">Outside Diameter</param>
	/// <param name="ID">Inside Diameter</param>
	/// <param name="flgthk">Flange Thickness</param>
	/// <param name="NbBolts">No.of Bolts</param>
	/// <param name="BoltDia">Bolt Diameter</param>
	/// <param name="BoltPCD">Bolt P.C.D</param>
	static void ModifyFlange(TDF_Label& FLG,
		Standard_Real OD, Standard_Real ID, Standard_Real flgthk,
		Standard_Real NbBolts, Standard_Real BoltDia, Standard_Real BoltPCD);


	static void GetDimensions(TDF_Label& FLG,
		Standard_Real& OD, Standard_Real& ID, Standard_Real& flgthk,
		Standard_Real& NbBolts, Standard_Real& BoltDia, Standard_Real& BoltPCD);
};
