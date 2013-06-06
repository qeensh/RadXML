#ifndef _RADXML_EVENTS_H_
	#define _RADXML_EVENTS_H_

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

	#include "RadXmlPreprocessor.h"

	namespace RadXml
	{
		class defRADXML_EXPORT Event
		{
			public:
				Event (std::string newName, std::string newJavascript);

				void setName (std::string newName);

				inline std::string getName ()
				{
					return (name);
				}

				inline unsigned long getHash ()
				{
					return (hash);
				}

				inline void setJavascript (std::string newJavascript)
				{
					javascript = newJavascript;
				}

				inline std::string getJavascript ()
				{
					return (javascript);
				}

			protected:
				std::string name;
				unsigned long hash;
				std::string javascript;
		};

		class defRADXML_EXPORT Events
		{
			public:
				Events ();
				Events (const Events &copy);
				~Events ();

				void addEvent (Event *newEvent);
				Event *getEvent (std::string findName);
				Event *getEvent (unsigned long findHash);
				void setJavascriptToEvent (std::string findName, std::string javascript);
				void setJavascriptToEvent (unsigned long findHash, std::string javascript);
				std::string getJavascriptFromEvent (std::string findName);
				std::string getJavascriptFromEvent (unsigned long findHash);
				bool executeJavascriptFromEvent (std::string findName);
				bool executeJavascriptFromEvent (unsigned long findHash);

			private:
				HASHMAP<unsigned long, Event *> hshEvents;
		};
	}
#endif

