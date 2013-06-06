#ifndef _RADXML_NET_NETCLASS_H_
	#define _RADXML_NET_NETCLASS_H_

	#include <string>

	#ifndef HASHMAP
		#ifdef __GNUC__
			#include <ext/hash_map>

			namespace std
			{
				using namespace __gnu_cxx;
			}

			#define HASHMAP	std::hash_map

			/// @note You may have to use this for upcoming GCC compilers for hash maps
			/// in the mean time, you may want to compile with -Wno-deprecated on compilers
			/// that complain about a deprecated file.
			/// #include <unordered_map>
			/// #define HASHMAP	std::tr1::unordered_map
		#else
			#include <hash_map>

			#define HASHMAP	stdext::hash_map
		#endif
	#endif

	#ifdef defNET_ON
		class s13NetworkingClass
		{
			public:
				void addHiddenData (std::string nameOfData, void *data);
				void *getHiddenData (unsigned long nameOfData);
				void *getHiddenData (std::string nameOfData);
				bool hasHiddenData (unsigned long nameOfData);
				bool hasHiddenData (std::string nameOfData);
				void removeHiddenData (std::string nameOfData);

			protected:
				HASHMAP<unsigned long, void *> hshHidden;
		};

		inline char *cItoS (int iInteger)
		{
			int iIdx = 0;
			bool bNeg = false;

			char *cTemp = new char [22];

			for (int iJdx = 0; iJdx < 22; iJdx++)
				cTemp[iJdx] = '\0';

			if (iInteger == 0)
			{
				cTemp[0] = '0';
				cTemp[1] = 0;

				return (cTemp);
			}

			if (iInteger < 0)
			{
				iInteger *= -1;
				bNeg = true;
			}

			while (iInteger > 0)
			{
				cTemp[iIdx++] = ((iInteger % 10) + '0');
				iInteger /= 10;
			}

			cTemp[iIdx] = 0;

			char *cStringRev = new char [22];

			for (int iJdx = 0; iJdx < 22; iJdx++)
				cStringRev[iJdx] = '\0';

			bool bFirst = false;

			for (int iJdx = 0; iJdx < 22; iJdx++)
			{
				if ((cTemp[iJdx] == '\0') && (bNeg == true) && (bFirst == false))
				{
					cStringRev[iJdx] = '-';
					bFirst = true;
				}
				else
					cStringRev[iJdx] = cTemp[iJdx];
			}

			for (int iKdx = 21, iUdx = 0; iKdx >= 0; iKdx--)
			{
				if (cStringRev[iKdx] != 0)
					cTemp[iUdx++] = cStringRev[iKdx];
			}

			delete []cStringRev;
			cStringRev = 0;

			return (cTemp);
		}

		inline bool bIsStringIPAddressOnly (const char *cString, int iLen)
		{
			for (int iIdx = 0; iIdx < iLen; iIdx++)
			{
				if (((int)cString[iIdx] < 48) || ((int)cString[iIdx] > 57))
				{
					if ((int)cString[iIdx] == 46)
						continue;

					return (false);
				}
			}

			return (true);
		}
	#endif
#endif

