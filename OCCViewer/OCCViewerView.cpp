
// OCCViewerView.cpp : implementation of the COCCViewerView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OCCViewer.h"
#endif


#include "OCCViewerView.h"



// COCCViewerView

IMPLEMENT_DYNCREATE(COCCViewerView, CView)

BEGIN_MESSAGE_MAP(COCCViewerView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_STANDARDVIEWS_FRONT, &COCCViewerView::OnStandardviewsFront)
	ON_COMMAND(ID_STANDARDVIEWS_BACK, &COCCViewerView::OnStandardviewsBack)
	ON_COMMAND(ID_STANDARDVIEWS_TOP, &COCCViewerView::OnStandardviewsTop)
	ON_COMMAND(ID_STANDARDVIEWS_BOTTOM, &COCCViewerView::OnStandardviewsBottom)
	ON_COMMAND(ID_STANDARDVIEWS_LEFT, &COCCViewerView::OnStandardviewsLeft)
	ON_COMMAND(ID_STANDARDVIEWS_RIGHT, &COCCViewerView::OnStandardviewsRight)
	ON_COMMAND(ID_STANDARDVIEWS_ISOMETRIC, &COCCViewerView::OnStandardviewsIsometric)
END_MESSAGE_MAP()

// COCCViewerView construction/destruction

COCCViewerView::COCCViewerView() noexcept
	: myUpdateRequests(0),
	myCurZoom(0.0),
	myCurrentMode(CurAction3d_Nothing)
{
	// TODO: add construction code here	
	myGraphicDevice = NULL;
	myViewer = NULL;
	myView = NULL;
	myAISContext = NULL;
	myDefaultGestures = myMouseGestureMap;
}

COCCViewerView::~COCCViewerView()
{
	if (!myView.IsNull())
	{
		myView->Remove();
		myView.Nullify();
	}
}

BOOL COCCViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COCCViewerView drawing

void COCCViewerView::OnDraw(CDC* /*pDC*/)
{
	// TODO: add draw code for native data here
	if (!myView.IsNull())
	{
		myView->InvalidateImmediate();
		FlushViewEvents(GetAISContext(), myView, true);
	}
}

void COCCViewerView::UpdateOCCView()
{
	if (!myView.IsNull())
	{
		if (++myUpdateRequests == 1)
		{
			Invalidate(FALSE);
			UpdateWindow();
		}
	}
}

void COCCViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// COCCViewerView diagnostics

#ifdef _DEBUG
void COCCViewerView::AssertValid() const
{
	CView::AssertValid();
}

void COCCViewerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COCCViewerDoc* COCCViewerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COCCViewerDoc)));
	return (COCCViewerDoc*)m_pDocument;
}
#endif //_DEBUG


// COCCViewerView message handlers


int COCCViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!CreateView())
		return -1;

	return 0;
}


void COCCViewerView::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (cx != 0
		&& cy != 0
		&& !myView.IsNull())
	{
		myView->Window()->DoResize();
		myView->MustBeResized();
		myView->Invalidate();
		UpdateOCCView();
	}
}


void COCCViewerView::OnInitialUpdate()
{
	__super::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	SetUpMouseGestures();
}

// ================================================================
// Function : CreateView
// Purpose  :
// ================================================================
bool COCCViewerView::CreateView()
{
	try
	{
		// Graphic Connection
		Handle(Aspect_DisplayConnection) aDisplayConnection;
		myGraphicDevice = new OpenGl_GraphicDriver(aDisplayConnection);

		// Graphic Driver
		Handle(OpenGl_GraphicDriver) aDriver = Handle(OpenGl_GraphicDriver)::DownCast(myGraphicDevice);

		// Viewer
		myViewer = new V3d_Viewer(aDriver);
		myViewer->SetDefaultLights();
		myViewer->SetLightOn();
		myViewer->SetDefaultViewProj(V3d_XposYposZpos);
		myViewer->SetDefaultLights();
		myViewer->SetLightOn();

		// View
		myView = myViewer->CreateView();
		Handle(WNT_Window) aWNTWindow = new WNT_Window(GetSafeHwnd());
		myView->SetWindow(aWNTWindow);
		myView->Camera()->SetProjectionType(aDriver->Options().contextStereo
			? Graphic3d_Camera::Projection_Stereo
			: Graphic3d_Camera::Projection_Orthographic);

		if (!aWNTWindow->IsMapped())
		{
			aWNTWindow->Map();
		}

		myView->SetProj(V3d_XposYposZpos, Standard_True);

		// Background color
		m_BackGroundColor = RGB(192, 192, 192);
		Standard_Real R1 = GetRValue(m_BackGroundColor) / 255.;
		Standard_Real G1 = GetGValue(m_BackGroundColor) / 255.;
		Standard_Real B1 = GetBValue(m_BackGroundColor) / 255.;
		myView->SetBackgroundColor(Quantity_TOC_RGB, R1, G1, B1);

		// Interactive Context
		myAISContext = new AIS_InteractiveContext(myViewer);
		myAISContext->UpdateCurrentViewer();

		// Triedron
		Color = Quantity_NOC_BLACK;
		m_TrihedronScale = 0.12;
		Position = Aspect_TOTP_RIGHT_LOWER;
		myView->TriedronDisplay(Position, Color, m_TrihedronScale, V3d_ZBUFFER);

		UnitsAPI::SetLocalSystem(UnitsAPI_MDTV);

		// Other initializations
		aEdgeAspt = new Prs3d_LineAspect(Quantity_NOC_BLACK, Aspect_TypeOfLine::Aspect_TOL_SOLID, 1.2);
		
		// Set Shaded with edges
		OnShadedEdges();

		SetupCsys();

		SetupAISTrihedron();
		
		ShowAISTrihedron(true);

	}
	catch (Standard_Failure)
	{
		AfxMessageBox(L"Fatal Error in graphic initialisation");
		return false;
	}

	return true;
}

