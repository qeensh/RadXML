#include "common/com_radxml_strconv.h"

namespace RadXmlCommon
{
	std::vector<std::string> s13StringSplit (std::string strString, std::string strDelim)
	{
		return (s13SplitString (strString, strDelim));
	}

	std::vector<std::string> s13SplitString (std::string strString, std::string strDelim)
	{
		std::vector<std::string> aryReturn;
		int iStartPos = 0;
		int iPos = 0;

		while (1)
		{
			iPos = strString.find (strDelim, iStartPos);

			if (iPos == -1)
			{
				if (iStartPos < (int)strString.size ())
					aryReturn.push_back (strString.substr (iStartPos, strString.size ()));

				break;
			}

			if (iStartPos == 0)
				aryReturn.push_back (strString.substr (iStartPos, iPos));
			else
				aryReturn.push_back (strString.substr (iStartPos, (iPos - iStartPos)));

			iStartPos = (iPos + 1);
		}

		return (aryReturn);
	}

	std::vector<std::string> s13SplitString_WithExceptions (std::string strString, std::string strDelim, 
				std::vector<int> aryExceptions_Begin, std::vector<int> aryExceptions_End)
	{
		std::vector<std::string> aryReturn;
		int iStartPos = 0;
		int iPos = 0;

		while (1)
		{
			iPos = strString.find (strDelim, iStartPos);

			if (iPos == -1)
			{
				if (iStartPos < (int)strString.size ())
					aryReturn.push_back (strString.substr (iStartPos, strString.size ()));

				break;
			}

			int iSkipStart = -1;
			int iSkipEnd = -1;
			bool bSkip = false;

			while (1)
			{
				for (int iJdx = 0; iJdx < (int)aryExceptions_Begin.size (); iJdx++)
				{
					int iBegin = aryExceptions_Begin.at (iJdx);
					int iEnd = aryExceptions_End.at (iJdx);

					if ((iPos >= iBegin) && (iPos <= iEnd))
					{
						bSkip = true;
						iSkipStart = iStartPos;
						iSkipEnd = iEnd;

						break;
					}
				}

				if (bSkip == true)
				{
					iPos = strString.find (strDelim, (iSkipEnd + 1));

					if (iPos == -1)
						iPos = strString.size ();

					bSkip = false;
				}
				else
					break;
			}

			if (bSkip == false)
			{
				if (iStartPos == 0)
					aryReturn.push_back (strString.substr (iStartPos, iPos));
				else
					aryReturn.push_back (strString.substr (iStartPos, (iPos - iStartPos)));
			}
			else
				aryReturn.push_back (strString.substr (iSkipStart, (iPos - iSkipStart)));

			iStartPos = (iPos + 1);
		}

		return (aryReturn);
	}

	std::string s13RemoveWhitespaces (std::string strString)
	{
		std::string strTemp = strString;

		for (unsigned int iIdx = 0; iIdx < strTemp.size (); iIdx++)
		{
			if (strTemp.at (iIdx) == ' ')
			{
				strTemp = strTemp.erase (iIdx, 1);
				continue;
			}

			if (strTemp.at (iIdx) == '\t')
			{
				strTemp = strTemp.erase (iIdx, 1);
				continue;
			}
		}

		return (strTemp);
	}

	std::string s13ASCII_StringToLower (std::string strString)
	{
		std::string strTemp = strString;

		for (int iIdx = 0; iIdx < (int)strTemp.size (); iIdx++)
		{
			int iChar = (int)strTemp.at (iIdx);

			if ((iChar >= 65) && (iChar <= 90))
				strTemp.at (iIdx) += 32;
		}

		return (strTemp);
	}

	void s13ASCII_StringToLowerRef (std::string &strString)
	{
		for (int iIdx = 0; iIdx < (int)strString.size (); iIdx++)
		{
			int iChar = (int)strString.at (iIdx);

			if ((iChar >= 65) && (iChar <= 90))
				strString.at (iIdx) += 32;
		}
	}

	std::string s13ASCII_StringToUpper (std::string strString)
	{
		std::string strTemp = strString;

		for (int iIdx = 0; iIdx < (int)strTemp.size (); iIdx++)
		{
			int iChar = (int)strTemp.at (iIdx);

			if ((iChar >= 97) && (iChar <= 122))
				strTemp.at (iIdx) -= 32;
		}

		return (strTemp);
	}

	void s13ASCII_StringToUpperRef (std::string &strString)
	{
		for (int iIdx = 0; iIdx < (int)strString.size (); iIdx++)
		{
			int iChar = (int)strString.at (iIdx);

			if ((iChar >= 97) && (iChar <= 122))
				strString.at (iIdx) -= 32;
		}
	}

