
// EightDigitalGame2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CEightDigitalGame2App:
// See EightDigitalGame2.cpp for the implementation of this class
//

class CEightDigitalGame2App : public CWinApp
{
public:
	CEightDigitalGame2App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEightDigitalGame2App theApp;