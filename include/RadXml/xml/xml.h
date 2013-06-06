// parser.h
// Modified by:
// Nathanael Coonrod
// ncoonrod@highersoftware.com
// Higher Edge Software, LLC
// www.highersoftware.com
// Modified: August 10, 2011

#ifndef _PARSER_H_
	#define _PARSER_H_

	#include <string>
	#include <vector>

	#ifdef defAPI_TINY_XML
		#include <tinyxml.h>
	#endif

	class XMLParser;

	class XMLElement
	{
		public:
			XMLElement ();
			~XMLElement ();

			XMLElement *findTag (std::string tagToFind, XMLElement *inElement = 0);
			std::vector<XMLElement *> findAllTags (std::string tagToFind, XMLElement *inElement = 0, bool traverseDown = true);
			std::string getAttributeValue (std::string attributeName);

			XMLElement *parent;
			std::vector<XMLElement *> *children;

			std::string tag;
			std::vector<std::string> aryAttributeNames;
			std::vector<std::string> aryAttributeValues;
			std::string content;

			XMLParser *parentParser;
	};

	class XMLParser
	{
		public:
			XMLParser ();
			XMLParser (std::string file);
			~XMLParser ();

			bool parseFile (std::string file);
			XMLElement *findTag (std::string tagToFind);
			std::vector<XMLElement *> findAllTags (std::string tagToFind);

			inline std::string getFilename ()
			{
				return (strFilename);
			}

		protected:
			#ifdef defAPI_TINY_XML
				void navigateChildren (int depth, XMLElement *parent, TiXmlElement *element);

				TiXmlDocument tdFile;
			#endif

			std::string strFilename;
			std::vector<XMLElement *> *aryElements;
	};
#endif

