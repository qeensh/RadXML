#ifndef _RADXML_SINGLETON_H_
	#define _RADXML_SINGLETON_H_

	namespace RadXml
	{
		template <class T>
		class Singleton
		{
			public:
				inline static T getSingleton ()
				{
					return (*rxSingleton);
				}

				inline static T *getSingletonPtr ()
				{
					return (rxSingleton);
				}

			protected:
				inline Singleton ()
				{
					if (rxSingleton == 0)
						rxSingleton = (T *)this;
				}

				static T *rxSingleton;
		};
	}
#endif

