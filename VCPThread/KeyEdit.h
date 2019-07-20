// KeyEdit.h 

// interface file
#pragma once


////////////////////////////////////////////////////////////////////////////////
// CKeyEdit class
////////////////////////////////////////////////////////////////////////////////
class CKeyEdit : public CEdit
{
	DECLARE_DYNAMIC(CKeyEdit)

public:
	CKeyEdit();
	virtual ~CKeyEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


