#include "common/com_radxml_algorithms.h"

namespace RadXmlCommon
{
	// s13GaussianRandomNumber
	s13GaussianRandomNumber::s13GaussianRandomNumber ()
	{
		dSeed = 0.0;
	}

	void s13GaussianRandomNumber::Init (double dSeedPass)
	{
		dSeed = dSeedPass;
	}

	double s13GaussianRandomNumber::GetNumber ()
	{
		double dX = 0.0;
		double dXSD = 3.2;
		double dXAV = 1.5;

		for (int iIdx = 0; iIdx < 12; iIdx++)
			dX = (dX + UniformRandomNumberGenerator ());

		dX = (dXSD * (dX - 6.0) + dXAV);

		return (dX);
	}

	double s13GaussianRandomNumber::UniformRandomNumberGenerator ()
	{
		dSeed = (16807 * dSeed);
		dSeed = (dSeed - (dSeed / 2147483647) * 2147483647);
		//dSeed -= (int (dSeed * 4.656612875245796924105750827168e-10) * 2147483647);

		//return (dSeed * 4.656612875245796924105750827168e-10);
		return (dSeed / 2147483647);
	}

	// s13MersenneTwister
	s13MersenneTwister::s13MersenneTwister ()
	{
		iIndex = 0;

		for (int iIdx = 0; iIdx < 624; iIdx++)
			iMT[iIdx] = 0;
	}
	s13MersenneTwister::s13MersenneTwister (int iSeed)
	{
		iIndex = 0;

		Init (iSeed);
	}

	void s13MersenneTwister::Init (int iSeed)
	{
		iMT[0] = iSeed;

		for (int iIdx = 1; iIdx < 624; iIdx++)
			iMT[iIdx] = ((1812433253 * (iMT[(iIdx - 1)] ^ 
					(iMT[(iIdx - 1)] >> 30)) + iIdx) & 0xffffffff);

		/*iMT[0] = iSeed;

		for (int iIdx = 1; iIdx < 624; iIdx++)
			iMT[iIdx] = ((1812433253 * (iMT[(iIdx - 1)] ^ 
					(iMT[(iIdx - 1)] >> 30)) + iIdx) & 0xffffffff);*/
	}

	void s13MersenneTwister::Generate ()
	{
		/*for (int iIdx = 0; iIdx < 624; iIdx++)
		{
			int iY = ((iMT[iIdx] + ((iMT[iIdx + 1] % 624) & 0xfffffffeUL)) & 0xfffffffcUL);	/// @bug 0xfffffffcUL possibly incorrect.
			iMT[iIdx] = (iMT[((iIdx + 397) % 624)] ^ (iY >> 1));

			if ((iY % 2) == 1)
				iMT[iIdx] = (iMT[iIdx] ^ 0x9908b0dfUL);
		}*/
	}

	int s13MersenneTwister::GetNumber ()
	{
		if (iIndex == 0)
		{
			for (int iIdx = 0; iIdx < 623; iIdx++)	/// @bug This 623 supposed to be 624?
			{
				int iY = ((iMT[iIdx] + ((iMT[iIdx + 1] % 624) & 0xfffffffe)) & 0xfffffffc);	/// @bug 0xfffffffcUL possibly incorrect.
				iMT[iIdx] = (iMT[((iIdx + 397) % 624)] ^ (iY >> 1));

				if ((iY % 2) == 1)
					iMT[iIdx] = (iMT[iIdx] ^ 0x9908b0df);
			}
		}

		int iY = iMT[iIndex];

		iY = (iY ^ (iY >> 11));
		iY = (iY ^ ((iY << 7) & (0x9d2c5680)));
		iY = (iY ^ ((iY << 15) & (0xefc60000)));
		iY = (iY ^ (iY >> 18));

		iIndex = ((iIndex + 1) % 624);

		return (iY);
	}

	int s13MersenneTwister::Twist (int iM, int iS0, int iS1)
	{
		return (iM ^ (((iS0 & 0x80000000UL) | (iS1 & 0x80000000UL)) >> 1) ^ 
				(-1 * (iS1 & 0x00000001UL) & 0x9908b0dfUL));
	}

	template<typename typType>
	void s13BubbleSort_Descending (std::vector<typType> &aryArray, std::vector<Swapper> *arySwappers)
	{
		if (aryArray.size () < 1)
			return;

		typType iSwap = 1;
		typType iTemp = 0;

		for (int iIdx = 1; ((iIdx < (int)aryArray.size ()) && (iSwap == 1)); iIdx++)
		{
			iSwap = 0;

			for (int iJdx = 0; iJdx < ((int)aryArray.size () - 1); iJdx++)
			{
				if (aryArray.at (iJdx + 1) < aryArray.at (iJdx))
				{
					iTemp = aryArray.at (iJdx);
					aryArray.at (iJdx) = aryArray.at (iJdx + 1);
					aryArray.at (iJdx + 1) = iTemp;
					iSwap = 1;
				}
			}
		}
	}

