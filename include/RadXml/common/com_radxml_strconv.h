#ifndef _COM_RADXML_STRCONV_H_
	#define _COM_RADXML_STRCONV_H_

	#include <string>
	#include <vector>
	#include <sstream>

	namespace RadXmlCommon
	{
		std::vector<std::string> s13StringSplit (std::string strString, std::string strDelim);
		std::vector<std::string> s13SplitString (std::string strString, std::string strDelim);
		std::vector<std::string> s13SplitString_WithExceptions (std::string strString, std::string strDelim, 
									std::vector<int> aryExceptions_Begin, std::vector<int> aryExceptions_End);

		std::string s13RemoveWhitespaces (std::string strString);
		std::string s13ASCII_StringToLower (std::string strString);
		void s13ASCII_StringToLowerRef (std::string &strString);
		std::string s13ASCII_StringToUpper (std::string strString);
		void s13ASCII_StringToUpperRef (std::string &strString);

		int s13StringToInt (std::string strString);
		std::string s13IntToString (int iInteger);
		bool s13IsInt (std::string strString);

		long s13StringToLong (std::string strString);
		std::string s13LongToString (long lInteger);

		unsigned long s13StringToUnsignedLong (std::string strString);
		std::string s13UnsignedLongToString (unsigned long ulInteger);

		double s13StringToDouble (std::string strString);
		std::string s13DoubleToString (double dDouble);
		bool s13IsDouble (std::string strString);

		bool s13StringToBool (std::string strString, bool bCapsMatters = true);
		std::string s13BoolToString (bool bBool);
		bool s13IsBool (std::string strString, bool bCapsMatters = true);

		int s13HexStringToInt (std::string hexString);
		std::string s13IntToHexString (int iInt);

		unsigned long s13StringToHash (std::string stringtohash);
	}
#endif

