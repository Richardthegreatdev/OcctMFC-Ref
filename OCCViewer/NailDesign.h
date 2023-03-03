#pragma once

#include "OccFunctions.h"

//=======================================================================
//Class    :	Nail Driver
//purpose  :	Create Nail
//=======================================================================
class NailDriver : public TFunction_Driver
{
public:

	static const Standard_GUID& GetID();

	NailDriver() {};
	
	virtual void Arguments(TDF_LabelList& args) const;
	virtual void Results(TDF_LabelList& res) const;
	virtual  Standard_Integer Execute(Handle(TFunction_Logbook)& log) const;

	// OCCT RTTI
	DEFINE_STANDARD_RTTIEXT(NailDriver, TFunction_Driver)
};


//=======================================================================
//Class    :	Nail Cone Driver
//purpose  :	Create Nail Cone
//=======================================================================
class NailConeDriver : public TFunction_Driver
{
public:

	static const Standard_GUID& GetID();

	NailConeDriver() {};

	virtual void Arguments(TDF_LabelList& args) const;
	virtual void Results(TDF_LabelList& res) const;
	virtual  Standard_Integer Execute(Handle(TFunction_Logbook)& log) const;

	// OCCT RTTI
	DEFINE_STANDARD_RTTIEXT(NailConeDriver, TFunction_Driver)
};


//=======================================================================
//Class    :	Nail Stem Driver
//purpose  :	Create Nail Stem
//=======================================================================
class NailStemDriver : public TFunction_Driver
{
public:

	static const Standard_GUID& GetID();

	NailStemDriver() {};

	virtual void Arguments(TDF_LabelList& args) const;
	virtual void Results(TDF_LabelList& res) const;
	virtual  Standard_Integer Execute(Handle(TFunction_Logbook)& log) const;

	// OCCT RTTI
	DEFINE_STANDARD_RTTIEXT(NailStemDriver, TFunction_Driver)
};

//=======================================================================
//Class    :	Nail Head Driver
//purpose  :	Create Nail Head
//=======================================================================
class NailHeadDriver : public TFunction_Driver
{
public:

	static const Standard_GUID& GetID();

	NailHeadDriver() {};

	virtual void Arguments(TDF_LabelList& args) const;
	virtual void Results(TDF_LabelList& res) const;
	virtual  Standard_Integer Execute(Handle(TFunction_Logbook)& log) const;

	// OCCT RTTI
	DEFINE_STANDARD_RTTIEXT(NailHeadDriver, TFunction_Driver)
};

//=======================================================================
//Class    :	Nail 
//purpose  :	Create Nail
//=======================================================================
class NailDesign
{
public:

	static TDF_Label CreateNail(TDF_Label& MainLab, 
		                        gp_Pnt& Pos, 
		                        Standard_Real& aConeRadius, 
		                        Standard_Real& aConeHeight);

	static void EditNail(TDF_Label& Nail,
						 gp_Pnt& Pos,
						 Standard_Real& aConeRadius,
						 Standard_Real& aConeHeight);

	static void UpdateNail(TDF_Label& Nail);

	static void GetDimensions(TDF_Label& Nail, gp_Pnt& Pos,
							  Standard_Real& aConeRadius,
							  Standard_Real& aConeHeight);

};