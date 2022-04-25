
#pragma once

char Serial_GetSerialChar (void);
bool Display8BitData (CEdit* PStaticText, unsigned char ubData, bool fHexOrDecimal, bool fAppend);
UINT SerialCommunicationThreadProc ( LPVOID pParam );