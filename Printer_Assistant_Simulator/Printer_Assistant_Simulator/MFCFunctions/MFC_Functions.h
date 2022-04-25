#pragma once
#ifndef __MFC_FUNCTIONS_H__
#define  __MFC_FUNCTIONS_H__

void MFC_MemSet (unsigned char* pubStartAddress, unsigned char ubData, unsigned int ulSize);
void MFC_MemCpy (unsigned char* pubSourceAddress, unsigned char* pubDestinationAddress, unsigned int ulSize);
CString MFC_ConvertCharToCString (unsigned char* pubData);
void MFC_ConvertCStringToU8 (CString MFCInputString, unsigned char* pubOutputData, int ulLength);
bool MFC_Functions_Strcmp (unsigned char* pubString1, unsigned char* pubString2);
void MFC_Functions_Strcat (char * pbDest, unsigned int ulSize, const char * pbSource);
bool MFC_Functions_CheckIfStringExists (unsigned char *pubMainStringPtr, unsigned char *pubFindStringPtr, bool bCaseSensitive);
void MFC_Functions_AlignTextLeft (unsigned char *pubDestination, unsigned char *pubSource);
void MFC_Functions_TextRemoveSpaceAtEnd (unsigned char *pubString);
void MFC_Functions_Strcpy (unsigned char * pbDest, const char * pbSource);
void MFC_Functions_Sprintf (char * pbDest, const char * pbSource,unsigned int item1,unsigned int item2=NULL);
void MFC_Functions_ConvertDecimalValueToString (unsigned int ulDecimalValue, unsigned char *pubOutString, unsigned int ulNumberOfDigits);
#endif
