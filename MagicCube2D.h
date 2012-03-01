// MagicCube2D.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CMagicCube2DApp:
// See MagicCube2D.cpp for the implementation of this class
//

class CMagicCube2DApp : public CWinApp
{
public:
	CMagicCube2DApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMagicCube2DApp theApp;