	int s13StringToInt (std::string strString)
	{
		std::istringstream ssStream (strString);
		int iReturn = 0;
		ssStream >> iReturn;

		return (iReturn);
	}

	std::string s13IntToString (int iInteger)
	{
		std::stringstream ssStream ("");
		ssStream << iInteger;

		return (ssStream.str ());
	}

	bool s13IsInt (std::string strString)
	{
		std::istringstream ssStream (strString);
		int iReturn = 0;
		ssStream >> iReturn;

		if (ssStream.fail () == true)
			return (false);

		return (true);
	}

	long s13StringToLong (std::string strString)
	{
		std::istringstream ssStream (strString);
		long lReturn = 0;
		ssStream >> lReturn;

		return (lReturn);
	}

	std::string s13LongToString (long lInteger)
	{
		std::stringstream ssStream ("");
		ssStream << lInteger;

		return (ssStream.str ());
	}

	unsigned long s13StringToUnsignedLong (std::string strString)
	{
		std::istringstream ssStream (strString);
		unsigned long ulReturn = 0;
		ssStream >> ulReturn;

		return (ulReturn);
	}

	std::string s13UnsignedLongToString (unsigned long ulInteger)
	{
		std::stringstream ssStream ("");
		ssStream << ulInteger;

		return (ssStream.str ());
	}

	double s13StringToDouble (std::string strString)
	{
		std::istringstream ssStream (strString);
		double dReturn = 0;
		ssStream >> dReturn;

		return (dReturn);
	}

	std::string s13DoubleToString (double dDouble)
	{
		std::stringstream ssStream ("");
		ssStream << dDouble;

		return (ssStream.str ());
	}

	bool s13IsDouble (std::string strString)
	{
		std::istringstream ssStream (strString);
		double dReturn = 0.0;
		ssStream >> dReturn;

		if (ssStream.fail () == true)
			return (false);

		return (true);
	}

	bool s13StringToBool (std::string strString, bool bCapsMatters)
	{
		if (bCapsMatters == false)
			RadXmlCommon::s13ASCII_StringToLowerRef (strString);

		if (strString == "yes")
			return (true);
		else if (strString == "no")
			return (false);
		else if (strString == "true")
			return (true);
		else if (strString == "false")
			return (false);
		else if (strString == "0")
			return (false);
		else
		{
			int iReturn = 0;

			#ifdef defUSE_EXCEPTIONS
			try
			{
			#endif
				std::istringstream ssStream (strString);
				ssStream >> iReturn;

				if (ssStream.fail () == true)
				{
					#ifdef defUSE_EXCEPTIONS
						throw COM_MSG1;
					#endif
				}
			#ifdef defUSE_EXCEPTIONS
			}
			catch (std::exception ex)
			{
				throw COM_MSG1;
			}
			#endif

			if (iReturn == 0)
				return (false);
			else
				return (true);
		}

		return (false);
	}

	std::string s13BoolToString (bool bBool)
	{
		std::string strReturn = "false";

		if (bBool == true)
			strReturn = "true";

		return (strReturn);
	}

	bool s13IsBool (std::string strString, bool bCapsMatters)
	{
		if (bCapsMatters == false)
			RadXmlCommon::s13ASCII_StringToLowerRef (strString);

		if (strString == "yes")
			return (true);
		else if (strString == "no")
			return (true);
		else if (strString == "true")
			return (true);
		else if (strString == "false")
			return (true);
		else if (strString == "on")
			return (true);
		else if (strString == "off")
			return (true);
		else if (strString == "0")
			return (true);
		else
		{
			int iReturn = 0;

			std::istringstream ssStream (strString);
			ssStream >> iReturn;

			if (iReturn == 0)
				return (false);
			else
				return (true);
		}

		return (false);
	}

	int s13HexStringToInt (std::string hexString)
	{
		int iHexCode = -1;
		std::stringstream ssStream (hexString);

		if (ssStream.fail () == true)
			return (0);

		ssStream >> std::hex >> iHexCode;

		return (iHexCode);
	}

	std::string s13IntToHexString (int iInt)
	{
		std::string strReturn = "";
		std::stringstream ssStream ("");

		ssStream << std::hex << iInt;
		strReturn = ssStream.str ();

		if (strReturn.size () == 1)
			strReturn.insert (0, "0");

		return (strReturn);
	}

	// RS Hash function made by Robert Sedgwicks, modified by Arash Partow. This code was taken from: 
	// http://www.partow.net/programming/hashfunctions/index.html
	unsigned long s13StringToHash (std::string stringToHash)
	{
		unsigned long b    = 378551;
		unsigned long a    = 63689;
		unsigned long iHash = 0;

		for(std::size_t i = 0; i < stringToHash.length(); i++)
		{
			iHash = iHash * a + stringToHash.at (i);
			a    = a * b;
		}

		return (iHash);
	}
}