// ================================================================
// Function : handleViewRedraw
// Purpose  :
// ================================================================
void COCCViewerView::handleViewRedraw(const Handle(AIS_InteractiveContext)& theCtx,
	const Handle(V3d_View)& theView)
{
	myUpdateRequests = 0;
	AIS_ViewController::handleViewRedraw(theCtx, theView);
}

// ================================================================
// Function : Mouse Message Handlers
// Purpose  :
// ================================================================

void COCCViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	const Aspect_VKeyFlags aFlags = WNT_Window::MouseKeyFlagsFromEvent(nFlags);
	PressMouseButton(Graphic3d_Vec2i(point.x, point.y), Aspect_VKeyMouse_LeftButton, aFlags, false);
	UpdateOCCView();
}


void COCCViewerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	const Aspect_VKeyFlags aFlags = WNT_Window::MouseKeyFlagsFromEvent(nFlags);
	ReleaseMouseButton(Graphic3d_Vec2i(point.x, point.y), Aspect_VKeyMouse_LeftButton, aFlags, false);
	UpdateOCCView();

	if (myCurrentMode != CurAction3d_Nothing)
	{
		setCurrentAction(CurAction3d_Nothing);
	}
}


void COCCViewerView::OnMButtonUp(UINT nFlags, CPoint point)
{
	const Aspect_VKeyFlags aFlags = WNT_Window::MouseKeyFlagsFromEvent(nFlags);
	ReleaseMouseButton(Graphic3d_Vec2i(point.x, point.y), Aspect_VKeyMouse_MiddleButton, aFlags, false);
	UpdateOCCView();

	if (myCurrentMode != CurAction3d_Nothing)
	{
		setCurrentAction(CurAction3d_Nothing);
	}
}


void COCCViewerView::OnMButtonDown(UINT nFlags, CPoint point)
{
	const Aspect_VKeyFlags aFlags = WNT_Window::MouseKeyFlagsFromEvent(nFlags);
	PressMouseButton(Graphic3d_Vec2i(point.x, point.y), Aspect_VKeyMouse_MiddleButton, aFlags, false);
	UpdateOCCView();
}

void COCCViewerView::OnRButtonDown(UINT nFlags, CPoint point)
{
	const Aspect_VKeyFlags aFlags = WNT_Window::MouseKeyFlagsFromEvent(nFlags);
	PressMouseButton(Graphic3d_Vec2i(point.x, point.y), Aspect_VKeyMouse_RightButton, aFlags, false);
	UpdateOCCView();
	myClickPos.SetValues(point.x, point.y);
}

void COCCViewerView::OnRButtonUp(UINT nFlags , CPoint point)
{
	const Aspect_VKeyFlags aFlags = WNT_Window::MouseKeyFlagsFromEvent(nFlags);
	ReleaseMouseButton(Graphic3d_Vec2i(point.x, point.y), Aspect_VKeyMouse_RightButton, aFlags, false);
	UpdateOCCView();

	if (myCurrentMode != CurAction3d_Nothing)
	{
		setCurrentAction(CurAction3d_Nothing);
	}

	if (aFlags == Aspect_VKeyFlags_NONE
		&& (myClickPos - Graphic3d_Vec2i(point.x, point.y)).cwiseAbs().maxComp() <= 4)
	{
		DoMousePopup(point.x, point.y, myView);
	}
}


