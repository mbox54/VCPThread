// KeyEdit.cpp 

// implementation file


#include "stdafx.h"
#include "VCPThreadDlg.h"
#include "VCPThread.h"
#include "KeyEdit.h"



////////////////////////////////////////////////////////////////////////////////
// CKeyEdit
////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CKeyEdit, CEdit)

CKeyEdit::CKeyEdit()
{

}

CKeyEdit::~CKeyEdit()
{
}


BEGIN_MESSAGE_MAP(CKeyEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


////////////////////////////////////////////////////////////////////////////////
// CKeyEdit message handlers


void CKeyEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN)
		return;


	CEdit::OnChar(nChar, nRepCnt, nFlags);
}


void CKeyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RETURN)
	{
		// Предполагаем, что родительское окно эдит-бокса -
		// диалог класса CVCPThreadDlg, который имеет буфер хранения
		// введенного текста m_strCommand типа CString.

		CVCPThreadDlg* pDlg = (CVCPThreadDlg*)GetParent();
		//GetWindowText(pDlg->m_strCommand);
		//


		//pDlg->GetNextDlgTabItem(this)->SetFocus();
		return;   // запрет обработки по-умолчанию
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
