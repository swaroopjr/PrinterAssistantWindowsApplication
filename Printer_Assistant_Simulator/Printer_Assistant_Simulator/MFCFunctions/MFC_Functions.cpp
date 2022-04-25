/**
* @file         MFC_Functions.cpp
* @brief       This file contains common functions required to work on MFC. This file can directly be used in other projects.
*
* <b> Copyright 2012 by Samsung Electronics, Inc. All rights reserved.</b>
*
* This software is the confidential and proprietary information
* of Samsung Electronics, Inc. ("Confidential Information").\n
* You shall not disclose such Confidential Information and shall use
* it only in accordance with the terms of the license agreement
* you entered into with Samsung.
*
* @designer    Swaroop
* @imp           Swaroop
* @dept          SW Development 2 Group / Digital Printing Division
* @date          June 2012
* @version      V1.0
*/

#include "stdafx.h"
#include "MFC_Functions.h"

/**
* @fn		: MFC_MemSet
* @brief		: Sets memory with given data and size starting from given address
* @input		: Stating address, Data, size of memory till memory should be set
* @output	: none
*/
void MFC_MemSet (unsigned char* pubStartAddress, unsigned char ubData, unsigned int ulSize)
{
 	unsigned int ulIndex = 0;
	
	while (ulIndex != ulSize)
	{
		*(pubStartAddress + ulIndex++) = ubData;
	}
}

/**
* @fn		: MFC_MemCpy
* @brief		: Copies required size of data from source address to destination address
* @input		: Source address, Destination Address, Size
* @output	    : none
*/
void MFC_MemCpy (unsigned char* pubSourceAddress, unsigned char* pubDestinationAddress, unsigned int ulSize)
{
 	unsigned int ulIndex = 0;
	
	while (ulIndex != ulSize)
	{
		*(pubDestinationAddress + ulIndex) = *(pubSourceAddress + ulIndex);
		ulIndex++;
	}
}

/**
* @fn		: MFC_ConvertCharToCString
* @brief		: Converts char string to CString
* @input		: char string
* @output	: CString
*/
CString MFC_ConvertCharToCString (unsigned char* pubData)
{
	CString mfcReturnString,mfcTempLetter;
	CString mfcEndString('\0');
	unsigned int ulIndex = 0;

	mfcReturnString.Empty();
	mfcTempLetter.Empty();

	if (pubData == NULL)
	{
		return mfcReturnString;
	}

	while(pubData[ulIndex] != '\0')
	{
		mfcTempLetter.Format(_T("%c"),pubData[ulIndex]);
		mfcReturnString.Append(mfcTempLetter);
		ulIndex++;
	}

	mfcReturnString.Append(mfcEndString);
	return mfcReturnString;
}

/**
* @fn		: MFC_ConvertCStringToU8
* @brief		: Converts CString to char string
* @input		: Cstring
* @output	: char string
*/
#if 1
void MFC_ConvertCStringToU8 (CString MFCInputString, unsigned char* pubOutputData, int ulLength)
{
	int slIndex = 0;

	MFC_MemSet (pubOutputData, NULL, ulLength);

	for (slIndex = 0; slIndex < MFCInputString.GetLength(); slIndex++)
	{
		if (slIndex >= ulLength)
			break;

		pubOutputData [slIndex] = MFCInputString.GetAt (slIndex);
	}
}
#endif
/**
* @fn		: MFC_Functions_Strcmp
* @brief		: Compares two given strings
* @input		: strings to be compared
* @output	: TRUE if given strings are same. FALSE if given strings are not same
*/
bool MFC_Functions_Strcmp (unsigned char* pubString1, unsigned char* pubString2)
{
	unsigned int ulIndex = 0;

	if ((pubString1 == NULL) || (pubString2 == NULL))
	{
		return FALSE;
	}

	while ((pubString1[ulIndex] != '\0') && (pubString2[ulIndex] != '\0'))
	{
		if (pubString1[ulIndex] != pubString2[ulIndex])
		{
			return FALSE;
		}
		ulIndex ++;
	}
	
	return TRUE;
}

/**
* @fn		: MFC_Functions_Strcat
* @brief		: String cat
* @input		: Pointers to o/p string and i/p string
* @output	: none. New string will be copied in o/p address provided
*/
//void MFC_Functions_Strcat (U8 * pbDest, U32 ulSize, U8 * pbSource)
void MFC_Functions_Strcat (char * pbDest, unsigned int ulSize, const char * pbSource)
{
}

/**
* @fn		: MFC_Functions_Strcpy
* @brief		: String copy
* @input		: Pointers to o/p string and i/p string
* @output	: none. String will be copied in o/p address provided
*/
void MFC_Functions_Strcpy (unsigned char * pbDest, const char * pbSource)
{
}

