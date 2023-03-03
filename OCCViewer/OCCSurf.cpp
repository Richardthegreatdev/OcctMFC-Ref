#include "pch.h"
#include "OCCSurf.h"

Handle(Geom_BSplineSurface) OCCSurf::MakeSurface(const TColGeom_ListOfCurve& theSections)
{
	//populate section generator
	GeomFill_SectionGenerator aSecGenerator;
	for (TColGeom_ListOfCurve::Iterator anIt(theSections); anIt.More();anIt.Next())
	{
		const Handle(Geom_Curve)& aCurve = anIt.Value();
		aSecGenerator.AddCurve(aCurve);
	}

	aSecGenerator.Perform(Precision::PConfusion());

	Handle(GeomFill_Line) aLine = new GeomFill_Line(theSections.Size());

	//parameters
	const Standard_Integer aMinDeg = 1, aMaxDeg = BSplCLib::MaxDegree(), aNbIt = 0;
	Standard_Real aTol3d = 1e-4, aTol2d = Precision::Parametric(aTol3d);

	// algorithm to approximate a BSplineSurface from the sections in section generator
	GeomFill_AppSurf anAlgo(aMinDeg, aMaxDeg, aTol3d, aTol2d, aNbIt);
	anAlgo.Perform(aLine, aSecGenerator);

	Handle(Geom_BSplineSurface) aRes;
	if (!anAlgo.IsDone()) 
		return aRes;

	aRes = new Geom_BSplineSurface(anAlgo.SurfPoles(), anAlgo.SurfWeights(),
								   anAlgo.SurfUKnots(), anAlgo.SurfVKnots(), 
								   anAlgo.SurfUMults(), anAlgo.SurfVMults(),
								   anAlgo.UDegree(), anAlgo.VDegree());

	return aRes;
}

/// <summary>
/// Create sweep surface with guide curve
/// </summary>
/// <param name="Path">Sweeping path</param>
/// <param name="FirstSec">Large Section</param>
/// <param name="SecondSec">Small Section</param>
/// <returns></returns>
Handle(Geom_BSplineSurface) OCCSurf::MakeSweptSurface(const Handle(Geom_Curve)& Path,
	                                                  const Handle(Geom_Curve)& FirstSec)
{
	// Sweep Section Generator
	GeomFill_SweepSectionGenerator aSecGenerator(Path, FirstSec);
	aSecGenerator.Perform(Standard_True);

	int asec = aSecGenerator.NbSections();
	Handle(GeomFill_Line) aLine = new GeomFill_Line(asec);

	//parameters
	const Standard_Integer aMinDeg = 2, aMaxDeg = BSplCLib::MaxDegree(), aNbIt = 0;
	Standard_Real aTol3d = 1e-4, aTol2d = Precision::Parametric(aTol3d);

	// algorithm to approximate a BSplineSurface from the sections in section generator
	GeomFill_AppSweep anAlgo(aMinDeg, aMaxDeg, aTol3d, aTol2d, aNbIt);
	anAlgo.Perform(aLine, aSecGenerator);

	Handle(Geom_BSplineSurface) aRes;
	if (!anAlgo.IsDone())
		return aRes;

	aRes = new Geom_BSplineSurface(anAlgo.SurfPoles(), anAlgo.SurfWeights(),
		anAlgo.SurfUKnots(), anAlgo.SurfVKnots(), anAlgo.SurfUMults(), anAlgo.SurfVMults(),
		anAlgo.UDegree(), anAlgo.VDegree());

	return aRes;

}

Handle(Geom_BSplineSurface) OCCSurf::MakeLoftSurface(const TColGeom_SequenceOfCurve& theSections)
{
	GeomFill_NSections surf(theSections);
	surf.ComputeSurface();

	return surf.BSplineSurface();
}

Handle(Geom_BSplineSurface) OCCSurf::MakeBoundarySurface(const TColGeom_Array1OfBSplineCurve& theBoundary)
{
	GeomFill_BSplineCurves bSPL;
	Handle(Geom_BSplineSurface) aRes;

	if (theBoundary.Upper() < 2)
		return aRes;

	if (theBoundary.Upper() > 4)
		return aRes;

	switch (theBoundary.Upper())
	{
	case 2:
		bSPL.Init(theBoundary.Value(1), theBoundary.Value(2), 
										GeomFill_FillingStyle::GeomFill_StretchStyle);
		break;
	case 3:
		bSPL.Init(theBoundary.Value(1), theBoundary.Value(2), theBoundary.Value(3), 
			                            GeomFill_FillingStyle::GeomFill_StretchStyle);
		break;
	case 4:
		bSPL.Init(theBoundary.Value(1), theBoundary.Value(2), theBoundary.Value(3), theBoundary.Value(4),
			                            GeomFill_FillingStyle::GeomFill_StretchStyle);
		break;
	default:
		break;
	}

	aRes = bSPL.Surface();

	return aRes;
}

//============================================================================================
//
//============================================================================================

Handle(Geom_BSplineCurve) OCCSurf::CurveToBSplineCurve(const Handle(Geom_Curve)& C)
{
	return GeomConvert::CurveToBSplineCurve(C);
}

Handle(Geom_BSplineSurface)	OCCSurf::SurfaceToBSplineSurface(const Handle(Geom_Surface)& S)
{
	return GeomConvert::SurfaceToBSplineSurface(S);
}

//============================================================================================
//
//============================================================================================

Handle(Geom_Surface) OCCSurf::CylindricalSurface(const gp_Ax3& A3, const Standard_Real Radius, const Standard_Real Height)
{

	Handle(Geom_CylindricalSurface) aCylSurf = 
		new Geom_CylindricalSurface(A3, Radius);

	// Trimmed surface 
	Handle(Geom_RectangularTrimmedSurface) aTrimmedSurface =
		new Geom_RectangularTrimmedSurface(aCylSurf, 0, Height, false);

	return aTrimmedSurface;
}

Handle(Geom_Surface) OCCSurf::SphericalSurface(const gp_Ax3& A3, const Standard_Real Radius)
{
	Handle(Geom_SphericalSurface) aSphSurf = 
		new Geom_SphericalSurface(A3, Radius);

	return aSphSurf;
}

Handle(Geom_Surface) OCCSurf::PlaneSurface(gp_Pln& aPlane)
{
	// Geometric Plane
	Handle(Geom_Plane) aGPlane = 
		GC_MakePlane(aPlane).Value();

	return aGPlane;
}