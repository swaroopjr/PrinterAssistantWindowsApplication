// Printer_Assistant_SimulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Printer_Assistant_Simulator.h"
#include "Printer_Assistant_SimulatorDlg.h"

//Additional Includes
#include "MFCFunctions\MFC_Functions.h"
#include "SerialDrv\SerialCommThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
////////////////// Extrn functions ///////////////////////////////////
extern UINT SerialCommunicationThreadProc(LPVOID pParam);
//extern void CopyCrumDataFromFileToBuffer(CString csPathName);
//////////////////////////////////////////////////////////////////////
////////////////// Global Variables //////////////////////////////////
extern CPrinter_Assistant_SimulatorDlg* pMainWindowClassPtr;

extern CString sComPortNumber;
extern DWORD GlobalWordBaudRate;
extern bool bComPortTaskFlag;
extern bool boolGlobalHexDisplayFlag;
extern BYTE aubSerialSendBuffer [512];

////////////////// Global Variables //////////////////////////////////
//////////////////////////////////////////////////////////////////////

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CPrinter_Assistant_SimulatorDlg dialog


CPrinter_Assistant_SimulatorDlg::CPrinter_Assistant_SimulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrinter_Assistant_SimulatorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPrinter_Assistant_SimulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPrinter_Assistant_SimulatorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_BN_CLICKED(ID_CONNECT_BUTTON, &CPrinter_Assistant_SimulatorDlg::OnBnClickedConnectButton)
	ON_BN_CLICKED(ID_DISCONNECT_BUTTON, &CPrinter_Assistant_SimulatorDlg::OnBnClickedDisconnectButton)
	ON_BN_CLICKED(IDC_BUTTON_SERIAL_CLEAN, &CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonSerialClean)
	ON_BN_CLICKED(IDC_CHECK_HEX_FLAG, &CPrinter_Assistant_SimulatorDlg::OnBnClickedCheckHexFlag)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonBrowse)

	ON_BN_CLICKED(IDC_BUTTON_PAPER_EMPTY, &CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonProcessPaperEmpty)
	ON_BN_CLICKED(IDC_BUTTON_CHECK_PRINT_ENABLED, &CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonCheckIfPrintEnabled)
	ON_BN_CLICKED(IDC_BUTTON_VERY_HIGH_DOTCOUNT_PRINT, &CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonVeryHighDotCountPrint)
	ON_BN_CLICKED(IDC_BUTTON_HIGH_DOTCOUNT_PRINT, &CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonHighDotCountPrint)
	ON_BN_CLICKED(IDC_BUTTON_MEDIUM_DOTCOUNT_PRINT, &CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonMediumDotCountPrint)
	ON_BN_CLICKED(IDC_BUTTON_LOW_DOTCOUNT_PRINT, &CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonLowDotCountPrint)
	ON_BN_CLICKED(IDC_BUTTON_VERY_LOW_DOTCOUNT_PRINT, &CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonVeryLowDotCountPrint)	

	ON_BN_CLICKED(ID_WRITE_USERDATA_BUTTON, &CPrinter_Assistant_SimulatorDlg::OnBnClickedWriteUserdataButton)
	ON_BN_CLICKED(ID_READ_USERDATA_BUTTON, &CPrinter_Assistant_SimulatorDlg::OnBnClickedReadUserdataButton)
	ON_BN_CLICKED(ID_WRITE_SYSTEMDATA_BUTTON, &CPrinter_Assistant_SimulatorDlg::OnBnClickedWriteSystemdataButton)
	ON_BN_CLICKED(ID_DISPLAY_USERDATA_BUTTON, &CPrinter_Assistant_SimulatorDlg::OnBnClickedDisplayUserdataButton)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_STATUS_DISPLAY, &CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonPrintStatusDisplay)
	ON_BN_CLICKED(ID_READ_SYSTEMDATA_BUTTON, &CPrinter_Assistant_SimulatorDlg::OnBnClickedReadSystemdataButton)
	ON_BN_CLICKED(ID_DISPLAY_SYSTEMDATA_BUTTON, &CPrinter_Assistant_SimulatorDlg::OnBnClickedDisplaySystemdataButton)
