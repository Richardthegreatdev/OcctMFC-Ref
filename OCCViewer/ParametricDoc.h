#pragma once

// ParametricDoc document
#include "OCAFApp.h"

class ParametricDoc : public OCAFDoc
{
	DECLARE_DYNCREATE(ParametricDoc)

public:
	ParametricDoc();
	virtual ~ParametricDoc();
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();
	virtual void UpdateDocument();

	int idesT = 0;
public:
	void CreateOCAFTree();
	
	// Flange Design
	void CreateFlange();
	void EditFlange();
	void UpdateFlange();

	// Nail Design
	void CreateNail();
	void EditNail();
	void UpdateNail();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFlangedesignCreateflange();
	afx_msg void OnFlangedesignEditflange();
	afx_msg void OnUpdateFlangedesignEditflange(CCmdUI* pCmdUI);
	afx_msg void OnOcafOcaftree();
	afx_msg void OnUpdateOcafOcaftree(CCmdUI* pCmdUI);
	afx_msg void OnNaildesignCreatenail();
	afx_msg void OnNaildesignEditnail();
};
