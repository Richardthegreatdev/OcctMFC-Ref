#include "pch.h"
#include "OCAFApp.h"

//=======================================================================
//Class    :	OCAFView
//purpose  :	Create OCAF based application
//=======================================================================

IMPLEMENT_DYNCREATE(OCAFView, COCCViewerView)

OCAFDoc* OCAFView::GetDocument() const
{
	return reinterpret_cast<OCAFDoc*>(m_pDocument);
}

void OCAFView::OnInitialUpdate()
{	
	__super::OnInitialUpdate();

	// Update 
	GetDocument()->InitialisePrs();
}