END_MESSAGE_MAP()


// CPrinter_Assistant_SimulatorDlg message handlers

BOOL CPrinter_Assistant_SimulatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CPrinter_Assistant_SimulatorDlg::ManualDialogInit();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPrinter_Assistant_SimulatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPrinter_Assistant_SimulatorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPrinter_Assistant_SimulatorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/////////////////////////////////////////////////////////////////////////////
BOOL CPrinter_Assistant_SimulatorDlg::ManualDialogInit()
{
	CString mfcString;

	//Add com port strings in drop down list
	mfcString = MFC_ConvertCharToCString((unsigned char *)("COM0"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->InsertString(0, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("COM1"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->InsertString(1, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("COM2"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->InsertString(2, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("COM3"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->InsertString(3, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("COM4"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->InsertString(4, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("COM5"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->InsertString(5, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("COM6"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->InsertString(6, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("COM7"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->InsertString(7, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("COM8"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->InsertString(8, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("COM9"));
	((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->SetCurSel(5);

	//Add baud rate strings in drop down list
	mfcString = MFC_ConvertCharToCString((unsigned char *)("9600"));
	((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->InsertString(0, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("38400"));
	((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->InsertString(1, mfcString);
	mfcString = MFC_ConvertCharToCString((unsigned char *)("115200"));
	((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->InsertString(2, mfcString);
	((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->SetCurSel(0);

	//Set default button status		
	pMainWindowClassPtr->GetDlgItem(ID_DISCONNECT_BUTTON)->EnableWindow(FALSE);

	//Set default hex flag check	
	((CButton*)GetDlgItem(IDC_CHECK_HEX_FLAG))->SetCheck(TRUE);
	boolGlobalHexDisplayFlag = TRUE;
	
	//AfxBeginThread(SerialCommunicationThreadProc, ((CStatic*)pMainWindowClassPtr), THREAD_PRIORITY_HIGHEST, 0, 0, NULL);

	CString csFilePath = MFC_ConvertCharToCString((unsigned char *)("Data.txt"));
	//CopyCrumDataFromFileToBuffer(csFilePath); //Load default file contents
	return TRUE;
}

void CPrinter_Assistant_SimulatorDlg::OnSerialComPortConnected()
{
	pMainWindowClassPtr->GetDlgItem(ID_CONNECT_BUTTON)->EnableWindow(FALSE);
	pMainWindowClassPtr->GetDlgItem(ID_DISCONNECT_BUTTON)->EnableWindow(TRUE);

	((CComboBox*)pMainWindowClassPtr->GetDlgItem(IDC_COMBO_COMPORT))->EnableWindow(FALSE);
	((CComboBox*)pMainWindowClassPtr->GetDlgItem(IDC_COMBO_BAUDRATE))->EnableWindow(FALSE);
	bComPortTaskFlag = TRUE;
}
/////////////////////////////////////////////////////////////////////////////
void CPrinter_Assistant_SimulatorDlg::OnSerialComPortDisConnected()
{
	pMainWindowClassPtr->GetDlgItem(ID_CONNECT_BUTTON)->EnableWindow(TRUE);
	pMainWindowClassPtr->GetDlgItem(ID_DISCONNECT_BUTTON)->EnableWindow(FALSE);

	((CComboBox*)pMainWindowClassPtr->GetDlgItem(IDC_COMBO_COMPORT))->EnableWindow(TRUE);
	((CComboBox*)pMainWindowClassPtr->GetDlgItem(IDC_COMBO_BAUDRATE))->EnableWindow(TRUE);
	bComPortTaskFlag = FALSE;
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedConnectButton()
{
	int IntLocalIndex;
	bool boolLocalFlag = TRUE;

	if (bComPortTaskFlag)
		return;

	IntLocalIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_COMPORT))->GetCurSel();
	switch (IntLocalIndex)
	{
	case 0:
		sComPortNumber = "Com0";
		break;
	case 1:
		sComPortNumber = "Com1";
		break;
	case 2:
		sComPortNumber = "Com2";
		break;
	case 3:
		sComPortNumber = "Com3";
		break;
	case 4:
		sComPortNumber = "Com4";
		break;
	case 5:
		sComPortNumber = "Com5";
		break;
	case 6:
		sComPortNumber = "Com6";
		break;
	case 7:
		sComPortNumber = "Com7";
		break;
	case 8:
		sComPortNumber = "Com8";
		break;
	default:
		sComPortNumber = "ERROR";
		boolLocalFlag = FALSE;
		break;
	}

	IntLocalIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE))->GetCurSel();
	switch (IntLocalIndex)
	{
	case 0:
		GlobalWordBaudRate = 9600;
		break;
	case 1:
		GlobalWordBaudRate = 38400;
		break;
	case 2:
		GlobalWordBaudRate = 115200;
		break;
	default:
		GlobalWordBaudRate = 0;
		boolLocalFlag = FALSE;
		break;
	}

	if (boolLocalFlag)
	{
		// Invoke Task for serial communication
		AfxBeginThread(SerialCommunicationThreadProc, ((CStatic*)pMainWindowClassPtr), THREAD_PRIORITY_HIGHEST, 0, 0, NULL);
	}
	else
	{
		// Terminate Task for serial communication
		bComPortTaskFlag = FALSE;

		TCHAR str[] = _TEXT("Please check Baudrate / Comport");
		AfxMessageBox(str);
	}

	return;
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedDisconnectButton()
{
	// Terminate Task for serial communication
	bComPortTaskFlag = FALSE;
}


void CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonSerialClean()
{
	char * pBuffer = NULL;
	CString str;

	str = pBuffer;
	((CStatic*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_SERIAL_DATA))->SetWindowText(str);
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedCheckHexFlag()
{
	if (boolGlobalHexDisplayFlag)
		boolGlobalHexDisplayFlag = FALSE;
	else
		boolGlobalHexDisplayFlag = TRUE;
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonBrowse()
{
	char strFilter[] = { "Text Files (*.txt)|*.txt|" };
	CFileDialog FileDlg(FALSE, CString(".txt"), NULL, 0, CString(strFilter));
	if (FileDlg.DoModal() == IDOK)
	{
		CString csFilePath = FileDlg.GetPathName();
		//CopyCrumDataFromFileToBuffer(csFilePath);
	}
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonProcessPaperEmpty()
{
	unsigned int ulIndex = 2;
	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = '1';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';	

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;	
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonCheckIfPrintEnabled()
{
	unsigned int ulIndex = 2;
	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = '2';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';	

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;	
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonPrintStatusDisplay()
{
	unsigned char ubPrintEnabled;
	bool fValidPacketReceived = FALSE;

	if ('[' == Serial_GetSerialChar())
	{
		if ('{' == Serial_GetSerialChar())
		{
			if ('<' == Serial_GetSerialChar())
			{
				ubPrintEnabled = Serial_GetSerialChar();

				if ('>' == Serial_GetSerialChar())
				{
					if ('}' == Serial_GetSerialChar())
					{
						if (']' == Serial_GetSerialChar())
						{
							fValidPacketReceived = TRUE;
						}
					}
				}
			}
		}
	}

	if (fValidPacketReceived)
	{
		CString str;

		if (ubPrintEnabled == 1)
			str = "Print Enabled";
		else
			str = "Print Disabled";

		((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_PRINT_ENABLED_STATUS))->SetWindowText(str);
	}
	else
	{
		CString str;
		str = "error";
		((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_PRINT_ENABLED_STATUS))->SetWindowText(str);
	}
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonVeryHighDotCountPrint()
{
	unsigned int ulIndex = 2;
	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = '3';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';	

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;	
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonHighDotCountPrint()
{
	unsigned int ulIndex = 2;
	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = '4';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';	

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;	
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonMediumDotCountPrint()
{
	unsigned int ulIndex = 2;
	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = '5';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';	

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;	
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonLowDotCountPrint()
{
	unsigned int ulIndex = 2;
	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = '6';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';	

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;	
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedButtonVeryLowDotCountPrint()
{
	unsigned int ulIndex = 2;
	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = '7';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';	

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;	
}

unsigned char GetCostDataFromEditor (CEdit* PStaticText)
{
	CString Str;
	unsigned char aubUserData[8];
	unsigned char ubCharValue = 0;
	unsigned int ulMultiplier = 1;

	PStaticText->GetWindowText(Str);
	memset (aubUserData, NULL, sizeof(aubUserData));
	MFC_ConvertCStringToU8 (Str, aubUserData,  Str.GetLength());

	if ((aubUserData[1] >= '0') && (aubUserData[1] <= '9'))
	{
		ubCharValue += (1  * (aubUserData[1] - '0'));
		ulMultiplier *= 10;
	}

	if ((aubUserData[0] >= '0') && (aubUserData[0] <= '9'))
		ubCharValue += (ulMultiplier * (aubUserData[0] - '0'));

	return ubCharValue;
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedWriteUserdataButton()
{
	unsigned int ulIndex = 2;
	CString Str;
	unsigned char aubUserData [16+1];
	unsigned int ulUserBalanceMSB = 0;
	unsigned int ulUserBalanceLSB = 0;
	int i;

	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = '8';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';

	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_USERNAME))->GetWindowText(Str);
	memset (aubUserData, NULL, sizeof(aubUserData));
	MFC_ConvertCStringToU8 (Str, aubUserData,  Str.GetLength());
	for (i=0; i<16; i++)
	{
		aubSerialSendBuffer[ulIndex ++] = aubUserData[i];
	}

	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_USERBALANCE_MSB));
	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_USERBALANCE_LSB));

	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedReadUserdataButton()
{
	unsigned int ulIndex = 2;

	//Send Read command to the board
	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = 'A';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedDisplayUserdataButton()
{
	bool fValidPacketReceived = FALSE;
	unsigned int i;
	unsigned char aubUserName[16+1];
	unsigned char ubMSBBalance, ubLSBBalance;

	//Check Serial Buffer for user data
	memset (aubUserName, NULL, sizeof(aubUserName));

	if ('[' == Serial_GetSerialChar())
	{
		if ('{' == Serial_GetSerialChar())
		{
			if ('<' == Serial_GetSerialChar())
			{
				for (i=0; i<sizeof(aubUserName)-1 ; i++)
					aubUserName[i] = Serial_GetSerialChar();

				ubMSBBalance = Serial_GetSerialChar();
				ubLSBBalance = Serial_GetSerialChar();

				if ('>' == Serial_GetSerialChar())
				{
					if ('}' == Serial_GetSerialChar())
					{
						if (']' == Serial_GetSerialChar())
						{
							fValidPacketReceived = TRUE;
						}
					}
				}
			}
		}
	}

	if (fValidPacketReceived)
	{
		CString str;
		str = aubUserName;
		((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_USERNAME))->SetWindowText(str);

		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_USERBALANCE_MSB)), (char)(ubMSBBalance), FALSE /*Decimal*/, FALSE /*Do not append*/);
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_USERBALANCE_LSB)), (char)(ubLSBBalance), FALSE /*Decimal*/, FALSE /*Do not append*/);
	}
	else
	{
		CString str;
		str = "error";
		((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_USERNAME))->SetWindowText(str);
	}
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedWriteSystemdataButton()
{
	unsigned int ulIndex = 2;
	unsigned int ulUserBalanceMSB = 0;
	unsigned int ulUserBalanceLSB = 0;
	//int i;

	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = '9';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';

	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_VERY_HIGH_DOT_MSB));
	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_VERY_HIGH_DOT_LSB));
	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_HIGH_DOT_MSB));
	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_HIGH_DOT_LSB));
	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_MEDIUM_DOT_MSB));
	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_MEDIUM_DOT_LSB));
	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_LOW_DOT_MSB));
	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_LOW_DOT_LSB));
	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_VERY_LOW_DOT_MSB));
	aubSerialSendBuffer[ulIndex ++] = GetCostDataFromEditor((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_VERY_LOW_DOT_LSB));

	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;
}


void CPrinter_Assistant_SimulatorDlg::OnBnClickedReadSystemdataButton()
{
	unsigned int ulIndex = 2;

	//Send Read command to the board
	aubSerialSendBuffer[ulIndex ++] = '[';
	aubSerialSendBuffer[ulIndex ++] = '{';
	aubSerialSendBuffer[ulIndex ++] = '<';	

	aubSerialSendBuffer[ulIndex ++] = 'B';	
	
	aubSerialSendBuffer[ulIndex ++] = '>';
	aubSerialSendBuffer[ulIndex ++] = '}';
	aubSerialSendBuffer[ulIndex ++] = ']';

	aubSerialSendBuffer[0] = ulIndex - 1;	
	aubSerialSendBuffer[1] = 0;
}

void CPrinter_Assistant_SimulatorDlg::OnBnClickedDisplaySystemdataButton()
{
	bool fValidPacketReceived = FALSE;
	unsigned char ubVeryHighDotMSB, ubVeryHighDotLSB;
	unsigned char ubHighDotMSB, ubHighDotLSB;
	unsigned char ubMediumDotMSB, ubMediumDotLSB;
	unsigned char ubLowDotMSB, ubLowDotLSB;
	unsigned char ubVeryLowDotMSB, ubVeryLowDotLSB;

	//Check Serial Buffer for System data
	if ('[' == Serial_GetSerialChar())
	{
		if ('{' == Serial_GetSerialChar())
		{
			if ('<' == Serial_GetSerialChar())
			{
				ubVeryHighDotMSB = Serial_GetSerialChar();
				ubVeryHighDotLSB = Serial_GetSerialChar();
				ubHighDotMSB     = Serial_GetSerialChar();
				ubHighDotLSB     = Serial_GetSerialChar();
				ubMediumDotMSB   = Serial_GetSerialChar();
				ubMediumDotLSB   = Serial_GetSerialChar();
				ubLowDotMSB      = Serial_GetSerialChar();
				ubLowDotLSB      = Serial_GetSerialChar();
				ubVeryLowDotMSB  = Serial_GetSerialChar();
				ubVeryLowDotLSB  = Serial_GetSerialChar();

				if ('>' == Serial_GetSerialChar())
				{
					if ('}' == Serial_GetSerialChar())
					{
						if (']' == Serial_GetSerialChar())
						{
							fValidPacketReceived = TRUE;
						}
					}
				}
			}
		}
	}

	if (fValidPacketReceived)
	{
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_VERY_HIGH_DOT_MSB)), (char)(ubVeryHighDotMSB), FALSE /*Decimal*/, FALSE /*Do not append*/);
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_VERY_HIGH_DOT_LSB)), (char)(ubVeryHighDotLSB), FALSE /*Decimal*/, FALSE /*Do not append*/);
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_HIGH_DOT_MSB)),      (char)(ubHighDotMSB),     FALSE /*Decimal*/, FALSE /*Do not append*/);
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_HIGH_DOT_LSB)),      (char)(ubHighDotLSB),     FALSE /*Decimal*/, FALSE /*Do not append*/);
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_MEDIUM_DOT_MSB)),    (char)(ubMediumDotMSB),   FALSE /*Decimal*/, FALSE /*Do not append*/);
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_MEDIUM_DOT_LSB)),    (char)(ubMediumDotLSB),   FALSE /*Decimal*/, FALSE /*Do not append*/);
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_LOW_DOT_MSB)),       (char)(ubLowDotMSB),      FALSE /*Decimal*/, FALSE /*Do not append*/);
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_LOW_DOT_LSB)),       (char)(ubLowDotLSB),      FALSE /*Decimal*/, FALSE /*Do not append*/);
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_VERY_LOW_DOT_MSB)),  (char)(ubVeryLowDotMSB),  FALSE /*Decimal*/, FALSE /*Do not append*/);
		Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_VERY_LOW_DOT_LSB)),  (char)(ubVeryLowDotLSB),  FALSE /*Decimal*/, FALSE /*Do not append*/);
	}
	else
	{
		CString str;
		str = "error";
		((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_VERY_HIGH_DOT_MSB))->SetWindowText(str);
	}
}