	void s13StringBubbleSort_Descending (std::vector<std::string> *aryArray, std::vector<Swapper> *arySwappers)
	{
		if (aryArray->size () < 1)
			return;

		if (arySwappers != 0)
			arySwappers->clear ();

		int iResult = 0;

		for (unsigned int iIdx = 0; iIdx < (aryArray->size () - 1); iIdx++)
		{
			for (unsigned int iJdx = 0; iJdx < ((aryArray->size () - 1) - iIdx); iJdx++)
			{
				iResult = aryArray->at (iJdx).compare (aryArray->at (iJdx + 1));

				if (iResult > 0)
				{
					std::string strTemp = aryArray->at (iJdx);
					aryArray->at (iJdx) = aryArray->at (iJdx + 1);
					aryArray->at (iJdx + 1) = strTemp;

					if (arySwappers != 0)
					{
						Swapper swap;
						swap.iSwap1 = iJdx;
						swap.iSwap2 = (iJdx + 1);

						arySwappers->push_back (swap);
					}
				}
			}
		}
	}

	// s13Sort
	s13Sort::s13Sort (float fArray[])
	{
		// Finish this later
	}

	// s13MergeSort
	template<typename typType>
	void s13MergeSort<typType>::sort (typType aryArray[], int iSize)
	{
		if (iSize <= 1)
			return;

		doSort (aryArray, 0, iSize);
	}

	template<typename typType>
	void s13MergeSort<typType>::sort (std::vector<typType> *aryArray)
	{
		if (aryArray->size () <= 1)
			return;

		doSort (aryArray, 0, aryArray->size ());
	}

	template<typename typType>
	void s13MergeSort<typType>::doSort (typType aryArray[], int iStart, int iSize)
	{
		if (iSize > 1)
		{
			int iMiddle = (iSize >> 1);
			iSize -= iMiddle;

			doSort (aryArray, iStart, iMiddle);
			doSort (aryArray, (iStart + iMiddle), iSize);

			doMerge (aryArray, iStart, iMiddle, iSize);
		}
	}

	template<typename typType>
	void s13MergeSort<typType>::doMerge (typType aryArray[], int iStart, int iMiddle, int iEnd)
	{
		typType *iSplitArray = new typType [(iMiddle + iEnd)];

		int iCopiedDtoT = 0;
		int iCopiedFHD = 0;
		int iCopiedSHD = 0;

		while ((iCopiedFHD < iMiddle) && (iCopiedSHD < iEnd))
		{
			if (aryArray[(iStart + iCopiedFHD)] < aryArray[(iStart + iMiddle + iCopiedSHD)])
				iSplitArray[iCopiedDtoT++] = aryArray[(iStart + iCopiedFHD++)];
			else
				iSplitArray[iCopiedDtoT++] = aryArray[(iStart + iMiddle + iCopiedSHD++)];
		}

		while (iCopiedFHD < iMiddle)
			iSplitArray[iCopiedDtoT++] = aryArray[(iStart + iCopiedFHD++)];

		while (iCopiedSHD < iEnd)
			iSplitArray[iCopiedDtoT++] = aryArray[(iStart + iMiddle + iCopiedSHD++)];

		for (int iIdx = 0; iIdx < (iMiddle + iEnd); iIdx++)
			aryArray[(iStart + iIdx)] = iSplitArray[iIdx];

		delete []iSplitArray;
		iSplitArray = 0;
	}

	template<typename typType>
	void s13MergeSort<typType>::doSort (std::vector<typType> *aryArray, int iStart, int iSize)
	{
		if (iSize > 1)
		{
			int iMiddle = (iSize >> 1);
			iSize -= iMiddle;

			doSort (aryArray, iStart, iMiddle);
			doSort (aryArray, (iStart + iMiddle), iSize);

			doMerge (aryArray, iStart, iMiddle, iSize);
		}
	}

	template<typename typType>
	void s13MergeSort<typType>::doMerge (std::vector<typType> *aryArray, int iStart, int iMiddle, int iEnd)
	{
		std::vector<typType> *arySplitArray = new std::vector<typType> (iMiddle + iEnd);

		int iCopiedDtoT = 0;
		int iCopiedFHD = 0;
		int iCopiedSHD = 0;

		while ((iCopiedFHD < iMiddle) && (iCopiedSHD < iEnd))
		{
			if (aryArray->at (iStart + iCopiedFHD) < aryArray->at (iStart + iMiddle + iCopiedSHD))
				arySplitArray->at (iCopiedDtoT++) = aryArray->at (iStart + iCopiedFHD++);
			else
				arySplitArray->at (iCopiedDtoT++) = aryArray->at (iStart + iMiddle + iCopiedSHD++);
		}

		while (iCopiedFHD < iMiddle)
			arySplitArray->at (iCopiedDtoT++) = aryArray->at (iStart + iCopiedFHD++);

		while (iCopiedSHD < iEnd)
			arySplitArray->at (iCopiedDtoT++) = aryArray->at (iStart + iMiddle + iCopiedSHD++);

		for (int iIdx = 0; iIdx < (iMiddle + iEnd); iIdx++)
			aryArray->at (iStart + iIdx) = arySplitArray->at (iIdx);

		delete arySplitArray;
		arySplitArray = 0;
	}
}