void COCCViewerView::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT aMouseEvent;          // for WM_MOUSELEAVE
	aMouseEvent.cbSize = sizeof(aMouseEvent);
	aMouseEvent.dwFlags = TME_LEAVE;
	aMouseEvent.hwndTrack = m_hWnd;
	aMouseEvent.dwHoverTime = HOVER_DEFAULT;
	if (!::_TrackMouseEvent(&aMouseEvent)) { TRACE("Track ERROR!\n"); }

	const Aspect_VKeyFlags aFlags = WNT_Window::MouseKeyFlagsFromEvent(nFlags);
	if (UpdateMousePosition(Graphic3d_Vec2i(point.x, point.y), PressedMouseButtons(), aFlags, false))
	{
		UpdateOCCView();
	}
}


BOOL COCCViewerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	const Standard_Real aDeltaF = Standard_Real(zDelta) / Standard_Real(WHEEL_DELTA);
	CPoint aCursorPnt = pt;
	ScreenToClient(&aCursorPnt);
	const Graphic3d_Vec2i  aPos(aCursorPnt.x, aCursorPnt.y);
	const Aspect_VKeyFlags aFlags = WNT_Window::MouseKeyFlagsFromEvent(nFlags);
	if (UpdateMouseScroll(Aspect_ScrollDelta(aPos, aDeltaF, aFlags)))
	{
		UpdateOCCView();
	}

	return true;
}

// ================================================================
// Function : Mouse Gesture
// Purpose  :
// ================================================================

void COCCViewerView::SetUpMouseGestures()
{
	if (bUseDefaultMouseGestures)
	{
		myMouseGestureMap = myDefaultGestures;
		return;
	}

	myMouseGestureMap.Clear();
	myMouseGestures.Clear();

	myMouseGestures.Bind(Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_SelectRectangle);
	myMouseGestures.Bind(Aspect_VKeyMouse_LeftButton | Aspect_VKeyFlags_CTRL, AIS_MouseGesture_Zoom);
	myMouseGestures.Bind(Aspect_VKeyMouse_LeftButton | Aspect_VKeyFlags_SHIFT, AIS_MouseGesture_Pan);

	myMouseGestures.Bind(Aspect_VKeyMouse_RightButton, AIS_MouseGesture_Zoom);
	
	myMouseGestures.Bind(Aspect_VKeyMouse_MiddleButton, AIS_MouseGesture_RotateOrbit);
	myMouseGestures.Bind(Aspect_VKeyMouse_MiddleButton | Aspect_VKeyFlags_CTRL, AIS_MouseGesture_Pan);

	myMouseGestureMap = myMouseGestures;
}

void COCCViewerView::ActivateMouseGestures()
{
	myMouseGestureMap.Clear();
	AIS_MouseGesture aRot = AIS_MouseGesture_RotateOrbit;
	switch (myCurrentMode)
	{
	case CurAction3d_Nothing:
	{
		if (bUseDefaultMouseGestures)
		{
			myMouseGestureMap = myDefaultGestures;
		}
		else
		{
			myMouseGestureMap = myMouseGestures;
		}
		break;
	}
	case CurAction3d_DynamicZooming:
	{
		myMouseGestureMap.Bind(Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_Zoom);
		break;
	}
	case CurAction3d_GlobalPanning:
	{
		break;
	}
	case CurAction3d_WindowZooming:
	{
		myMouseGestureMap.Bind(Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_ZoomWindow);
		break;
	}
	case CurAction3d_DynamicPanning:
	{
		myMouseGestureMap.Bind(Aspect_VKeyMouse_LeftButton, AIS_MouseGesture_Pan);
		break;
	}
	case CurAction3d_DynamicRotation:
	{
		myMouseGestureMap.Bind(Aspect_VKeyMouse_LeftButton, aRot);
		break;
	}
	}
}

// =======================================================================
// function : SetOrientation
// purpose  :
// =======================================================================
void COCCViewerView::SetOrientation(ViewOrientation aOrient)
{
	switch (aOrient)
	{
	case Top:
	{
		if (!myView.IsNull())
		{
			myView->SetProj(V3d_Ypos, Standard_True);
		}
		break;
	}
	case Bottom:
	{
		if (!myView.IsNull())
			myView->SetProj(V3d_Yneg, Standard_True);
		break;
	}
	case Right:
	{
		if (!myView.IsNull())
			myView->SetProj(V3d_Xpos, Standard_True);
		break;
	}
	case Left:
	{
		if (!myView.IsNull())
			myView->SetProj(V3d_Xneg, Standard_True);
		break;
	}
	case Front:
	{
		if (!myView.IsNull())
			myView->SetProj(V3d_Zpos, Standard_True);
		break;
	}
	case Back:
	{
		if (!myView.IsNull())
			myView->SetProj(V3d_Zneg, Standard_True);
		break;
	}
	case Isometric:
	{
		if (!myView.IsNull())
			myView->SetProj(V3d_XposYposZpos, Standard_True);
		break;
	}
	}
	if (!myView.IsNull())
		FitAll();
}

