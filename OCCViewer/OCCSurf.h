#pragma once

#include "OCCheaders.h"

class OCCSurf
{

public:
	// Elementary Surfaces
	static Handle(Geom_Surface) PlaneSurface(gp_Pln& aPlane);
	static Handle(Geom_Surface) SphericalSurface(const gp_Ax3& A3, const Standard_Real Radius);
	static Handle(Geom_Surface) CylindricalSurface(const gp_Ax3& A3, const Standard_Real Radius , const Standard_Real Height);

	// Swept Surfaces

	// Bounded Surfaces
	static Handle(Geom_BSplineSurface) MakeBoundarySurface(const TColGeom_Array1OfBSplineCurve& theBoundary);
	static Handle(Geom_BSplineSurface) MakeLoftSurface(const TColGeom_SequenceOfCurve& theSections);


	// Advanced Surfacing functions

	static Handle(Geom_BSplineSurface) MakeSurface(const TColGeom_ListOfCurve& theSections);

	static Handle(Geom_BSplineSurface) MakeSweptSurface(const Handle(Geom_Curve)& Path,
		                                                const Handle(Geom_Curve)& FirstSec);

	// Conversion functions
	static Handle(Geom_BSplineCurve) CurveToBSplineCurve(const Handle(Geom_Curve)& C);
	static Handle(Geom_BSplineSurface)	SurfaceToBSplineSurface(const Handle(Geom_Surface)& S);
};

