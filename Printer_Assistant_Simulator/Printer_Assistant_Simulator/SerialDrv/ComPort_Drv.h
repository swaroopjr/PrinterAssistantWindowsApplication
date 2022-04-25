//////////////////////////////////////////////////////////////////////
// ComPort.h: interface for the CComPort class.
//////////////////////////////////////////////////////////////////////


class ComPortClass  
{
public:
	ComPortClass();
	virtual ~ComPortClass();
	BOOL Initialize(CString& sComPort, DWORD BaudRate);
	bool Read(char* pBuffer, DWORD BytesToRead, DWORD* RetBytesRead);
	bool Write(BYTE* pBuffer, size_t Count);	
	void Terminate();

protected:
	CString	m_sComPort;
	BOOL	m_bPortReady;
	HANDLE	m_hCom;
	DCB		m_dcb;
	COMMTIMEOUTS m_CommTimeouts;

};


