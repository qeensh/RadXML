#ifndef _COM_RADXML_ALGORITHMS_H_
	#define _COM_RADXML_ALGORITHMS_H_

	#include <vector>
	#include <string>

	namespace RadXmlCommon
	{
		/// @bug Does this work?
		class s13GaussianRandomNumber
		{
			public:
				s13GaussianRandomNumber ();

				void Init (double dSeedPass);
				double GetNumber ();

			private:
				double dSeed;

				double UniformRandomNumberGenerator ();
		};

		class s13MersenneTwister
		{
			public:
				s13MersenneTwister ();
				s13MersenneTwister (int iSeed);

				void Init (int iSeed);
				int GetNumber ();

			private:
				int iIndex;
				int iMT[624];

				void Generate ();
				int Twist (int iM, int iS0, int iS1);
		};

		class Swapper
		{
			public:
				int iSwap1;
				int iSwap2;
		};

		template<typename typType>
		void s13BubbleSort_Descending (std::vector<typType> &aryArray, std::vector<Swapper> *arySwappers = 0);
		void s13StringBubbleSort_Descending (std::vector<std::string> *aryArray, std::vector<Swapper> *arySwappers = 0);

		template<typename typType>
		inline void s13SortArrayWithSwappers (std::vector<Swapper> *arySwappers, std::vector<typType> *aryArray)
		{
			for (unsigned int iIdx = 0; iIdx < arySwappers->size (); iIdx++)
			{
				Swapper sSwap = arySwappers->at (iIdx);

				typType iTemp = aryArray->at (sSwap.iSwap1);
				aryArray->at (sSwap.iSwap1) = aryArray->at (sSwap.iSwap2);
				aryArray->at (sSwap.iSwap2) = iTemp;
			}
		}

		class s13Sort
		{
			s13Sort (float fArray[]);
		};

		template<typename typType>
		class s13MergeSort
		{
			public:
				static void sort (typType aryArray[], int iSize);
				static void sort (std::vector<typType> *aryArray);

			protected:
				static void doSort (typType aryArray[], int iStart, int iSize);
				static void doMerge (typType aryArray[], int iStart, int iMiddle, int iEnd);
				static void doSort (std::vector<typType> *aryArray, int iStart, int iSize);
				static void doMerge (std::vector<typType> *aryArray, int iStart, int iMiddle, int iEnd);
		};
	}
#endif

