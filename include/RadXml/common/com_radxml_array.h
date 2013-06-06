#ifndef _COM_RADXML_ARRAY_H_
	#define _COM_RADXML_ARRAY_H_

	#include <vector>
	#include <list>
	#include <map>

	namespace RadXmlCommon
	{
		// Arrays
		template <typename typType>
		void ArrayZero (typType *aryMem, unsigned int uiArrayLength);
		template <typename typType>
		void ArraySet (typType *aryArrayToSet, unsigned int uiArrayLength, typType tData);
		template <typename typType>
		void ArraySet (std::vector<typType> aryArrayToSet, typType tData);
		template <typename typType>
		void ArrayCopy (std::vector<typType> *arySource, std::vector<typType> *aryDest);
		template <typename typType>
		void ArrayCopy (std::vector<typType> arySource, std::vector<typType> &aryDest);
		template <typename typType>
		void ArrayCopy (typType *arySource, typType *aryDest, unsigned int uiArrayLength);
		template <typename typType>
		void ArrayDeleteElement (std::vector<typType> &aryTarget, int iIndexToRemove);
		template <typename typType>
		void ArrayDeleteElement (std::vector<typType> *aryTarget, int iIndexToRemove);
		template <typename typType>
		void ArrayDeleteAllElements (const std::vector<typType *> &aryTarget);
		template <typename typType>
		void ArrayDeleteAllElements (std::vector<typType *> *aryTarget);
		template <typename typType>
		std::vector<typType> *ArrayCreate ();
		template <typename typType>
		std::vector<typType *> *ArrayCreateRef ();
		template <typename typType>
		void ArrayDestroy (std::vector<typType> *aryTarget);

		// Lists
		template <typename typType1>
		void ListCopy (std::list<typType1> lstSource, std::list<typType1> *lstDest);
		template <typename typType1>
		void ListCopy (std::list<typType1> *lstSource, std::list<typType1> *lstDest);
		template <typename typType1>
		void ListDeleteAllElements (std::list<typType1> *lstTarget);
		template <typename typType1>
		void ListDestroy (std::list<typType1> *lstTarget);

		// Hash tables
		template <typename typType1, typename typType2>
		void HashCopy (std::map<typType1, typType2> *arySource, std::map<typType1, typType2> *aryDest);
		template <typename typType1, typename typType2>
		void HashDeleteAllElements (std::map<typType1, typType2> hshTarget);
		template <typename typType1, typename typType2>
		void HashDeleteAllElements (std::map<typType1, typType2> *hshTarget);
		template <typename typType1, typename typType2>
		void HashDestroy (std::map<typType1, typType2> *hshTarget);
	}
#endif

