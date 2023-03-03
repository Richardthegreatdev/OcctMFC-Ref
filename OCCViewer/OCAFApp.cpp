#include "pch.h"
#include "OCAFApp.h"

#include "FlangeDesign.h"
#include "NailDesign.h"

//=======================================================================
//Class    :	OCAFApp
//purpose  :	Create OCAF based application
//=======================================================================

IMPLEMENT_STANDARD_RTTIEXT(OCAFApp, TDocStd_Application)

OCAFApp::OCAFApp()
{
	InitializeDrivers();
}

void OCAFApp::InitializeDrivers()
{
	// Define Driver functions

	TFunction_DriverTable::Get()->AddDriver(FlangeDriver::GetID(), new FlangeDriver());

	TFunction_DriverTable::Get()->AddDriver(NailDriver::GetID(), new NailDriver());
	TFunction_DriverTable::Get()->AddDriver(NailConeDriver::GetID(), new NailConeDriver());
	TFunction_DriverTable::Get()->AddDriver(NailStemDriver::GetID(), new NailStemDriver());
	TFunction_DriverTable::Get()->AddDriver(NailHeadDriver::GetID(), new NailHeadDriver());

}


