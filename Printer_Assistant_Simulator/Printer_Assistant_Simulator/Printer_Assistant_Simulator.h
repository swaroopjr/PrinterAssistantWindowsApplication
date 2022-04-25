// Printer_Assistant_Simulator.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPrinter_Assistant_SimulatorApp:
// See Printer_Assistant_Simulator.cpp for the implementation of this class
//

class CPrinter_Assistant_SimulatorApp : public CWinApp
{
public:
	CPrinter_Assistant_SimulatorApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPrinter_Assistant_SimulatorApp theApp;