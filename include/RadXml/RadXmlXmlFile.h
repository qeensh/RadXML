#ifndef _RADXML_XML_FILE_H_
	#define _RADXML_XML_FILE_H_

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

	#include "RadXmlPreprocessor.h"
	#include "RadXmlApp.h"
	#include "RadXmlGame.h"

	class XMLParser;

	namespace RadXml
	{
		class defRADXML_EXPORT XmlFile: public App, public Game
		{
			public:
				XmlFile ();
				XmlFile (std::string file);
				~XmlFile ();

				bool loadFile (std::string file);
				void parseXMLFile (XMLParser *xmlFile);

				bool loadExternal (std::string name, std::string filename);
				void executeExternalFunction (std::string name, std::string function);
				bool unloadExternal (std::string name);

				void processDeleteObjects ();

				inline void setFilename (std::string path)
				{
					filename = path;
				}

				inline std::string getFilename ()
				{
					return (filename);
				}

				inline void setDirectoryPath (std::string path)
				{
					dirPath = path;
				}

				inline std::string getDirectoryPath ()
				{
					return (dirPath);
				}

				inline std::string getDirPath ()
				{
					return (dirPath);
				}

			protected:
				void init ();

				double dRadXmlVersion;

				std::string filename;
				std::string dirPath;

				HASHMAP<unsigned long, int> hshDLLs;
		};
	}
#endif