void COCCViewerView::OnStandardviewsFront()
{
	// TODO: Add your command handler code here
	SetOrientation(ViewOrientation::Front);
}


void COCCViewerView::OnStandardviewsBack()
{
	// TODO: Add your command handler code here
	SetOrientation(ViewOrientation::Back);
}


void COCCViewerView::OnStandardviewsTop()
{
	// TODO: Add your command handler code here
	SetOrientation(ViewOrientation::Top);
}


void COCCViewerView::OnStandardviewsBottom()
{
	// TODO: Add your command handler code here
	SetOrientation(ViewOrientation::Bottom);
}


void COCCViewerView::OnStandardviewsLeft()
{
	// TODO: Add your command handler code here
	SetOrientation(ViewOrientation::Left);
}


void COCCViewerView::OnStandardviewsRight()
{
	// TODO: Add your command handler code here
	SetOrientation(ViewOrientation::Right);
}


void COCCViewerView::OnStandardviewsIsometric()
{
	// TODO: Add your command handler code here
	SetOrientation(ViewOrientation::Isometric);
}

// =======================================================================
// function : Background
// purpose  :
// =======================================================================

void COCCViewerView::InitializeBackgroundColors()
{
	m_GradientBackGround = true;

	m_BackGroundColor = RGB(192, 192, 192);
	m_TopGradientC = RGB(10.0f, 128.0f, 200.0f);
	m_BottomGradientC = RGB(255.0f, 255.0f, 255.0f);

	R1 = GetRValue(m_TopGradientC) / 255.;
	G1 = GetGValue(m_TopGradientC) / 255.;
	B1 = GetBValue(m_TopGradientC) / 255.;

	R2 = GetRValue(m_BottomGradientC) / 255.;
	G2 = GetGValue(m_BottomGradientC) / 255.;
	B2 = GetBValue(m_BottomGradientC) / 255.;

	m_BackGroundColorDef = RGB(192, 192, 192);
	m_TopGradientCDef = RGB(10.0f, 128.0f, 200.0f);
	m_BottomGradientCDef = RGB(255.0f, 255.0f, 255.0f);
}

void COCCViewerView::SetTopGradientColor(COLORREF TopGradientC)
{
	m_TopGradientC = TopGradientC;
	// Top Gradient
	R1 = GetRValue(m_TopGradientC) / 255.;
	G1 = GetGValue(m_TopGradientC) / 255.;
	B1 = GetBValue(m_TopGradientC) / 255.;
}

void COCCViewerView::SetBottomGradientColor(COLORREF BottomGradientC)
{
	m_BottomGradientC = BottomGradientC;
	// Bottom Gradient
	R2 = GetRValue(m_BottomGradientC) / 255.;
	G2 = GetGValue(m_BottomGradientC) / 255.;
	B2 = GetBValue(m_BottomGradientC) / 255.;
}

void COCCViewerView::SetBackGroundColor(COLORREF BackGroundColor)
{
	m_BackGroundColor = BackGroundColor;
}

void COCCViewerView::DrawBackground()
{
	if (m_GradientBackGround)
	{
		myView->SetBgGradientStyle(Aspect_GradientFillMethod::Aspect_GFM_VER);

		myView->SetBgGradientColors(Quantity_Color(R1, G1, B1, Quantity_TOC_RGB), Quantity_Color(R2, G2, B2, Quantity_TOC_RGB), Aspect_GradientFillMethod::Aspect_GFM_VER);
		myView->Update();
		myView->Redraw();

	}
	else
	{

		myView->SetBgGradientStyle(Aspect_GradientFillMethod::Aspect_GFM_NONE);
		R1 = GetRValue(m_BackGroundColor) / 255.;
		G1 = GetGValue(m_BackGroundColor) / 255.;
		B1 = GetBValue(m_BackGroundColor) / 255.;

		myView->SetBackgroundColor(Quantity_TOC_RGB, R1, G1, B1);
		myView->Update();
		myView->Redraw();
	}
}

