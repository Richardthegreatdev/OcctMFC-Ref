#include "pch.h"
#include "framework.h"

#include "OCCViewerView.h"


TopoDS_Shape COCCViewerView::GetSelectedShape()
{
	myAISContext->InitSelected();
	if (myAISContext->NbSelected() == 1)
	{
		return myAISContext->SelectedShape();
	}
	return TopoDS_Shape();
}

Handle(AIS_InteractiveObject) COCCViewerView::GetSelectedObj()
{
	myAISContext->InitSelected();
	if (myAISContext->NbSelected() == 1)
	{
		return myAISContext->SelectedInteractive();
	}

	return NULL;
}

Handle(TopTools_HSequenceOfShape) COCCViewerView::GetSelectedShapeSeq()
{
	Handle(TopTools_HSequenceOfShape) aSequence = 
		                              new TopTools_HSequenceOfShape();

	Handle(AIS_InteractiveObject) picked;

	for (myAISContext->InitSelected(); myAISContext->MoreSelected(); 
		                               myAISContext->NextSelected())
	{
		picked = myAISContext->SelectedInteractive();

		if (picked->IsKind(STANDARD_TYPE(AIS_Shape)))
		{
			Handle(AIS_Shape) aisShape = Handle(AIS_Shape)::DownCast(picked);
			TopoDS_Shape aShape = aisShape->Shape();
			aSequence->Append(aShape);
		}
	}
	return aSequence;
}

void COCCViewerView::SelectAllObjects()
{
	// Get objects from context
	AIS_ListOfInteractive aList;
	myAISContext->ObjectsInside(aList);

	// Iterate on the list to get shape objects
	if (aList.Size() > 0)
	{
		for (AIS_ListIteratorOfListOfInteractive aLI(aList); aLI.More(); aLI.Next())
		{
			if (aLI.Value()->Type() == AIS_KindOfInteractive::AIS_KOI_Shape)
			{
				myAISContext->AddSelect(aLI.Value());
			}
		}
	}

}

TopoDS_Compound COCCViewerView::GetAllShapes()
{
	BRep_Builder builder;
	TopoDS_Compound allShapes;
	builder.MakeCompound(allShapes);

	AIS_ListOfInteractive alist = GetListofBodies();

	for (AIS_ListIteratorOfListOfInteractive aLI(alist); aLI.More(); aLI.Next())
	{
		Handle(AIS_Shape) aisShape = Handle(AIS_Shape)::DownCast(aLI.Value());
		TopoDS_Shape aShape = aisShape->Shape();
		builder.Add(allShapes, aShape);
	}

	return allShapes;
}

void COCCViewerView::SetSelectionFilter(Standard_Integer theMode)
{
	myAISContext->Deactivate();
	
	if (myAISContext->Filters().Size() > 0)
	{
		myAISContext->RemoveFilters();
	}

	myAISContext->Activate(theMode);
}

void COCCViewerView::SetEdgeFilter(StdSelect_TypeOfEdge aTypeOfEdge)
{
	// Activate filters for edges
	SetSelectionFilter(AIS_Shape::SelectionMode(TopAbs_EDGE));

	// Add custom edge filter to select aTypeOfEdge only 	
	Handle(StdSelect_EdgeFilter) aFil1 = new StdSelect_EdgeFilter(aTypeOfEdge);

	myAISContext->AddFilter(aFil1);
}

void COCCViewerView::SetFaceFilter(StdSelect_TypeOfFace aTypeOfFace)
{
	// Activate filters for faces
	SetSelectionFilter(AIS_Shape::SelectionMode(TopAbs_FACE));

	// Add custom face filter to select aTypeOfFace only 	
	Handle(StdSelect_FaceFilter) aFil1 = new StdSelect_FaceFilter(aTypeOfFace);

	myAISContext->AddFilter(aFil1);
}

void COCCViewerView::RemoveAllFilters()
{
	// Activate filters for shapes
	SetSelectionFilter(AIS_Shape::SelectionMode(TopAbs_SHAPE));

	myAISContext->RemoveFilters();
}

void COCCViewerView::SetHighilightColor(Quantity_Color aNameOfColor)
{
	const Handle(Prs3d_Drawer) HighlightStyle = new Prs3d_Drawer();
	HighlightStyle->SetColor(aNameOfColor);
	myAISContext->SetHighlightStyle(HighlightStyle);
}

void COCCViewerView::SetSelectionColor(Quantity_Color aNameOfColor)
{
	const Handle(Prs3d_Drawer) selectionStyle = new Prs3d_Drawer();
	selectionStyle->SetColor(aNameOfColor);
	myAISContext->SetSelectionStyle(selectionStyle);
}