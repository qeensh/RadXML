#include "common/com_radxml_array.h"
#include "common/com_radxml_algorithms.h"

namespace RadXmlCommon
{
	template <typename typType>
	void ArrayZero (typType *aryMem, unsigned int uiArrayLength)
	{
		for (unsigned int iIdx = 0; iIdx < uiArrayLength; iIdx++)
			aryMem[iIdx] = 0;
	}

	template <typename typType>
	void ArraySet (typType *aryArrayToSet, unsigned int uiArrayLength, typType tData)
	{
		for (unsigned int iIdx = 0; iIdx < uiArrayLength; iIdx++)
			aryArrayToSet[iIdx] = tData;
	}

	template <typename typType>
	void ArraySet (std::vector<typType> aryArrayToSet, typType tData)
	{
		for (unsigned int iIdx = 0; iIdx < aryArrayToSet.size (); iIdx++)
			aryArrayToSet.at (iIdx) = tData;
	}

	template <typename typType>
	void ArrayCopy (std::vector<typType> *arySource, std::vector<typType> *aryDest)
	{
		for (unsigned int iIdx = 0; iIdx < arySource->size (); iIdx++)
			aryDest->push_back (arySource->at (iIdx));
	}

	template <typename typType>
	void ArrayCopy (std::vector<typType> arySource, std::vector<typType> &aryDest)
	{
		for (unsigned int iIdx = 0; iIdx < arySource.size (); iIdx++)
			aryDest.push_back (arySource.at (iIdx));
	}

	template <typename typType>
	void ArrayCopy (typType *arySource, typType *aryDest, unsigned int uiArrayLength)
	{
		for (unsigned int iIdx = 0; iIdx < uiArrayLength; iIdx++)
			aryDest[iIdx] = arySource[iIdx];
	}

	template <typename typType>
	void ArrayDeleteElement (std::vector<typType> &aryTarget, int iIndexToRemove)
	{
		#ifdef defGCC
			aryTarget.erase ((aryTarget.begin () + iIndexToRemove));
		#else
			std::vector<typType>::iterator itBegin = aryTarget.begin ();
			itBegin += iIndexToRemove;
			aryTarget.erase (itBegin);
		#endif
	}

	template <typename typType>
	void ArrayDeleteElement (std::vector<typType> *aryTarget, int iIndexToRemove)
	{
		#ifdef defGCC
			aryTarget->erase ((aryTarget->begin () + iIndexToRemove));
		#else
			std::vector<typType>::iterator itBegin = aryTarget->begin ();
			itBegin += iIndexToRemove;
			aryTarget->erase (itBegin);
		#endif
	}

	template <typename typType>
	void ArrayDeleteAllElements (const std::vector<typType *> &aryTarget)
	{
		for (unsigned int uiIdx = 0; uiIdx < aryTarget.size (); uiIdx++)
		{
			delete aryTarget.at (uiIdx);
			aryTarget.at (uiIdx) = 0;
		}
	}

	template <typename typType>
	void ArrayDeleteAllElements (std::vector<typType *> *aryTarget)
	{
		for (unsigned int uiIdx = 0; uiIdx < aryTarget->size (); uiIdx++)
		{
			delete aryTarget->at (uiIdx);
			aryTarget->at (uiIdx) = 0;
		}
	}

	template <typename typType>
	std::vector<typType> *ArrayCreate ()
	{
		return (new std::vector<typType> ());
	}

	template <typename typType>
	std::vector<typType *> *ArrayCreateRef ()
	{
		return (new std::vector<typType *> ());
	}

	template <typename typType>
	void ArrayDestroy (std::vector<typType> *aryTarget)
	{
		if (aryTarget != 0)
		{
			delete aryTarget;
			aryTarget = 0;
		}
	}

	template <typename typType1>
	void ListCopy (std::list<typType1> lstSource, std::list<typType1> *lstDest)
	{
		typename std::list<typType1>::iterator itBegin = lstSource.begin ();
		typename std::list<typType1>::iterator itEnd = lstSource.end ();

		while (itBegin != itEnd)
		{
			lstDest->push_back (*itBegin);

			itBegin++;
		}
	}

	template <typename typType1>
	void ListCopy (std::list<typType1> *lstSource, std::list<typType1> *lstDest)
	{
		typename std::list<typType1>::iterator itBegin = lstSource->begin ();
		typename std::list<typType1>::iterator itEnd = lstSource->end ();

		while (itBegin != itEnd)
		{
			lstDest->push_back (*itBegin);

			itBegin++;
		}
	}

	template <typename typType1>
	typType1 ListGetAtIndex (std::list<typType1> *lstTarget, unsigned int index)
	{
		if (lstTarget != 0)
		{
			int iIdx = 0;
			typename std::list<typType1>::iterator itBegin = lstTarget->begin ();
			typename std::list<typType1>::iterator itEnd = lstTarget->end ();

			while (itBegin != itEnd)
			{
				if (iIdx == index)
					return ((*itBegin));

				itBegin++;
				iIdx++;
			}
		}

		return (0);
	}

	template <typename typType1>
	void ListDeleteAllElements (std::list<typType1> *lstTarget)
	{
		if (lstTarget != 0)
		{
			typename std::list<typType1>::iterator itBegin = lstTarget->begin ();
			typename std::list<typType1>::iterator itEnd = lstTarget->end ();

			while (itBegin != itEnd)
			{
				delete (*itBegin);
				(*itBegin) = 0;

				itBegin++;
			}
		}
	}

	template <typename typType1>
	void ListDestroy (std::list<typType1> *lstTarget)
	{
		if (lstTarget != 0)
		{
			delete lstTarget;
			lstTarget = 0;
		}
	}

	template <typename typType1, typename typType2>
	void HashCopy (std::map<typType1, typType2> *arySource, std::map<typType1, typType2> *aryDest)
	{
		typename std::map<typType1, typType2>::iterator itBegin = arySource->begin ();
		typename std::map<typType1, typType2>::iterator itEnd = arySource->end ();

		while (itBegin != itEnd)
		{
			aryDest->insert ((*itBegin));

			itBegin++;
		}
	}

	template <typename typType1, typename typType2>
	void HashDeleteAllElements (std::map<typType1, typType2> hshTarget)
	{
		typename std::map<typType1, typType2>::iterator itBegin = hshTarget.begin ();
		typename std::map<typType1, typType2>::iterator itEnd = hshTarget.end ();

		while (itBegin != itEnd)
		{
			delete (*itBegin).second;
			(*itBegin).second = 0;
			itBegin++;
		}
	}

	template <typename typType1, typename typType2>
	void HashDeleteAllElements (std::map<typType1, typType2> *hshTarget)
	{
		if (hshTarget != 0)
		{
			typename std::map<typType1, typType2>::iterator itBegin = hshTarget->begin ();
			typename std::map<typType1, typType2>::iterator itEnd = hshTarget->end ();

			while (itBegin != itEnd)
			{
				delete (*itBegin).second;
				(*itBegin).second = 0;
				itBegin++;
			}
		}
	}

	template <typename typType1, typename typType2>
	void HashDestroy (std::map<typType1, typType2> *hshTarget)
	{
		if (hshTarget != 0)
		{
			delete hshTarget;
			hshTarget = 0;
		}
	}
}

