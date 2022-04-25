//////////////////////////////////////////////////////////////////////
// This sample was developed using Visual C++ V5.0 with MFC.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include "ComPort_Drv.h"

ComPortClass::ComPortClass()
{
	m_bPortReady = FALSE;
	m_hCom = INVALID_HANDLE_VALUE;
}

ComPortClass::~ComPortClass()
{

}

BOOL ComPortClass::Initialize(CString& sComPort, DWORD BaudRate)
{
	DWORD	dwError;

	m_bPortReady = TRUE;
	m_sComPort = sComPort;

	m_hCom = CreateFile(m_sComPort, 
		GENERIC_READ | GENERIC_WRITE,
		0, // exclusive access
		NULL, // no security
		OPEN_EXISTING,
		0, // no overlapped I/O
		NULL); // null template

	if (m_hCom == INVALID_HANDLE_VALUE)
	{
		m_bPortReady = FALSE;
		dwError = GetLastError();
		return FALSE;
	}

	if (m_bPortReady)
	{
		m_bPortReady = SetupComm(m_hCom, 128, 128); // set buffer sizes
		if (!m_bPortReady)
		{
			dwError = GetLastError();
			return FALSE;
		}
	} 

	//GetCommState to fill in its default values, override the values that you want to change and then call SetCommState to set the values
	if (m_bPortReady)
	{
		m_bPortReady = GetCommState(m_hCom, &m_dcb);
		if (!m_bPortReady)
		{
			dwError = GetLastError();
			return FALSE;
		}

		m_dcb.BaudRate = BaudRate; //38400; //115200; //BaudRate;
		m_dcb.ByteSize = 8;
		m_dcb.Parity = NOPARITY;
		m_dcb.StopBits = ONESTOPBIT;
		
		//m_dcb.fAbortOnError = TRUE;
		m_dcb.fAbortOnError = FALSE;

		m_dcb.fDtrControl = DTR_CONTROL_DISABLE ;
		m_dcb.fOutxDsrFlow = FALSE;

		m_dcb.fRtsControl = RTS_CONTROL_DISABLE ;
		m_dcb.fOutxCtsFlow  = FALSE;

		
		m_bPortReady = SetCommState(m_hCom, &m_dcb);
		if (!m_bPortReady)
		{
			dwError = GetLastError();
			return FALSE;
		}
	}

#if 1 //Timeout setting (optional)
	if (m_bPortReady)
	{
		m_bPortReady = GetCommTimeouts (m_hCom, &m_CommTimeouts);
		if (!m_bPortReady)
		{
			dwError = GetLastError();
			return FALSE;
		}

		m_CommTimeouts.ReadIntervalTimeout = 50;
		m_CommTimeouts.ReadTotalTimeoutConstant = 50;
		m_CommTimeouts.ReadTotalTimeoutMultiplier = 10;
		m_CommTimeouts.WriteTotalTimeoutConstant = 50;
		m_CommTimeouts.WriteTotalTimeoutMultiplier = 10;
		m_bPortReady = SetCommTimeouts (m_hCom, &m_CommTimeouts);
		if (!m_bPortReady)
		{
			dwError = GetLastError();
			return FALSE;
		}
	}
#endif

	return TRUE;
}

/********************************************************************************
BytesToRead has to be less than size of sBuffer (128).
pBuffer has to be allocated with sufficient space before calling and should be less than size of sBuffer (128).
RetBytesRead: will have bytes read sucussesfully.
********************************************************************************/
bool ComPortClass::Read(char* pBuffer, DWORD BytesToRead, DWORD* RetBytesRead)
{
	BOOL	bReadRC;
	DWORD	dwBytesRead;
	//DWORD	dwError;
	char	sBuffer[128];

	if(m_hCom == INVALID_HANDLE_VALUE)
		return FALSE;

	memset(sBuffer,0,sizeof(sBuffer));
	bReadRC = ReadFile(m_hCom, &sBuffer, BytesToRead, &dwBytesRead, NULL);
	*RetBytesRead = dwBytesRead;

	if (bReadRC && (dwBytesRead > 0))
	{
		memcpy(pBuffer, sBuffer, dwBytesRead);
		return TRUE;
	}

	return FALSE;
}

bool ComPortClass::Write(BYTE* pBuffer, size_t Count)
{
	BOOL	bWriteRC;
	DWORD	iBytesWritten; 
	DWORD	dwError;
	//char	sBuffer[128];

	if(m_hCom == INVALID_HANDLE_VALUE)
		return FALSE;	
#if 0
	iBytesWritten = 0;
	bWriteRC = WriteFile(m_hCom, (LPCVOID)pBuffer,(DWORD)(Count), &iBytesWritten, NULL);
	if ((!bWriteRC) || (iBytesWritten != Count))
	{
		dwError = GetLastError();
		return FALSE;
	}
#else
	for (int i=0; i<(int)Count; i++)
	{
		iBytesWritten = 0;
		bWriteRC = WriteFile(m_hCom, (LPCVOID)(&pBuffer[i]),(DWORD)(1), &iBytesWritten, NULL);
		if ((!bWriteRC) || (iBytesWritten != 1))
		{
			dwError = GetLastError();
			return FALSE;
		}
		Sleep (10);
	}
#endif
	return TRUE;
}

void ComPortClass::Terminate()
{
	CloseHandle(m_hCom);
	m_hCom = INVALID_HANDLE_VALUE;
}