/**
* @fn		: MFC_Functions_Sprintf
* @brief		: sprintf
* @input		: 
* @output	: none
*/
void MFC_Functions_Sprintf (char * pbDest, const char * pbSource,unsigned int item1,unsigned int item2)
{
}

/**
* @fn		: MFC_Functions_CheckIfStringExists
* @brief		: Finds a string in given string (like strstr)
* @input		: Main String, String to be found in main string, case sensitive flag
* @output	: TRUE / FALSE
*/
bool MFC_Functions_CheckIfStringExists (unsigned char *pubMainStringPtr, unsigned char *pubFindStringPtr, bool bCaseSensitive)
{
	unsigned int ulMainStringLength=0, ulFindStringLength=0, ulCount=0, ulIndex = 0;
	unsigned char ubMainChar = 0, ubFindChar = 0; //To avoid case sensitive

	ulMainStringLength = strlen ((const char *)pubMainStringPtr);
	ulFindStringLength = strlen ((const char *)pubFindStringPtr);

	for (ulIndex=0; ulIndex<ulMainStringLength-1; ulIndex++)
	{
		ubMainChar = pubMainStringPtr[ulIndex];
		ubFindChar = pubFindStringPtr[ulCount];
	
		if (!bCaseSensitive)
		{
			if ((pubMainStringPtr[ulIndex] >= 'A') && (pubMainStringPtr[ulIndex] <= 'Z'))
				ubMainChar = pubMainStringPtr[ulIndex] - 'A' + 'a';
			if ((pubFindStringPtr[ulCount] >= 'A') && (pubFindStringPtr[ulCount] <= 'Z'))
				ubFindChar = pubFindStringPtr[ulCount] - 'A' + 'a';		
		}

		if(ubMainChar == ubFindChar)
			ulCount++;
		else
			ulCount = 0;

		if (ulCount == ulFindStringLength ) //Find String Found in MainString
			return(TRUE);
	}
	return (FALSE);
}

/**
* @fn		: MFC_Functions_AlignTextLeft
* @brief		: Removes spaces at left
* @input		: destination pointer and source string
* @output	: none
*/
void MFC_Functions_AlignTextLeft (unsigned char *pubDestination, unsigned char *pubSource)
{
	unsigned int ulIndex1 = 0, ulIndex2 = 0;

	if (pubSource == NULL)
	{
		pubDestination = NULL;
		return;
	}

	
	while (pubSource[ulIndex1++] == ' '); /* Remove all spaces*/
	ulIndex1--;
	while (pubSource[ulIndex1] != NULL)
	{
		pubDestination[ulIndex2++] = pubSource[ulIndex1++];
	}
}

/**
* @fn		: MFC_Functions_TextRemoveSpaceAtEnd
* @brief		: Removes spaces at right
* @input		: destination pointer and source string
* @output	: none
*/
void MFC_Functions_TextRemoveSpaceAtEnd (unsigned char *pubString)
{
	unsigned int ulIndex = 0;

	if (pubString == NULL)
		return;

	ulIndex = strlen ((const char *) pubString) - 1;
	
	while (pubString[ulIndex] == ' ')  /* Remove all spaces*/
	{
		pubString[ulIndex] = NULL;

		if (ulIndex == 0)
			break;

		ulIndex --;
	}
}

/**
* @fn		: MFC_Functions_ConvertDecimalValueToString
* @brief		: 
* @input		: 
* @output	: none
*/
void MFC_Functions_ConvertDecimalValueToString (unsigned int ulDecimalValue, unsigned char *pubOutString, unsigned int ulNumberOfDigits)
{
/*	unsigned int ulIndex = 0;
	unsigned int ulDividend = 1;
	unsigned int i;

	for (i=0; i<ulNumberOfDigits-1; i++)
		ulDividend *= 10;
	
	pubOutString[ulNumberOfDigits] = NULL;

	while (ulDecimalValue)
	{
		pubOutString [-- ulNumberOfDigits] = '0' + (ulDecimalValue / ulDividend);
		ulDecimalValue = ulDecimalValue % ulDividend;
		ulDividend = ulDividend / 10;
	}
*/
	unsigned int ulIndex;

	pubOutString[ulNumberOfDigits] = NULL;

	for (ulIndex=1; ulIndex <= ulNumberOfDigits; ulIndex++)
	{
		pubOutString [ulNumberOfDigits - ulIndex] = '0' + (ulDecimalValue % 10);
		ulDecimalValue /= 10;
	}
}