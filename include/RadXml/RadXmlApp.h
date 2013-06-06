#ifndef _RADXML_APP_H_
	#define _RADXML_APP_H_

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

	#include <string>
	#include <vector>

	#include "RadXmlPreprocessor.h"
	#include "RadXmlNative.h"

	class XMLElement;

	namespace RadXml
	{
		enum AppTypes
		{
			AT_NATIVE			= 1,
			AT_RENDER			= 2
		};

		enum AppObjectTypes
		{
			AOT_WINDOW			= 1,
			AOT_MENU			= 2,
			AOT_MENU_ITEM		= 3,
			AOT_LABEL			= 4,
			AOT_TEXT_BOX		= 5,
			AOT_COMBO_BOX		= 6,
			AOT_BUTTON			= 7
		};

		class AppObject;
		class XmlFile;

		class defRADXML_EXPORT App
		{
			public:
				App (std::string newName);
				~App ();

				void destroyAllAppObjects ();

				AppObject *createAppObject (App *file, int creationType, int type, std::string name, AppObject *parent = 0, XMLElement *element = 0);
				void addAppObject (AppObject *newAppObject);
				AppObject *getAppObject (std::string findName, bool skipErrorMessage = false);
				AppObject *getAppObject (unsigned long hash, bool skipErrorMessage = false);
				void removeAppObject (std::string findName);
				void removeAppObject (unsigned long hash);

				void setName (std::string newName);
				inline std::string getName ()
				{
					return (appName);
				}

				inline unsigned long getHash ()
				{
					return (appHash);
				}

				void parseApp (XmlFile *file, int type, XMLElement *element);
				void parseXMLAppObjects (int creationType, XMLElement *element, AppObject *parent = 0);
				void parseXMLAppObject (int creationType, int type, XMLElement *element, AppObject *parent = 0);

				void addAppObjectForDeletion (AppObject *object);

				void processDeleteAppObjects ();

			protected:
				std::string appName;
				unsigned long appHash;

				std::vector<AppObject *> aryAppObjectsToDelete;

				HASHMAP<unsigned long, AppObject *> hshAppObjects;
		};

		class AppClass
		{
			public:
				static void setupAppJS ();

				static Variable *getAppObjectByName (Variable *thisObject, Variable *arguments);
				static Variable *createObject (Variable *thisObject, Variable *arguments);
				static Variable *deleteObject (Variable *thisObject, Variable *arguments);
		};
	}
#endif

