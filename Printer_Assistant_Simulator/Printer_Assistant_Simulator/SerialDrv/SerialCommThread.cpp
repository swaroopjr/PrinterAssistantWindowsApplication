
#include "stdafx.h"
//#include "stdlib.h" // There is no problem if you include this file. Might be required for itoa
#include "SerialCommThread.h"
#include "ComPort_Drv.h"
#include <time.h>
#include "..\Resource.h"
#include "..\Printer_Assistant_Simulator.h"
#include "..\Printer_Assistant_SimulatorDlg.h"
#include "..\MFCFunctions\MFC_Functions.h"

//#include "..\A1000_Commands\A1000_Commands.h" //Application specific include file

/////////////////////////////////////////////////////////////////////////
////////////////// Global Variables /////////////////////////////////////
extern CPrinter_Assistant_SimulatorDlg* pMainWindowClassPtr;
BYTE aubSerialSendBuffer [512];
CString sComPortNumber;
DWORD GlobalWordBaudRate;
bool bComPortTaskFlag;
bool boolGlobalHexDisplayFlag = FALSE;

////////////////// Global Variables /////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#if 0 //For debuging
void CollectSerialReceiveData(char ubData)
{
	static char aubSerialReceiveDataPacket[128];
	static int ulIndex = 0;

	aubSerialReceiveDataPacket[ulIndex] = ubData;
	ulIndex++;

	if (ulIndex >= 64)
	{
		CString str;
		str = aubSerialReceiveDataPacket;
		((CStatic*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_SERIAL_DATA))->SetWindowText(str);

		memset(aubSerialReceiveDataPacket, NULL, sizeof(aubSerialReceiveDataPacket));
		ulIndex = 0;
	}
}
/////////////////////////////////////////////////////////////////////////
#else
/////////////////////////////////////////////////////////////////////////
volatile unsigned char gaubSerial_Buf[64];
volatile unsigned int gaulSerial_Head = 0;
volatile unsigned int gaulSerial_Tail = 0;

char Serial_GetSerialChar (void) 				// Read character from Serial Port   
{
    unsigned long long ulCount = 0;
    unsigned char ubCh = 0xFF;
	unsigned long long ulCountLimit = 10;
    
    if (gaulSerial_Tail >= sizeof (gaubSerial_Buf))
    {
        gaulSerial_Tail = 0; 
    }	
    
    while (1)
    {
        if (gaulSerial_Head != gaulSerial_Tail)
        {
            ubCh = gaubSerial_Buf[gaulSerial_Tail ++];
            break;
        }
        
        if (ulCount > ulCountLimit)
        {
            ubCh = 0xFF;
            break;
        }
        
		Sleep(100);
        ulCount ++;
    }
    
    return (ubCh);
}
void CollectSerialReceiveData(char ubData)
{
	gaubSerial_Buf[gaulSerial_Head ++] = ubData;

	if (gaulSerial_Head >= sizeof (gaubSerial_Buf))
		gaulSerial_Head = 0; 
}
#endif
/////////////////////////////////////////////////////////////////////////////
bool Display8BitData (CEdit* PStaticText, unsigned char ubData, bool fHexOrDecimal, bool fAppend)
{
	char TempCharBuf[8];
	memset (TempCharBuf, NULL, sizeof(TempCharBuf));

	if (fHexOrDecimal == TRUE) //Hex
		_itoa_s ((int)(ubData), TempCharBuf, 16);
	else //Decimal
		MFC_Functions_ConvertDecimalValueToString ((int)(ubData), (unsigned char *)TempCharBuf, 2);

#if 0
	if ((TempCharBuf[0] == NULL) || (TempCharBuf[2] != NULL))
	{
		TCHAR str[] = _TEXT("itoa error");
		AfxMessageBox(str);
		return FALSE;
	}
#endif
	if (TempCharBuf[1] == NULL)
	{
		TempCharBuf[2] = NULL;
		TempCharBuf[1] = TempCharBuf[0] ;
		TempCharBuf[0] = '0';
	}

	strcat_s (TempCharBuf, sizeof (TempCharBuf), " ");
	CString str;
	if (fAppend == TRUE)
	{
		PStaticText->GetWindowText(str);
		str += (TempCharBuf);
	}
	else
	{
		str = TempCharBuf;
	}
	PStaticText->SetWindowText(str);

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
void AddSendHeader(CEdit* PStaticText)
{
	CString str;
	PStaticText->GetWindowText(str);
	str += ("\r\nSend=>");
	PStaticText->SetWindowText(str);
}
/////////////////////////////////////////////////////////////////////////////
UINT SerialCommunicationThreadProc ( LPVOID pParam )
{
	ComPortClass ComPortObj;
	unsigned char ubRxData;
	DWORD BytesRead;
	unsigned int ulSerialDataLenthToSend;

	//sComPortNumber = "Com5";
	//GlobalWordBaudRate = 115200;

	if (sComPortNumber == "ERROR")
	{
		TCHAR str[] = _TEXT("TASK: COM Port not supported");
		AfxMessageBox(str);
		return 0;
	}

	if ((GlobalWordBaudRate != 9600) && (GlobalWordBaudRate != 38400) && (GlobalWordBaudRate != 115200))
	{
		TCHAR str[] = _TEXT("TASK: COM Port wrong baud rate");
		AfxMessageBox(str);
		return 0;
	}

	if (FALSE == ComPortObj.Initialize(sComPortNumber, GlobalWordBaudRate))
	{
		TCHAR str[] = _TEXT("Unable to open COM port");  
		AfxMessageBox(str);
		return 0;
	}

	pMainWindowClassPtr->OnSerialComPortConnected();

	while (bComPortTaskFlag)
	{
		if (aubSerialSendBuffer[0] != 0x00)
		{
		        ulSerialDataLenthToSend = (aubSerialSendBuffer[1] << 8) + aubSerialSendBuffer[0];

				//Display Send Data
				if (boolGlobalHexDisplayFlag)
				{
					AddSendHeader ((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_SERIAL_DATA));
					for (unsigned int i = 2; i <= ulSerialDataLenthToSend; i++)
						Display8BitData(((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_SERIAL_DATA)), (char)(aubSerialSendBuffer[i]), TRUE /*Hex*/, TRUE/*Append*/);
				}
				
				if(FALSE == ComPortObj.Write(&(aubSerialSendBuffer[2]), ulSerialDataLenthToSend))
				{
					TCHAR str[] = _TEXT("Unable to send data");  
					AfxMessageBox(str);
				}

				memset (aubSerialSendBuffer, NULL, sizeof(aubSerialSendBuffer));
		}

		if (ComPortObj.Read((char *)(&ubRxData), 1, &BytesRead)) //Read just one byte
		{
			if (1) //(ubRxData != 0) //This is not required. Read will give data only when there is valid Data in uart channel
			{
				if (boolGlobalHexDisplayFlag)
				{
					Display8BitData((CEdit*)pMainWindowClassPtr->GetDlgItem(IDC_EDIT_SERIAL_DATA), ubRxData, TRUE /*Hex*/, TRUE/*Append*/);
				}
				CollectSerialReceiveData(ubRxData);
			}
		}
	}

	ComPortObj.Terminate();
	pMainWindowClassPtr->OnSerialComPortDisConnected();
	return 0;   // thread completed successfully
}
/////////////////////////////////////////////////////////////////////////////
